// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the Apache 2.0 License.
#include "../tpcc_serializer.h"
#include "apps/utils/metrics_tracker.h"
#include "ccf/app_interface.h"
#include "ccf/user_frontend.h"
#include "tpcc_global.h"
#include "tpcc_setup.h"
#include "tpcc_tables.h"
#include "tpcc_transactions.h"

#include <charconv>

using namespace std;
using namespace nlohmann;
using namespace ccf;

namespace ccfapp
{
  class TpccHandlers : public UserEndpointRegistry
  {
  private:
    metrics::Tracker metrics_tracker;

    void set_error_status(
      ccf::endpoints::EndpointContext& args, int status, std::string&& message)
    {
      args.rpc_ctx->set_response_status(status);
      args.rpc_ctx->set_response_header(
        http::headers::CONTENT_TYPE, http::headervalues::contenttype::TEXT);
      args.rpc_ctx->set_response_body(std::move(message));
    }

    void set_ok_status(ccf::endpoints::EndpointContext& args)
    {
      args.rpc_ctx->set_response_status(HTTP_STATUS_OK);
      args.rpc_ctx->set_response_header(
        http::headers::CONTENT_TYPE,
        http::headervalues::contenttype::OCTET_STREAM);
    }

    void set_no_content_status(ccf::endpoints::EndpointContext& args)
    {
      args.rpc_ctx->set_response_status(HTTP_STATUS_NO_CONTENT);
    }

  public:
    TpccHandlers(AbstractNodeContext& context) : UserEndpointRegistry(context)
    {}

