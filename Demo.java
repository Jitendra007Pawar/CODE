import java.io.IOException;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.LongWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

public class Demo {

	
	public static class mapper extends Mapper<LongWritable,Text,Text,IntWritable>{
		@Override
		protected void map(LongWritable key, Text value,Context context)
				throws IOException, InterruptedException {
			String line[] = value.toString().split("\\s+");
			for(String word : line)
			{
				Text outputKey = new Text(word.toUpperCase().trim());
				IntWritable outputValue = new IntWritable(1);
				context.write(outputKey, outputValue);
			}
			
		}
	}
	
	public static class reducer extends Reducer<Text, IntWritable, Text, IntWritable>{
		@Override
		protected void reduce(Text arg0, Iterable<IntWritable> arg1,Context arg2) throws IOException, InterruptedException {
			int sum=0;
			
			for(IntWritable val:arg1)
			{
				sum += val.get();
			}
			
			arg2.write(arg0,new IntWritable(sum));
			
		}
	}
	
	public static void main(String[] args) throws Exception {

		Configuration c = new Configuration();
		
		Job j = new Job(c,"wordcount");
		j.setJarByClass(Demo.class);
		j.setMapperClass(mapper.class);
		j.setReducerClass(reducer.class);
		j.setOutputKeyClass(Text.class);
		j.setOutputValueClass(IntWritable.class);
		
		FileInputFormat.addInputPath(j,new Path(args[0]));
		FileOutputFormat.setOutputPath(j,new Path(args[1]));
		
		System.exit(j.waitForCompletion(true)?0:1);
	}

}
