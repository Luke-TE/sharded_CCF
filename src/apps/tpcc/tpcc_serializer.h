// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the Apache 2.0 License.
#pragma once

#include "ds/serialized.h"
#include <cstring>
#include <stdint.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <array>
#include <string>
#include "app/tpcc_common.h"

namespace Address
{
  static const int MIN_STREET = 10;
  static const int MAX_STREET = 20;
  static const int MIN_CITY = 10;
  static const int MAX_CITY = 20;
  static const int STATE = 2;
  static const int ZIP = 9;
};

namespace tpcc
{
  struct DbCreation
  {
    int32_t new_orders_per_district;
    int32_t seed;

    std::vector<uint8_t> serialize() const
    {
      auto size = sizeof(new_orders_per_district) + sizeof(seed);
      std::vector<uint8_t> v(size);
      auto data = v.data();
      serialized::write(data, size, new_orders_per_district);
      serialized::write(data, size, seed);
      return v;
    }

    static DbCreation deserialize(const uint8_t* data, size_t size)
    {
      DbCreation db;
      db.new_orders_per_district =
        serialized::read<decltype(new_orders_per_district)>(data, size);
      db.seed = serialized::read<decltype(seed)>(data, size);
      return db;
    }
  };

  struct StockLevel
  {
    int32_t seed;
    int32_t warehouse_id;
    int32_t district_id;
    int32_t threshold;

    std::vector<uint8_t> serialize() const
    {
      auto size = sizeof(seed) + sizeof(warehouse_id) + sizeof(district_id) +
        sizeof(threshold);
      std::vector<uint8_t> v(size);
      auto data = v.data();
      serialized::write(data, size, seed);
      serialized::write(data, size, warehouse_id);
      serialized::write(data, size, district_id);
      serialized::write(data, size, threshold);
      return v;
    }

    static StockLevel deserialize(const uint8_t* data, size_t size)
    {
      StockLevel db;
      db.seed = serialized::read<decltype(seed)>(data, size);
      db.warehouse_id = serialized::read<decltype(warehouse_id)>(data, size);
      db.district_id = serialized::read<decltype(district_id)>(data, size);
      db.threshold = serialized::read<decltype(threshold)>(data, size);
      return db;
    }
  };

  struct TxInfo
  {
    int32_t seed;

    std::vector<uint8_t> serialize() const
    {
      auto size = sizeof(seed);
      std::vector<uint8_t> v(size);
      auto data = v.data();
      serialized::write(data, size, seed);
      return v;
    }

    static TxInfo deserialize(const uint8_t* data, size_t size)
    {
      TxInfo info;
      info.seed = serialized::read<decltype(seed)>(data, size);
      return info;
    }
  };

  struct TestStruct
  {
    int32_t int_val;

    std::vector<uint8_t> serialize() const
    {
      auto size = sizeof(int_val);
      std::vector<uint8_t> v(size);
      auto data = v.data();
      serialized::write(data, size, int_val);
      return v;
    }

    static TestStruct deserialize(const uint8_t* data, size_t size)
    {
      TestStruct test_struct;
      test_struct.int_val = serialized::read<decltype(int_val)>(data, size);
      return test_struct;
    }
  };

  struct TestStructResponse
  {
    int32_t int_val;

    std::vector<uint8_t> serialize() const
    {
      auto size = sizeof(int_val);
      std::vector<uint8_t> v(size);
      auto data = v.data();
      serialized::write(data, size, int_val);
      return v;
    }

    static TestStructResponse deserialize(const uint8_t* data, size_t size)
    {
      TestStructResponse response;
      response.int_val = serialized::read<decltype(int_val)>(data, size);
      return response;
    }
  };

  struct DistrictRequest
  {
    int32_t id;
    int32_t w_id;
  };

  struct OrderLineRequest
  {
    int32_t o_id;
    int32_t d_id;
    int32_t w_id;
    int32_t number;

