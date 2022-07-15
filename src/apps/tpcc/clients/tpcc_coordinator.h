#include <set>
#include "../tpcc_tables.h"
#include "../tpcc_serializer.h"
#include "perf_client.h"
#include "tpcc_client_read_writer.h"
#include "tpcc_transactions_local.h"
#include <chrono>
#include <thread>

namespace tpcc
{
  class TpccCoordinator
  {
  public:
    TpccCoordinator(std::shared_ptr<RpcTlsClient>& c, tpcc::ClientReadWriter r, int p) : connection(c), read_writer(r), prop_delay_ms(p) {
    }
    double two_phase_commit(int tx_id) {
      auto can_commit = prepare(tx_id);

      if (can_commit)
      {
        return commit(tx_id);
      }
      else
      {
        abort(tx_id);
        return 0.0;
      }
    }

  private:
    std::shared_ptr<RpcTlsClient>& connection;
    tpcc::ClientReadWriter read_writer;
    int prop_delay_ms;

    bool prepare(int tx_id) {
      tpcc::WriteSet2pc write_set_2pc;
      write_set_2pc.tx_id = tx_id;
      write_set_2pc.update_set = read_writer.update_set;
      write_set_2pc.keys_deleted = read_writer.keys_deleted;

      const auto body = write_set_2pc.serialize();
      std::this_thread::sleep_for(std::chrono::milliseconds(options.prop_delay_ms));
      const auto response =
        connection->call("prepare_2pc", CBuffer{body.data(), body.size()});
      return http::status_success(response.status);
    }

    double commit(int tx_id) {
      using std::chrono::high_resolution_clock;
      using std::chrono::duration_cast;
      using std::chrono::duration;
      using std::chrono::milliseconds;

      tpcc::CommitRequest commit_request;
      commit_request.tx_id = tx_id;

      const auto body = commit_request.serialize();
      auto tx_start_time = high_resolution_clock::now();
      std::this_thread::sleep_for(std::chrono::milliseconds(options.prop_delay_ms));
      const auto response =
        connection->call("commit_2pc", CBuffer{body.data(), body.size()});
      auto tx_finish_time = high_resolution_clock::now();

      duration<double, std::milli> s_double = tx_finish_time - tx_start_time;
      return s_double.count();
    }

    void abort(int tx_id) {
      throw std::logic_error("abort occurred");
    }
  };
}

