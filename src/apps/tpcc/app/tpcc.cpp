// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the Apache 2.0 License.
#include "../tpcc_serializer.h"
#include "../tpcc_tables.h"
#include "apps/utils/metrics_tracker.h"
#include "ccf/app_interface.h"
#include "ccf/user_frontend.h"
#include "tpcc_global.h"
#include "tpcc_setup.h"
#include "tpcc_transactions.h"

#include <charconv>
#include <string.h>
#include <vector>

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
        auto key = tpcc::District::Key::deserialize(body.data(), body.size());

        auto districts_table = args.tx.ro(tpcc::TpccTables::districts);
        auto optional_district = districts_table->get(key);

        if (true)
        {
          set_error_status(args, HTTP_STATUS_BAD_REQUEST, "district does not exist");
        }
        else
        {
          auto response = optional_district.value();

          set_ok_status(args);
          args.rpc_ctx->set_response_body(response.serialize());
        }
      };

      auto get_order_line = [this](auto& args) {
        const auto& body = args.rpc_ctx->get_request_body();
        auto key = tpcc::OrderLine::Key::deserialize(body.data(), body.size());

        auto order_lines_table = args.tx.ro(tpcc::TpccTables::order_lines);
        auto optional_order_line = order_lines_table->get(key);

        if (!optional_order_line.has_value())
        {
          throw std::logic_error("order_line does not exist");
        }

        auto response = optional_order_line.value();

        set_ok_status(args);
        args.rpc_ctx->set_response_body(response.serialize());
      };

      auto get_item = [this](auto& args) {
        const auto& body = args.rpc_ctx->get_request_body();
        auto key = tpcc::Item::Key::deserialize(body.data(), body.size());

        auto items_table = args.tx.ro(tpcc::TpccTables::items);
        auto optional_item = items_table->get(key);

        if (!optional_item.has_value())
        {
          throw std::logic_error("item does not exist");
        }

        auto response = optional_item.value();

        set_ok_status(args);
        args.rpc_ctx->set_response_body(response.serialize());
      };

      auto get_stock = [this](auto& args) {
        const auto& body = args.rpc_ctx->get_request_body();
        auto key = tpcc::Stock::Key::deserialize(body.data(), body.size());

        auto stocks_table = args.tx.ro(tpcc::TpccTables::stocks);
        auto optional_stock = stocks_table->get(key);

        if (!optional_stock.has_value())
        {
          throw std::logic_error("stock does not exist");
        }

        auto response = optional_stock.value();

        set_ok_status(args);
        args.rpc_ctx->set_response_body(response.serialize());
      };

      auto get_warehouse = [this](auto& args) {
        const auto& body = args.rpc_ctx->get_request_body();
        auto key = tpcc::Warehouse::Key::deserialize(body.data(), body.size());

        auto warehouses_table = args.tx.ro(tpcc::TpccTables::warehouses);
        auto optional_warehouse = warehouses_table->get(key);

        if (!optional_warehouse.has_value())
        {
          throw std::logic_error("warehouse does not exist");
        }

        auto response = optional_warehouse.value();

        set_ok_status(args);
        args.rpc_ctx->set_response_body(response.serialize());
      };

      auto get_customer = [this](auto& args) {
        const auto& body = args.rpc_ctx->get_request_body();
        auto request = tpcc::CustomerRequest::deserialize(body.data(), body.size());

        tpcc::TpccTables::DistributeKey table_key;
        table_key.v.w_id = request.w_id;
        table_key.v.d_id = request.d_id;
        auto it = tpcc::TpccTables::customers.find(table_key.k);
        tpcc::Customer::Key key = {request.id};
        auto customers_table = args.tx.ro(it->second);
        auto optional_customer = customers_table->get(key);

        if (!optional_customer.has_value())
        {
          throw std::logic_error("customer does not exist");
        }
        auto response = optional_customer.value();

        set_ok_status(args);
        args.rpc_ctx->set_response_body(response.serialize());
      };

      auto get_customer_by_name = [this](auto& args) {
        const auto& body = args.rpc_ctx->get_request_body();
        auto request = tpcc::CustomerByNameRequest::deserialize(body.data(), body.size());

        tpcc::Customer response;
        tpcc::TpccTables::DistributeKey table_key;
        table_key.v.w_id = request.w_id;
        table_key.v.d_id = request.d_id;
        auto it = tpcc::TpccTables::customers.find(table_key.k);
        auto customers_table = args.tx.ro(it->second);
        const char *c_last = request.last_name.c_str();
        customers_table->foreach([&](const tpcc::Customer::Key&, const tpcc::Customer& c) {
          if (strcmp(c.last.data(), c_last) == 0)
          {
            response = c;
            return false;
          }
          return true;
        });

        set_ok_status(args);
        args.rpc_ctx->set_response_body(response.serialize());
      };

      auto get_order = [this](auto& args) {
        const auto& body = args.rpc_ctx->get_request_body();
        auto request = tpcc::OrderRequest::deserialize(body.data(), body.size());

        tpcc::TpccTables::DistributeKey table_key;
        table_key.v.w_id = request.w_id;
        table_key.v.d_id = request.d_id;

        auto it = tpcc::TpccTables::orders.find(table_key.k);
        auto orders_table = args.tx.ro(it->second);
        tpcc::Order::Key key = {request.id};
        auto optional_order = orders_table->get(key);
        if (!optional_order.has_value())
        {
          throw std::logic_error("order does not exist");
        }

        auto response = optional_order.value();

        set_ok_status(args);
        args.rpc_ctx->set_response_body(response.serialize());
      };

      auto get_last_order_by_customer = [this](auto& args) {
        const auto& body = args.rpc_ctx->get_request_body();
        auto request = tpcc::LastOrderRequest::deserialize(body.data(), body.size());

        tpcc::Order response;
        tpcc::TpccTables::DistributeKey table_key;
        table_key.v.w_id = request.w_id;
        table_key.v.d_id = request.d_id;
        auto it = tpcc::TpccTables::orders.find(table_key.k);

        auto orders_table = args.tx.ro(it->second);
        orders_table->foreach([&](const tpcc::Order::Key&, const tpcc::Order& o) {
          if (o.c_id == request.id)
          {
            response = o;
            return false;
          }
          return true;
        });

        set_ok_status(args);
        args.rpc_ctx->set_response_body(response.serialize());
      };

      auto get_last_new_order = [this](auto& args) {
        const auto& body = args.rpc_ctx->get_request_body();
        auto request = tpcc::LastNewOrderRequest::deserialize(body.data(), body.size());

        tpcc::NewOrder response;

        tpcc::TpccTables::DistributeKey table_key;
        table_key.v.w_id = request.w_id;
        table_key.v.d_id = request.d_id;
        auto it = tpcc::TpccTables::new_orders.find(table_key.k);

        auto new_orders_table = args.tx.rw(it->second);
        tpcc::NewOrder::Key new_order_key = {request.w_id, request.d_id, 1};
        int32_t o_id;
        new_orders_table->foreach(
          [&](const tpcc::NewOrder::Key& k, const tpcc::NewOrder& no) {
            new_order_key = k;
            o_id = no.o_id;
            return false;
          });

        response.w_id = request.w_id;
        response.d_id = request.d_id;
        response.o_id = o_id;

        set_ok_status(args);
        args.rpc_ctx->set_response_body(response.serialize());
      };

      auto do_test_vector = [this](auto& args) {
        const auto& body = args.rpc_ctx->get_request_body();
        auto test_struct = tpcc::TestStruct::deserialize(body.data(), body.size());

        std::vector<int> ints;
        ints.push_back(123);
        ints.push_back(456);

        tpcc::TestVectorStruct test_vector_struct;
        test_vector_struct.num_ints = 0;
        test_vector_struct.ints = ints;

        set_ok_status(args);
        args.rpc_ctx->set_response_body(test_vector_struct.serialize());
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
        make_endpoint("do_test_vector", verb, do_test_vector, user_sig_or_cert)
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