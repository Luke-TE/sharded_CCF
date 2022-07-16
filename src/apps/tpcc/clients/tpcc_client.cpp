// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the Apache 2.0 License.
#include "../tpcc_serializer.h"
#include "perf_client.h"
#include "../tpcc_tables.h"
#include "tpcc_client_read_writer.h"
#include "tpcc_transactions_local.h"
#include "../serialise_msgpack.h"
#include "tpcc_coordinator.h"

#include <chrono>
#include <thread>

using namespace std;
using namespace nlohmann;

struct TpccClientOptions : public client::PerfOptions
{
  bool sharded = false;
  int prop_delay_ms = 0;

  TpccClientOptions(CLI::App& app, const std::string& default_pid_file) :
    client::PerfOptions("Tpcc_ClientCpp", default_pid_file, app)
  {
    app
      .add_flag(
        "--sharded",
        sharded,
        "Use the sharded client")
      ->capture_default_str();

    app
      .add_option(
        "--delay",
        prop_delay_ms,
        "Propagation delay in ms")
      ->capture_default_str();
  }
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

  timing::Results call_raw_batch(
    std::shared_ptr<RpcTlsClient>& connection, const PreparedTxs& txs) override
  {
    size_t read;
    size_t written;

    if (options.transactions_per_s > 0)
    {
      write_delay_ns =
        std::chrono::nanoseconds{1000000000 / options.transactions_per_s};
      connection->set_tcp_nodelay(true);
    }

    last_write_time = std::chrono::high_resolution_clock::now();
    kick_off_timing();

    // Repeat for each session
    for (size_t session = 1; session <= options.session_count; ++session)
    {
      read = 0;
      written = 0;

      // Write everything
      while (written < txs.size())
      {
        // maybe already implemented above? still do it here
        std::this_thread::sleep_for(std::chrono::milliseconds(options.prop_delay_ms));
        write(txs[written], read, written, connection);
      }

      blocking_read(read, written, connection);

      // Reconnect for each session (except the last)
      if (session != options.session_count)
      {
        reconnect(connection);
      }
    }

    if (!options.no_wait)
    {
      // Create a new connection, because we need to do some GETs
      // and when all you have is a WebSocket, everything looks like a POST!
      auto c = create_connection(true, false);
      wait_for_global_commit(last_response_tx_id);
    }
    const auto last_commit = last_response_tx_id.seqno;
    auto timing_results = end_timing(last_commit);
    LOG_INFO_FMT("Timing ended");
    return timing_results;
  }

  void wait_for_global_commit(const ccf::TxID& target)
  {
    response_times.wait_for_global_commit(target, true);
  }