    void init_handlers() override
    {
      UserEndpointRegistry::init_handlers();

      auto create = [this](auto& args) {
        LOG_DEBUG_FMT("Creating tpcc database");
        const auto& body = args.rpc_ctx->get_request_body();
        auto db = tpcc::DbCreation::deserialize(body.data(), body.size());
        tpcc::SetupDb setup_db(args, db.new_orders_per_district, db.seed);
        setup_db.run();
        LOG_DEBUG_FMT("Creating tpcc database - end");

        set_no_content_status(args);
      };

      auto do_stock_level = [this](auto& args) {
        LOG_DEBUG_FMT("stock level");
        const auto& body = args.rpc_ctx->get_request_body();
        auto info = tpcc::StockLevel::deserialize(body.data(), body.size());
        tpcc::TpccTransactions tx(args, info.seed);
        tx.stock_level(info.warehouse_id, info.district_id, info.threshold);
        LOG_DEBUG_FMT("stock level - end");

        set_no_content_status(args);
      };

      auto do_order_status = [this](auto& args) {
        LOG_DEBUG_FMT("order status");
        const auto& body = args.rpc_ctx->get_request_body();
        auto info = tpcc::TxInfo::deserialize(body.data(), body.size());
        tpcc::TpccTransactions tx(args, info.seed);
        tx.order_status();
        LOG_DEBUG_FMT("order status - end");

        set_no_content_status(args);
      };

      auto do_delivery = [this](auto& args) {
        LOG_DEBUG_FMT("delivery");
        const auto& body = args.rpc_ctx->get_request_body();
        auto info = tpcc::TxInfo::deserialize(body.data(), body.size());
        tpcc::TpccTransactions tx(args, info.seed);
        tx.delivery();
        LOG_DEBUG_FMT("delivery - end");

        set_no_content_status(args);
      };

      auto do_payment = [this](auto& args) {
        LOG_DEBUG_FMT("payment");
        const auto& body = args.rpc_ctx->get_request_body();
        auto info = tpcc::TxInfo::deserialize(body.data(), body.size());
        tpcc::TpccTransactions tx(args, info.seed);
        tx.payment();
        LOG_DEBUG_FMT("payment - end");

        set_no_content_status(args);
      };

      auto do_new_order = [this](auto& args) {
        LOG_DEBUG_FMT("new order");
        const auto& body = args.rpc_ctx->get_request_body();
        auto info = tpcc::TxInfo::deserialize(body.data(), body.size());
        tpcc::TpccTransactions tx(args, info.seed);
        tx.new_order();
        LOG_DEBUG_FMT("new order - end");

        set_no_content_status(args);
      };

      auto do_test = [this](auto& args) {
        const auto& body = args.rpc_ctx->get_request_body();
        auto test_struct = tpcc::TestStruct::deserialize(body.data(), body.size());

        tpcc::TestStructResponse response;
        response.int_val = test_struct.int_val * 2;

        set_ok_status(args);
//        args.rpc_ctx->set_response_body(response.serialize());
      };

      auto get_district = [this](auto& args) {
        const auto& body = args.rpc_ctx->get_request_body();
        auto request = tpcc::DistrictRequest::deserialize(body.data(), body.size());

        tpcc::DistrictResponse response;

        District::Key key = {request.id, request.w_id};
        auto districts_table = args.tx.ro(tpcc::TpccTables::districts);
        auto districts = districts_table->get(key);

        if (!districts.has_value())
        {
          throw std::logic_error("district does not exist");
        }
        auto district = districts.value();
        response.id = district.id;
        response.w_id = district.w_id;
        response.tax = district.tax;
        response.ytd = district.ytd;
        response.next_o_id = district.next_o_id;
        response.name = district.name;
        response.street_1 = district.street_1;
        response.street_2 = district.street_2;
        response.city = district.city;
        response.state = district.state;
        response.zip = district.zip;

        set_ok_status(args);
        args.rpc_ctx->set_response_body(response.serialize());
      };

      auto get_order_line = [this](auto& args) {
        const auto& body = args.rpc_ctx->get_request_body();
        auto request = tpcc::OrderLineRequest::deserialize(body.data(), body.size());

        tpcc::OrderLineResponse response;
        // TODO fetch it from local store

        set_ok_status(args);
        args.rpc_ctx->set_response_body(response.serialize());
      };

      auto get_item = [this](auto& args) {
        const auto& body = args.rpc_ctx->get_request_body();
        auto request = tpcc::ItemRequest::deserialize(body.data(), body.size());

        tpcc::ItemResponse response;
        // TODO fetch it from local store

        set_ok_status(args);
        args.rpc_ctx->set_response_body(response.serialize());
      };

      auto get_stock = [this](auto& args) {
        const auto& body = args.rpc_ctx->get_request_body();
        auto request = tpcc::StockRequest::deserialize(body.data(), body.size());

        tpcc::StockResponse response;
        // TODO fetch it from local store

        set_ok_status(args);
        args.rpc_ctx->set_response_body(response.serialize());
      };

      auto get_warehouse = [this](auto& args) {
        const auto& body = args.rpc_ctx->get_request_body();
        auto request = tpcc::WarehouseRequest::deserialize(body.data(), body.size());

        tpcc::WarehouseResponse response;
        // TODO fetch it from local store

        set_ok_status(args);
        args.rpc_ctx->set_response_body(response.serialize());
      };

      auto get_customer = [this](auto& args) {
        const auto& body = args.rpc_ctx->get_request_body();
        auto request = tpcc::CustomerRequest::deserialize(body.data(), body.size());

        tpcc::CustomerResponse response;
        // TODO fetch it from local store

        set_ok_status(args);
        args.rpc_ctx->set_response_body(response.serialize());
      };

      auto get_customer_by_name = [this](auto& args) {
        const auto& body = args.rpc_ctx->get_request_body();
        auto request = tpcc::CustomerByNameRequest::deserialize(body.data(), body.size());

        tpcc::CustomerResponse response;
        // TODO fetch it from local store

        set_ok_status(args);
        args.rpc_ctx->set_response_body(response.serialize());
      };

      auto get_order = [this](auto& args) {
        const auto& body = args.rpc_ctx->get_request_body();
        auto request = tpcc::OrderRequest::deserialize(body.data(), body.size());

        tpcc::OrderResponse response;
        // TODO fetch it from local store

        set_ok_status(args);
        args.rpc_ctx->set_response_body(response.serialize());
      };

      auto get_last_order_by_customer = [this](auto& args) {
        const auto& body = args.rpc_ctx->get_request_body();
        auto request = tpcc::LastOrderRequest::deserialize(body.data(), body.size());

        tpcc::OrderResponse response;
        // TODO fetch it from local store

        set_ok_status(args);
        args.rpc_ctx->set_response_body(response.serialize());
      };

      auto get_last_new_order = [this](auto& args) {
        const auto& body = args.rpc_ctx->get_request_body();
        auto request = tpcc::LastNewOrderRequest::deserialize(body.data(), body.size());

        tpcc::NewOrderResponse response;
        // TODO fetch it from local store

        set_ok_status(args);
        args.rpc_ctx->set_response_body(response.serialize());
      };

      const ccf::AuthnPolicies user_sig_or_cert = {user_signature_auth_policy,
                                                   user_cert_auth_policy};

      std::vector<ccf::RESTVerb> verbs = {HTTP_POST, ws::Verb::WEBSOCKET};
      for (auto verb : verbs)
      {
        make_endpoint("tpcc_create", verb, create, user_sig_or_cert).install();
        make_endpoint("stock_level", verb, do_stock_level, user_sig_or_cert)
          .install();
        make_endpoint("order_status", verb, do_order_status, user_sig_or_cert)
          .install();
        make_endpoint("delivery", verb, do_delivery, user_sig_or_cert)
          .install();
        make_endpoint("payment", verb, do_payment, user_sig_or_cert).install();
        make_endpoint("new_order", verb, do_new_order, user_sig_or_cert)
          .install();
        make_endpoint("test", verb, do_test, user_sig_or_cert)
          .install();
        make_endpoint("get_district", verb, get_district, user_sig_or_cert)
          .install();
        make_endpoint("get_order_line", verb, get_order_line, user_sig_or_cert)
          .install();
        make_endpoint("get_item", verb, get_item, user_sig_or_cert)
          .install();
        make_endpoint("get_stock", verb, get_stock, user_sig_or_cert)
          .install();
        make_endpoint("get_warehouse", verb, get_warehouse, user_sig_or_cert)
          .install();
        make_endpoint("get_customer", verb, get_customer, user_sig_or_cert)
          .install();
        make_endpoint("get_customer_by_name", verb, get_customer_by_name, user_sig_or_cert)
          .install();
        make_endpoint("get_order", verb, get_order, user_sig_or_cert)
          .install();
        make_endpoint("get_last_order_by_customer", verb, get_last_order_by_customer, user_sig_or_cert)
          .install();
        make_endpoint("get_last_new_order", verb, get_last_new_order, user_sig_or_cert)
          .install();
      }

      metrics_tracker.install_endpoint(*this);
    }

