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