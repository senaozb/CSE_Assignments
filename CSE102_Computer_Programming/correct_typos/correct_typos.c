#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void find_best(char arr_list[3][100], int i, int index, char wordarr[100])
{
	int j, counter = 0;
	for(j = 0; j < i; j++)
	{
		/*to choose the word having vowel letter in the misspelled index of the word*/
		if(arr_list[j][index] == 'a' || arr_list[j][index] == 'e' || arr_list[j][index] == 'i' || arr_list[j][index] == 'u' || arr_list[j][index] == 'o')
		{
			if(counter < 2) /*to get the second occurence if there are more than one option*/
			{
				strcpy(arr_list[0], arr_list[j]);
				counter++;
			}
			else
				continue;
		}	
		
	}

	if(counter == 0)
		strcpy(wordarr, arr_list[1]); /*to get the second occurence*/	
	else 
		strcpy(wordarr, arr_list[0]);	
}

void find(char arr[100], int size, char correct_word[100], char file_ar[50])
{
	char dictionary[100];
	char listofwords[3][100];
	int check, i, sum = 0, count = 0, diff, flagofcorrection = 0;

	FILE *fptr2;
	fptr2 = fopen(file_ar, "rt");
	
	if(fptr2 == NULL)
	{
		printf("Could not open text file.\n");
		exit(1);
	}

	strcpy(correct_word, "0");
	
	while(fscanf(fptr2,"%s", dictionary) == 1)
	{	
		if(strlen(dictionary) == strlen(arr)) /*if the lengths are the same then compare the characters one by one*/
		{
			for(i = 0; i < size; i++)
			{
				check = strncmp(&arr[i], &dictionary[i], 1);
				if(check == 0)
					sum++;	
			}
			
			if(sum == size) /*if the word is correct, break the loop*/
			{
				flagofcorrection = 1;
				break;
			}
			else if(sum == size - 1) /*if there is a wrong letter in the word then find the correct version*/
			{
				if(count < 3) /*to store first 3 words matching to the misspelled word*/
				{
					for(i = 0; i < size; i++)
					{
						check = strncmp(&arr[i], &dictionary[i], 1);
						if(check != 0)
							diff = i;
					}
					strcpy(listofwords[count], dictionary);
					count++;
				}
				else /*not to store more than 3 words*/
					continue;
			}
		}

		sum = 0;
	}

	if(count > 1) /*if there are more than 1 option for the word, call find_best function*/
		find_best(listofwords, count, diff, correct_word);
	if(count == 1)	/*if not then copy the only correct version*/
		strcpy(correct_word, listofwords[0]);
	if(strcmp(correct_word, "0") == 0) /*if there is nothing wrong then return the same word*/
		strcpy(correct_word, arr);
	
	if(flagofcorrection == 1) /*to return the original word if it is in the dictionary*/
		strcpy(correct_word, arr);

	fclose(fptr2);
}

void fix_spelling_error(char file_arr1[50], char file_arr2[50])
{
	/*NOTE: This function keeps storing until it encounters a punctuation mark,
	  so dictionary file should include one letter situations e.g. r&b -> r and b or album's -> s*/
	
	int length, i, j, flag, indicator, index;
	char word[100], correct_w[100], sent_w[100], printed_w[100];
	int capital_check[100] = {0}, counter = 0;	
	FILE *fptr1;
	fptr1 = fopen(file_arr1,"r+"); 
	
	if(fptr1 == NULL)
	{
		printf("Could not open text file.\n");
		exit(1);
	}

	/*to read the file at the end of it*/
	while(fscanf(fptr1,"%s", word) == 1)
	{
		j = 0; /* sent_w counter */
		flag = 0; /* punctuation indicator*/
		indicator = 0; /* end letter indicator */
		length = strlen(word);	
		counter = 0; /*capital_check counter*/
		
		/*to make arrays empty for new words*/
		strcpy(printed_w, "0");
		strcpy(sent_w, "0");
		strcpy(correct_w, "0");
		
		for(i = 0; i <length; i++)
		{	
			if(flag == 1)
			{
				/*to store the remaining after punctuation mark in an empty array*/
				strcpy(sent_w, "0");
				j = 0;	
				flag = 0;
			}
				
		
			if(word[i] >= 65 && word[i] <= 90)
			{
				word[i] += 32; /*to convert the capital letters into lower case to compare*/
				sent_w[j] = word[i];
				j++;
				capital_check[counter] = i; /*to store the index of the capital letters*/
				counter++;
			}
			else if(word[i] >= 97 && word[i] <= 122)
			{
				sent_w[j] = word[i];
				j++;
			}
			else /*if there is a punctuation mark*/
			{
				sent_w[j] = '\0';
				length = strlen(sent_w);
				find(sent_w, length, correct_w, file_arr2); /*to find the correct version*/
				
				if(strcmp(printed_w, "0") == 0) /*if this part comes before punctuation then copy to printed_w*/
				{
					strcpy(printed_w, correct_w);
					strncat(printed_w, &word[i], 1); /*to copy the punctuation mark*/
				}
				else /*if this part comes after punctuation then append to printed_w*/
				{
					strncat(printed_w, correct_w, length);
					strncat(printed_w, &word[i], 1);
				}
		
				flag = 1; /*to make the sent_w array ready for the new part, flag is an indicator*/
				length = strlen(word);
				
				if(i == length - 1) /*to indicate if the punctuation mark is at the end of the word*/
					indicator = 1;	
				else 
					indicator = 0;	
			}
		}
		
		if(indicator == 0)
		{
			sent_w[j] = '\0';
			length = strlen(sent_w);
			find(sent_w, length, correct_w, file_arr2);
			
			if(strcmp(printed_w, "0") == 0)
				strcpy(printed_w, correct_w);
			else
				strcat(printed_w, correct_w);
		}
		else
			strncat(printed_w, "\0", 1);
		
		/*to change the letters if it is originally a capital letter*/
		for(i = 0; i < counter; i++)
		{
			index = capital_check[i];
			printed_w[index] -= 32;
		}	
			
		/*to print the words using fseek and fputs*/
		length = strlen(printed_w);
		fseek(fptr1, (-1)*length, SEEK_CUR);
		fputs(printed_w, fptr1);
		fseek(fptr1, 0, SEEK_CUR);
		
	}

	fclose(fptr1);
}

int main()
{
	/*text file names in a string*/
	char file1[50] = "text.txt";
	char file2[50] = "dictionary.txt";
	fix_spelling_error(file1,file2);
	printf("\nCorrection is done.\n");
	return 0;
}

