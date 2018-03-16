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



