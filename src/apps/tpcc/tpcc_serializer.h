// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the Apache 2.0 License.
#pragma once

#include "ds/serialized.h"
#include "app/tpcc_tables.h"

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
    District::Key key;

    std::vector<uint8_t> serialize() const
    {
      auto size = sizeof(key.id) + sizeof(key.w_id);
      std::vector<uint8_t> v(size);
      auto data = v.data();
      serialized::write(data, size, key.id);
      serialized::write(data, size, key.w_id);
      return v;
    }

    static DistrictRequest deserialize(const uint8_t* data, size_t size)
    {
      DistrictRequest district_request;
      auto id = serialized::read<decltype(key.id)>(data, size);
      auto w_id = serialized::read<decltype(key.w_id)>(data, size)
      district_request.key = { id, w_id };
      return district_request;
    }
  };

  struct DistrictResponse
  {
    District district;

    std::vector<uint8_t> serialize() const
    {
      auto size = sizeof(district.id) + sizeof(district.w_id) + sizeof(district.tax) + sizeof(district.ytd) + sizeof(district.next_o_id)
        + sizeof(district.name) + sizeof(district.street_1) + sizeof(district.street_2) + sizeof(district.city) + sizeof(district.state) + sizeof(district.zip);
      std::vector<uint8_t> v(size);
      auto data = v.data();
      serialized::write(data, size, district.id);
      serialized::write(data, size, district.w_id);
      serialized::write(data, size, district.tax);
      serialized::write(data, size, district.ytd);
      serialized::write(data, size, district.next_o_id);
      serialized::write(data, size, district.name);
      serialized::write(data, size, district.street_1);
      serialized::write(data, size, district.street_2);
      serialized::write(data, size, district.city);
      serialized::write(data, size, district.state);
      serialized::write(data, size, district.zip);
      return v;
    }

    static District deserialize(const uint8_t* data, size_t size)
    {
      District district;
      district.id = serialized::read<decltype(district.id)>(data, size);
      district.w_id = serialized::read<decltype(district.w_id)>(data, size);
      district.tax = serialized::read<decltype(district.tax)>(data, size);
      district.ytd = serialized::read<decltype(district.ytd)>(data, size);
      district.next_o_id = serialized::read<decltype(district.next_o_id)>(data, size);
      district.name = serialized::read<decltype(district.name)>(data, size);
      district.street_1 = serialized::read<decltype(district.street_1)>(data, size);
      district.street_2 = serialized::read<decltype(district.street_2)>(data, size);
      district.city = serialized::read<decltype(district.city)>(data, size);
      district.state = serialized::read<decltype(district.state)>(data, size);
      district.zip = serialized::read<decltype(district.zip)>(data, size);

      return district;
    }
  };
}
