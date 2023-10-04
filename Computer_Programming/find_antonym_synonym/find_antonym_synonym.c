#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define text1 "antonyms.txt"
#define text2 "synonyms.txt"
#define data_f "username.worddat"

typedef struct node_a
{
	char word_a[50];
	char ant[100];
	struct node_a *next;
}node_a;

typedef struct node_s
{
	char word_s[50];
	char syn[100];
	struct node_s *next;
}node_s;

void read(node_a *a, node_s *s, int *f, FILE *fptr1, FILE *fptr2, int *count)
{
	char arr[100];
	const char s1[3] = "<>";
	const char s2[2] = "=";
    char *token;

	/*to read both two files*/
	if(fgets(arr, 100, fptr1) != NULL)
	{
		token = strtok(arr, s1);
		strcpy(a->word_a, token);
		token = strtok(NULL, s1);
		strcpy(a->ant, token);
	}
	else
		*f = -1;
		
	if(fgets(arr, 100, fptr2) != NULL)
	{
		token = strtok(arr, s2);
		strcpy(s->word_s, token);
		token = strtok(NULL, s2);
		strcpy(s->syn, token);
	}	
	else
		*f = -1;
	
	(*count)++;
	a->next = NULL;
	s->next = NULL;
}

int random_word(int *arr1, int *arr2, int count, int op)
{
	int a;
	/*to generate random numbers to choose random words from the files*/
	if(op == 1)
	{
		do
		{
			a = rand() % count;
			if(arr1[a] == 0)
			 return a; 
		}
		while(arr1[a] != 0);
	}
	else
	{
		do
		{
			a = rand() % count;
			if(arr2[a] == 0)
			 return a; 
		}
		while(arr2[a] != 0);
	}
	return 0;
}

int get_answer(char *arr, char *word)
{
	/*to find out if the word is in the file or not*/
	if(strstr(arr, word) == NULL)
		return 0;
	else
		return 1;
}

void data_write(char *name, int ct)
{
	/*to write to a binary file*/
	FILE *bf;
	bf = fopen(data_f, "ab");
	fwrite(name, 1, 50,bf);
	fwrite(&ct, sizeof(int), 1,bf);
	fclose(bf);
}

int rec_read(char *name, FILE *f, int *ct_r)
{
	/*to read from a binary file recursively*/
	char n[50];
	int res = 0, ct = 0;
	if(fread(n, 1, 50,f) != 50)
		return res;		
	
	fread(&ct, sizeof(int), 1,f);
	
	res =  rec_read(name, f, ct_r);
	if(strcmp(name, n) == 0 && res == 0)
	{
		printf("\nWelcome Back!\n");
		res = 1;
		*ct_r = ct;
	}
	return res;	
}

int data_read(char *name)
{
	FILE *bf;
	int ct = 0;
	bf = fopen(data_f, "rb");
	if(bf == NULL)
		return 0;

	rec_read(name, bf, &ct);
	fclose(bf);
	return ct;
}

int main()
{
	int flag = 0, ch, counter = 0, ind, i;
	int ant_syn_counter = 0, answer, read_counter = 0;
	int *array1, *array2;
	char ans[50], name[50];
	FILE *fptr1;
	FILE *fptr2;
	node_a *a = NULL, *arr1 = NULL, *start1;
	node_s *s = NULL, *arr2 = NULL, *start2;
	fptr1 = fopen(text1,"rt");
	fptr2 = fopen(text2,"rt");
	printf("-|-|- Welcome to the English Learning Program -|-|-\n\n");
	printf("Please enter your username:");
	scanf("%s", name);
	read_counter = data_read(name);
	while(1)
	{
		/*to read and store the words in a linked list*/
		a = (node_a*)malloc(sizeof(node_a));
		s = (node_s*)malloc(sizeof(node_s));
		read(a, s, &flag, fptr1, fptr2, &counter);
		if(flag == -1)
			break;
		if(counter == 1)
		{
			arr1 = a;
			arr2 = s;
			start1 = arr1;
			start2 = arr2;
		}
		else
		{
			arr1->next = a;
			arr2->next = s;
			arr1 = arr1->next;
			arr2 = arr2->next;
		}
	}
	fclose(fptr1);
	fclose(fptr2);

	/*for random word generator*/
	array1 = calloc(counter, sizeof(int));
	array2 = calloc(counter, sizeof(int));

	while(1)
	{
		/*to continue with the word which user has not done yet*/
		if(ant_syn_counter < read_counter)
		{
			random_word(array1, array2, counter, 1);
			ant_syn_counter++;
			continue;
		}
		else
		{	
			printf("\nGet ready for the question...\n\n");
			printf("! ! Please, enter 0 if you want to exit, enter 1 if you want to continue ! !\n");
			scanf("%d", &ch);
			if(ch == 0)
			{
				/*to store the username, number of words and finish the program*/
				data_write(name, ant_syn_counter);
				break;	
			}
			a = start1;
			s = start2;	
			
			/*to get the random word and receive the answer from the user*/
			if(ant_syn_counter % 2 == 0)
			{
				ind = random_word(array1, array2, counter, 1);
				for(i = 0; i < ind; i++)
					a = a->next;	
				printf("What is the antonym of %s\n", a->word_a);
				scanf("%s", ans);
				answer = get_answer(a->ant, ans);
			}
			else 
			{
				ind = random_word(array1, array2, counter, 2);
				for(i = 0; i < ind; i++)
					s = s->next;	
				printf("What is the synonym of %s\n", s->word_s);
				scanf("%s", ans);
				answer = get_answer(s->syn, ans);
			}
			/*after calling get_answer function, to indicate if the answer is correct*/
			if(answer == 1)
				printf("The answer is correct.\n\n");
			else
				printf("The answer is wrong.\n\n");	
			ant_syn_counter++;
		}
	}
	return 0;
}
