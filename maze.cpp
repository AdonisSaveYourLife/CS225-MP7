/**
 * @file maze.cpp
 * Implementation of SquareMaze class.
 */

#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cstdlib>
#include <sys/time.h>
#include <time.h>
#include <map>
#include <math.h>
#include "maze.h"
#include "dsets.h"

using namespace std;

SquareMaze::SquareMaze ( )
{
}

void SquareMaze::makeMaze ( int width, int height )
{
	// Clear Vectors if there is already a maze present
	if ( !right.empty() )
	{
		right.clear();
	}

	if ( !down.empty() )
	{
		down.clear();
	}

	// Store the width and height of the maze
	mazeWidth = width;
	mazeHeight = height;

	// Set all walls in the maze to exist
	for ( int i = 0; i < ( width*height ); i++ )
	{
		right.push_back ( true );
		down.push_back ( true );
	}

	// Create a DisjointSets to keep track of connected cells
	DisjointSets D;
	D.addelements ( width*height );

	bool allVerticesConnected = false;
	srand ( time(NULL) );

	int edges = 0;

	/* Until there are n-1 edges in the maze ( which is the requirement
	 * for a MST, randomly find walls to delete
	 */
	while ( edges !=  (width*height) - 1 )
	{
		allVerticesConnected = true;

		int wall = rand() % (width*height);
		int neighbor = rand() % 2;

		if ( neighbor == 0 && findX (wall) != width-1 )
		{
			if ( D.find( wall ) != D.find( wall+1 ) )
			{
				setWall( findX( wall ), findY( wall ), 0, false );
				D.setunion ( wall, wall + 1);
				edges++;
			}
		}
		else if ( neighbor == 1 && findY( wall ) != height -1 )
		{
			if ( D.find( wall ) != D.find( wall + width ) )
			{
				setWall( findX( wall ), findY( wall ), 1, false );
				D.setunion( wall, wall + width );
				edges++;
			}
		}
	}
}

bool SquareMaze::canTravel ( int x, int y, int dir ) const
{
	// For each direction, check if the corresponding wall exists
	if ( dir == 0 )
	{
		if ( x == (mazeWidth-1) )
			return false;
		else if ( right[ findPos( x,y ) ] == false )
			return true;
		else
			return false;
	}

	else if ( dir == 1 )
	{
		if ( y == (mazeHeight-1) )
			return false;
		else if ( down[ findPos( x,y ) ] == false )
			return true;
		else
			return false;
	}

	else if ( dir == 2 )
	{
		if ( x == 0 )
			return false;
		else if ( right[ findPos( x-1,y ) ] == false )
			return true;
		else
			return false;
	}

	else if ( dir == 3 )
	{
		if ( y == 0 )
			return false;
		else if ( down[ findPos( x,y-1 ) ] == false )
			return true;
		else
			return false;
	}
	else
	{
		return false;
	}
}

void SquareMaze::setWall ( int x, int y, int dir, bool exists )
{
	// For each direction, set existence of wall based on exists
	if ( dir == 0 )
	{
		if ( exists == true )
		{
			right[ findPos ( x,y ) ] = true;
		}
		else
		{
			right[ findPos ( x,y ) ] = false;
		}
	}
	else
	{
		if ( exists == true )
		{
			down[ findPos ( x,y ) ] = true;
		}
		else
		{
			down[ findPos (x,y) ] = false;
		}
	}
}

