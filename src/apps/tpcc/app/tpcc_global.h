#pragma once

//#include "ds/serialized.h"
#include "../tpcc_tables.h"
#include "tpcc_common.h"
#include "tpcc_output.h"

#include <cinttypes>
#include <string.h>
#include <vector>

namespace tpcc
{
  class ReadWriter {
  public:
    ReadWriter(){}
    virtual std::optional<District> get_district(District::Key key) = 0;
    virtual std::optional<OrderLine> get_order_line(OrderLine::Key key) = 0;
    virtual std::optional<Item> get_item(Item::Key key) = 0;
    virtual std::optional<Stock> get_stock(Stock::Key key) = 0;
    virtual std::optional<Warehouse> get_warehouse(Warehouse::Key key) = 0;

    virtual std::optional<Customer> get_customer(TpccTables::DistributeKey table_key, Customer::Key key) = 0;
    virtual std::optional<Customer> get_customer_by_name(TpccTables::DistributeKey table_key, std::string last_name) = 0;
    virtual std::optional<Order> get_order(TpccTables::DistributeKey table_key, Order::Key key) = 0;

    virtual std::optional<Order> get_last_order_by_customer(TpccTables::DistributeKey table_key, Customer::Key key) = 0;
    virtual std::optional<NewOrder> get_last_new_order(TpccTables::DistributeKey table_key) = 0;

    virtual void put_order(TpccTables::DistributeKey table_key, Order::Key key, Order order) = 0;
    virtual void put_new_order(TpccTables::DistributeKey table_key, NewOrder::Key key, NewOrder new_order) = 0;
    virtual void put_order_line(OrderLine::Key key, OrderLine order_line) = 0;
    virtual void put_history(History::Key key, History history) = 0;
    virtual void remove_new_order(NewOrder::Key key) = 0;
  };

  struct OrderFullKey {
    tpcc::TpccTables::DistributeKey table_key;
    tpcc::Order::Key key;

    bool operator==(const OrderFullKey &o) const {
      return table_key.k == o.table_key.k && key.id == o.key.id;
    }

    bool operator<(const OrderFullKey &o) const {
      return table_key.k < o.table_key.k || (table_key.k == o.table_key.k && key.id < o.key.id);
    }

    static size_t get_size() {
      return sizeof(table_key.k) + sizeof(key.id);
    }

    std::vector<uint8_t> serialize() const
    {
      auto size = get_size();
      std::vector<uint8_t> v(size);
      auto data = v.data();
      serialize_to_buffer(data, size);
      return v;
    }

    void serialize_to_buffer(uint8_t*& data, size_t& size) const
    {
      serialized::write(data, size, table_key.v.w_id);
      serialized::write(data, size, table_key.v.d_id);
      serialized::write(data, size, key.id);
    }

    static OrderFullKey deserialize(const uint8_t* data, size_t size)
    {
      OrderFullKey order_full_key;
      order_full_key.table_key.v.w_id = serialized::read<decltype(table_key.v.w_id)>(data, size);
      order_full_key.table_key.v.d_id = serialized::read<decltype(table_key.v.d_id)>(data, size);
      order_full_key.key.id = serialized::read<decltype(key.id)>(data, size);
      return order_full_key;
    }

  };

  struct KeysDeleted {
    std::set<tpcc::NewOrder::Key> new_order_keys;
  };

  struct UpdateSet {
    std::map<OrderFullKey, tpcc::Order> orders;
    std::map<OrderFullKey, tpcc::NewOrder> new_orders;
    std::map<tpcc::OrderLine::Key, tpcc::OrderLine> order_lines;
    std::map<tpcc::History::Key, tpcc::History> histories;
  };
}
