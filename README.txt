TODO: GUI in c++

Algorithm - Basically I keep track of the live cells of every generation and check each live cell surroundings. That means
			count how many live neighbors it got and how many live neighbors each one of his neighbors got. Simple enough.

The only function that concern the user is play(file_name). It recieves the file name that contains the grid.

I made a small python program that creates a grid file with a size of your choosing. You can then edit it with your life pattern
and run the play function with this file. Note that your grid must match the LIFE DEATH and GRID_SIZE symbolic constants or else the program will run incorrectly or exit with an error. To use the python program run:
python grid_creator.py <death character> <grid size>
and it will create a file named grid.txt in the current dirrectory.
Thats about it.

BTW I thought of using a much simpler algorithm, but this one seems cooler.
			
