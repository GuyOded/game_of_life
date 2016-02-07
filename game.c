/*
 * game.c
 *
 *  Created on: Jan 9, 2016
 *      Author: guy
 */

#include <stdio.h>
#include "game.h"
#include "linked_list.h"
#include <time.h>

#define IN_BOUNDRIES(x) (x >= 0 && x < GRID_SIZE) ? 1 : 0

/* declarations */
static bool has_live_neighbors(char grid[][GRID_SIZE],
		Point cell, int num_live_neighbors);


static bool is_alive(char grid[][GRID_SIZE],
		Point cell);

static void init_grid(char grid[][GRID_SIZE]);

static void print_grid(char grid[][GRID_SIZE]);

static void add_neighbors_to_ressurect_list(char grid[][GRID_SIZE], List *resurrect,
		Point cell);

static void kill_doomed(char grid[][GRID_SIZE], List *live_cells);

static void set_cell_state(char grid[][GRID_SIZE], Point cell, char state);

static void update_grid(char grid[][GRID_SIZE], List *live_cells);

static void do_turn(char grid[][GRID_SIZE], List *live_cells);

static void init_live_list(char grid[][GRID_SIZE], List *live_cells);

static bool import_from_file(char grid[][GRID_SIZE], char *file_name);

/* definitions */
void play(char *file_name)  // T: not the same declaration as the header file. should be const char* file_name
{
	char grid[GRID_SIZE][GRID_SIZE];
	init_grid(grid);

	bool result = import_from_file(grid, file_name);
	if(result == false)
		return;

	List *live_cells = init_list();
	char c;

	init_live_list(grid, live_cells);

	while((c = getchar()) != 'q')
	{
		print_grid(grid);
		do_turn(grid, live_cells);
	}
	destroy(live_cells); // T: looks nice
}


/**
 * The format of the file must match the following:
 * #start file
 * 0000\n
 * 0110\n
 * 0000\n
 * 0000EOF
 * #end file
 * In the above example the size of the grid is 4x4.
 * Anyway, it is merely a simple matrix so it should be easily
 * understandable.
 * I will provide a python program that creates a text file that follows
 * the above format for any grid size.
 *
 * Any different character that isn't LIFE or DEATH will be assumed as
 * DEATH.
 *
 *
 * @param grid-current game grid
 * @param file_name-the name of the file you wish to import the grid from.
 * @return false if no error occurred while trying to open or close
 * 	       the file true otherwise.
 */
static bool import_from_file(char grid[][GRID_SIZE], char *file_name)
{
	char c;
	int row, col;

	FILE *f = fopen(file_name, "r");
	/*if cant open file return false*/
	if(f == NULL)
	{
		printf("Can't open file %s\n", file_name);
		return false;
	}

	for(row=0; row < GRID_SIZE && c != EOF; row++)
	{
		for(col=0; col < GRID_SIZE && (c = fgetc(f)) != EOF; col++)
		{
			if(c == LIFE)
				grid[row][col] = c;
		}
		/*We expect a new line character in the
		 * end of every row except for the last one*/
		if(col != GRID_SIZE || (c = fgetc(f)) != '\n')
		{
			if(row != (GRID_SIZE - 1) )
			{
				printf("The file you have provided does not match the"
						" required structure\n%d,%d\n", row, col);
				return false;
			}
		}
	}
	if(row != GRID_SIZE || col != GRID_SIZE)
	{
		printf("The file you have provided does not match the"
				" required structure\n%d,%d\n", row, col);
		return false;
	}


	return true;
}

/**
 * The function inserts all the live cells coordinates according
 * to the grid. It is necessary only at the beginning.
 *
 * @param grid- the game grid
 * @param live_cells-the list that keeps the live cells positions
 * 					 in the grid
 */
static void init_live_list(char grid[][GRID_SIZE], List *live_cells)
{
	int row, col;
	Point temp;
	for(row = 0; row < GRID_SIZE; row++)
	{
		for(col = 0; col < GRID_SIZE; col++)
		{
			temp.x = row;
			temp.y = col;
			if(is_alive(grid, temp))
			{

				add_node(live_cells, temp);
			}
		}
	}
}

/**
 * The function initializes the grid so it contains dead cells
 * and only dead cells
 * @param grid - the current game grid
 */
static void init_grid(char grid[][GRID_SIZE])
{
	int i, j;
	for(i = 0; i < GRID_SIZE; i++)
	{
		for(j = 0; j < GRID_SIZE; j++)
		{
			grid[i][j] = DEATH;
		}
	}
}


/**
 * The function merely prints the grid in its current state to the
 * standard input
 *
 * @param grid
 */
static void print_grid(char grid[][GRID_SIZE])
{
	int i, j;
	for(i = 0; i < GRID_SIZE; i++)
	{
		for(j = 0; j < GRID_SIZE; j++)
		{
			printf("%-2c", grid[i][j]);
		}
		printf("\n");
	}
}


/**
 * The function checks if a given cell is alive or dead
 *
 * @param grid
 * @param cell
 * @return true if the cell is alive or false if it is dead
 */
