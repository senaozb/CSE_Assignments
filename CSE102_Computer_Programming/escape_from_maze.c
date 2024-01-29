#include <stdio.h>

typedef enum { cell_wall, cell_free, cell_target, cell_p1, cell_p2
}cell_type;

typedef enum { move_left, move_right, move_up, move_down
}move_type;

void print_maze(cell_type maze[][8], int i, int j)
{
	/*to print the maze with recursive*/
	if(i >= 8)
        printf("\n");
    else if(j >= 8)
    {
        printf("\n");
        j = 0;
        print_maze(maze, i+1, j);
    }
    else
    {
		if(maze[i][j] == cell_wall)
			printf("***");
		else if(maze[i][j] == cell_free)
			printf("   ");
		else if(maze[i][j] == cell_p1)
			printf(" 1 ");	
		else if(maze[i][j] == cell_p2)
			printf(" 2 ");	
		else if(maze[i][j] == cell_target)
			printf(" x ");	
			
		print_maze(maze, i, j+1);		
	}
}

void current_position(cell_type maze[][8], cell_type p, int i, int j, int* a, int* b)
{
	/*to find the coordinates of the current position*/
	if(j >= 8)
    {
        j = 0;      
        current_position(maze, p, i+1, j, a, b);
    }
    else if(maze[i][j] == p)
    {
		*a = i;
		*b = j;		
	}
	else
		current_position(maze, p, i, j+1, a, b);
	
}

int maze_move(cell_type maze[][8], cell_type player, move_type move)
{
	int result=10;
	int playerx, playery;
	char m;

	/*to get the movement details from user*/
	printf("\n");
	print_maze(maze, 0,0);
	printf("\n\nPlayer %d, enter the type of movement as a character (r, l, u, d): ", player-2);
	scanf(" %c", &m);
	switch(m)
	{
		case 'r':
			move = move_right;
			break;
		case 'l':
			move = move_left;
			break;
		case 'u':
			move = move_up;
			break;
		case 'd':	
			move = move_down;
			break;	
	}
	
	/*to find the current position*/
	current_position(maze, player, 0, 0, &playerx, &playery);
	
	
	switch(move) /*to check the movement then move the player*/
	{
		case move_right:
			if(maze[playerx][playery+1] == cell_free)
			{
				result = 1;
				maze[playerx][playery+1] = player;
				maze[playerx][playery] = cell_free;
			}
			else if(maze[playerx][playery+1] == cell_target)
				result = 2;
			else
				result = 0;		
			break;
		case move_left:
			if(maze[playerx][playery-1] == cell_free)
			{
				result = 1;
				maze[playerx][playery-1] = player;
				maze[playerx][playery] = cell_free;
			}
			else if(maze[playerx][playery-1] == cell_target)
				result = 2;
			else
				result = 0;	
			break;
		case move_up:
			if(maze[playerx-1][playery] == cell_free)
			{
				result = 1;
				maze[playerx-1][playery] = player;
				maze[playerx][playery] = cell_free;
			}
			else if(maze[playerx-1][playery] == cell_target)
				result = 2;
			else
				result = 0;	
			break;
		case move_down:	
			if(maze[playerx+1][playery] == cell_free)
			{
				result = 1;
				maze[playerx+1][playery] = player;
				maze[playerx][playery] = cell_free;
			}
			else if(maze[playerx+1][playery] == cell_target)
				result = 2;
			else
				result = 0;	
			break;
	}		
	
	
	if(result == 0)/*to finish the game*/
	{
		printf("\nPlayer %d chose the wrong direction. Game over! \n\n", player-2);
		return result;
	}
	else if(result == 2)
	{
		printf("\nPlayer %d arrived to the target. Player %d won! \n\n", player-2, player-2);
		return result;
	}	
	else
	{
		if(player == cell_p1)
			player = cell_p2;
		else
			player = cell_p1;

		maze_move(maze, player, move);
	}	
}

void test_maze_move ()
{
	cell_type m;
	cell_type p = cell_p1;
	cell_type maze[8][8] = 
	{
		{cell_wall, cell_wall, cell_wall, cell_wall, cell_wall, cell_wall, cell_wall, cell_wall,},
		{cell_wall, cell_p2, cell_free, cell_free, cell_wall, cell_free, cell_target, cell_wall,},
		{cell_wall, cell_free, cell_wall, cell_free, cell_wall, cell_free, cell_wall, cell_wall,},
		{cell_wall, cell_free, cell_wall, cell_free, cell_wall, cell_free, cell_free, cell_wall,},
		{cell_wall, cell_free, cell_wall, cell_free, cell_free, cell_wall, cell_free, cell_wall,},
		{cell_wall, cell_free, cell_wall, cell_wall, cell_free, cell_wall, cell_free, cell_wall,},
		{cell_wall, cell_p1, cell_free, cell_free, cell_free, cell_free, cell_free, cell_wall,},
		{cell_wall, cell_wall, cell_wall, cell_wall, cell_wall, cell_wall, cell_wall, cell_wall,}
	};
	
	printf("\n\n----------- Maze Game ------------\n\n");
	maze_move(maze, p, m);
}

int main(void) {
	test_maze_move ();
	return (0);
} 
