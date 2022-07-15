#pragma once

//#include "ds/serialized.h"
#include "../app/tpcc_global.h"
#include "../tpcc_tables.h"
#include "../tpcc_serializer.h"
#include <map>
#include <set>
#include <exception>
#include <iostream>
#include <stdexcept>

namespace tpcc
{
  struct OrderFullKey {
    tpcc::TpccTables::DistributeKey table_key;
    tpcc::Order::Key key;

    bool operator==(const OrderFullKey &o) const {
      return table_key.k == o.table_key.k && key.id == o.key.id;
    }

    bool operator<(const OrderFullKey &o) const {
      return table_key.k < o.table_key.k || (table_key.k == o.table_key.k && key.id < o.key.id);
    }
  };

  struct NewOrderFullKey {
    tpcc::TpccTables::DistributeKey table_key;
    tpcc::NewOrder::Key key;

    bool operator==(const NewOrderFullKey &o) const {
      return table_key.k == o.table_key.k && key.o_id == o.key.o_id;
    }

    bool operator<(const NewOrderFullKey &o) const {
      return table_key.k < o.table_key.k || (table_key.k == o.table_key.k && key.o_id < o.key.o_id);
    }
  };

  struct KeysDeleted {
    std::set<tpcc::NewOrder::Key> new_order_keys;
  };

  struct WriteSet {
    std::map<OrderFullKey, tpcc::Order> orders;
    std::map<NewOrderFullKey, tpcc::NewOrder> new_orders;
    std::map<tpcc::OrderLine::Key, tpcc::OrderLine> order_lines;
    std::map<tpcc::History::Key, tpcc::History> histories;
  };

  class ClientReadWriter : public ReadWriter {
    std::shared_ptr<RpcTlsClient> connection;

  public:
    WriteSet write_set;
    KeysDeleted keys_deleted;

    ClientReadWriter(std::shared_ptr<RpcTlsClient> conn) : connection(conn)
    {
      std::map<OrderFullKey, tpcc::Order> orders;
      std::map<NewOrderFullKey::Key, tpcc::NewOrder> new_orders;
      std::map<tpcc::OrderLine::Key, tpcc::OrderLine> order_lines;
      std::map<tpcc::History::Key, tpcc::History> histories;

      write_set = {
        orders,
        new_orders,
        order_lines,
        histories
      };

      std::set<tpcc::NewOrder::Key> new_order_keys;
      keys_deleted = {new_order_keys};
    }

    std::optional<District> get_district(tpcc::District::Key key) override {
      const auto body = key.serialize();
      const auto response =
        connection->call("get_district", CBuffer{body.data(), body.size()});
//      check_response(response);

      if (http::status_success(response.status) && response.body.size() > 0)
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
//      check_response(response);

      if (http::status_success(response.status) && response.body.size() > 0)
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
//      check_response(response);

      if (http::status_success(response.status) && response.body.size() > 0)
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
//      check_response(response);

      if (http::status_success(response.status) && response.body.size() > 0)
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
//      check_response(response);

      if (http::status_success(response.status) && response.body.size() > 0)
      {
        return tpcc::Warehouse::deserialize(response.body.data(), response.body.size());
      }
      else
      {
        return std::nullopt;
      }
    }

    std::optional<Customer> get_customer(TpccTables::DistributeKey table_key, Customer::Key key) override {
      tpcc::CustomerRequest customer_request;
      customer_request.id = key.id;
      customer_request.w_id = table_key.v.w_id;
      customer_request.d_id = table_key.v.d_id;

      const auto body = customer_request.serialize();
      const auto response =
        connection->call("get_customer", CBuffer{body.data(), body.size()});
      //      check_response(response);

      if (http::status_success(response.status) && response.body.size() > 0)
      {
        return tpcc::Customer::deserialize(response.body.data(), response.body.size());
      }
      else
      {
        return std::nullopt;
      }
    };
    std::optional<Customer> get_customer_by_name(TpccTables::DistributeKey table_key, std::string last_name) override {
      tpcc::CustomerByNameRequest customer_request;
      customer_request.last_name = last_name;
      customer_request.w_id = table_key.v.w_id;
      customer_request.d_id = table_key.v.d_id;

      const auto body = customer_request.serialize();
      const auto response =
        connection->call("get_customer_by_name", CBuffer{body.data(), body.size()});
      //      check_response(response);

      if (http::status_success(response.status) && response.body.size() > 0)
      {
        return tpcc::Customer::deserialize(response.body.data(), response.body.size());
      }
      else
      {
        return std::nullopt;
      }
    };
    std::optional<Order> get_order(TpccTables::DistributeKey table_key, Order::Key key) override {
      tpcc::OrderRequest order_request;
      order_request.id = key.id;
      order_request.w_id = table_key.v.w_id;
      order_request.d_id = table_key.v.d_id;

      const auto body = order_request.serialize();
      const auto response =
        connection->call("get_order", CBuffer{body.data(), body.size()});
      //      check_response(response);

      if (http::status_success(response.status) && response.body.size() > 0)
      {
        return tpcc::Order::deserialize(response.body.data(), response.body.size());
      }
      else
      {
        return std::nullopt;
      }
    };

    std::optional<Order> get_last_order_by_customer(TpccTables::DistributeKey table_key, Customer::Key key) override {
      tpcc::LastOrderRequest order_request;
      order_request.id = key.id;
      order_request.w_id = table_key.v.w_id;
      order_request.d_id = table_key.v.d_id;

      const auto body = order_request.serialize();
      const auto response =
        connection->call("get_last_order_by_customer", CBuffer{body.data(), body.size()});
      //      check_response(response);

      if (http::status_success(response.status) && response.body.size() > 0)
      {
        return tpcc::Order::deserialize(response.body.data(), response.body.size());
      }
      else
      {
        return std::nullopt;
      }
    };
    std::optional<NewOrder> get_last_new_order(TpccTables::DistributeKey table_key) override {
      tpcc::LastNewOrderRequest new_order_request;
      new_order_request.w_id = table_key.v.w_id;
      new_order_request.d_id = table_key.v.d_id;

      const auto body = new_order_request.serialize();
      const auto response =
        connection->call("get_last_new_order", CBuffer{body.data(), body.size()});
      //      check_response(response);

      if (http::status_success(response.status) && response.body.size() > 0)
      {
        return tpcc::NewOrder::deserialize(response.body.data(), response.body.size());
      }
      else
      {
        return std::nullopt;
      }
    };

    void put_order(TpccTables::DistributeKey table_key, Order::Key key, Order order) override {
      auto order_full_key = { table_key, key };
      write_set.orders[order_full_key] = order;
    };
    void put_new_order(TpccTables::DistributeKey table_key, NewOrder::Key key, NewOrder new_order) override {
      auto new_order_full_key = { table_key, key };
      write_set.new_orders[new_order_full_key] = new_order;
    };
    void put_order_line(OrderLine::Key key, OrderLine order_line) override {
      write_set.order_lines[key] = order_line;
    };
    void put_history(History::Key key, History history) override {
      write_set.histories[key] = history;
    };

    void remove_new_order(NewOrder::Key key) override {
      keys_deleted.insert(key);
    };

//  private:
//    bool check_response(const RpcTlsClient::Response& r)
//    {
//      if (!http::status_success(r.status))
//      {
//        const std::string error_msg(r.body.begin(), r.body.end());
//        throw logic_error(error_msg);
//        return false;
//      }
//
//      return true;
//    }
  };
}
