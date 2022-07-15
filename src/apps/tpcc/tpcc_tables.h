// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the Apache 2.0 License.
#pragma once

#include "serialise_msgpack.h"

#include <cstring>
#include <msgpack/msgpack.hpp>
#include <nlohmann/json.hpp>
#include <stdint.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace tpcc
{
  namespace Address
  {
    static const int MIN_STREET = 10;
    static const int MAX_STREET = 20;
    static const int MIN_CITY = 10;
    static const int MAX_CITY = 20;
    static const int STATE = 2;
    static const int ZIP = 9;
  };

  struct Item
  {
    static const int MIN_IM = 1;
    static const int MAX_IM = 10000;
    static constexpr float MIN_PRICE = 1.00;
    static constexpr float MAX_PRICE = 100.00;
    static const int MIN_NAME = 14;
    static const int MAX_NAME = 24;
    static const int MIN_DATA = 26;
    static const int MAX_DATA = 50;
    static const int NUM_ITEMS = 100000;

    struct Key
    {
      int32_t id;

      std::vector<uint8_t> serialize() const
      {
        auto size = sizeof(id);
        std::vector<uint8_t> v(size);
        auto data = v.data();
        serialized::write(data, size, id);
        return v;
      }

      static Item::Key deserialize(const uint8_t* data, size_t size)
      {
        Item::Key key;
        key.id = serialized::read<decltype(id)>(data, size);
        return key;
      }

      MSGPACK_DEFINE(id);
    };

    Key get_key()
    {
      return {id};
    }

    int32_t id;
    int32_t im_id;
    float price;
    std::array<char, MAX_NAME + 1> name;
    std::array<char, MAX_DATA + 1> data;

    std::vector<uint8_t> serialize() const
    {
      auto size = sizeof(id) + sizeof(im_id) + sizeof(price) + sizeof(name) + sizeof(data);
      std::vector<uint8_t> v(size);
      auto data = v.data();
      serialized::write(data, size, id);
      serialized::write(data, size, im_id);
      serialized::write(data, size, price);
      serialized::write(data, size, name);
      serialized::write(data, size, data);
      return v;
    }

    static Item deserialize(const uint8_t* buffer, size_t size)
    {
      Item item;
      item.id = serialized::read<decltype(id)>(buffer, size);
      item.im_id = serialized::read<decltype(im_id)>(buffer, size);
      item.price = serialized::read<decltype(price)>(buffer, size);
      item.name = serialized::read<decltype(name)>(buffer, size);
      item.data = serialized::read<decltype(data)>(buffer, size);

      return item;
    }

    MSGPACK_DEFINE(id, im_id, price, name, data);
  };
  DECLARE_JSON_TYPE(Item::Key);
  DECLARE_JSON_REQUIRED_FIELDS(Item::Key, id);
  DECLARE_JSON_TYPE(Item);
  DECLARE_JSON_REQUIRED_FIELDS(Item, id, im_id, price, name, data);

  struct Warehouse
  {
    static constexpr float MIN_TAX = 0;
    static constexpr float MAX_TAX = 0.2000f;
    static constexpr float INITIAL_YTD = 300000.00f;
    static const int MIN_NAME = 6;
    static const int MAX_NAME = 10;
    // TPC-C 1.3.1 (page 11) requires 2*W. This permits testing up to 50
    // warehouses. This is an arbitrary limit created to pack ids into integers.
    static const int MAX_WAREHOUSE_ID = 100;
    struct Key
    {
      int32_t id;

      std::vector<uint8_t> serialize() const
      {
        auto size = sizeof(id);
        std::vector<uint8_t> v(size);
        auto data = v.data();
        serialized::write(data, size, id);
        return v;
      }

      static Warehouse::Key deserialize(const uint8_t* data, size_t size)
      {
        Warehouse::Key key;
        key.id = serialized::read<decltype(id)>(data, size);
        return key;
      }

      MSGPACK_DEFINE(id);
    };

    Key get_key()
    {
      return {id};
    }

    int32_t id;
    float tax;
    float ytd;
    std::array<char, MAX_NAME + 1> name;
    std::array<char, Address::MAX_STREET + 1> street_1;
    std::array<char, Address::MAX_STREET + 1> street_2;
    std::array<char, Address::MAX_STREET + 1> city;
    std::array<char, Address::STATE + 1> state;
    std::array<char, Address::ZIP + 1> zip;

    std::vector<uint8_t> serialize() const
    {
      auto size = sizeof(id) + sizeof(tax) + sizeof(ytd) + sizeof(name) +
        sizeof(street_1) + sizeof(street_2) + sizeof(city) + sizeof(state) +
        sizeof(zip);
      std::vector<uint8_t> v(size);
      auto data = v.data();
      serialized::write(data, size, id);
      serialized::write(data, size, tax);
      serialized::write(data, size, ytd);
      serialized::write(data, size, name);
      serialized::write(data, size, street_1);
      serialized::write(data, size, street_2);
      serialized::write(data, size, city);
      serialized::write(data, size, state);
      serialized::write(data, size, zip);
      return v;
    }

    static Warehouse deserialize(const uint8_t* data, size_t size)
    {
      Warehouse warehouse;
      warehouse.id = serialized::read<decltype(id)>(data, size);
      warehouse.tax = serialized::read<decltype(tax)>(data, size);
      warehouse.ytd = serialized::read<decltype(ytd)>(data, size);
      warehouse.name = serialized::read<decltype(name)>(data, size);
      warehouse.street_1 = serialized::read<decltype(street_1)>(data, size);
      warehouse.street_2 = serialized::read<decltype(street_2)>(data, size);
      warehouse.city = serialized::read<decltype(city)>(data, size);
      warehouse.state = serialized::read<decltype(state)>(data, size);
      warehouse.zip = serialized::read<decltype(zip)>(data, size);
      return warehouse;
    }

    MSGPACK_DEFINE(id, tax, ytd, name, street_1, street_2, city, state, zip);
  };
  DECLARE_JSON_TYPE(Warehouse::Key);
  DECLARE_JSON_REQUIRED_FIELDS(Warehouse::Key, id);
  DECLARE_JSON_TYPE(Warehouse);
  DECLARE_JSON_REQUIRED_FIELDS(
    Warehouse, id, tax, ytd, name, street_1, street_2, city, state, zip);

  struct District
  {
    static constexpr float MIN_TAX = 0;
    static constexpr float MAX_TAX = 0.2000f;
    static constexpr float INITIAL_YTD = 30000.00;
    static const int INITIAL_NEXT_O_ID = 3001;
    static const int MIN_NAME = 6;
    static const int MAX_NAME = 10;
    static const int NUM_PER_WAREHOUSE = 10;

    struct Key
    {
      int32_t id;
      int32_t w_id;
      MSGPACK_DEFINE(id, w_id);

      std::vector<uint8_t> serialize() const
      {
        auto size = sizeof(id) + sizeof(w_id);
        std::vector<uint8_t> v(size);
        auto data = v.data();
        serialized::write(data, size, id);
        serialized::write(data, size, w_id);
        return v;
      }

      static District::Key deserialize(const uint8_t* data, size_t size)
      {
        District::Key key;
        key.id = serialized::read<decltype(id)>(data, size);
        key.w_id = serialized::read<decltype(w_id)>(data, size);
        return key;
      }
    };

    int32_t id;
    int32_t w_id;
    float tax;
    float ytd;
    int32_t next_o_id;
    std::array<char, MAX_NAME + 1> name;
    std::array<char, Address::MAX_STREET + 1> street_1;
    std::array<char, Address::MAX_STREET + 1> street_2;
    std::array<char, Address::MAX_CITY + 1> city;
    std::array<char, Address::STATE + 1> state;
    std::array<char, Address::ZIP + 1> zip;

    Key get_key()
    {
      return {id, w_id};
    }

    std::vector<uint8_t> serialize() const
    {
      auto size = sizeof(id) + sizeof(w_id) + sizeof(tax) + sizeof(ytd) + sizeof(next_o_id)
        + sizeof(name) + sizeof(street_1) + sizeof(street_2) + sizeof(city) + sizeof(state) + sizeof(zip);
      std::vector<uint8_t> v(size);
      auto data = v.data();
      serialized::write(data, size, id);
      serialized::write(data, size, w_id);
      serialized::write(data, size, tax);
      serialized::write(data, size, ytd);
      serialized::write(data, size, next_o_id);
      serialized::write(data, size, name);
      serialized::write(data, size, street_1);
      serialized::write(data, size, street_2);
      serialized::write(data, size, city);
      serialized::write(data, size, state);
      serialized::write(data, size, zip);
      return v;
    }

    static District deserialize(const uint8_t* data, size_t size)
    {
      District district;
      district.id = serialized::read<decltype(id)>(data, size);
      district.w_id = serialized::read<decltype(w_id)>(data, size);
      district.tax = serialized::read<decltype(tax)>(data, size);
      district.ytd = serialized::read<decltype(ytd)>(data, size);
      district.next_o_id = serialized::read<decltype(next_o_id)>(data, size);
      district.name = serialized::read<decltype(name)>(data, size);
      district.street_1 = serialized::read<decltype(street_1)>(data, size);
      district.street_2 = serialized::read<decltype(street_2)>(data, size);
      district.city = serialized::read<decltype(city)>(data, size);
      district.state = serialized::read<decltype(state)>(data, size);
      district.zip = serialized::read<decltype(zip)>(data, size);
      return district;
    }

    MSGPACK_DEFINE(
      id,
      w_id,
      tax,
      ytd,
      next_o_id,
      name,
      street_1,
      street_2,
      city,
      state,
      zip);
  };
  DECLARE_JSON_TYPE(District::Key);
  DECLARE_JSON_REQUIRED_FIELDS(District::Key, id, w_id);
  DECLARE_JSON_TYPE(District);
  DECLARE_JSON_REQUIRED_FIELDS(
    District,
    id,
    w_id,
    tax,
    ytd,
    next_o_id,
    name,
    street_1,
    street_2,
    city,
    state,
    zip);

  struct Stock
  {
    static const int MIN_QUANTITY = 10;
    static const int MAX_QUANTITY = 100;
    static const int DIST = 24;
    static const int MIN_DATA = 26;
    static const int MAX_DATA = 50;
    static const int NUM_STOCK_PER_WAREHOUSE = 100000;

    int32_t i_id;
    int32_t w_id;
    int32_t quantity;
    int32_t ytd;
    int32_t order_cnt;
    int32_t remote_cnt;
    std::array<std::array<char, DIST + 1>, District::NUM_PER_WAREHOUSE> dist;
    std::array<char, MAX_DATA + 1> data;

    Stock() = default;

    struct Key
    {
      int32_t i_id;
      int32_t w_id;

      std::vector<uint8_t> serialize() const
      {
        auto size = sizeof(i_id) + sizeof(w_id);
        std::vector<uint8_t> v(size);
        auto data = v.data();
        serialized::write(data, size, i_id);
        serialized::write(data, size, w_id);
        return v;
      }

      static Stock::Key deserialize(const uint8_t* data, size_t size)
      {
        Stock::Key key;
        key.i_id = serialized::read<decltype(i_id)>(data, size);
        key.w_id = serialized::read<decltype(w_id)>(data, size);
        return key;
      }

      MSGPACK_DEFINE(i_id, w_id);
    };

    Key get_key()
    {
      return {i_id, w_id};
    }

    std::vector<uint8_t> serialize() const
    {
      auto size = sizeof(i_id) + sizeof(w_id) + sizeof(quantity) + sizeof(ytd) +
        sizeof(order_cnt) + sizeof(remote_cnt) + sizeof(dist) + sizeof(data);
      std::vector<uint8_t> v(size);
      auto data = v.data();
      serialized::write(data, size, i_id);
      serialized::write(data, size, w_id);
      serialized::write(data, size, quantity);
      serialized::write(data, size, ytd);
      serialized::write(data, size, order_cnt);
      serialized::write(data, size, remote_cnt);
      serialized::write(data, size, dist);
      serialized::write(data, size, data);
      return v;
    }

    static Stock deserialize(const uint8_t* buffer, size_t size)
    {
      Stock stock;
      stock.i_id = serialized::read<decltype(i_id)>(buffer, size);
      stock.w_id = serialized::read<decltype(w_id)>(buffer, size);
      stock.quantity = serialized::read<decltype(quantity)>(buffer, size);
      stock.ytd = serialized::read<decltype(ytd)>(buffer, size);
      stock.order_cnt = serialized::read<decltype(order_cnt)>(buffer, size);
      stock.remote_cnt = serialized::read<decltype(remote_cnt)>(buffer, size);
      stock.dist = serialized::read<decltype(dist)>(buffer, size);
      stock.data = serialized::read<decltype(data)>(buffer, size);

      return stock;
    }

    MSGPACK_DEFINE(
      i_id, w_id, quantity, ytd, order_cnt, remote_cnt, dist, data);
  };
  DECLARE_JSON_TYPE(Stock::Key);
  DECLARE_JSON_REQUIRED_FIELDS(Stock::Key, i_id, w_id);
  DECLARE_JSON_TYPE(Stock);
  DECLARE_JSON_REQUIRED_FIELDS(
    Stock, i_id, w_id, quantity, ytd, order_cnt, remote_cnt, dist, data);

  struct Customer
  {
    static constexpr float INITIAL_CREDIT_LIM = 50000.00;
    static constexpr float MIN_DISCOUNT = 0.0000;
    static constexpr float MAX_DISCOUNT = 0.5000;
    static constexpr float INITIAL_BALANCE = -10.00;
    static constexpr float INITIAL_YTD_PAYMENT = 10.00;
    static const int INITIAL_PAYMENT_CNT = 1;
    static const int INITIAL_DELIVERY_CNT = 0;
    static const int MIN_FIRST = 6;
    static const int MAX_FIRST = 10;
    static const int MIDDLE = 2;
    static const int MAX_LAST = 16;
    static const int PHONE = 16;
    static const int CREDIT = 2;
    static const int MIN_DATA = 300;
    static const int MAX_DATA = 500;
    static const int NUM_PER_DISTRICT = 3000;
    static constexpr char GOOD_CREDIT[] = "GC";
    static constexpr char BAD_CREDIT[] = "BC";

    struct Key
    {
      int32_t id;
      MSGPACK_DEFINE(id);
    };

    Key get_key()
    {
      return {id};
    }

    int32_t id;
    int32_t d_id;
    int32_t w_id;
    float credit_lim;
    float discount;
    float balance;
    float ytd_payment;
    int32_t payment_cnt;
    int32_t delivery_cnt;
    std::array<char, MAX_FIRST + 1> first;
    std::array<char, MIDDLE + 1> middle;
    std::array<char, MAX_LAST + 1> last;
    std::array<char, Address::MAX_STREET + 1> street_1;
    std::array<char, Address::MAX_STREET + 1> street_2;
    std::array<char, Address::MAX_CITY + 1> city;
    std::array<char, Address::STATE + 1> state;
    std::array<char, Address::ZIP + 1> zip;
    std::array<char, PHONE + 1> phone;
    std::array<char, DATETIME_SIZE + 1> since;
    std::array<char, CREDIT + 1> credit;
    std::array<char, MAX_DATA + 1> data;

    std::vector<uint8_t> serialize() const
    {
      auto size = sizeof(id) + sizeof(d_id) + sizeof(w_id) +
        sizeof(credit_lim) + sizeof(discount) + sizeof(balance) +
        sizeof(ytd_payment) + sizeof(payment_cnt) + sizeof(delivery_cnt) +
        sizeof(first) + sizeof(middle) + sizeof(last) + sizeof(street_1) +
        sizeof(street_2) + sizeof(city) + sizeof(state) + sizeof(zip) +
        sizeof(phone) + sizeof(since) + sizeof(credit) + sizeof(data);
      std::vector<uint8_t> v(size);
      auto data = v.data();
      serialized::write(data, size, id);
      serialized::write(data, size, d_id);
      serialized::write(data, size, w_id);
      serialized::write(data, size, credit_lim);
      serialized::write(data, size, discount);
      serialized::write(data, size, balance);
      serialized::write(data, size, ytd_payment);
      serialized::write(data, size, payment_cnt);
      serialized::write(data, size, delivery_cnt);
      serialized::write(data, size, first);
      serialized::write(data, size, middle);
      serialized::write(data, size, last);
      serialized::write(data, size, street_1);
      serialized::write(data, size, street_2);
      serialized::write(data, size, city);
      serialized::write(data, size, state);
      serialized::write(data, size, zip);
      serialized::write(data, size, phone);
      serialized::write(data, size, since);
      serialized::write(data, size, credit);
      serialized::write(data, size, data);
      return v;
    }

    static Customer deserialize(const uint8_t* buffer, size_t size)
    {
      Customer customer;
      customer.id = serialized::read<decltype(id)>(buffer, size);
      customer.d_id = serialized::read<decltype(d_id)>(buffer, size);
      customer.w_id = serialized::read<decltype(w_id)>(buffer, size);
      customer.credit_lim = serialized::read<decltype(credit_lim)>(buffer, size);
      customer.discount = serialized::read<decltype(discount)>(buffer, size);
      customer.balance = serialized::read<decltype(balance)>(buffer, size);
      customer.ytd_payment = serialized::read<decltype(ytd_payment)>(buffer, size);
      customer.payment_cnt = serialized::read<decltype(payment_cnt)>(buffer, size);
      customer.delivery_cnt = serialized::read<decltype(delivery_cnt)>(buffer, size);
      customer.first = serialized::read<decltype(first)>(buffer, size);
      customer.middle = serialized::read<decltype(middle)>(buffer, size);
      customer.last = serialized::read<decltype(last)>(buffer, size);
      customer.street_1 = serialized::read<decltype(street_1)>(buffer, size);
      customer.street_2 = serialized::read<decltype(street_2)>(buffer, size);
      customer.city = serialized::read<decltype(city)>(buffer, size);
      customer.state = serialized::read<decltype(state)>(buffer, size);
      customer.zip = serialized::read<decltype(zip)>(buffer, size);
      customer.phone = serialized::read<decltype(phone)>(buffer, size);
      customer.since = serialized::read<decltype(since)>(buffer, size);
      customer.credit = serialized::read<decltype(credit)>(buffer, size);
      customer.data = serialized::read<decltype(data)>(buffer, size);
      return customer;
    }

    MSGPACK_DEFINE(
      id,
      d_id,
      w_id,
      credit_lim,
      discount,
      balance,
      ytd_payment,
      payment_cnt,
      delivery_cnt,
      first,
      middle,
      last,
      street_1,
      street_2,
      city,
      state,
      zip,
      phone,
      since,
      credit,
      data);
  };
  DECLARE_JSON_TYPE(Customer::Key);
  DECLARE_JSON_REQUIRED_FIELDS(Customer::Key, id);
  DECLARE_JSON_TYPE(Customer);
  DECLARE_JSON_REQUIRED_FIELDS(
    Customer,
    id,
    d_id,
    w_id,
    credit_lim,
    discount,
    balance,
    ytd_payment,
    payment_cnt,
    delivery_cnt,
    first,
    middle,
    last,
    street_1,
    street_2,
    city,
    state,
    zip,
    phone,
    since,
    credit,
    data);

  struct Order
  {
    static const int MIN_CARRIER_ID = 1;
    static const int MAX_CARRIER_ID = 10;
    static const int NULL_CARRIER_ID = 0;
    static const int NULL_CARRIER_LOWER_BOUND = 2101;
    static const int MIN_OL_CNT = 5;
    static const int MAX_OL_CNT = 15;
    static const int INITIAL_ALL_LOCAL = 1;
    static const int INITIAL_ORDERS_PER_DISTRICT = 3000;
    // See TPC-C 1.3.1 (page 15)
    static const int MAX_ORDER_ID = 10000000;

    struct Key
    {
      int32_t id;
      MSGPACK_DEFINE(id);
    };

    Key get_key()
    {
      return {id};
    }

    static size_t get_size() {
      return sizeof(id) + sizeof(c_id) + sizeof(d_id) + sizeof(w_id) +
        sizeof(carrier_id) + sizeof(ol_cnt) + sizeof(all_local) + sizeof(entry_d);
    }

    int32_t id;
    int32_t c_id;
    int32_t d_id;
    int32_t w_id;
    int32_t carrier_id;
    int32_t ol_cnt;
    int32_t all_local;
    std::array<char, DATETIME_SIZE + 1> entry_d;

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
      serialized::write(data, size, id);
      serialized::write(data, size, c_id);
      serialized::write(data, size, d_id);
      serialized::write(data, size, w_id);
      serialized::write(data, size, carrier_id);
      serialized::write(data, size, ol_cnt);
      serialized::write(data, size, all_local);
      serialized::write(data, size, entry_d);
    }

    static Order deserialize(const uint8_t* data, size_t size)
    {
      Order order;
      order.id = serialized::read<decltype(id)>(data, size);
      order.c_id = serialized::read<decltype(c_id)>(data, size);
      order.d_id = serialized::read<decltype(d_id)>(data, size);
      order.w_id = serialized::read<decltype(w_id)>(data, size);
      order.carrier_id = serialized::read<decltype(carrier_id)>(data, size);
      order.ol_cnt = serialized::read<decltype(ol_cnt)>(data, size);
      order.all_local = serialized::read<decltype(all_local)>(data, size);
      order.entry_d = serialized::read<decltype(entry_d)>(data, size);
      return order;
    }

    MSGPACK_DEFINE(
      id, c_id, d_id, w_id, carrier_id, ol_cnt, all_local, entry_d);
  };
  DECLARE_JSON_TYPE(Order::Key);
  DECLARE_JSON_REQUIRED_FIELDS(Order::Key, id);
  DECLARE_JSON_TYPE(Order);
  DECLARE_JSON_REQUIRED_FIELDS(
    Order, id, c_id, d_id, w_id, carrier_id, ol_cnt, all_local, entry_d);

  struct OrderLine
  {
    static const int MIN_I_ID = 1;
    static const int MAX_I_ID = 100;
    static const int INITIAL_QUANTITY = 5;
    static constexpr float MIN_AMOUNT = 0.01f;
    static constexpr float MAX_AMOUNT = 9999.99f;
    static const int REMOTE_PROBABILITY_MILLIS = 10;

    struct Key
    {
      int32_t w_id;
      int32_t d_id;
      int32_t o_id;
      int32_t number;

      bool operator==(const Key &o) const {
        return o_id == o.o_id && d_id == o.d_id && w_id == o.w_id && number == o.number;
      }

      bool operator<(const Key &o) const {
        return w_id < o.w_id
          || (w_id == o.w_id && d_id < o.d_id)
          || (w_id == o.w_id && d_id == o.d_id && o_id < o.o_id)
          || (w_id == o.w_id && d_id == o.d_id && o_id == o.o_id && number < o.number);
      }

      static size_t get_size() {
        return sizeof(o_id) + sizeof(d_id) + sizeof(w_id) + sizeof(number);
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
        serialized::write(data, size, o_id);
        serialized::write(data, size, d_id);
        serialized::write(data, size, w_id);
        serialized::write(data, size, number);
      }

      static OrderLine::Key deserialize(const uint8_t* data, size_t size)
      {
        OrderLine::Key key;
        key.o_id = serialized::read<decltype(o_id)>(data, size);
        key.d_id = serialized::read<decltype(d_id)>(data, size);
        key.w_id = serialized::read<decltype(w_id)>(data, size);
        key.number = serialized::read<decltype(number)>(data, size);
        return key;
      }

      MSGPACK_DEFINE(o_id, d_id, w_id, number);
    };

    Key get_key()
    {
      return {o_id, d_id, w_id, number};
    }

    static size_t get_size() {
      return sizeof(o_id) + sizeof(d_id) + sizeof(w_id) + sizeof(number) +
        sizeof(i_id) + sizeof(supply_w_id) + sizeof(quantity) + sizeof(amount) +
        sizeof(delivery_d) + sizeof(dist_info);
    }

    int32_t o_id;
    int32_t d_id;
    int32_t w_id;
    int32_t number;
    int32_t i_id;
    int32_t supply_w_id;
    int32_t quantity;
    float amount;
    std::array<char, DATETIME_SIZE + 1> delivery_d;
    std::array<char, Stock::DIST + 1> dist_info;

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
      serialized::write(data, size, o_id);
      serialized::write(data, size, d_id);
      serialized::write(data, size, w_id);
      serialized::write(data, size, number);
      serialized::write(data, size, i_id);
      serialized::write(data, size, supply_w_id);
      serialized::write(data, size, quantity);
      serialized::write(data, size, amount);
      serialized::write(data, size, delivery_d);
      serialized::write(data, size, dist_info);
    }

    static OrderLine deserialize(const uint8_t* data, size_t size)
    {
      OrderLine order_line;
      order_line.o_id = serialized::read<decltype(o_id)>(data, size);
      order_line.d_id = serialized::read<decltype(d_id)>(data, size);
      order_line.w_id = serialized::read<decltype(w_id)>(data, size);
      order_line.number = serialized::read<decltype(number)>(data, size);
      order_line.i_id = serialized::read<decltype(i_id)>(data, size);
      order_line.supply_w_id = serialized::read<decltype(supply_w_id)>(data, size);
      order_line.quantity = serialized::read<decltype(quantity)>(data, size);
      order_line.delivery_d = serialized::read<decltype(delivery_d)>(data, size);
      order_line.dist_info = serialized::read<decltype(dist_info)>(data, size);

      return order_line;
    }

    MSGPACK_DEFINE(
      o_id,
      d_id,
      w_id,
      number,
      i_id,
      supply_w_id,
      quantity,
      amount,
      delivery_d,
      dist_info);
  };
  DECLARE_JSON_TYPE(OrderLine::Key);
  DECLARE_JSON_REQUIRED_FIELDS(OrderLine::Key, o_id, d_id, w_id, number);
  DECLARE_JSON_TYPE(OrderLine);
  DECLARE_JSON_REQUIRED_FIELDS(
    OrderLine,
    o_id,
    d_id,
    w_id,
    number,
    i_id,
    supply_w_id,
    quantity,
    amount,
    delivery_d,
    dist_info);

  struct NewOrder
  {
    static const int INITIAL_NUM_PER_DISTRICT = 900;

    struct Key
    {
      int32_t w_id;
      int32_t d_id;
      int32_t o_id;

      bool operator==(const Key &o) const {
        return o_id == o.o_id && d_id == o.d_id && w_id == o.w_id;
      }

      bool operator<(const Key &o) const {
        return w_id < o.w_id
          || (w_id == o.w_id && d_id < o.d_id)
          || (w_id == o.w_id && d_id == o.d_id && o_id < o.o_id);
      }

      MSGPACK_DEFINE(w_id, d_id, o_id);
    };

    Key get_key()
    {
      return {w_id, d_id, o_id};
    }

    int32_t w_id;
    int32_t d_id;
    int32_t o_id;

    static size_t get_size() {
      return sizeof(w_id) + sizeof(d_id) + sizeof(o_id);
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
      serialized::write(data, size, w_id);
      serialized::write(data, size, d_id);
      serialized::write(data, size, o_id);
    }

    static NewOrder deserialize(const uint8_t* data, size_t size)
    {
      NewOrder new_order;
      new_order.w_id = serialized::read<decltype(w_id)>(data, size);
      new_order.d_id = serialized::read<decltype(d_id)>(data, size);
      new_order.o_id = serialized::read<decltype(o_id)>(data, size);
      return new_order;
    }

    MSGPACK_DEFINE(w_id, d_id, o_id);
  };
  DECLARE_JSON_TYPE(NewOrder::Key);
  DECLARE_JSON_REQUIRED_FIELDS(NewOrder::Key, w_id, d_id, o_id);
  DECLARE_JSON_TYPE(NewOrder);
  DECLARE_JSON_REQUIRED_FIELDS(NewOrder, w_id, d_id, o_id);

  struct History
  {
    static const int MIN_DATA = 12;
    static const int MAX_DATA = 24;
    static constexpr float INITIAL_AMOUNT = 10.00f;

    struct Key
    {
      int32_t c_id;
      int32_t c_d_id;
      int32_t c_w_id;
      int32_t d_id;
      int32_t w_id;

      static size_t get_size() {
        return sizeof(c_id) + sizeof(c_d_id) + sizeof(c_w_id) + sizeof(d_id) + sizeof(w_id);
      }

      bool operator==(const Key &o) const {
        return c_id == o.c_id && d_id == o.d_id && w_id == o.w_id && c_d_id == o.c_d_id && c_w_id == o.c_w_id;
      }

      bool operator<(const Key &o) const {
        return c_id < o.c_id
          || (c_id == o.c_id && c_d_id < o.c_d_id)
          || (c_id == o.c_id && c_d_id == o.c_d_id && c_w_id < o.c_w_id)
          || (c_id == o.c_id && c_d_id == o.c_d_id && c_w_id == o.c_w_id && d_id < o.d_id)
          || (c_id == o.c_id && c_d_id == o.c_d_id && c_w_id == o.c_w_id && d_id == o.d_id && w_id < o.w_id);
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
        serialized::write(data, size, c_id);
        serialized::write(data, size, c_d_id);
        serialized::write(data, size, c_w_id);
        serialized::write(data, size, d_id);
        serialized::write(data, size, w_id);
      }

      static History::Key deserialize(const uint8_t* data, size_t size)
      {
        History::Key key;
        key.c_id = serialized::read<decltype(c_id)>(data, size);
        key.c_d_id = serialized::read<decltype(c_d_id)>(data, size);
        key.c_w_id = serialized::read<decltype(c_w_id)>(data, size);
        key.d_id = serialized::read<decltype(d_id)>(data, size);
        key.w_id = serialized::read<decltype(w_id)>(data, size);
        return key;
      }


      MSGPACK_DEFINE(c_id, c_d_id, c_w_id, d_id, w_id);
    };

    Key get_key()
    {
      return {c_id, c_d_id, c_w_id, d_id, w_id};
    }

    static size_t get_size() {
      return sizeof(c_id) + sizeof(c_d_id) + sizeof(c_w_id) + sizeof(d_id) + sizeof(w_id)
        + sizeof(amount) + sizeof(date) + sizeof(data);
    }

    int32_t c_id;
    int32_t c_d_id;
    int32_t c_w_id;
    int32_t d_id;
    int32_t w_id;
    float amount;
    std::array<char, DATETIME_SIZE + 1> date;
    std::array<char, MAX_DATA + 1> data;

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
      serialized::write(data, size, c_id);
      serialized::write(data, size, c_d_id);
      serialized::write(data, size, c_w_id);
      serialized::write(data, size, d_id);
      serialized::write(data, size, w_id);
      serialized::write(data, size, amount);
      serialized::write(data, size, date);
      serialized::write(data, size, data);
    }

    static History deserialize(const uint8_t* data, size_t size)
    {
      History history;
      history.c_id = serialized::read<decltype(c_id)>(data, size);
      history.c_d_id = serialized::read<decltype(c_d_id)>(data, size);
      history.c_w_id = serialized::read<decltype(c_w_id)>(data, size);
      history.d_id = serialized::read<decltype(d_id)>(data, size);
      history.w_id = serialized::read<decltype(w_id)>(data, size);
      history.amount = serialized::read<decltype(amount)>(data, size);
      history.date = serialized::read<decltype(date)>(data, size);
      history.data = serialized::read<decltype(data)>(data, size);
      return history;
    }

    MSGPACK_DEFINE(c_id, c_d_id, c_w_id, d_id, w_id, amount, date, data);
  };
  DECLARE_JSON_TYPE(History::Key);
  DECLARE_JSON_REQUIRED_FIELDS(History::Key, c_id, c_d_id, c_w_id, d_id, w_id);
  DECLARE_JSON_TYPE(History);
  DECLARE_JSON_REQUIRED_FIELDS(
    History, c_id, c_d_id, c_w_id, d_id, w_id, amount, date, data);

  // TPCC KV tables use msgpack as key and value serialisation format.
  // See serialise_msgpack.h for serialiser definition.
  template <typename K, typename V>
  using TpccMap = kv::MapSerialisedWith<K, V, MsgPackSerialiser>;

  struct TpccTables
  {
    union DistributeKey
    {
      struct
      {
        int32_t w_id;
        int32_t d_id;
      } v;
      uint64_t k;
    };
    static_assert(
      sizeof(DistributeKey) == sizeof(uint64_t),
      "Distribute key is the wrong size");

    static TpccMap<Stock::Key, Stock> stocks;
    static TpccMap<Warehouse::Key, Warehouse> warehouses;
    static TpccMap<District::Key, District> districts;
    static TpccMap<History::Key, History> histories;
    static std::unordered_map<uint64_t, TpccMap<Customer::Key, Customer>>
      customers;
    static std::unordered_map<uint64_t, TpccMap<Order::Key, Order>> orders;
    static TpccMap<OrderLine::Key, OrderLine> order_lines;
    static std::unordered_map<uint64_t, TpccMap<NewOrder::Key, NewOrder>>
      new_orders;
    static TpccMap<Item::Key, Item> items;
  };
}