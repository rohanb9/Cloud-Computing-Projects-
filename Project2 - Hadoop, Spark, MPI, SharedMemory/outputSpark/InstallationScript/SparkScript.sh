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

# ---Install Scala---
# Download scala
wget http://www.scala-lang.org/files/archive/scala-2.11.6.tgz
# Extract the Scala tar file
tar xvf scala-2.11.6.tgz
mv scala-2.11.6 /usr/local/scala
# set the following in .bashrc file
export PATH = $PATH:/usr/local/scala/bin
# reboot bashrc
source ~/.bashrc
# or $ wget http://downloads.typesafe.com/scala/2.11.6/scala-2.11.6.tgz
# Check if scala installed
scala -version

# ---Install Spark---
# Download spark
wget http://mirrors.gigenet.com/apache/spark/spark-2.2.0/spark-2.2.0-bin-hadoop2.7.tgz
# Extracting Spark tar
tar xvf spark-2.2.0-bin-hadoop2.7.tgz
mv spark-2.2.0-bin-hadoop2.7 /usr/local/spark 
# set the following in .bashrc file
export PATH=$PATH:/usr/local/spark/bin
# reboot bashrc
source ~/.bashrc
# Check if spark is installed properly or not
spark-shell

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

# ---Configure Hadoop and run spark job---
# Perform all the steps specified in the hadoop configuration to install hadoop. 
# Set hadoop configuration files with bashrc file configurations and start the hadoop using .start-all.sh or (./start-dfs.sh and ./start-yarn.sh). 
# copy input file to hdfs.
# Launch spark master and slave
cd ephemeral-hdfs/bin
bin/hadoop fs -mkdir /drive1
bin/hadoop fs -Ddfs.replication=1 -put /drive/64/in128GBfile /drive1/
bin/hadoop dfs -ls /drive1/
# Check head and tail values
head in128GBfile
tail in128GBfile

# create new scala file to run and copy below code 
vi BigDataSortSpark.scala
# Run below code using command:
spark-shell -i BigDataSortSpark.scala

# Scala code of sorting
import org.apache.spark._
import org.apache.spark.SparkContext._

object BigDataSortSpark {
    
  
    def main(p_args: Array[String]) {

      if(p_args.length < 2) {
	  show (" Very few parameters ")

      } else {
	      val inFileLoc = p_args(0)
	      val outFileLoc = p_args(1)

	      show (" --- Processing started --- ")

	      val startTime = System.currentTimeMillis()
	      
	      // Load our input data.
	      val inFile =  sc.textFile(inFileLoc)

	      val sortFileVal=inFile.map(line => (line.take(10), line.drop(10)))
	      val sortVal = sortFileVal.sortByKey()
	      val ans=sortVal.map {case (key,value) => s"$key $value"}
	      
	      // Save the output back out to a text file
	      ans.saveAsTextFile(outFileLoc)

	      val endTime = System.currentTimeMillis()

	      show ("Big data sorting time = " + (endTime - startTime) + " milisec")
      }
     
    }

   def show(showval:String) {
      // show or log the results
      println (showval)
   }
}


# run the above code by specifying the input(file stored in hdfs) and output diectory. Here, arr is the input and output file location.
BigDataSortSpark.main(arr)





