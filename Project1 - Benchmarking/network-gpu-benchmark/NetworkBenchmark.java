import java.util.Random;
import java.util.Scanner;

public class NetworkBenchmark {

	public static void main(String[] args) {
		//System.out.println(args[0]);
		Scanner in = new Scanner(System.in);
		int node_type,host_port,data_size,conn_type,thread_count;
		String conn_type_str=null,host_ip=null;
		String choice_string;
		String[] choices;

		//System.out.println("-----Benchmark Simulator-----");

		//System.out.println("Enter choice string! \n Pattern:Server or client | Tcp or Udp | Port | Threads | Data[MB] | ServerIP \n Ex:1|1|52003 or 2|1|52003|2|1|0.0.0.0");
		//choice_string=in.nextLine();
		choice_string=args[0];
		choices=choice_string.split("\\|");
		//System .out.println(choices.length);
		if(choices.length>3 && choices.length<7)
		{
			node_type = Integer.parseInt(choices[0]);
			conn_type = Integer.parseInt(choices[1]);
			host_port = Integer.parseInt(choices[2]);
			thread_count = Integer.parseInt(choices[3]);
			data_size = Integer.parseInt(choices[4]);
			host_ip = choices[5];

			if(conn_type==1)
				conn_type_str="TCP";
			else
				conn_type_str="UDP";

			//System.out.println("Starting Client...");
			new ClientClass(thread_count, conn_type_str, data_size, host_ip, host_port);

		}else if(choices.length==3){

			node_type = Integer.parseInt(choices[0]);
			conn_type = Integer.parseInt(choices[1]);
			host_port = Integer.parseInt(choices[2]);

			if(conn_type==1)
				conn_type_str="TCP";
			else
				conn_type_str="UDP";

			System.out.println("Starting Server...");
			new ServerClass(host_port, conn_type_str);

		}



	}

}
