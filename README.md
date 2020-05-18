# SD_Assignments
Home assignments for Year 1 Sem 2 Data Structures course at the Faculty of Automatic Control and Computers

## SD1

This first assignment is a toy-implementation of a music player that uses .mp3 
files and a list of commands to create a playlist. This was done to experience 
working with Doubly Linked Linear Lists.

**List of commands:**
* Adding:
	* ADD_FIRST < song_name >
	* ADD_LAST < song_name > 
	* ADD_AFTER < song_name >
* Deleting:
	* DEL_FIRST
	* DEL_LAST
	* DEL_CURR
	* DEL_SONG < song_name >
* Moving the cursor (cursor points to playing song):
	* MOVE_NEXT
	* MOVE_PREV
* Showing:
	* SHOW_FIRST
	* SHOW_LAST
	* SHOW_CURR
	* SHOW_PLAYLIST

## SD2

This second assignment revolves arround the problem of counting distinct 
elements in large sets of data. The first part implements a basic frequency 
vector to count the number of appearances for numbers from 0 to 2000000. The 
second part makes use of the Hashtable Data Structure to store the frequency 
of different strings. The final part implements the HyperLogLog probabilistic 
algorithm to estimate the number of distinct elements with an accuracy of 
~3%. This is a simmilar procedure to the way YouTube displays it's estimated 
view count for videos and Reddit displays it's estimated view count for posts. 
The algorithm uses a hashing function to compute hashes for values in given 
dataset and uses a 11/21 split setup. This means that first 11 bits (MSB to 
LSB order) of the hash are used to determine the index for the bucket that 
will be used to store the value, value which represents the maximum of all 
the values for all the elements which will point to this bucket. For a given 
element the value to be potentially added represents the leftmost position of 
a 1 bit in the 21 bit portion that is left of the hash (positions are indexed 
from one). 

## SD3

The third assignment puts together an API that aggregates data about 
scientific papers. These papers are represented in a JSON format with only the 
most important elements such as title, authors, venue of publication, year of 
publication, fields discussed, referenced papers. These files are parsed using 
the "parson" JSON parser, available on github and stored. The API supports 
the mixing of updates and queries and allows for queries which tackle graph 
tasks such as finding the oldest influence of a paper, building and navigating 
a graph of papers linked through references or a graph of authors linked 
through their collaborations on papers. The implementation uses different 
variants of Hashtables to emulate and perform graph opperations. The 
implementation prioritised time complexity over space complexity, hence why 
a large ammount of memory is allocated. The assignment was not completed due 
to time constraints and bugs are present in 2 of 10 tasks, these tasks only 
working partially, as well as one task not being implemented yet.


