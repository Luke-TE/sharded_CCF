#!/bin/bash
cd ../build
for i in {1..5}
do
   ./tpcc_client --rpc-address 127.0.0.1:8000 --cert workspace/sandbox_common/user0_cert.pem --pk workspace/sandbox_common/user0_privk.pem --ca workspace/sandbox_common/networkcert.pem
done