static bool is_alive(char grid[][GRID_SIZE],
		Point cell)
{
	if(!IN_BOUNDRIES(cell.x) || !IN_BOUNDRIES(cell.y))
	{
		return false;
	}
	return (grid[cell.x][cell.y] == LIFE) ? true : false;
}


/**
 *
 * bool has_live_neighbors
 *
 * basically the function checks whether a particular cell has a
 * specific number of live neighbors.
 *
 * @param grid - the current game grid (with all the cells and stuff)
 * @param cell - a Point representing the cell we would like to check the
 * 				 surrounding of.
 * @param num_live_neighbors - the number of live neighbors we would like
 * 		  					   the cell to have.
 * @return true if the cell has exactly num_live_neighbors live cells
 * 		   surrounding it or false otherwise
 */
static bool has_live_neighbors(char grid[][GRID_SIZE],
		Point cell, int num_live_neighbors)
{
	int row,col;
	Point neighbor;
	neighbor.x = -1;
	neighbor.y = -1;

	for(row = cell.x - 1; (row <= cell.x + 1); row++)
	{
		for(col = cell.y - 1; col <= (cell.y + 1); col++)
		{
			/* Do not perform the check on the given cell itself
			 * - that would be stupid.
			 */
			if(!(row == cell.x && col == cell.y))
			{
				neighbor.x = row;
				neighbor.y = col;
				if(is_alive(grid, neighbor))
				{
					num_live_neighbors--;
				}
			}
		}
	}
	return num_live_neighbors == 0 ? true : false;
}


/**
 * Given a live valid cell in the game grid, the function
 * resurrects all of his dead neighbor cells that have exactly
 * 3 live neighbors
 *
 * @param grid - the current game grid
 * @param resurrect - the list that contains all the cells that
 * 	    			  should be alive next round
 * @param cell - some cell in the game grid
 */
static void add_neighbors_to_ressurect_list(char grid[][GRID_SIZE], List *resurrect,
		Point cell)
{
	int row,col;
	Point neighbor;

	for(row = cell.x - 1; (row <= cell.x + 1); row++)
	{
		for(col = cell.y - 1; col <= (cell.y + 1); col++)
		{
			neighbor.x = row;
			neighbor.y = col;
			/* Do not check if the given cell itself is
			 * alive - that would be stupid.
			 */
			if(!(row == cell.x && col == cell.y) &&
					!is_alive(grid, neighbor))
			{

				neighbor.x = row;
				neighbor.y = col;
				/* if the neighbor has 3 live neighbors and is a valid
				 * cell it will be added*/
				if(has_live_neighbors(grid, neighbor, 3) &&
						!exist(resurrect, neighbor))
				{
					add_node(resurrect, neighbor);
				}
			}
		}
	}


}


/**
 * kill the cells that are lonely or in an over-populated area.
 *
 * @param grid-current game grid
 * @param live_cells-the list that contain the live cells
 */
static void kill_doomed(char grid[][GRID_SIZE], List *live_cells)
{
	Node *pos = get_first(live_cells);
	List *cells_to_kill = init_list();

	while(pos != NULL)
	{
		if(!has_live_neighbors(grid, pos->p, 2) &&
				!has_live_neighbors(grid, pos->p, 3))
		{
			if(!exist(cells_to_kill, pos->p))
				add_node(cells_to_kill, pos->p);
			pos = delete(live_cells, pos);
		}
		else
		{
			pos = get_next(pos);
		}
	}

	pos = get_first(cells_to_kill);
	while(pos != NULL)
	{
		set_cell_state(grid, pos->p, DEATH);
		pos = get_next(pos);
	}

	destroy(cells_to_kill);


}

/**
 * The function changes the state of a given cell
 *
 * @param grid-current game grid
 * @param cell-the point in the grid that represents a cell
 * @param state-a char that is either DEATH or LIFE
 */
static void set_cell_state(char grid[][GRID_SIZE], Point cell, char state)
{
	if(IN_BOUNDRIES(cell.x) && IN_BOUNDRIES(cell.y))
	{
		grid[cell.x][cell.y] = state;
	}
}

/**
 * Changes the state of all the cells in the live_cells list
 * to LIFE.
 *
 * @param grid-current game grid
 * @param live_cells-a list that contains all the living cells
 */
static void update_grid(char grid[][GRID_SIZE], List *live_cells)
{
	Node *pos = get_first(live_cells);
	while(pos != NULL)
	{
		set_cell_state(grid, pos->p, LIFE);
		pos = get_next(pos);
	}
}

/**
 * Basically a main function that calls all the other functions
 * in the correct order.
 *
 * @param grid
 * @param live_cells
 */
static void do_turn(char grid[][GRID_SIZE], List *live_cells)
{
	List *to_resurrect = init_list();
	Node *pos = get_first(live_cells);
	while(pos != NULL)
	{
		add_neighbors_to_ressurect_list(grid, to_resurrect, pos->p);
		pos = get_next(pos);
	}

	kill_doomed(grid, live_cells);
	copy_lists(to_resurrect, live_cells);
	destroy(to_resurrect);
	update_grid(grid, live_cells);
}