vector< int > SquareMaze::solveMaze()
{
	vector< int > distance( mazeWidth*mazeHeight, 99999 );
	vector< int > previous( mazeWidth*mazeHeight, -1 );
	queue< int > Q;

	// Push all cells/vertices into the Queue
	for ( int i = 0; i < mazeWidth*mazeHeight; i++ )
	{
		Q.push( i );
	}

	// Distance from source to source = 0
	distance[ 0 ] = 0;

	// Nodes that are unoptimized are in the queue
	while ( !Q.empty() )
	{
		// Remove the vertex with the smallest distance
		int u = Q.front();
		for ( int i = 0; i < Q.size(); i++ )
		{
			if ( distance[ Q.front() ] < distance [ u ] )
			{
				u = Q.front();
			}
			Q.push( Q.front() );
			Q.pop();
		}

		while ( Q.front() != u )
		{
			Q.push( Q.front() );
			Q.pop();
		}

		Q.pop();
		
		// If all remaining vertices are inaccessible, break
		if ( distance[ u ] == 99999 )
		{
			break;
		}

		// For each neighbor of u, calculate distance to source
		vector< int > neighbors;
		vector< bool > hasTravelled( mazeWidth*mazeHeight, false );
		neighbors.clear();
		int x = findX( u );
		int y = findY( u );
		if ( canTravel( x, y, 0 ) && !hasTravelled[ findPos( x+1,y) ] )
		{
			neighbors.push_back( findPos( x+1,y ) );
		}
		if ( canTravel( x, y, 1 ) && !hasTravelled[ findPos( x,y+1 ) ] )
		{
			neighbors.push_back( findPos( x,y+1 ) );
		}
		if ( canTravel( x, y, 2 ) && !hasTravelled[ findPos( x-1,y) ] )
		{
			neighbors.push_back( findPos( x-1,y ) );
		}
		if ( canTravel( x, y, 3 ) && !hasTravelled[ findPos( x,y-1) ] )
		{
			neighbors.push_back( findPos( x, y-1 ) );
		}

		for ( int i = 0; i < neighbors.size(); i++ )
		{
			int alt = distance[ u ] + 1;
		if ( alt < distance[ neighbors[ i ] ] )
			{
				distance[ neighbors[i] ] = alt;
				previous[ neighbors[i] ] = u;
			}
		}
	}

	// Find the vertex along the bottom with the longest distance to source
	int maxDistance = 0;
	int maxDistanceVertex = 0;
	for ( int i = 0; i < mazeWidth; i++ )
	{
		if ( distance[ findPos( i, mazeHeight - 1 ) ] > maxDistance )
		{
			maxDistance = distance[ findPos( i, mazeHeight-1 ) ];
			maxDistanceVertex = findPos( i, mazeHeight-1 );
		}
	}

	// Store in reverse the directions from source to target
	vector< int > directions;

	int v = maxDistanceVertex;
	int curr = previous[ v ];
	while ( curr != -1 )
	{
		if ( curr + 1 == v )
		{
			directions.push_back( 0 );
		}
		else if ( curr + mazeWidth == v  )
		{
			directions.push_back( 1 );
		}
		else if ( curr - 1 == v )
		{
			directions.push_back( 2 );
		}
		else if ( curr - mazeWidth == v  )
		{
			directions.push_back( 3 );
		}
		v = curr;
		curr = previous[ curr ];
	}

	// Reverse the directions for the final set of directions
	reverse( directions.begin(), directions.end() );

	return directions;
}

PNG * SquareMaze::drawMaze ( ) const
{
	// Create a new PNG with dimensions based on mazeWidth and mazeHeight
	PNG* maze = new PNG( (mazeWidth*10)+1, (mazeHeight*10)+1 );
	RGBAPixel black( 0, 0, 0 );

	// Blacken entire top and left column, except the entrance
	*(*maze)( 0,0 ) = black;
	for ( int i = 10; i < (mazeWidth*10)+1; i++)
	{
		*(*maze)( i, 0) = black;
	}
	for ( int j  = 0; j < (mazeHeight*10+1); j++)
	{
		*(*maze)( 0, j) = black;
	}

	// Color walls in the maze black
	for ( int i = 0; i < mazeWidth; i++ )
	{
		for ( int j = 0; j < mazeHeight; j++ )
		{
			if ( !canTravel( i, j, 0 ) )
			{
				for ( int k = 0; k <= 10; k++ )
				{
					*(*maze)( (i+1)*10, (j*10)+k ) = black;
				}
			}
			if ( !canTravel( i, j, 1 ) )
			{
				for ( int k = 0; k <= 10; k++ )
				{
					*(*maze)( (i*10)+k, (j+1)*10 ) = black;
				}
			}
		}
	}
	return maze;
}

PNG * SquareMaze::drawMazeWithSolution ( )
{
	PNG* maze = drawMaze();
	vector< int > solution = solveMaze();
	int x = 5;
	int y = 5;
	RGBAPixel red( 255, 0, 0 );
	RGBAPixel white( 255, 255, 255 );

	// Color the path through the maze passing through the center of each cell
	for ( int i = 0; i < solution.size(); i++ )
	{
		if ( solution[i] == 0 )
		{
			for ( int j = 0; j <= 10; j++ )
			{
				*(*maze)( (x+j), y ) = red;
			}
			x = x + 10;
		}
		if ( solution[i] == 1 )
		{
			for ( int j = 0; j <= 10; j++ )
			{
				*(*maze)( x, (y+j) ) = red;
			}
			y = y + 10;
		}
		if ( solution[i] == 2 )
		{
			for ( int j = 0; j <= 10; j++ )
			{
				*(*maze)( (x-j), y ) = red;
			}
			x = x - 10;
		}
		if ( solution[i] == 3 )
		{
			for ( int j = 0; j <= 10; j++ )
			{
				*(*maze)( x, (y-j) ) = red;
			}
			y = y - 10;
		}
	}

	x -= 5;
	y += 5;

	// Color the exit white
	for ( int j = 1; j < 10; j++ )
	{
		*(*maze)( (x+j), y )  = white;
	}

	return maze;
}

/* Helper Functions */

int SquareMaze::findX ( int a ) const
{
	int x = a;
	x = ( int ) fmod ( a, mazeWidth );
	return x;
}

int SquareMaze::findY ( int a ) const
{
	int y = a;
	int count = 0;
	while ( y >= mazeWidth )
	{
		y -= mazeWidth;
		count++;
	}
	return count;
}

int SquareMaze::findPos ( int x, int y ) const
{
	int position = mazeWidth*y + x;
	return position;
}
