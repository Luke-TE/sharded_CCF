// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the Apache 2.0 License.
#include "../tpcc_serializer.h"
#include "perf_client.h"
#include "../tpcc_tables.h"
#include "tpcc_client_read_writer.h"
#include "tpcc_transactions_local.h"
#include "../serialise_msgpack.h"

#include <chrono>

using namespace std;
using namespace nlohmann;

struct TpccClientOptions : public client::PerfOptions
{
  TpccClientOptions(CLI::App& app, const std::string& default_pid_file) :
    client::PerfOptions("Tpcc_ClientCpp", default_pid_file, app)
  {}
};

using Base = client::PerfBase<TpccClientOptions>;

class TpccClient : public Base
{
private:
  enum class TransactionTypes : uint8_t
  {
    stock_level = 0,
    order_status,
    delivery,
    payment,
    new_order,

    NumberTransactions
  };

  const char* OPERATION_C_STR[5]{
    "stock_level", "order_status", "delivery", "payment", "new_order"};

  std::optional<RpcTlsClient::Response> send_creation_transactions() override
  {
    auto connection = get_connection();
    tpcc::DbCreation db;
    db.new_orders_per_district = 1000;
    db.seed = 42;
    const auto body = db.serialize();
    const auto response =
      connection->call("tpcc_create", CBuffer{body.data(), body.size()});
    check_response(response);

    return response;
  }

  void prepare_transactions() override
  {
    auto connection = create_connection(true, false);
    tpcc::ClientReadWriter read_writer(connection);

//    tpcc::District::Key key;
//    key.id = 123;
//    key.w_id = 456;
//
//    auto optional_district = read_writer.get_district(key);
//    if (optional_district.has_value())
//    {
//      auto district = optional_district.value();
//      LOG_INFO_FMT("New Value: {0}, {1}", std::to_string(district.id), std::to_string(district.w_id));
//    }
//    else {
//      LOG_INFO_FMT("No Value :(");
//    }



    tpcc::TestStruct test_struct;
    test_struct.int_val = 999;

    const auto body = test_struct.serialize();
    const auto response =
      connection->call("do_test_vector", CBuffer{body.data(), body.size()});

    if (http::status_success(response.status))
    {
      if (response.body.size() > 0) {
        auto test_vector_struct = tpcc::TestOrderLineMapStruct::deserialize(response.body.data(), response.body.size());
        LOG_INFO_FMT("Num of Entries: {0}", std::to_string(test_vector_struct.order_lines.size()));

        for (auto const& entry : test_vector_struct.order_lines)
        {
          LOG_INFO_FMT("Orderline Key w_id: {0}, Orderline w_id {1}", std::to_string(entry.first.w_id), std::to_string(entry.second.w_id));
          LOG_INFO_FMT("Orderline Key number: {0}, Orderline number {1}", std::to_string(entry.first.number), std::to_string(entry.second.number));
        }
      }
      else {
        LOG_INFO_FMT("No vals");
      }
    }
    else {
      LOG_INFO_FMT("bad response");
     }

    // Reserve space for transfer transactions
    prepared_txs.resize(options.num_transactions);

    for (decltype(options.num_transactions) i = 0; i < options.num_transactions;
         i++)
    {
      uint8_t operation;
      uint8_t x = rand_range(100);

      std::vector<uint8_t> serialized_body;

      if (x < 4)
      {
        tpcc::StockLevel sl;
        sl.warehouse_id = 1;
        sl.district_id = 1;
        sl.threshold = 1000;
        sl.seed = rand_range<int32_t>();
        serialized_body = sl.serialize();
        operation = (uint8_t)TransactionTypes::stock_level;
      }
      else if (x < 8)
      {
        tpcc::TxInfo info;
        info.seed = rand_range<int32_t>();
        serialized_body = info.serialize();
        operation = (uint8_t)TransactionTypes::delivery;
      }
      else if (x < 12)
      {
        tpcc::TxInfo info;
        info.seed = rand_range<int32_t>();
        serialized_body = info.serialize();
        operation = (uint8_t)TransactionTypes::order_status;
      }
      else if (x < (12 + 43))
      {
        tpcc::TxInfo info;
        info.seed = rand_range<int32_t>();
        serialized_body = info.serialize();
        operation = (uint8_t)TransactionTypes::payment;
      }
      else
      {
        tpcc::TxInfo info;
        info.seed = rand_range<int32_t>();
        serialized_body = info.serialize();
        operation = (uint8_t)TransactionTypes::new_order;
      }
      // TODO change this to add to a new data structure

      add_prepared_tx(
        OPERATION_C_STR[operation],
        CBuffer{serialized_body.data(), serialized_body.size()},
        true, // expect commit
        i);

    }
  }

