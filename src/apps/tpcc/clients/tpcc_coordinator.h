#include <set>
#include "../tpcc_tables.h"

namespace tpcc
{
  struct KeysWrittenTo {
    std::set<tpcc::Order::Key> order_keys;
    std::set<tpcc::NewOrder::Key> new_order_keys;
    std::set<tpcc::OrderLine::Key> order_line_keys;
    std::set<tpcc::History::Key> history_keys;
  };



  class TpccCoordinator
  {
  public:
//    enum class 2pcResult : uint8_t
//    {
//      commit = 0,
//      abort,
//      delivery,
//      payment,
//      new_order,
//
//      NumberTransactions
//    };

    void two_phase_commit(tpcc::KeysWrittenTo keys_written_to) {

    }


    //def sendPrepare(tx, writeSet, versionedKeysRead, keysAccessed, shardsCommunicatedWith):
    //    shardToKeysAccessed = partitionKeys(keysAccessed)
    //
    //    // must be made parallel
    //    req = createPrepareRequest(tx, versionedKeysRead, writeSet)
    //    for shard, keysAccessedInShard in shardToKeysAccessed.items():
    //        response = sendRequest(req, shard)
    //        committeeDecision = deserialise<txResponse>(response)
    //        switch (result):
    //            case 2PCResponse.COMMIT:
    //                shardsCommunicatedWith.append(shardAccessedNum)
    //                continue
    //
    //            case 2PCResponse.ABORT:
    //                return 2PCResponse.ABORT
    //
    //            default:
    //                throw error("unknown")
    //
    //        return 2PCResponse.COMMIT
    //
    void prepare() {

    }

    //def sendCommit(tx, shards):
    //    req = createCommitRequest(tx)
    //    for shard in shards:
    //        try:
    //            response = sendRequest(req, shard)
    //        except e:
    //            throw error("oh no, commit failed because... " + e.message)
    void commit() {

    }

    //def sendRollback(tx, shardsCommunicatedWith, keysAccessed):
    //    shardToKeysAccessed = partitionKeys(keysAccessed)
    //
    //    for shard in shardsCommunicatedWith:
    //        keysAccessedInShard = shardToKeysAccessed[shard]
    //        req = createRollbackRequest(tx, keysAccessedInShard)
    //        try:
    //            response = sendRequest(req, shard)
    //        except e:
    //            throw error("oh no, rollback failed because... " + e.message)
    void abort() {

    }

  private:

  };
}

//class shardCoordinatorClient:
//def init(shardToNodes: map shard => nodeId):
//    this.shardToNodes = shardToNodes
//
//def deriveShard(hash):
//    // e.g.
//    return hash \% numOfShards
//
//def calcShard(key):
//    keyDigest = hashFun(key)
//    return deriveShard(keyDigest)
//
//def sendRequest(req, shard):
//    nodes = this.shardToNodes[shard]
//    for node in nodes:
//        try:
//            response = await rpcClient.send(req)
//            return response
//        except:
//            continue
//
//def sendReadRequest(key):
//    shard = calcShard(key)
//    req = createReadRequest(key)
//    response = sendRequest(req, shard)
//    versionKeyVals = deserialise<readResponse>(response)
//    return versionKeyVals
//
//
//def partitionKeys(keys):
//    shardToKeys = map shard => set(key)
//    for key in keys:
//        shard = calcShard(key)
//        shardToKeys[shard].append(key)
//
//    return shardToKeys
//
//def sendReadRequests(keys):
//    shardToKeysRequired = partitionKeys(keys)
//
//    keyVals = map: key => val
//    for shard, keysRequired in shardToKeysRequired.items():
//        req = createReadRequest(keysRequired)
//        response = sendRequest(req, shard)
//        shardKeyVals = deserialise<readResponse>(response)
//        keyVals.add(shardKeyVals)
//
//    return keyVals
//


//
