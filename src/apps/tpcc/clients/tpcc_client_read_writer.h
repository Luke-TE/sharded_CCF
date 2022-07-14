#pragma once

//#include "ds/serialized.h"
#include "../app/tpcc_global.h"
#include "../tpcc_tables.h"
#include <stdexcept>

namespace tpcc
{
  class ClientReadWriter : public ReadWriter {
    std::shared_ptr<RpcTlsClient> connection;

  public:
    ClientReadWriter(std::shared_ptr<RpcTlsClient> conn) : connection(conn)
    {
    }

    std::optional<District> get_district(tpcc::District::Key key) override {
      const auto body = key.serialize();
      const auto response =
        connection->call("get_district", CBuffer{body.data(), body.size()});
      check_response(response);

      if (response.body.size() > 0)
      {
        return tpcc::District::deserialize(response.body.data(), response.body.size());
      }
      else
      {
        return std::nullopt;
      }
    }

    std::optional<OrderLine> get_order_line(tpcc::OrderLine::Key key) override {
      const auto body = key.serialize();
      const auto response =
        connection->call("get_order_line", CBuffer{body.data(), body.size()});
      check_response(response);

      if (response.body.size() > 0)
      {
        return tpcc::OrderLine::deserialize(response.body.data(), response.body.size());
      }
      else
      {
        return std::nullopt;
      }
    }

    std::optional<Item> get_item(Item::Key key) override {
      const auto body = key.serialize();
      const auto response =
        connection->call("get_item", CBuffer{body.data(), body.size()});
      check_response(response);

      if (response.body.size() > 0)
      {
        return tpcc::Item::deserialize(response.body.data(), response.body.size());
      }
      else
      {
        return std::nullopt;
      }
    }

    std::optional<Stock> get_stock(Stock::Key key) override {
      const auto body = key.serialize();
      const auto response =
        connection->call("get_stock", CBuffer{body.data(), body.size()});
      check_response(response);

      if (response.body.size() > 0)
      {
        return tpcc::Stock::deserialize(response.body.data(), response.body.size());
      }
      else
      {
        return std::nullopt;
      }
    }

    std::optional<Warehouse> get_warehouse(Warehouse::Key key) override {
      const auto body = key.serialize();
      const auto response =
        connection->call("get_warehouse", CBuffer{body.data(), body.size()});
      check_response(response);

      if (response.body.size() > 0)
      {
        return tpcc::Warehouse::deserialize(response.body.data(), response.body.size());
      }
      else
      {
        return std::nullopt;
      }
    }

    std::optional<Customer> get_customer(TpccTables::DistributeKey table_key, Customer::Key key) override {
      return std::nullopt;
    };
    std::optional<Customer> get_customer_by_name(TpccTables::DistributeKey table_key, std::string last_name) override {
      return std::nullopt;
    };
    std::optional<Order> get_order(TpccTables::DistributeKey table_key, Order::Key key) override {
      return std::nullopt;
    };

    std::optional<Order> get_last_order_by_customer(TpccTables::DistributeKey table_key, Customer::Key key) override {
      return std::nullopt;
    };
    std::optional<NewOrder> get_last_new_order(TpccTables::DistributeKey table_key, NewOrder::Key key) override {
      return std::nullopt;
    };

    void put_order(TpccTables::DistributeKey table_key, Order::Key key, Order order) override {};
    void put_new_order(TpccTables::DistributeKey table_key, NewOrder::Key key, NewOrder new_order) override {};
    void put_order_line(OrderLine::Key key, OrderLine order_line) override {};
    void put_history(History::Key key, History history) override {};
    void remove_new_order(NewOrder::Key key) override {};

  private:
    bool check_response(const RpcTlsClient::Response& r)
    {
      if (!http::status_success(r.status))
      {
        const std::string error_msg(r.body.begin(), r.body.end());
        throw logic_error(error_msg);
        return false;
      }

      return true;
    }
  };
}
