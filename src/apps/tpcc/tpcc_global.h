#pragma once

//#include "ds/serialized.h"
#include "app/tpcc_tables.h"

namespace tpcc
{
  class ReadWriter {
    ReadWriter(){}
    virtual std::optional<District> get_district(District::Key key) = 0;
    virtual std::optional<OrderLine> get_order_line(OrderLine::Key key) = 0;
    virtual std::optional<Item> get_item(Item::Key key) = 0;
    virtual std::optional<Stock> get_stock(Stock::Key key) = 0;
    virtual std::optional<Warehouse> get_warehouse(Warehouse::Key key) = 0;
    virtual std::optional<Customer> get_customer(TpccTables::DistributeKey table_key, Customer::Key key) = 0;
    virtual std::optional<Customer> get_customer_by_name(TpccTables::DistributeKey table_key, string last_name) = 0;
    virtual std::optional<Order> get_order(TpccTables::DistributeKey table_key, Order::Key key) = 0;

//    TODO how to enforce "last" order?
    virtual std::optional<Order> get_last_order_by_customer(TpccTables::DistributeKey table_key, Customer::Key key) = 0;
    virtual std::optional<NewOrder> get_last_new_order(TpccTables::DistributeKey table_key, NewOrder::Key key) = 0;

    virtual void put_order(TpccTables::DistributeKey table_key, Order::Key key, Order order) = 0;
    virtual void put_new_order(TpccTables::DistributeKey table_key, NewOrder::Key key, NewOrder new_order) = 0;
    virtual void put_order_line(OrderLine::Key key, OrderLine order_line) = 0;
    virtual void put_history(History::Key key, History history) = 0;
    virtual void remove_new_order(NewOrder::Key key) = 0;
  };
}
