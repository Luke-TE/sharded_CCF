#!/bin/bash
cd ../build
mkdir results

touch results/non_sharded_vary_txs.txt
echo "" > results/non_sharded_vary_txs.txt
   
for txs in 2000 4000 6000 8000 10000
do  
    for i in {1..5}
    do 
    echo "Non-sharded - $txs transactions - no prop delay - experiment $i out of 5"
    echo "Non-sharded - $txs transactions - no prop delay - experiment $i out of 5" >> results/non_sharded_vary_txs.txt
    ./tpcc_client --rpc-address 127.0.0.1:8000 --cert workspace/sandbox_common/user0_cert.pem --pk workspace/sandbox_common/user0_privk.pem --ca workspace/sandbox_common/networkcert.pem --transactions $txs --randomise >> results/non_sharded_vary_txs.txt
    echo "\n" >> results/non_sharded_vary_txs.txt
    done       
done

touch results/sharded_vary_txs.txt
echo "" > results/sharded_vary_txs.txt
for txs in 2000 4000 6000 8000 10000
do    
    for i in {1..5}
    do  
        echo "Sharded - $txs transactions - no prop delay - experiment $i out of 5"
        echo "Sharded - $txs transactions - no prop delay - experiment $i out of 5" >> results/sharded_vary_txs.txt
        ./tpcc_client --rpc-address 127.0.0.1:8000 --cert workspace/sandbox_common/user0_cert.pem --pk workspace/sandbox_common/user0_privk.pem --ca workspace/sandbox_common/networkcert.pem --transactions $txs --randomise --sharded >> results/sharded_vary_txs.txt
        echo "\n" >> results/sharded_vary_txs.txt
    done       
done


touch results/non_sharded_vary_rtt.txt
echo "" > results/non_sharded_vary_rtt.txt
for rtt in 20 40 60 80 100
do    
    for i in {1..5}
    do  
        echo "Non-sharded - 2000 transactions - rtt=$rtt - experiment $i out of 5"
        echo "Non-sharded - 2000 transactions - rtt=$rtt - experiment $i out of 5" >> results/non_sharded_vary_rtt.txt
        ./tpcc_client --rpc-address 127.0.0.1:8000 --cert workspace/sandbox_common/user0_cert.pem --pk workspace/sandbox_common/user0_privk.pem --ca workspace/sandbox_common/networkcert.pem --transactions 2000 --randomise --delay $rtt >> results/non_sharded_vary_rtt.txt
        echo "\n" >> results/non_sharded_vary_rtt.txt
    done       
done

touch results/sharded_vary_rtt.txt
echo "" > results/sharded_vary_rtt.txt
for rtt in 1 2 3 4 5
do    
    for i in {1..5}
    do  
        echo "Sharded - 2000 transactions - rtt=$rtt - experiment $i out of 5"
        echo "Sharded - 2000 transactions - rtt=$rtt - experiment $i out of 5" >> results/sharded_vary_rtt.txt
        ./tpcc_client --rpc-address 127.0.0.1:8000 --cert workspace/sandbox_common/user0_cert.pem --pk workspace/sandbox_common/user0_privk.pem --ca workspace/sandbox_common/networkcert.pem --transactions 2000 --randomise --sharded --delay $rtt >> results/sharded_vary_rtt.txt
        echo "\n" >> results/sharded_vary_rtt.txt
    done       
done