  bool check_response(const RpcTlsClient::Response& r) override
  {
    if (!http::status_success(r.status))
    {
      const std::string error_msg(r.body.begin(), r.body.end());
      throw logic_error(error_msg);
      return false;
    }

    return true;
  }

  void verify_params(const nlohmann::json& expected) override
  {
    Base::verify_params(expected);
  }

  void send_commit_request(std::shared_ptr<RpcTlsClient>& connection, tpcc::ClientReadWriter read_writer) {
    tpcc::CommitRequest commitRequest;
    commitRequest.write_set = read_writer.write_set;
    commitRequest.keys_deleted = read_writer.keys_deleted;

    const auto body = commitRequest.serialize();
    const auto response =
      connection->call("commit_2pc", CBuffer{body.data(), body.size()});
    check_response(response);
  }

  void send_transactions(std::shared_ptr<RpcTlsClient>& connection, const PreparedTxs& txs) {
    last_write_time = std::chrono::high_resolution_clock::now();

    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    auto start_time = high_resolution_clock::now();

    for (decltype(options.num_transactions) i = 0; i < options.num_transactions;
         i++)
    {
      uint8_t x = rand_range(100);

      if (x < 4) // Stock Level
      {
//        LOG_INFO_FMT("Stock Level");
        tpcc::ClientReadWriter client_read_writer(connection);
        tpcc::TpccTransactionsClient tx_client(client_read_writer, rand_range<int32_t>());
        tx_client.stock_level(1, 1, 1000);
        send_commit_request(connection, client_read_writer);
      }
      else if (x < 8) // Delivery
      {
//        LOG_INFO_FMT("Delivery");
        tpcc::ClientReadWriter client_read_writer(connection);
        tpcc::TpccTransactionsClient tx_client(client_read_writer, rand_range<int32_t>());
        tx_client.delivery();
        send_commit_request(connection, client_read_writer);
      }
      else if (x < 12) // Order Status
      {
//        LOG_INFO_FMT("Order Status");
        tpcc::ClientReadWriter client_read_writer(connection);
        tpcc::TpccTransactionsClient tx_client(client_read_writer, rand_range<int32_t>());
        tx_client.order_status();
        send_commit_request(connection, client_read_writer);
      }
      else if (x < (12 + 43)) // Payment
      {
//        LOG_INFO_FMT("Payment");
        tpcc::ClientReadWriter client_read_writer(connection);
        tpcc::TpccTransactionsClient tx_client(client_read_writer, rand_range<int32_t>());
        tx_client.payment();
        send_commit_request(connection, client_read_writer);
      }
      else // New Order
      {
//        LOG_INFO_FMT("New Order");
        tpcc::ClientReadWriter client_read_writer(connection);
        tpcc::TpccTransactionsClient tx_client(client_read_writer, rand_range<int32_t>());
        tx_client.new_order();
        send_commit_request(connection, client_read_writer);
      }
    }

    auto finish_time = high_resolution_clock::now();
    duration<double, std::milli>  = finish_time - start_time;
    auto dur = s_double.count() / 1000.0;
    LOG_INFO_FMT("Total duration (seconds): {}", std::to_string(dur));
    LOG_INFO_FMT("Txs per second: {}", std::to_string(options.num_transactions / dur));
  }

public:
  TpccClient(const TpccClientOptions& o) : Base(o) {}

  void run() override
  {
    files::dump(fmt::format("{}", ::getpid()), options.pid_file);

    LOG_INFO_FMT(
      "Random choices determined by seed: {}", options.generator_seed);
    rand_generator.seed(options.generator_seed);

    send_all_creation_transactions();
//    prepare_all_transactions();

    LOG_TRACE_FMT(
      "Sending {} transactions from {} clients {} times...",
      options.num_transactions,
      options.thread_count,
      options.session_count);

    //     = send_all_prepared_transactions();
    init_connection();
    try
    {
      send_transactions(rpc_connection, prepared_txs);
      LOG_INFO_FMT("Done");
    }
    catch (std::exception& e)
    {
      LOG_FAIL_FMT("Transaction exception: {}", e.what());
      throw e;
    }
  }
};

int main(int argc, char** argv)
{
  CLI::App cli_app{"Tpcc Client"};
  TpccClientOptions options(cli_app, argv[0]);
  CLI11_PARSE(cli_app, argc, argv);

  TpccClient client(options);
  client.run();

  return 0;
}
