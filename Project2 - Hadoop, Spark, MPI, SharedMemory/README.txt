------- Hadoop ------

Source code to run the hadoop job is written in java which is BigDataSort.java

Required installation version:
Linux version:- Ubuntu 16.04 LTS 
Java Version:- java-1.8.0
Hadoop Version:- hadoop-2.7.4
Scala version: scala-2.11.6 
Spark Version:- spark-2.2.0


Files required to onfigure:
	 - Mapred-site.xml: The mapred-site.xml file contains the configuration settings for MapReduce daemons. The job tracker and the task-trackers.
	 - hdfs-site.xml: It contains the configuration settings for HDFS daemons.
    - mapred-site.xml: It contains the configuration settings for MapReduce daemons.
    - slaves: The ‘slaves’ file at Master node contains a list of hosts, one per line, that are to host Data Node and Task Tracker servers.
    - masters: This file informs about the Secondary Namenode location to hadoop daemon. The ‘masters’ file at Master server contains a hostname Secondary Name Node servers.
    - hadoop-env.sh: Setting up environment variables.


To run the hadoop job please follow instructions specified in hadoopScript.sh

------- Spark ------

1. Source code to run the spark job is written in scala which is BigDataSortSpark.scala

2. To run the spark job please follow instructions specified in sparkScript.sh
   Main steps:
   		- Install basic packages
   		- Install Java, scala and spark on your machine
   		- Mount EBS 
   		- Generate input data
   		- Do hadoop and spark configurations
   		- Run the code (BigDataSortSpark.scala)
   		- Check output

3. Hadoop configuration files remain same for spark configurations.


---- shared memory ----

Share Memory Running Steps:

#Connect to amazon instance
ssh -i "i3large.pem" ubuntu@ec2-52-91-6-142.compute-1.amazonaws.com

#Steps to install java
sudo apt-add-repository ppa:webupd8team/java -y
sudo apt-get install oracle-java8-installer -y
sudo apt-get update

#step to install gensort
wget www.ordinal.com/try.cgi/gensort-linux-1.5.tar.gz
tar -xzvf gensort-linux-1.5.tar.gz
cd 64
./gensort -a 1280000000 test

#copy test file from folder 64 to home
cd
cp 64/test ~/.

#transfer file to AWS EC2 fromlocal machine
ssh -i "i3large.pem" sharememory.jar ubuntu@ec2-52-91-6-142.compute-1.amazonaws.com:~/.

#make sure that generated file name is test and its location is same as sharememory.jar

#Now run jar file
java -jar sharememory.jar


---- MPI ----

#MPI 

sudo apt-get update
sudo apt-get upgrade
sudo apt-get install python-dev
sudo apt-get install python-setuptools
sudo apt-get install build-essential
sudo easy_install StarCluster

#Configuring StartCluster
starcluster help
2

vi ~/.starcluster/config

#enter following details
AWS_ACCESS_KEY_ID = # Your Access Key ID here
AWS_SECRET_ACCESS_KEY = # Your Secret Access Key here
AWS_USER_ID = # Your 12-digit AWS Account ID here (no hyphens)

starcluster createkey mykey -o ~/.ssh/mykey.rsa

vi ~/.starcluster/config

#change CLUSTER_SIZE and  NODE_INSTANCE_TYPE

#Enable mpich2 plugin for Starcluster (http://star.mit.edu/cluster/docs/0.93.3/plugins/mpich2.html)

#start cluster
starcluster start mpicluster

#Connecting master node
starcluster sshmaster mycluster

su - sgeadmin

emacs ~/hosts
#add host and node details like following
master
node001


vi mpi.c

#Copy mpi code in mpi.c file

#compile mpi code
mpicc mpi.c -o mpi

#run mpi code
mpirun.mpich2 -np 10 -f hosts ./mpi 








 




 