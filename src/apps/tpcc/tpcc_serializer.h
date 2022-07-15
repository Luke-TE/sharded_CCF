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
#include <map>
#include <string>
#include "app/tpcc_common.h"
#include "tpcc_tables.h"
#include "app/tpcc_global.h"

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
    std::vector<int> ints;

    std::vector<uint8_t> serialize() const
    {
      int num_ints = ints.size();
      auto size = sizeof(num_ints) + num_ints * sizeof(int);
      std::vector<uint8_t> v(size);
      auto data = v.data();
      serialized::write(data, size, num_ints);

      for(auto it = std::begin(ints); it != std::end(ints); ++it) {
        serialized::write(data, size, *it);
      }

      return v;
    }

    static TestVectorStruct deserialize(const uint8_t* data, size_t size)
    {
      TestVectorStruct test_struct;
      int num_ints = serialized::read<int>(data, size);
      if (num_ints > 0) {
        for (int i = 0; i < num_ints; i++) {
          test_struct.ints.push_back(serialized::read<int>(data, size));
        }
      }

      return test_struct;
    }
  };

  struct TestOrderLineMapStruct
  {
    std::map<tpcc::OrderLine::Key, tpcc::OrderLine> order_lines;

    std::vector<uint8_t> serialize() const
    {
      int num_entries = order_lines.size();
      auto size = sizeof(int) + (tpcc::OrderLine::get_size() + tpcc::OrderLine::Key::get_size()) * num_entries;
      std::vector<uint8_t> v(size);
      auto data = v.data();
      serialized::write(data, size, num_entries);

      for (auto const& entry : order_lines)
      {
        entry.first.serialize_to_buffer(data, size);
        entry.second.serialize_to_buffer(data, size);
      }

      return v;
    }

    static TestOrderLineMapStruct deserialize(const uint8_t* data, size_t size)
    {
      auto order_line_key_size = tpcc::OrderLine::Key::get_size();
      auto order_line_size = tpcc::OrderLine::get_size();

      TestOrderLineMapStruct test_struct;
      int num_entries = serialized::read<int>(data, size);
      if (num_entries > 0) {
        for (int i = 0; i < num_entries; i++) {
          auto key = tpcc::OrderLine::Key::deserialize(data, size);
          data += order_line_key_size;
          size -= order_line_key_size;

          auto order_line = tpcc::OrderLine::deserialize(data, size);
          data += order_line_size;
          size -= order_line_size;

          test_struct.order_lines[key] = order_line;
        }
      }

      return test_struct;
    }
  };

  struct CommitRequest
  {
    tpcc::WriteSet write_set;
    tpcc::KeysDeleted keys_deleted;

    std::vector<uint8_t> serialize() const
    {
      int num_orders = write_set.orders.size();
      int num_new_orders = write_set.new_orders.size();
      int num_order_lines = write_set.order_lines.size();
      int num_histories = write_set.histories.size();
      int num_new_orders_deleted = keys_deleted.new_order_keys.size();

      auto size =
        sizeof(int) + (tpcc::Order::get_size() + tpcc::OrderFullKey::get_size()) * num_orders
        + sizeof(int) + (tpcc::NewOrder::get_size() + tpcc::OrderFullKey::get_size()) * num_new_orders
        + sizeof(int) + (tpcc::OrderLine::get_size() + tpcc::OrderLine::Key::get_size()) * num_order_lines
        + sizeof(int) + (tpcc::History::get_size() + tpcc::History::Key::get_size()) * num_histories
        + sizeof(int) + tpcc::NewOrder::Key::get_size() * num_new_orders_deleted;

      std::vector<uint8_t> v(size);
      auto data = v.data();

      serialized::write(data, size, num_orders);
      for (auto const& entry : write_set.orders)
      {
        entry.first.serialize_to_buffer(data, size);
        entry.second.serialize_to_buffer(data, size);
      }

      serialized::write(data, size, num_new_orders);
      for (auto const& entry : write_set.new_orders)
      {
        entry.first.serialize_to_buffer(data, size);
        entry.second.serialize_to_buffer(data, size);
      }

      serialized::write(data, size, num_order_lines);
      for (auto const& entry : write_set.order_lines)
      {
        entry.first.serialize_to_buffer(data, size);
        entry.second.serialize_to_buffer(data, size);
      }

      serialized::write(data, size, num_histories);
      for (auto const& entry : write_set.histories)
      {
        entry.first.serialize_to_buffer(data, size);
        entry.second.serialize_to_buffer(data, size);
      }

      serialized::write(data, size, num_new_orders_deleted);
      for(auto it = std::begin(keys_deleted.new_order_keys); it != std::end(keys_deleted.new_order_keys); ++it) {
        serialized::write(data, size, *it);
      }

      return v;
    }

    static CommitRequest deserialize(const uint8_t* data, size_t size)
    {
      auto order_full_key_size = tpcc::OrderFullKey::get_size();
      auto order_size = tpcc::Order::get_size();
//      auto new_order_key_size = tpcc::NewOrder::Key::get_size();
      auto new_order_size = tpcc::NewOrder::get_size();
      auto order_line_key_size = tpcc::OrderLine::Key::get_size();
      auto order_line_size = tpcc::OrderLine::get_size();
      auto history_key_size = tpcc::History::Key::get_size();
      auto history_size = tpcc::History::get_size();
      auto new_order_key_size = tpcc::OrderLine::Key::get_size();

      CommitRequest commit_request;

      int num_orders = serialized::read<int>(data, size);
      for (int i = 0; i < num_orders; i++) {
        auto key = tpcc::OrderFullKey::deserialize(data, size);
        data += order_full_key_size;
        size -= order_full_key_size;

        auto order = tpcc::Order::deserialize(data, size);
        data += order_size;
        size -= order_size;

        commit_request.write_set.orders[key] = order;
      }

      int num_new_orders = serialized::read<int>(data, size);
      for (int i = 0; i < num_new_orders; i++) {
        auto key = tpcc::OrderFullKey::deserialize(data, size);
        data += order_full_key_size;
        size -= order_full_key_size;

        auto new_order = tpcc::NewOrder::deserialize(data, size);
        data += new_order_size;
        size -= new_order_size;

        commit_request.write_set.new_orders[key] = new_order;
      }

      int num_order_lines = serialized::read<int>(data, size);
      for (int i = 0; i < num_order_lines; i++) {
        auto key = tpcc::OrderLine::Key::deserialize(data, size);
        data += order_line_key_size;
        size -= order_line_key_size;

        auto order_line = tpcc::OrderLine::deserialize(data, size);
        data += order_line_size;
        size -= order_line_size;

        commit_request.write_set.order_lines[key] = order_line;
      }

      int num_histories = serialized::read<int>(data, size);
      for (int i = 0; i < num_histories; i++) {
        auto key = tpcc::History::Key::deserialize(data, size);
        data += history_key_size;
        size -= history_key_size;

        auto history = tpcc::History::deserialize(data, size);
        data += history_size;
        size -= history_size;

        commit_request.write_set.histories[key] = history;
      }

      int num_new_orders_deleted = serialized::read<int>(data, size);
      for (int i = 0; i < num_new_orders_deleted; i++) {
        auto key = tpcc::NewOrder::Key::deserialize(data, size);
        data += new_order_key_size;
        size -= new_order_key_size;

        commit_request.keys_deleted.new_order_keys.insert(key);
      }

      return commit_request;
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