    void tick(std::chrono::milliseconds elapsed, size_t tx_count) override
    {
      metrics_tracker.tick(elapsed, tx_count);

      ccf::UserEndpointRegistry::tick(elapsed, tx_count);
    }
  };

  class Tpcc : public ccf::RpcFrontend
  {
  private:
    TpccHandlers tpcc_handlers;

  public:
    Tpcc(kv::Store& store, AbstractNodeContext& context) :
      RpcFrontend(store, tpcc_handlers),
      tpcc_handlers(context)
    {}
  };

  std::shared_ptr<ccf::RpcFrontend> get_rpc_handler(
    NetworkTables& nwt, AbstractNodeContext& context)
  {
    return make_shared<Tpcc>(*nwt.tables, context);
  }
}

tpcc::TpccMap<tpcc::Stock::Key, tpcc::Stock> tpcc::TpccTables::stocks("stocks");
tpcc::TpccMap<tpcc::Warehouse::Key, tpcc::Warehouse> tpcc::TpccTables::
  warehouses("warehouses");
tpcc::TpccMap<tpcc::District::Key, tpcc::District> tpcc::TpccTables::districts(
  "districts");
tpcc::TpccMap<tpcc::History::Key, tpcc::History> tpcc::TpccTables::histories(
  "histories");
std::unordered_map<uint64_t, tpcc::TpccMap<tpcc::Customer::Key, tpcc::Customer>>
  tpcc::TpccTables::customers;
std::unordered_map<uint64_t, tpcc::TpccMap<tpcc::Order::Key, tpcc::Order>>
  tpcc::TpccTables::orders;
tpcc::TpccMap<tpcc::OrderLine::Key, tpcc::OrderLine> tpcc::TpccTables::
  order_lines("order_lines");
std::unordered_map<uint64_t, tpcc::TpccMap<tpcc::NewOrder::Key, tpcc::NewOrder>>
  tpcc::TpccTables::new_orders;
tpcc::TpccMap<tpcc::Item::Key, tpcc::Item> tpcc::TpccTables::items("items");