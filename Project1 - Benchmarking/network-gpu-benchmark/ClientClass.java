import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.concurrent.ConcurrentHashMap;


public class ClientClass {

	private List<Thread> thread_base;
	private static Map<Long,Double> thread_times;
	private int data_size;
	private String host_name;
	private int port;
	private int packet_count;
	private int tsize=0;

	ClientClass(int thread_count, String ctype, int size, String host_name, int port)
	{

		this.data_size=size;
		this.port=port;
		this.host_name=host_name;
		ClientClass.thread_times = new ConcurrentHashMap<Long,Double>();
		this.thread_base = new ArrayList<Thread>();

		if(ctype.equalsIgnoreCase("TCP"))
		{
			startTcpClient(thread_count, thread_base, this.data_size, this.host_name, this.port);

		}
		else if(ctype.equalsIgnoreCase("UDP"))
		{
			startUdpClient(thread_count, thread_base, this.data_size, this.host_name, this.port);
		}
	}


	public void startTcpClient(int thread_count,List<Thread> thread_base, int size, String host_name, int port)
	{
		for(int i=0;i<thread_count;i++)
		{
			tsize = (int)Math.ceil(size/thread_count);
			Thread curr_thread=new Thread(new TcpClient(tsize,host_name,port));
			curr_thread.start();
			thread_base.add(curr_thread);
		}
		//System.out.println("thread_base.size() "+thread_base.size());
		for(Thread each_thread:thread_base)
		{
			try {
				//System.out.println("each_thread.join() execution");
				each_thread.join();
				//System.out.println("each_thread.join() executed");
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}

		displayStats();
	}

	public void startUdpClient(int thread_count,List<Thread> thread_base, int size, String host_name, int port)
	{
		if(host_name.equals("0.0.0.0") || host_name.equals("localhost"))
		{
			try {
				host_name=InetAddress.getLocalHost().getHostAddress();
			} catch (UnknownHostException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		for(int i=0;i<thread_count;i++)
		{
			tsize = (int)Math.ceil(size/thread_count);
			Thread curr_thread=new Thread(new UdpClient(tsize,host_name,port));
			curr_thread.start();
			try {
				curr_thread.join();
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			thread_base.add(curr_thread);
		}


		displayStats();
	}

	public double threadSum(Map<Long,Double> thread_set)
	{
		double threadTotalTime=0;
		for(Double eachTime:thread_set.values())
		{
			threadTotalTime=threadTotalTime+eachTime.doubleValue();
		}

		return threadTotalTime;
	}

	public void displayStats()
	{
		System.out.println("--------------------------------------------------");
		System.out.println("Port "+this.port+" Ip: "+this.host_name+" Threads "+thread_base.size()+" Data(MB): "+this.data_size);
		//System.out.println("threadSum "+threadSum(ClientClass.thread_times));
		//System.out.println("threadbase "+thread_base.size()+" data_size "+data_size);
		System.out.println("Throughput for 64K packets (Mbits/Sec): "+(data_size*2*8)/(double)(threadSum(ClientClass.thread_times)/1000000000));
		System.out.println("Latency for 64K packets (milliSeconds): "+(threadSum(ClientClass.thread_times)/1000000)/(data_size*2));
		//}
		System.exit(0);
	}

	class UdpClient implements Runnable{
		private int packet_size;
		private String host_name;
		private int port;
		private int packet_count;
		private InetAddress inet_addr;
		private Socket socket;
		private DataInputStream tcp_reader;
		private DataOutputStream tcp_writer;
		private byte[] packet_chunk;
		private DatagramSocket clientSocket;
		private DatagramPacket sendPacket;
		private DatagramPacket receivePacket;
		private byte[] receive_buf;

		UdpClient(int packet_size, String host_name, int port){
			this.packet_size=packet_size;
			this.host_name=host_name;
			this.port=port;
			this.packet_count= this.packet_size*16;
			this.packet_chunk= byteGenerator(64000);
			this.receive_buf = new byte[64000];

			try {
				this.inet_addr=InetAddress.getByName(this.host_name);
				this.sendPacket = new DatagramPacket(this.packet_chunk, this.packet_chunk.length, this.inet_addr, this.port);
				this.receivePacket = new DatagramPacket(this.receive_buf, this.receive_buf.length);
				this.clientSocket = new DatagramSocket();

			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}

		@Override
		public void run() {
			// TODO Auto-generated method stub
			//System.out.println("Thread ID launch "+Thread.currentThread().getId());
			double startTime,execDuration;
			long threadId;
			try
			{
				//System.out.println("Packet count "+this.packet_count);
				startTime = System.nanoTime();
				for(int i=0;i<this.packet_count;i++)
				{
					this.clientSocket.send(this.sendPacket);
					//System.out.println("Packet count current"+i+"Sent");
					//this.receivePacket = new DatagramPacket(this.receive_buf, this.receive_buf.length);
					this.clientSocket.receive(this.receivePacket);
					//System.out.println("Packet count current"+i+"Receiving");

				}
				execDuration = System.nanoTime() - startTime;
//				tcp_writer.flush();
				this.clientSocket.close();
				this.clientSocket.disconnect();
				threadId = Thread.currentThread().getId();
				//System.out.println("threadId "+threadId+" execDuration "+execDuration);
				ClientClass.thread_times.put(threadId, execDuration);
				//System.out.println("threadId "+threadId+" execDuration Completed"+execDuration);
			}
			catch(Exception e)
			{
				System.out.println(e);
			}


		}

		public byte[] byteGenerator(int len)
		{
			Random rnd = new Random();
			byte[] nbytes = new byte[len];
			rnd.nextBytes(nbytes);
			return nbytes;
		}

	}

	class TcpClient implements Runnable{
		private int packet_size;
		private String host_name;
		private int port;
		private int packet_count;
		private InetAddress inet_addr;
		private Socket socket;
		private DataInputStream tcp_reader;
		private DataOutputStream tcp_writer;
		private byte[] packet_chunk;

		TcpClient(int packet_size, String host_name, int port){
			this.packet_size=packet_size;
			this.host_name=host_name;
			this.port=port;
			this.packet_count= this.packet_size*16;
			this.packet_chunk= byteGenerator(64000);

			try {
				this.inet_addr=InetAddress.getByName(this.host_name);
				this.socket=new Socket(this.inet_addr,this.port);
				this.socket.setReuseAddress(true);
				InputStream isr = this.socket.getInputStream();
				tcp_reader=new DataInputStream(isr);
				tcp_writer=new DataOutputStream(this.socket.getOutputStream());
	            //System.out.println("Input Stream confirmed? "+this.tcp_reader);
	            //System.out.println("Output Stream confirmed? "+this.tcp_writer);
			} catch (UnknownHostException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

		}

		public byte[] byteGenerator(int len)
		{
			Random rnd = new Random();
			byte[] nbytes = new byte[len];
			rnd.nextBytes(nbytes);
			return nbytes;
		}

		@Override
		public void run() {
			// TODO Auto-generated method stub
			double startTime,execDuration;
			long threadId;
			byte[] buf = new byte[64000];
			try
			{
				//System.out.println("Packet count "+this.packet_count);
				startTime = System.nanoTime();
				for(int i=0;i<this.packet_count;i++)
				{
					//System.out.println("Packet count current"+i);
					tcp_writer.write(this.packet_chunk);
					tcp_reader.read(buf);
				}
				execDuration = System.nanoTime() - startTime;
				tcp_writer.flush();
				this.socket.shutdownOutput();
				threadId = Thread.currentThread().getId();
				//System.out.println("threadId "+threadId+" execDuration "+execDuration);
				ClientClass.thread_times.put(threadId, execDuration);

			}
			catch(Exception e)
			{
				System.out.println(e);
			}


		}

	}


}
