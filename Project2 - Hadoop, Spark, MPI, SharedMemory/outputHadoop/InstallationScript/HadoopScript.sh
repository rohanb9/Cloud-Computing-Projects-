# ---Install basic packages---
sudo apt-get install ssh
sudo apt-get install rsync
sudo apt-get install ant
sudo apt-get update

# ---Install Java on your machine---
sudo apt-get install default-jdk 
# Check the java is installed or not
java -version 
# check java location and set the JAVA_HOME based on location in .bashrc file
# add following line in bashrc file
export JAVA_HOME=/usr/lib/jvm/java-1.8.0-openjdk-amd64
# reboot bashrc
source ~/.bashrc

# ---Mount EBS volume (More Details : https://devopscube.com/mount-ebs-volume-ec2-instance/)---
# To check space disk used\left on EC2 instance
df -h
# List the available disks
lsblk
# Check if the volume has any data. 
sudo file -s /dev/nvme0n1
# Create directory where mount the EBS
sudo mkdir drive
# mount EBS
sudo mkfs -t ext4 /dev/nvme0n1
sudo mount /dev/nvme0n1 drive
sudo chmod 777 drive
#The permissions in numeric (octal) format:
stat -c %a directory

# ---Install gensort and generate input data---
sudo wget www.ordinal.com/try.cgi/gensort-linux-1.5.tar.gz
sudo tar -xzvf gensort-linux-1.5.tar.gz
cd 64
sudo ./gensort -a 1280000000 in128GBfile #Same way we can create 1 TB of data

# ---Configure Hadoop and hadoop job---
# Perform following steps specified for hadoop configuration to install hadoop. 
# Set hadoop configuration files with bashrc file configurations and start the hadoop using .start-all.sh or (./start-dfs.sh and ./start-yarn.sh). 
# To stop cluster use .stop-all.sh 
# Launch master and slave
# Copy all the configuration files from local system to the EC2 
scp -i roh.pem -r roh ubuntu@ec2-13-58-78-191.us-east-2.compute.amazonaws.com:./ 
scp -i hadoopFiles -r roh ubuntu@ec2-13-58-78-191.us-east-2.compute.amazonaws.com:./ 
chmod 400 roh.pem

# Download and install hadoop
wget http://www-us.apache.org/dist/hadoop/common/hadoop-2.7.4/hadoop-2.7.4.tar.gz
tar -xzvf hadoop-2.7.4.tar.gz
chmod 777 hadoop-2.7.4

# Do hadoop configs in bashrc files as follows.
vi .bashrc
export CONF=/home/ubuntu/hadoop-2.7.4/etc/hadoop
export JAVA_HOME=/usr/lib/jvm/java-1.8.0-openjdk-amd64
export PATH=$PATH:$/home/ubuntu/hadoop-2.7.4/bin
export PATH=${JAVA_HOME}/bin:${PATH}
export HADOOP_CLASSPATH=${JAVA_HOME}/lib/tools.jar
source ~/.bashrc

# create new java file to run sort code 
vi BigDataSort.java

# Generate jar file
cd hadoop-2.7.4
bin/hadoop com.sun.tools.javac.Main BigDataSort.java
jar cf BigDataSort.jar BigDataSort.class

# Replace existing configuration files with the files specified in hadoopFiles
cd hadoop-2.7.2/etc/hadoop
# Replace  core-site.xml, hdfs-site.xml, yarn-site.xml, hosts, slaves
# Set JAVA_HOME parameter with java location in hadoop-env.sh
set JAVA_HOME=/usr/lib/jvm/java-1.8.0-openjdk-amd64

# SSH configuration.. Run these on all nodes. Also, masters DNS on all the slaves machine.
eval "$(ssh-agent)"
ssh-add roh.pem
ssh-keygen -t rsa
ssh-copy-id -i ~/.ssh/id_rsa.pub ubuntu@ec2-13-58-78-191.us-east-2.compute.amazonaws.com
chmod 0600 ~/.ssh/authorized_keys

# Format name node and start hadoop
cd hadoop-2.7.4/
bin/hadoop namenode -format
ssh localhost 
cd sbin
./start-all.sh 

# Check status of hadoop where we can see all services running or not
jps

# Copy file to hdfs
cd hadoop-2.7.4/
bin/hadoop dfs -copyFromLocal /drive/64/in128GBfile /drive1

# Run hadoop program
bin/hadoop jar BigDataSort.jar BigDataSort /drive1/in128GBfile /drive1/output


# To stop hadoop 
./stop-all.sh 

# Check head and tail values
head in128GBfile
tail in128GBfile


# Java code of sorting
package com.bigdata.sort;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

import java.io.IOException;
import java.util.logging.FileHandler;
import java.util.logging.Logger;
import java.util.logging.SimpleFormatter;


public class BigDataSort {
		
	public BigDataSort() {
		super();
	}
	
	public static void executeJob() {
		Path inputpath = new Path(args[0]);
		Path outputpath = new Path(args[1]);
		
		Configuration conf = new Configuration();
		Job job = new Job(conf, "Big Data Sort");
		job.setJarByClass(BigDataSort.class);
		
		// set mapper and reducer classed
		job.setMapperClass(SortMapperClass.class);
		job.setCombinerClass(SortReducerClass.class);
		job.setReducerClass(SortReducerClass.class);
		
		// set output key and value class
		job.setOutputKeyClass(Text.class);
		job.setOutputValueClass(Text.class);
		
		FileInputFormat.addInputPath(job, inputpath);
		FileOutputFormat.setOutputPath(job, outputpath);
		
	}

	public static void main(String args[]) {
		
		try {
			
			if(args.length < 2) {
				 System.err.println("Very few parameters.");
				 System.exit(-1);
			}
			
			long start_time = System.currentTimeMillis();
			
			// Execute job
			executeJob();
			
			long end_time = System.currentTimeMillis();
			long time_in_sec = (endTime-startTime)/1000;
			
			System.out.println(" Total time of execution : " + time_in_sec);
			if(job.waitForCompletion(true)) {
				System.exit(0);
			} else {
				System.exit(1);
			}
		} catch(Exception e) {
			System.out.println("Error occurred: "+ e.getMessage());
			e.printStackTrace();
		}

	}
	

}

class SortMapperClass extends Mapper<Object, Text, Text, Text> {

	private Text firstParam = new Text();
	private Text secParam = new Text();

	public void map(Object key, Text value, Context context) throws IOException, InterruptedException {

		String l = value.toString();
		firstParam.set(l.substring(0, 10));
		secParam.set(l.substring(10));
		context.write(firstParam, secParam);

	}
}

class SortReducerClass extends Reducer<Text, Text, Text, Text> {

	private Text firstParam = new Text();
	private Text secParam = new Text();

	public void reduce(Text key, Text value, Context context) throws IOException, InterruptedException {
		firstParam.set(key.toString() + value.toString());
		secParam.set("");
		context.write(firstParam, secParam);
	}
}