    std::vector<uint8_t> serialize() const
    {
      auto size = sizeof(o_id) + sizeof(d_id) + sizeof(w_id) + sizeof(number);
      std::vector<uint8_t> v(size);
      auto data = v.data();
      serialized::write(data, size, o_id);
      serialized::write(data, size, d_id);
      serialized::write(data, size, w_id);
      serialized::write(data, size, number);
      return v;
    }

    static OrderLineRequest deserialize(const uint8_t* data, size_t size)
    {
      OrderLineRequest order_line_request;
      order_line_request.o_id = serialized::read<decltype(o_id)>(data, size);
      order_line_request.d_id = serialized::read<decltype(d_id)>(data, size);
      order_line_request.w_id = serialized::read<decltype(w_id)>(data, size);
      order_line_request.number = serialized::read<decltype(number)>(data, size);
      return order_line_request;
    }
  };

  struct ItemRequest
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

    static ItemRequest deserialize(const uint8_t* data, size_t size)
    {
      ItemRequest item_request;
      item_request.id = serialized::read<decltype(id)>(data, size);
      return item_request;
    }
  };

  struct StockRequest
  {
    int32_t id;
    int32_t w_id;

    std::vector<uint8_t> serialize() const
    {
      auto size = sizeof(id) + sizeof(w_id);
      std::vector<uint8_t> v(size);
      auto data = v.data();
      serialized::write(data, size, id);
      serialized::write(data, size, w_id);
      return v;
    }

    static StockRequest deserialize(const uint8_t* data, size_t size)
    {
      StockRequest stock_request;
      stock_request.id = serialized::read<decltype(id)>(data, size);
      stock_request.w_id = serialized::read<decltype(w_id)>(data, size);
      return stock_request;
    }
  };

  struct WarehouseRequest
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

    static WarehouseRequest deserialize(const uint8_t* data, size_t size)
    {
      WarehouseRequest warehouse_request;
      warehouse_request.id = serialized::read<decltype(id)>(data, size);
      return warehouse_request;
    }
  };

  struct CustomerRequest
  {
    int32_t id;
    int32_t w_id;
    int32_t d_id;

    std::vector<uint8_t> serialize() const
    {
      auto size = sizeof(id) + sizeof(w_id) + sizeof(d_id);
      std::vector<uint8_t> v(size);
      auto data = v.data();
      serialized::write(data, size, id);
      serialized::write(data, size, w_id);
      serialized::write(data, size, d_id);
      return v;
    }

    static CustomerRequest deserialize(const uint8_t* data, size_t size)
    {
      CustomerRequest customer_request;
      customer_request.id = serialized::read<decltype(id)>(data, size);
      customer_request.w_id = serialized::read<decltype(w_id)>(data, size);
      customer_request.d_id = serialized::read<decltype(d_id)>(data, size);
      return customer_request;
    }
  };

  struct CustomerByNameRequest
  {
    std::string last_name;
    int32_t w_id;
    int32_t d_id;

    std::vector<uint8_t> serialize() const
    {
      auto size = sizeof(last_name) + sizeof(w_id) + sizeof(d_id);
      std::vector<uint8_t> v(size);
      auto data = v.data();
      serialized::write(data, size, last_name);
      serialized::write(data, size, w_id);
      serialized::write(data, size, d_id);
      return v;
    }

    static CustomerByNameRequest deserialize(const uint8_t* data, size_t size)
    {
      CustomerByNameRequest customer_by_name_request;
      customer_by_name_request.last_name = serialized::read<decltype(last_name)>(data, size);
      customer_by_name_request.w_id = serialized::read<decltype(w_id)>(data, size);
      customer_by_name_request.d_id = serialized::read<decltype(d_id)>(data, size);
      return customer_by_name_request;
    }
  };

  struct OrderRequest
  {
    int32_t id;
    int32_t w_id;
    int32_t d_id;

    std::vector<uint8_t> serialize() const
    {
      auto size = sizeof(id) + sizeof(w_id) + sizeof(d_id);
      std::vector<uint8_t> v(size);
      auto data = v.data();
      serialized::write(data, size, id);
      serialized::write(data, size, w_id);
      serialized::write(data, size, d_id);
      return v;
    }

    static OrderRequest deserialize(const uint8_t* data, size_t size)
    {
      OrderRequest order_request;
      order_request.id = serialized::read<decltype(id)>(data, size);
      order_request.w_id = serialized::read<decltype(w_id)>(data, size);
      order_request.d_id = serialized::read<decltype(d_id)>(data, size);
      return order_request;
    }
  };

  struct LastOrderRequest
  {
    int32_t id;
    int32_t w_id;
    int32_t d_id;

    std::vector<uint8_t> serialize() const
    {
      auto size = sizeof(id) + sizeof(w_id) + sizeof(d_id);
      std::vector<uint8_t> v(size);
      auto data = v.data();
      serialized::write(data, size, id);
      serialized::write(data, size, w_id);
      serialized::write(data, size, d_id);
      return v;
    }

    static LastOrderRequest deserialize(const uint8_t* data, size_t size)
    {
      LastOrderRequest last_order_request;
      last_order_request.id = serialized::read<decltype(id)>(data, size);
      last_order_request.w_id = serialized::read<decltype(w_id)>(data, size);
      last_order_request.d_id = serialized::read<decltype(d_id)>(data, size);
      return last_order_request;
    }
  };

  struct LastNewOrderRequest
  {
    int32_t w_id;
    int32_t d_id;

    std::vector<uint8_t> serialize() const
    {
      auto size = sizeof(w_id) + sizeof(d_id);
      std::vector<uint8_t> v(size);
      auto data = v.data();
      serialized::write(data, size, w_id);
      serialized::write(data, size, d_id);
      return v;
    }

    static LastNewOrderRequest deserialize(const uint8_t* data, size_t size)
    {
      LastNewOrderRequest last_new_order_request;
      last_new_order_request.w_id = serialized::read<decltype(w_id)>(data, size);
      last_new_order_request.d_id = serialized::read<decltype(d_id)>(data, size);
      return last_new_order_request;
    }
  };


  struct DistrictResponse
  {
    static constexpr float MIN_TAX = 0;
    static constexpr float MAX_TAX = 0.2000f;
    static constexpr float INITIAL_YTD = 30000.00;
    static const int INITIAL_NEXT_O_ID = 3001;
    static const int MIN_NAME = 6;
    static const int MAX_NAME = 10;
    static const int NUM_PER_WAREHOUSE = 10;

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

    static DistrictResponse deserialize(const uint8_t* data, size_t size)
    {
      DistrictResponse district_response;
      district_response.id = serialized::read<decltype(id)>(data, size);
      district_response.w_id = serialized::read<decltype(w_id)>(data, size);
      district_response.tax = serialized::read<decltype(tax)>(data, size);
      district_response.ytd = serialized::read<decltype(ytd)>(data, size);
      district_response.next_o_id = serialized::read<decltype(next_o_id)>(data, size);
      district_response.name = serialized::read<decltype(name)>(data, size);
      district_response.street_1 = serialized::read<decltype(street_1)>(data, size);
      district_response.street_2 = serialized::read<decltype(street_2)>(data, size);
      district_response.city = serialized::read<decltype(city)>(data, size);
      district_response.state = serialized::read<decltype(state)>(data, size);
      district_response.zip = serialized::read<decltype(zip)>(data, size);

      return district_response;
    }
  };

  struct StockResponse
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
    std::array<std::array<char, DIST + 1>, DistrictResponse::NUM_PER_WAREHOUSE> dist;
    std::array<char, MAX_DATA + 1> data_field;

    std::vector<uint8_t> serialize() const
    {
      auto size = sizeof(i_id) + sizeof(w_id) + sizeof(quantity) + sizeof(ytd) +
        sizeof(order_cnt) + sizeof(remote_cnt) + sizeof(dist) + sizeof(data_field);
      std::vector<uint8_t> v(size);
      auto data = v.data();
      serialized::write(data, size, i_id);
      serialized::write(data, size, w_id);
      serialized::write(data, size, quantity);
      serialized::write(data, size, ytd);
      serialized::write(data, size, order_cnt);
      serialized::write(data, size, remote_cnt);
      serialized::write(data, size, dist);
      serialized::write(data, size, data_field);
      return v;
    }

    static StockResponse deserialize(const uint8_t* data, size_t size)
    {
      StockResponse stock_response;
      stock_response.i_id = serialized::read<decltype(i_id)>(data, size);
      stock_response.w_id = serialized::read<decltype(w_id)>(data, size);
      stock_response.quantity = serialized::read<decltype(quantity)>(data, size);
      stock_response.ytd = serialized::read<decltype(ytd)>(data, size);
      stock_response.order_cnt = serialized::read<decltype(order_cnt)>(data, size);
      stock_response.remote_cnt = serialized::read<decltype(remote_cnt)>(data, size);
      stock_response.dist = serialized::read<decltype(dist)>(data, size);
      stock_response.data_field = serialized::read<decltype(data_field)>(data, size);

      return stock_response;
    }
  };

  struct OrderLineResponse
  {
    static const int MIN_I_ID = 1;
    static const int MAX_I_ID = 100;
    static const int INITIAL_QUANTITY = 5;
    static constexpr float MIN_AMOUNT = 0.01f;
    static constexpr float MAX_AMOUNT = 9999.99f;
    static const int REMOTE_PROBABILITY_MILLIS = 10;

    int32_t o_id;
    int32_t d_id;
    int32_t w_id;
    int32_t number;
    int32_t i_id;
    int32_t supply_w_id;
    int32_t quantity;
    float amount;
    std::array<char, DATETIME_SIZE + 1> delivery_d;
    std::array<char, StockResponse::DIST + 1> dist_info;

    std::vector<uint8_t> serialize() const
    {
      auto size = sizeof(o_id) + sizeof(d_id) + sizeof(w_id) + sizeof(number) +
        sizeof(i_id) + sizeof(supply_w_id) + sizeof(quantity) + sizeof(amount) +
        sizeof(delivery_d) + sizeof(dist_info);
      std::vector<uint8_t> v(size);
      auto data = v.data();
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
      return v;
    }

    static OrderLineResponse deserialize(const uint8_t* data, size_t size)
    {
      OrderLineResponse order_line_response;
      order_line_response.o_id = serialized::read<decltype(o_id)>(data, size);
      order_line_response.d_id = serialized::read<decltype(d_id)>(data, size);
      order_line_response.w_id = serialized::read<decltype(w_id)>(data, size);
      order_line_response.number = serialized::read<decltype(number)>(data, size);
      order_line_response.i_id = serialized::read<decltype(i_id)>(data, size);
      order_line_response.supply_w_id = serialized::read<decltype(supply_w_id)>(data, size);
      order_line_response.quantity = serialized::read<decltype(quantity)>(data, size);
      order_line_response.delivery_d = serialized::read<decltype(delivery_d)>(data, size);
      order_line_response.dist_info = serialized::read<decltype(dist_info)>(data, size);

      return order_line_response;
    }
  };

  struct ItemResponse
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

    int32_t id;
    int32_t im_id;
    float price;
    std::array<char, MAX_NAME + 1> name;
    std::array<char, MAX_DATA + 1> data_field;

    std::vector<uint8_t> serialize() const
    {
      auto size = sizeof(id) + sizeof(im_id) + sizeof(price) + sizeof(name) + sizeof(data_field);
      std::vector<uint8_t> v(size);
      auto data = v.data();
      serialized::write(data, size, id);
      serialized::write(data, size, im_id);
      serialized::write(data, size, price);
      serialized::write(data, size, name);
      serialized::write(data, size, data_field);
      return v;
    }

    static ItemResponse deserialize(const uint8_t* data, size_t size)
    {
      ItemResponse item_response;
      item_response.id = serialized::read<decltype(id)>(data, size);
      item_response.im_id = serialized::read<decltype(im_id)>(data, size);
      item_response.price = serialized::read<decltype(price)>(data, size);
      item_response.name = serialized::read<decltype(name)>(data, size);
      item_response.data_field = serialized::read<decltype(data_field)>(data, size);

      return item_response;
    }
  };

  struct WarehouseResponse
  {
    static constexpr float MIN_TAX = 0;
    static constexpr float MAX_TAX = 0.2000f;
    static constexpr float INITIAL_YTD = 300000.00f;
    static const int MIN_NAME = 6;
    static const int MAX_NAME = 10;
    static const int MAX_WAREHOUSE_ID = 100;

    int32_t id;
    float tax;
    float ytd;
    std::array<char,  MAX_NAME + 1> name;
    std::array<char, Address::MAX_STREET + 1> street_1;
    std::array<char, Address::MAX_STREET + 1> street_2;
    std::array<char, Address::MAX_CITY + 1> city;
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

    static WarehouseResponse deserialize(const uint8_t* data, size_t size)
    {
      WarehouseResponse warehouse_response;
      warehouse_response.id = serialized::read<decltype(id)>(data, size);
      warehouse_response.tax = serialized::read<decltype(tax)>(data, size);
      warehouse_response.ytd = serialized::read<decltype(ytd)>(data, size);
      warehouse_response.name = serialized::read<decltype(name)>(data, size);
      warehouse_response.street_1 = serialized::read<decltype(street_1)>(data, size);
      warehouse_response.street_2 = serialized::read<decltype(street_2)>(data, size);
      warehouse_response.city = serialized::read<decltype(city)>(data, size);
      warehouse_response.state = serialized::read<decltype(state)>(data, size);
      warehouse_response.zip = serialized::read<decltype(zip)>(data, size);
      return warehouse_response;
    }
  };

  struct CustomerResponse
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
    std::array<char, MAX_DATA + 1> data_field;

    std::vector<uint8_t> serialize() const
    {
      auto size = sizeof(id) + sizeof(d_id) + sizeof(w_id) +
        sizeof(credit_lim) + sizeof(discount) + sizeof(balance) +
        sizeof(ytd_payment) + sizeof(payment_cnt) + sizeof(delivery_cnt) +
        sizeof(first) + sizeof(middle) + sizeof(last) + sizeof(street_1) +
        sizeof(street_2) + sizeof(city) + sizeof(state) + sizeof(zip) +
        sizeof(phone) + sizeof(since) + sizeof(credit) + sizeof(data_field);
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
      serialized::write(data, size, data_field);
      return v;
    }

    static CustomerResponse deserialize(const uint8_t* data, size_t size)
    {
      CustomerResponse customer_response;
      customer_response.id = serialized::read<decltype(id)>(data, size);
      customer_response.d_id = serialized::read<decltype(d_id)>(data, size);
      customer_response.w_id = serialized::read<decltype(w_id)>(data, size);
      customer_response.credit_lim = serialized::read<decltype(credit_lim)>(data, size);
      customer_response.discount = serialized::read<decltype(discount)>(data, size);
      customer_response.balance = serialized::read<decltype(balance)>(data, size);
      customer_response.ytd_payment = serialized::read<decltype(ytd_payment)>(data, size);
      customer_response.payment_cnt = serialized::read<decltype(payment_cnt)>(data, size);
      customer_response.delivery_cnt = serialized::read<decltype(delivery_cnt)>(data, size);
      customer_response.first = serialized::read<decltype(first)>(data, size);
      customer_response.middle = serialized::read<decltype(middle)>(data, size);
      customer_response.last = serialized::read<decltype(last)>(data, size);
      customer_response.street_1 = serialized::read<decltype(street_1)>(data, size);
      customer_response.street_2 = serialized::read<decltype(street_2)>(data, size);
      customer_response.city = serialized::read<decltype(city)>(data, size);
      customer_response.state = serialized::read<decltype(state)>(data, size);
      customer_response.zip = serialized::read<decltype(zip)>(data, size);
      customer_response.phone = serialized::read<decltype(phone)>(data, size);
      customer_response.since = serialized::read<decltype(since)>(data, size);
      customer_response.credit = serialized::read<decltype(credit)>(data, size);
      customer_response.data_field = serialized::read<decltype(data_field)>(data, size);
      return customer_response;
    }
  };

  struct OrderResponse
  {
    static const int MIN_CARRIER_ID = 1;
    static const int MAX_CARRIER_ID = 10;
    static const int NULL_CARRIER_ID = 0;
    static const int NULL_CARRIER_LOWER_BOUND = 2101;
    static const int MIN_OL_CNT = 5;
    static const int MAX_OL_CNT = 15;
    static const int INITIAL_ALL_LOCAL = 1;
    static const int INITIAL_ORDERS_PER_DISTRICT = 3000;
    static const int MAX_ORDER_ID = 10000000;

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
      auto size = sizeof(id) + sizeof(c_id) + sizeof(d_id) + sizeof(w_id) +
        sizeof(carrier_id) + sizeof(ol_cnt) + sizeof(all_local) + sizeof(entry_d);
      std::vector<uint8_t> v(size);
      auto data = v.data();
      serialized::write(data, size, id);
      serialized::write(data, size, c_id);
      serialized::write(data, size, d_id);
      serialized::write(data, size, w_id);
      serialized::write(data, size, carrier_id);
      serialized::write(data, size, ol_cnt);
      serialized::write(data, size, all_local);
      serialized::write(data, size, entry_d);
      return v;
    }

    static OrderResponse deserialize(const uint8_t* data, size_t size)
    {
      OrderResponse order_response;
      order_response.id = serialized::read<decltype(id)>(data, size);
      order_response.c_id = serialized::read<decltype(c_id)>(data, size);
      order_response.d_id = serialized::read<decltype(d_id)>(data, size);
      order_response.w_id = serialized::read<decltype(w_id)>(data, size);
      order_response.carrier_id = serialized::read<decltype(carrier_id)>(data, size);
      order_response.ol_cnt = serialized::read<decltype(ol_cnt)>(data, size);
      order_response.all_local = serialized::read<decltype(all_local)>(data, size);
      order_response.entry_d = serialized::read<decltype(entry_d)>(data, size);
      return order_response;
    }
  };

  struct NewOrderResponse
  {
    static const int INITIAL_NUM_PER_DISTRICT = 900;

    int32_t w_id;
    int32_t d_id;
    int32_t o_id;

    std::vector<uint8_t> serialize() const
    {
      auto size = sizeof(w_id) + sizeof(d_id) + sizeof(o_id);
      std::vector<uint8_t> v(size);
      auto data = v.data();
      serialized::write(data, size, w_id);
      serialized::write(data, size, d_id);
      serialized::write(data, size, o_id);
      return v;
    }

    static NewOrderResponse deserialize(const uint8_t* data, size_t size)
    {
      NewOrderResponse new_order_response;
      new_order_response.w_id = serialized::read<decltype(w_id)>(data, size);
      new_order_response.d_id = serialized::read<decltype(d_id)>(data, size);
      new_order_response.o_id = serialized::read<decltype(o_id)>(data, size);
      return new_order_response;
    }
  };
}
