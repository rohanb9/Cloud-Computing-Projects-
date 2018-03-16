local_ip="127.0.0.1"
echo "iperf test case 1" >>log.txt
iperf -s &
iperf -c $local_ip -n 4000000K -i 1 -f m -P 1 >>log.txt
echo "iperf test case 2" >>log.txt
iperf -c $local_ip -n 2000000K -i 1 -f m -P 2 >>log.txt
echo "iperf test case 3" >>log.txt
iperf -c $local_ip -n 1000000K -i 1 -f m -P 4 >>log.txt
echo "iperf test case 4" >>log.txt
iperf -c $local_ip -n 500000K -i 1 -f m -P 8 >>log.txt
echo "Complete!!!" >>log.txt
pkill iperf