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

    std::vector<uint8_t> serialize() const
    {
      auto size = sizeof(id) + sizeof(w_id);
      std::vector<uint8_t> v(size);
      auto data = v.data();
      serialized::write(data, size, id);
      serialized::write(data, size, w_id);
      return v;
    }

    static DistrictRequest deserialize(const uint8_t* data, size_t size)
    {
      DistrictRequest district_request;
      district_request.id = serialized::read<decltype(id)>(data, size);
      district_request.w_id = serialized::read<decltype(w_id)>(data, size);
      return district_request;
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
}
