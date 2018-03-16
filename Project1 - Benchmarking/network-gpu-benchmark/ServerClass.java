import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.UnknownHostException;

public class ServerClass {

	private int port;
	private ServerSocket curr_server_socket;
	private DatagramSocket curr_datagram_socket;
	private Socket curr_socket;
	private String server_ip;
	private InetAddress iAddress;

	ServerClass(int port, String ctype){

		iAddress = null;
		this.port = port;

		try {
			iAddress = InetAddress.getLocalHost();
			this.server_ip = iAddress.getHostAddress();
	        System.out.println("Server IP address : " +server_ip);
		} catch (UnknownHostException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

		if(ctype.equalsIgnoreCase("TCP"))
		{
			startTcpServer();
		}
		else if(ctype.equalsIgnoreCase("UDP"))
		{
			startUdpServer();
		}

		System.exit(0);

	}


	public void startTcpServer()
	{
		try {
			this.curr_server_socket = new ServerSocket(this.port,1,null);
			curr_server_socket.setReuseAddress(true);
			System.out.println("Listening on "+curr_server_socket.getLocalPort());
			this.port=curr_server_socket.getLocalPort();
			while(true)
			{
				curr_socket = curr_server_socket.accept();
				new Thread(new TcpServer(this.port,curr_socket)).start();

			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public void startUdpServer()
	{
		byte[] buf=new byte[64000];
		byte[] reply_buf=new byte[64000];
		DatagramPacket curr_packet=new DatagramPacket(buf, buf.length);
		DatagramPacket reply_packet;
		try {
			this.curr_datagram_socket = new DatagramSocket(this.port, this.iAddress);
			System.out.println("Listening on "+curr_datagram_socket.getLocalPort());
			this.port=curr_datagram_socket.getLocalPort();
			while(true)
			{
				curr_datagram_socket.receive(curr_packet);
				InetAddress nowIPAddress = curr_packet.getAddress();
                int nowport = curr_packet.getPort();
                reply_packet=new DatagramPacket(reply_buf, reply_buf.length,nowIPAddress,nowport);
				curr_datagram_socket.send(reply_packet);
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	class UdpServer implements Runnable{

		UdpServer(){

		}

		@Override
		public void run() {
			// TODO Auto-generated method stub

		}

	}

	class TcpServer implements Runnable{

		private Socket tcp_socket;
		private int tcp_port;
		private DataInputStream tcp_reader;
		private DataOutputStream tcp_writer;
		private byte[] buf;

		TcpServer(int port,Socket socket){
			this.buf=new byte[64000];
			this.tcp_port=port;
			this.tcp_socket=socket;


			try {
				this.tcp_socket.setReuseAddress(true);
				OutputStream os = tcp_socket.getOutputStream();
				this.tcp_writer = new DataOutputStream(os);
	            InputStream isr = tcp_socket.getInputStream();
	            this.tcp_reader = new DataInputStream(isr);
//	            System.out.println("Connection confirmed from "+this.tcp_port);
//	            System.out.println("Input Stream confirmed? "+this.tcp_reader);
//	            System.out.println("Output Stream confirmed? "+this.tcp_writer);
//
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

		}


		@Override
		public void run() {
			// TODO Auto-generated method stub
			int local_count=0;
			try {
				local_count = tcp_reader.read(buf);
				while(local_count>0){
				//System.out.println("Data: "+buf.toString()+" local_count "+local_count);
					try{
						tcp_writer.write(buf);
					}
					catch(Exception e)
					{
						System.out.println("Closed");
						System.out.println(local_count);
						System.out.println(tcp_writer);
						System.out.println(tcp_reader);
					}

					local_count = tcp_reader.read(buf);
				}
				//tcp_reader.close();
				//tcp_writer.close();
				tcp_socket.close();
				//System.out.println("Closed!");
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}

		}

	}


}
