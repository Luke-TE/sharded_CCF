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


  struct TestVectorStruct
  {
    int num_ints;
    std::vector<int> ints;

    std::vector<uint8_t> serialize() const
    {
      auto size = sizeof(num_ints) + ints.size();
//      auto size = sizeof(num_ints) + ints.size() * sizeof(int);
      std::vector<uint8_t> v(size);
      auto data = v.data();
      serialized::write(data, size, num_ints);

//      for(auto it = std::begin(ints); it != std::end(ints); ++it) {
//        serialized::write(data, size, it);
//      }

      return v;
    }

    static TestVectorStruct deserialize(const uint8_t* data, size_t size)
    {
      TestVectorStruct test_struct;
      test_struct.num_ints = serialized::read<decltype(num_ints)>(data, size);
//      if (test_struct.num_ints > 0) {
//        for (int i = 0; i < test_struct.num_ints; i++) {
//          test_struct.ints.push_back(serialized::read<int>(data, size));
//        }
//      }

      return test_struct;
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
}
