#!/bin/bash
PORT=$RANDOM
quit=0

while [ "$quit" -ne 1 ]; do
  netstat -a | grep $PORT >> /dev/null
  if [ $? -gt 0 ]; then
    quit=1
  else
    PORT=`expr $PORT + 1`
  fi
done

# Server or client | Tcp or Udp | Port | Threads | Data[MB] | ServerIP

echo "Starting Test Case 1:TCP"
javac \*\.java
java NetworkBenchmark 1\|1\|$((PORT)) &
java NetworkBenchmark 2\|1\|$((PORT))\|1\|4000\|0\.0\.0\.0 &>> log.txt
echo "Completed"
((PORT=PORT+1))
echo "Starting Test Case 2:TCP"
java NetworkBenchmark 1\|1\|$((PORT)) &
java NetworkBenchmark 2\|1\|$((PORT))\|2\|4000\|0\.0\.0\.0 &>> log.txt
echo "Completed"
((PORT=PORT+1))
echo "Starting Test Case 3:TCP"
java NetworkBenchmark 1\|1\|$((PORT)) &
java NetworkBenchmark 2\|1\|$((PORT))\|4\|4000\|0\.0\.0\.0 &>> log.txt
echo "Completed"
((PORT=PORT+1))
echo "Starting Test Case 4:TCP"
java NetworkBenchmark 1\|1\|$((PORT)) &
java NetworkBenchmark 2\|1\|$((PORT))\|8\|4000\|0\.0\.0\.0 &>> log.txt
echo "Completed"
((PORT=PORT+1))
echo "Starting Test Case 1:UDP"
javac \*\.java
java NetworkBenchmark 1\|2\|$((PORT)) &
java NetworkBenchmark 2\|2\|$((PORT))\|1\|4000\|0\.0\.0\.0 &>> log.txt
echo "Completed"
((PORT=PORT+1))
echo "Starting Test Case 2:UDP"
java NetworkBenchmark 1\|2\|$((PORT)) &
java NetworkBenchmark 2\|2\|$((PORT))\|2\|4000\|0\.0\.0\.0 &>> log.txt
echo "Completed"
((PORT=PORT+1))
echo "Starting Test Case 3:UDP"
java NetworkBenchmark 1\|2\|$((PORT)) &
java NetworkBenchmark 2\|2\|$((PORT))\|4\|4000\|0\.0\.0\.0 &>> log.txt
echo "Completed"
((PORT=PORT+1))
echo "Starting Test Case 4:UDP"
java NetworkBenchmark 1\|2\|$((PORT)) &
java NetworkBenchmark 2\|2\|$((PORT))\|8\|4000\|0\.0\.0\.0 &>> log.txt
echo "Completed"
