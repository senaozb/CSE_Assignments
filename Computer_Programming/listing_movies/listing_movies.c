#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define text "Movies.txt"

typedef struct
{
	double budget; 
	int genre;
	char name[100];
	double score;
	int year;
}movie_s;

movie_s* read(FILE *fptr, int *flag, char genre[50][20], int *counter)
{
	/*read function to get the movie info*/
	int i, ch = 0;
	const char p[2] = ",";
	char arr[200], *temp;
	movie_s *movie;
	movie = (movie_s*)malloc(sizeof(movie_s));
	
	if(fgets(arr, 200, fptr) == NULL)
		*flag = 1;
	else
	{
		temp = strtok(arr, p); /*strtok to separate the information*/
		if(strcmp(temp, "unknown") == 0)
			sscanf("-1", "%lf", &(movie->budget));
		else
			sscanf(temp, "%lf", &(movie->budget));
		
		temp = strtok(NULL, p);
		if(*counter != 0)
		{
			/*to find and store the types of genres and assign the index number to the movie struct*/
			for(i = 0; i < *counter; i++)
			{
				if(strcmp(temp, genre[i]) == 0)
				{
					movie->genre = i;
					ch = 1;
					break;
				}
			}
			if(ch == 0)
			{
				strcpy(genre[*counter], temp);
				movie->genre = *counter;
				(*counter)++;
			}
		}
		else
		{
			strcpy(genre[*counter], temp);
			movie->genre = *counter;
			(*counter)++;
		}
		
		temp = strtok(NULL, p);
		strcpy(movie->name, temp);
	
		temp = strtok(NULL, p);
		sscanf(temp, "%lf", &(movie->score));
		
		temp = strtok(NULL, p);
		sscanf(temp, "%d", &(movie->year));
	}	
	return movie;
}

void sort_func(movie_s *arr, int n, int op)
{
	/*sort function for a chosen option*/
    int i, j, min_i;
	movie_s temp;
 
    for(i = 0; i < n - 1; i++) 
	{
        min_i = i;
        for(j = i + 1; j < n; j++)
		{	
			if(op == 1)
			{
				if(arr[j].budget < arr[min_i].budget)
                	min_i = j;
			}
			else if(op == 2)
			{
				if(arr[j].genre < arr[min_i].genre)
					min_i = j;
			}
			else if(op == 3)
			{
				if(arr[j].name[0] < arr[min_i].name[0])
					min_i = j;
			}
			else if(op == 4)
			{
				if(arr[j].score < arr[min_i].score)
                	min_i = j;
			}
			else if(op == 5)
			{
				if(arr[j].year < arr[min_i].year)
                	min_i = j;
			}
		}
   	 	temp = arr[min_i];
    	arr[min_i] = arr[i];
    	arr[i] = temp;
    }
}

