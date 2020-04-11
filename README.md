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
+/-10%. This is a simmilar procedure to the way YouTube displays its estimated 
view count for videos.

## SD3
