/* This program performs the game "Eight Puzzle" and the number of tiles can be increased changing N
   Level: Hard                                                                                       */

#include <stdio.h>
#include <stdlib.h>
#define N 3

typedef enum 
{left, right, up, down}
direction;

int move_left(int stx, int sty, int arr[N][N], int x, int y)
{
	int i, check = 0, temp;
	
	if(stx == x && y != 0 && y >= sty) /*to check if this movement can be performed*/
		check = 1;
	if(check == 1) /*to change the location of the numbers*/
	{
		temp = arr[stx][sty];
		for(i = sty; i < y; i++)
			arr[stx][i] = arr[stx][i+1];
		arr[x][y] = temp;
		return 1;	
	}
	else
		return 0; /*if this movement cannot be performed, then return 0*/
}
int move_right(int stx, int sty, int arr[N][N], int x, int y)
{
	int i, check = 0, temp;

	if(stx == x && y != N - 1 && y <= sty) /*to check if this movement can be performed*/
		check = 1;	
	if(check == 1) /*to change the location of the numbers*/
	{
		temp = arr[stx][sty];
		for(i = sty; i > y; i--)
			arr[stx][i] = arr[stx][i-1];
		arr[x][y] = temp;
		return 1;	
	}
	else
		return 0; /*if this movement cannot be performed, then return 0*/
}
int move_up(int stx, int sty, int arr[N][N], int x, int y)
{
	int i, check = 0, temp;
	
	if(sty == y && x != 0 && x >= stx) /*to check if this movement can be performed*/
		check = 1;
	if(check == 1) /*to change the location of the numbers*/
	{
		temp = arr[stx][sty];
		for(i = stx; i < x; i++)
			arr[i][sty] = arr[i+1][sty];
		arr[x][y] = temp;
		return 1;	
	}
	else
		return 0; /*if this movement cannot be performed, then return 0*/
}
int move_down(int stx, int sty, int arr[N][N], int x, int y)
{
	int i, check = 0, temp;
	
	if(sty == y && x != N - 1 && x <= stx) /*to check if this movement can be performed*/
		check = 1;
	if(check == 1) /*to change the location of the numbers*/ 
	{
		temp = arr[stx][sty];
		for(i = stx; i > x; i--)
			arr[i][sty] = arr[i-1][sty];
		arr[x][y] = temp;
		return 1;	
	}
	else
		return 0; /*if this movement cannot be performed, then return 0*/
}
int movement(direction d, int stx, int sty, int arr[N][N], int x, int y)
{
	int result;

	switch(d) /*to call the right function for movement*/
	{
		case right:
			result = move_right(stx,sty,arr,x,y);
			break;
		case left:
			result = move_left(stx,sty,arr,x,y);
			break;
		case up:
			result = move_up(stx,sty,arr,x,y);
			break;
		case down:	
			result = move_down(stx,sty,arr,x,y);
			break;
	}		
	return result;
}

int check_solve(int arr[N][N])
{
	int check_arr[N][N];
	int i, j, count = 1, check = 0;
	for(i = 0; i < N; i++) /*to create a solved puzzle*/
	{
		for(j = 0; j < N; j++)
		{
			check_arr[i][j] = count;
			count++;
		}
	}	
	
	for(i = 0; i < N; i++) /*to compare all numbers in the puzzle*/
	{
		for(j = 0; j < N; j++)
		{
			if(check_arr[i][j] == arr[i][j])
				check++;
		}
	}	
	
	if(check == N*N)
		check = 1;
	else
		check = 0;
		
	return check;		
}

void print_func(int arr[N][N])
{
	int i, j, k = 0, l = 0;
	for(i = -1; i <= 2*N - 1; i++)
	{
		for(j = -1; j <= 2*N - 1; j++)
		{
			if(i == -1 || i % 2 == 1) /*to print the borders with *'s */
				printf("*  ");
			else if(j == -1 || j % 2 == 1)
				printf("* ");	
			else
			{
				if(arr[k][l] == N*N)
					printf("    "); /*not to print start location number which is N*N */
				else
					printf("%2d  ", arr[k][l]);
				l++;	
			}
		}
		printf("\n");
		if(i % 2 == 0)
		{
			k++;
			l = 0;
		}
	}
}
void generate_arr(int arr[N][N])
{
	int random_num, i ,j;
	int check_arr[N*N] = {0};
	
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
			do
			{
				random_num = rand()%(N*N) + 1;
			}
			while(check_arr[random_num-1] != 0); /*to create unique random number array by checking whether this number is already taken*/
				
			check_arr[random_num-1] = 1; /*to change the flag of number to indicate that this number is taken*/
			arr[i][j] = random_num;
		}
	}	
}

int main()
{
	int num_arr[N][N];
	int x, y;
	int i, j;
	int startx, starty;
	int check, check_win;
	char input_c;
	direction input_e;
	/*to create the puzzle*/
	generate_arr(num_arr);
	
	while(1)
	{
		/*to call print function*/
		printf("\n");
		print_func(num_arr);
		
		do /*to get the correct input from user*/
		{
			printf("\n\nPlease enter the coordinates as index of array (e.g 0 0): ");
			scanf("%d %d", &x, &y);
			printf("\n\nPlease enter the direction as a character(r, l, u, d): ");
			scanf(" %c", &input_c);
		
			/*user warning part*/
			if(x < 0 || x >= N || y < 0 || y >= N)
				printf("\nThese coordinates are not valid.\n\n");
			if((input_c == 'r' || input_c == 'l' || input_c == 'u' || input_c == 'd') == 0)	
				printf("\nThis direction is not valid.\n\n");
		}
		while(x < 0 || x >= N || y < 0 || y >= N || (input_c != 'r' && input_c != 'l' && input_c != 'u' && input_c != 'd'));
		
		switch(input_c)
		{
			case 'r':
				input_e = right;
				break;
			case 'l':
				input_e = left;
				break;
			case 'u':
				input_e = up;
				break;
			case 'd':	
				input_e = down;
				break;
		}		
		
		/*to find which coordinates belongs to start location*/
		for(i = 0; i < N; i++)
		{
			for(j = 0; j < N; j++)
			{
				if(num_arr[i][j] == N*N)
				{
					startx = i;
					starty = j;
				}
			}
		}	
		
		check = movement(input_e, startx, starty, num_arr, x, y);
		if(check == 0)
		{
			printf("\nPlease, enter a valid movement!\n");
			continue;
		}
		else
		{
			check_win = check_solve(num_arr); /*to find out if user won*/
			if(check_win == 1)
			{
				printf("\n");
				print_func(num_arr);
				printf("\nYou won.\n\n");
				break;
			}
			else
				continue;	
		}	
	}
	return 0;
}
