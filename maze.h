/**
 * @file maze.h
 * SquareMaze Implementation
 *
 * @author Shuotian Chen
 * @date Fall 2012
 *
 */

#ifndef _MAZE_H_
#define _MAZE_H_

#include <vector>
#include "png.h"

using std::vector;

class SquareMaze
{
	public:

	/* Constructor 
	 * No-parameter constructor.
	 * 
	 * Creates an empty maze.
	 */
	SquareMaze ( );

	/* Make Maze
	 * Makes a new SquareMaze of the given height and width.
	 *
	 * If this object already represents a maze it will clear all the existing
	 * data before doing so. You will start with a square grid (like graph 
	 * paper) with the specified height and width. You will select random walls
	 * to delete without creating a cycle, until there are no more walls that
	 * could be deleted without creating a cycle. Do not delete walls on the
	 * perimeter of the grid.
	 *
	 * Hints: you only need to store 2 bits per square: the "down" and "right"
	 * walls. The finished maze is always a tree of corridors.)
	 *
	 * Parameters
	 * width: The width of the SquareMaze (number of cells)
	 * height: The height of the SquareMaze (number of cells)
	 */
	void makeMaze ( int width, int height );

	/* Can Travel
	 * This uses your representation of the maze to determine whether it is
	 * possible to travel in the given direction from the square at
	 * coordinates (x,y). 
	 *
	 * For example, after makeMaze(2,2), the possibble input coordinates will
	 * (0,0), (0,1), (1,0) and (1,1).
	 *
	 * - dir = 0 represents a rightward step (+1 to the x coordinate)
	 * - dir = 1 represents a downward step (+1 to the y coordinate)
	 * - dir = 2 represents a leftward step (-1 to the x coordinate)
	 * - dir = 3 represents an upward stemp (-1 to the y coordinate)
	 *
	 * You cannot step off of the maze or through a wall.
	 *
	 * This function will be very helpful in solving the maze. It will also
	 * be used by the grading program to verify that your maze is a tree that
	 * occupies the whole grid, and to verify your maze solution. So make sure
	 * that this function works!
	 *
	 * Parameters
	 * x: The x coordinate of the current cell
	 * y: The y coordinate of the current cell
	 * dir: The desired direction to move from the current cell
	 *
	 * Returns
	 * Whether you can travel in the specified direction
	 */
	bool canTravel ( int x, int y, int dir ) const;

	/* Set Wall
	 * Sets whether or not the specified wall exists.
	 *
	 * This function should be fast (constant time). You can assume that in
	 * grading we will not make your maze a non-tree and then call one of the
	 * other member functions. setWall should not prevent cycles from occuring,
	 * but should simply set a wall to be present or not present. Our tests
	 * will call setWall to copy a specific maze into your implementation.
	 *
	 * Parameters
	 * x: The x coordinate of the current cell
	 * y: The y coordinate of the current cell
	 * dir: Either 0(right) or 1(down) which specifies which wall to set
	 *      (same as the encoding explained in canTravel). You only need to
	 *      support setting the bottom and right walls of every square in
	 *      the grid.
	 * exists: true if setting the wall to exist, false otherwise
	 */
	void setWall ( int x, int y, int dir, bool exists );

	/* Solve Maze
	 * Solves this SquareMaze.
	 * 
	 * For each square on the bottom row (maximum y coordinate), there is a
	 * distance from the origin (i.e. the top-left cell), which is define as
	 * the length (measured as a numbber of steps) of the only path through
	 * the maze from the origin to that square.
	 *
	 * Select the square in the bottom row with the largest distance from the
	 * origin as the destination of the maze. solveMaze(0 returns the winning
	 * path from the origin to the destination as a vector of integers, where
	 * each integer represents the direction of a step, using the same
	 * encoding as in canTravel().
	 *
	 * If multiple paths of maximum length exists, use the one with the
	 * destination cell that has the smallest x value.
	 *
	 * Hint: this function should run in time linear in the number of cells
	 * in the maze.
	 *
	 * Returns
	 * A vector of directions taken to solve the maze
	 */
	vector< int > solveMaze ( );

	/* Draw Maze
	 * Draws the maze without the solution.
	 *
	 * First, create a new PNG> Set the dimensions of the PNG to
	 * (width*10+1, height*10+1), where height and width were the arguments
	 * to makeMaze. Blacken the entire topmost row and leftmost column of
	 * pixels, except the entrance (1,0) through (9,0). For each square in the
	 * maze, call its maze coordinates (x,y). If the right wall exists, then
	 * blacken the pixels with coordinates ((x+1)*10, y*10+k) for k from 0 to
	 * 10. If the bbottom wall exists, the blacken the pixels with coordinates
	 * (x*10+k, (y+1)*10) for k from 0 to 10.
	 *
	 * THe resulting PNG will look like the sample image, except there will be
	 * no exit from the maze and the red line will be missing.
	 *
	 * Returns
	 * A PNG of the unsolved SquareMaze
	 */
	PNG * drawMaze ( ) const;

	/* Draw Maze With Solution
	 * This function calls drawMaze, the solveMaze; it modifies the PNG from
	 * drawMaze to show the solution vector and the exit.
	 *
	 * Start at pixel (5,5). Each direction in the solution vector corresponds
	 * to a trail of 11 red pixels in the given direction. If the first step
	 * is downward, colro pixels (5,5) through (5,15) red. (Red is 255,0,0 in
	 * RGB). The if the second step is right, color pixels (5,15) through
	 * (15, 15) red. Then if the third step is up, color pixels (15, 15)
	 * through (15, 5) red. Continue in this manner until you get to the end
	 * of the solution vector, so that your output looks analogus the above
	 * picture.
	 *
	 * Returns
	 * A PNG of the solved SquareMaze
	 */
	PNG * drawMazeWithSolution ( );

	private:
	
	/* Representation of the Maze
	 * mazeWidth represents the width of the maze in cells
	 * mazeHeight represents the height of the maze in cells
	 * right keeps track of whether the right walls of each cell exists
	 * down keeps track of whether the bottom walls of each cell exists
	 */
	int mazeWidth;
	int mazeHeight;

	vector< bool > right;
	vector< bool > down;

	/* Helper Functions */

	/* findX locates the x coordinate of a cell given its number */
	int findX ( int a ) const;

	/* findY locates the y coordinate of a cell given its number */
	int findY ( int a ) const;

	/* findPos locates the cell number given its x and y coordinates */
	int findPos ( int x, int y ) const;
};

#endif