  void prepare_transactions() override
  {
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

  void send_transactions(std::shared_ptr<RpcTlsClient>& connection, const PreparedTxs& txs) {
    last_write_time = std::chrono::high_resolution_clock::now();

    using std::chrono::high_resolution_clock;
    using std::chrono::duration_cast;
    using std::chrono::duration;
    using std::chrono::milliseconds;

    auto total_commit_response_time = 0.0;

    auto total_new_order_commit_response_time = 0.0;
    auto total_payment_commit_response_time = 0.0;
    auto total_delivery_commit_response_time = 0.0;
    auto total_stock_level_commit_response_time = 0.0;
    auto total_order_status_commit_response_time = 0.0;

    auto total_response_time = 0.0;

    auto total_new_order_response_time = 0.0;
    auto total_payment_response_time = 0.0;
    auto total_delivery_response_time = 0.0;
    auto total_stock_level_response_time = 0.0;
    auto total_order_status_response_time = 0.0;

    auto num_new_order_txs = 0.0;
    auto num_payment_txs = 0.0;
    auto num_delivery_txs = 0.0;
    auto num_stock_level_txs = 0.0;
    auto num_order_status_txs = 0.0;

    auto start_time = high_resolution_clock::now();
    for (decltype(options.num_transactions) i = 0; i < options.num_transactions;
         i++)
    {
      uint8_t x = rand_range(100);

      if (x < 4) // Stock Level
      {
        // LOG_INFO_FMT("Stock Level");
        auto tx_start_time = high_resolution_clock::now();
        tpcc::ClientReadWriter client_read_writer(connection, options.prop_delay_ms);
        tpcc::TpccTransactionsClient tx_client(client_read_writer, rand_range<int32_t>());
        tx_client.stock_level(1, 1, 1000);
        tpcc::TpccCoordinator coordinator(connection, client_read_writer, options.prop_delay_ms);

        auto commit_response_time = coordinator.two_phase_commit(i);
        auto tx_end_time = high_resolution_clock::now();
        duration<double, std::milli> tx_ms_double = tx_end_time - tx_start_time;
        auto response_time = tx_ms_double.count();

        total_response_time += response_time;
        total_stock_level_response_time += response_time;
        total_commit_response_time += commit_response_time;
        total_stock_level_commit_response_time += commit_response_time;
        num_stock_level_txs++;
      }
      else if (x < 8) // Delivery
      {
        // LOG_INFO_FMT("Delivery");
        auto tx_start_time = high_resolution_clock::now();
        tpcc::ClientReadWriter client_read_writer(connection, options.prop_delay_ms);
        tpcc::TpccTransactionsClient tx_client(client_read_writer, rand_range<int32_t>());
        tx_client.delivery();
        tpcc::TpccCoordinator coordinator(connection, client_read_writer, options.prop_delay_ms);

        auto commit_response_time = coordinator.two_phase_commit(i);
        auto tx_end_time = high_resolution_clock::now();
        duration<double, std::milli> tx_ms_double = tx_end_time - tx_start_time;
        auto response_time = tx_ms_double.count();

        total_response_time += response_time;
        total_delivery_response_time += response_time;
        total_commit_response_time += commit_response_time;
        total_delivery_commit_response_time += commit_response_time;
        num_delivery_txs++;
      }
      else if (x < 12) // Order Status
      {
        // LOG_INFO_FMT("Order Status");
        auto tx_start_time = high_resolution_clock::now();
        tpcc::ClientReadWriter client_read_writer(connection, options.prop_delay_ms);
        tpcc::TpccTransactionsClient tx_client(client_read_writer, rand_range<int32_t>());
        tx_client.order_status();
        tpcc::TpccCoordinator coordinator(connection, client_read_writer, options.prop_delay_ms);

        auto commit_response_time = coordinator.two_phase_commit(i);
        auto tx_end_time = high_resolution_clock::now();
        duration<double, std::milli> tx_ms_double = tx_end_time - tx_start_time;
        auto response_time = tx_ms_double.count();

        total_response_time += response_time;
        total_order_status_response_time += response_time;
        total_commit_response_time += commit_response_time;
        total_order_status_commit_response_time += commit_response_time;
        num_order_status_txs++;
      }
      else if (x < (12 + 43)) // Payment
      {
        // LOG_INFO_FMT("Payment");
        auto tx_start_time = high_resolution_clock::now();
        tpcc::ClientReadWriter client_read_writer(connection, options.prop_delay_ms);
        tpcc::TpccTransactionsClient tx_client(client_read_writer, rand_range<int32_t>());
        tx_client.payment();
        tpcc::TpccCoordinator coordinator(connection, client_read_writer, options.prop_delay_ms);

        auto commit_response_time = coordinator.two_phase_commit(i);
        auto tx_end_time = high_resolution_clock::now();
        duration<double, std::milli> tx_ms_double = tx_end_time - tx_start_time;
        auto response_time = tx_ms_double.count();

        total_response_time += response_time;
        total_payment_response_time += response_time;
        total_commit_response_time += commit_response_time;
        total_payment_commit_response_time += commit_response_time;
        num_payment_txs++;
      }
      else // New Order
      {
        // LOG_INFO_FMT("New Order");
        auto tx_start_time = high_resolution_clock::now();
        tpcc::ClientReadWriter client_read_writer(connection, options.prop_delay_ms);
        tpcc::TpccTransactionsClient tx_client(client_read_writer, rand_range<int32_t>());
        tx_client.new_order();
        tpcc::TpccCoordinator coordinator(connection, client_read_writer, options.prop_delay_ms);

        auto commit_response_time = coordinator.two_phase_commit(i);
        auto tx_end_time = high_resolution_clock::now();
        duration<double, std::milli> tx_ms_double = tx_end_time - tx_start_time;
        auto response_time = tx_ms_double.count();

        total_response_time += response_time;
        total_new_order_response_time += response_time;
        total_commit_response_time += commit_response_time;
        total_new_order_commit_response_time += commit_response_time;
        num_new_order_txs++;
      }
    }

    auto finish_time = high_resolution_clock::now();
    duration<double, std::milli> s_double = finish_time - start_time;
    auto dur = s_double.count() / 1000.0;
    LOG_INFO_FMT("Total duration (seconds): {}", std::to_string(dur));
    LOG_INFO_FMT("Txs per second: {}", std::to_string(options.num_transactions / dur));

    LOG_INFO_FMT("Avg response time (ms): {}", std::to_string(total_response_time / options.num_transactions));

    LOG_INFO_FMT("Avg new order tx response time (ms): {}", std::to_string(total_new_order_response_time / num_new_order_txs));
    LOG_INFO_FMT("Avg payment tx response time (ms): {}", std::to_string(total_payment_response_time / num_payment_txs));
    LOG_INFO_FMT("Avg delivery tx response time (ms): {}", std::to_string(total_delivery_response_time / num_delivery_txs));
    LOG_INFO_FMT("Avg stock level tx response time (ms): {}", std::to_string(total_stock_level_response_time / num_stock_level_txs));
    LOG_INFO_FMT("Avg order status tx response time (ms): {}", std::to_string(total_order_status_response_time / num_order_status_txs));


    LOG_INFO_FMT("Avg commit response time (ms): {}", std::to_string(total_commit_response_time / options.num_transactions));

    LOG_INFO_FMT("Avg new order tx commit response time (ms): {}", std::to_string(total_new_order_commit_response_time / num_new_order_txs));
    LOG_INFO_FMT("Avg payment tx commit response time (ms): {}", std::to_string(total_payment_commit_response_time / num_payment_txs));
    LOG_INFO_FMT("Avg delivery tx commit response time (ms): {}", std::to_string(total_delivery_commit_response_time / num_delivery_txs));
    LOG_INFO_FMT("Avg stock level tx commit response time (ms): {}", std::to_string(total_stock_level_commit_response_time / num_stock_level_txs));
    LOG_INFO_FMT("Avg order status tx commit response time (ms): {}", std::to_string(total_order_status_commit_response_time / num_order_status_txs));
  }

public:
  TpccClient(const TpccClientOptions& o) : Base(o) {}

  void run_() {
    if (options.sharded) {
      run_sharded();
    } else {
      run();
    }
  }

  void run_sharded()
  {
    files::dump(fmt::format("{}", ::getpid()), options.pid_file);

    if (options.randomise)
    {
      options.generator_seed = std::random_device()();
    }

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
//  client.run_sharded();
  client.run_();
  return 0;
}
