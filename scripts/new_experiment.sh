#!/bin/bash
cd ../build
mkdir results
output=$1
shift
echo "Output file: $output"
echo "Params: $@"
for i in {1..5}
do 
    ./tpcc_client --rpc-address 127.0.0.1:8000 --cert workspace/sandbox_common/user0_cert.pem --pk workspace/sandbox_common/user0_privk.pem --ca workspace/sandbox_common/networkcert.pem --randomise $@ >> $output
    echo "\n" >> output
done