int main()
{
	int op, op1, svalue, evalue, check = 0, counter_st = 0, number, i, counter_gn = 0, unknownc = 0;
	int *freq;
	double average_imdb = 0, value;
	char first_arr[100], name[100];
	char genre[50][20];
	FILE *fptr;
	movie_s *s, *a;
	
	fptr = fopen(text, "rt");
	fgets(first_arr, 100, fptr);
	
	a = (movie_s*)calloc(10000, sizeof(movie_s));
	/*to get the movies from the file and save in an array*/
	while(1)
	{
		s = read(fptr, &check, genre, &counter_gn);
		if(check == 1)
		{
			number = counter_st;
			break;
		}
		else 
		{
			a[counter_st] = *s;
			if(a[counter_st].budget == -1)
				unknownc++; /*to find how many movies have unknown budget*/
			counter_st++;
		}
	}
	fclose(fptr);
	
	while(1)
	{
		printf("\n---------------------------------------\n");
		printf("1.List of the Sorted Data\n");
		printf("2.List of the Genres\n");
		printf("3.List of the Movie Through the Years\n");
		printf("4.List of the Movie Through the Scores\n");
		printf("5.All Informations of a Single Movie\n");
		printf("6.Average of the IMDB Scores\n");
		printf("7.Frequance of the Genres\n");
		printf("8.Exit\n");
		printf("---------------------------------------\n");
		printf("\nPlease select an operation:\n");
		scanf("%d", &op);
	
		switch(op)
		{
			case 1:
				do /*to get the valid inputs from user*/
				{
					printf("\n1.Budget\n");
					printf("2.Genre\n");
					printf("3.Name\n");
					printf("4.Score\n");
					printf("5.Year\n");
					printf("Please select an operation: ");
					scanf("%d", &op);
					printf("1.Single Selection\n");
					printf("2.Multiple Selection\n");
					scanf("%d", &op1);
				}
				while(op <= 0 || op > 5 || op1 <= 0 || op1 > 2);
				
				if(op1 == 1) /*for only one movie*/
				{
					printf("Enter a value: ");
					scanf("%d", &svalue);
					svalue -= 1;
			
					printf("%d.Movie:\n", svalue+1);
					sort_func(a, number, op); /*to sort for the chosen option*/
					if(op != 1)
					{	
						if(a[svalue].budget == -1)
							printf("unknown        ");
						else
							printf("%-14.0f ", a[svalue].budget);
						printf("%-12s\t %-50s\t%-4.1f %d\n\n", genre[a[svalue].genre], a[svalue].name, a[svalue].score, a[svalue].year);
					}	
					else
					{
						svalue += unknownc; /*to skip the movies with unknown budget*/
						if(a[svalue].budget == -1)
							printf("unknown        ");
						else
							printf("%-14.0f ", a[svalue].budget);
						printf("%-12s\t %-50s\t%-4.1f %d\n\n", genre[a[svalue].genre], a[svalue].name, a[svalue].score, a[svalue].year);
					}	
				}
				else if(op1 == 2) /*for multiple movies*/
				{
					printf("Enter start value: ");
					scanf("%d", &svalue);
					printf("Enter end value: ");
					scanf("%d", &evalue);
					sort_func(a, number, op); /*to sort for the chosen option*/
					svalue -= 1;
					
					if(op == 1)
					{
						printf("Movies between %d and %d, sorted by budget:\n", svalue+1, evalue);
						svalue += unknownc; /*to skip the movies with unknown budget*/
						evalue += unknownc;
						for(i = svalue; i < evalue; i++)
						{
							if(a[i].budget == -1)
								printf("unknown        ");
							else
								printf("%-14.0f ", a[i].budget);
							printf("%-12s\t %-50s\t%-4.1f %d\n\n", genre[a[i].genre], a[i].name, a[i].score, a[i].year);
						}	
					}
					else if(op == 2)
					{
						printf("Movies between %d and %d, sorted by genre:\n", svalue+1, evalue);
						for(i = svalue; i < evalue; i++)
						{
							if(a[i].budget == -1)
								printf("unknown        ");
							else
								printf("%-14.0f ", a[i].budget);
							printf("%-12s\t %-50s\t%-4.1f %d\n\n", genre[a[i].genre], a[i].name, a[i].score, a[i].year);
						}	
					}
					else if(op == 3)
					{
						printf("Movies between %d and %d, sorted by name:\n", svalue+1, evalue);
						for(i = svalue; i < evalue; i++)
						{
							if(a[i].budget == -1)
								printf("unknown        ");
							else
								printf("%-14.0f ", a[i].budget);
							printf("%-12s\t %-50s\t%-4.1f %d\n\n", genre[a[i].genre], a[i].name, a[i].score, a[i].year);
						}	
					}
					else if(op == 4)
					{
						printf("Movies between %d and %d, sorted by score:\n", svalue+1, evalue);
						for(i = svalue; i < evalue; i++)
						{
							if(a[i].budget == -1)
								printf("unknown        ");
							else
								printf("%-14.0f ", a[i].budget);
							printf("%-12s\t %-50s\t%-4.1f %d\n\n", genre[a[i].genre], a[i].name, a[i].score, a[i].year);
						}	
					}
					else if(op == 5)
					{
						printf("Movies between %d and %d, sorted by year:\n", svalue+1, evalue);
						for(i = svalue; i < evalue; i++)
						{
							if(a[i].budget == -1)
								printf("unknown        ");
							else
								printf("%-14.0f ", a[i].budget);
							printf("%-12s\t %-50s\t%-4.1f %d\n\n", genre[a[i].genre], a[i].name, a[i].score, a[i].year);
						}	
					}
				}
				
				break;
			case 2:
				for(i = 0; i < counter_gn; i++) /*to print the all types of genre*/
					printf("%s\n", genre[i]);
				break;
			case 3:
				sort_func(a, number, 5); /*sort the movies for year*/
				do
				{
					printf("Enter a year: ");
					scanf("%d", &svalue);
					printf("Until(0) or Since(1): ");
					scanf("%d", &op);
					if(op == 0 && svalue <= a[0].year) /*if the inputs are invalid then print error message*/
						printf("\nTry Again\n\n");
				}
				while(op == 0 && svalue <= a[0].year);
				
				printf("\n");
					
				/*to get the scores by looking at the chosen option*/		
				if(op == 0)
				{
					i = 0;
					while(i != number)
					{
						if(a[i].year < svalue)
							printf("%s\n", a[i].name);
						i++;
					}
				}
				else if(op == 1)
				{
					i = 0;
					while(i != number)
					{
						if(a[i].year > svalue)
							printf("%s\n", a[i].name);
						i++;	
					}
				}
				break;
			case 4:
				sort_func(a, number, 4); /*sort the movies for score*/
				do
				{
					printf("Enter a score: ");
					scanf("%lf", &value);
					printf("Less(0) or Greater(1): ");
					scanf("%d", &op);
					if(op == 0 && value <= a[0].score) /*if the inputs are invalid then print error message*/
						printf("\nTry Again\n\n");
				}
				while(op == 0 && value <= a[0].score);
				
				printf("\n");
					
				/*to get the scores by looking at the chosen option*/	
				if(op == 0)
				{
					i = 0;
					while(i != number)
					{
						if(a[i].score < value)
							printf("%s\n", a[i].name);
						i++;
					}
				}
				else if(op == 1)
				{
					i = 0;
					while(i != number)
					{
						if(a[i].score > value)
							printf("%s\n", a[i].name);
						i++;	
					}
				}
				break;
			case 5:  /*getting all information about a chosen movie*/
				printf("Please enter the name of the movie: ");
				scanf(" %[^\n]s", name);
			
				for(i = 0; i < number; i++)
				{
					if(strcmp(name, a[i].name) == 0) /*to compare the name of movies*/
					{
						if(a[i].budget == -1)
							printf("Budget: unknown\n");
						else
							printf("Budget: %.f\n", a[i].budget);	
						printf("Genre: %s\n", genre[a[i].genre]);
						printf("Name: %s\n", name);
						printf("Score: %.1f\n", a[i].score);
						printf("Year: %d\n\n", a[i].year);
						break;
					}
					
					if(i == number - 1)
						printf("There is no movie called %s\n\n", name);
				}
				break;
			case 6: /*calculation for average*/
				for(i = 0; i < number; i++)	
					average_imdb += a[i].score;
				average_imdb /= number;
				printf("Average: %f\n\n", average_imdb);
				break;
			case 7: /*calculation for frequences*/
				freq = (int*)calloc(counter_gn, sizeof(int));
				for(i = 0; i < number; i++)
					freq[a[i].genre]++; /*getting the frequence of genres*/
				for(i = 0; i < counter_gn; i++)
					printf("%s : %d\n", genre[i], freq[i]);
				free(freq);	
				break;
			case 8: /*exit to the program*/
				break;
			default: /*warning for invalid option*/
				printf("\nPlease select a valid operation:\n");
		}
		if(op == 8)
			break;
	}
	free(a);
	return 0;
}
