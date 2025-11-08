<H1> Project 2 - Health Determinant </h1>
This program takes in an excel spread sheet of medical information for individuals, converts it, and sorts it into a hashmap of factors. These factors are gender, smoker, alcohol, at risk, and family risk. Each bucket represents a yes or no/ male or female answer to each of these factors, thus there are 10 buckets total.
every bucket has a vector and foward list of the person struct and a risk() method that calculates a risk value given their information. The risk is evaluated based off a healthy value and the quantity that the individual's stat deviates from the ideal value, multiplied by a weight. Every bucket except for male and female only have a vector while gender has a forward list as well, for comparison using quick sort and merge sort. This program has a few commands for the user that utilize quick sort and merge sort for their risk value. Any commands by the program do not affect the csv file.
<p> </p>

The program upon runnning will give a short summary and instructions on using the commnands.

<h2> Commands </h2>
For each command, you intially type in the respective command, and then it will prompt you for the id, and any other necessary information sequentially.
If your wish to cancel your current command you can do so by typing in -1 at any point except when being asked for yes/no or male/female.
If an invalid id or value is inputed, the program will prompt you to re-enter that value.
<p>  </p>
<p>   </p> 
Insert
	- Insert will ask for a valid id, greater than 0, and not already present, then ask all the neccesary questions to fill the medical values. Insert will determine also determine which buckets the individual belongs in, and pushes them into each bucket.
<p>  </p>
<p>  </p>
Customize
	- Customize will take in a valid id, greater than 0, and already present, then ask for all neccesary questions to fill the medical values. Customize will delete the Person objects in each bucket of the same id with the old values, and then insert the new Person object into the same buckets.
	<p> </p>
	<p> </p>
Delete
	- delete will take in a valid id, i.e. an id greater than 0 and already present, and search the male and female bucket to see if they exist. male and female are the first to check since they are guranteed to belong in one of them. once we confirm they exist, we collect the int values of which buckets they will also be in, and also delete them from those other buckets as well.
	<p>  </p>
	<p>  </p>
Display
	- display takes in a valid id, and then displays their complete information.
	<p>  </p>
	<p>  </p>
Nhighest
	- Nhighest uses quicksort (and mergesort if in gender buckets) to get the n highest risk-value individuals of a given factor. The command completes the sorts by utilizing the risk-value. The command then prints out the ids and associated risk-value.
	<p>  </p> 
	<p>  </p>
Nlowest
	- Nlowest uses quicksort (and mergesort if in gender buckets) to get the n lowest risk-value individuals of a given factor. The command completes the sorts by utilizing the risk-value. The command then prints out the ids and associated risk-value.
	<p>  </p>
	<p>  </p>
Help
	- Help just re-displays the command menu and the same command instructions.
	<p>  </p>
	<p>  </p>
Exit
	- exit just exits the program.
