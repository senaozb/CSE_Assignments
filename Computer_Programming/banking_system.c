#include <stdio.h>

typedef struct
{
	char name[20];
	int age;
	int label;
}customer_s;

int check_label(int arr[5][5][2], int label, int *ind)
{
	/*to chech the numbers of labels and find a free index then return checking result and this index*/
	int i, check = 0;
	for(i = 0; i < 5; i++)
	{
		if(arr[label][i][0] != 100)
			check++;
		else
			*ind = i;	
	}
	
	return check;
}

int main()
{
	int label_array[5][5][2]= { {{100,100},{100,100},{100,100},{100,100},{100,100}},
								{{100,100},{100,100},{100,100},{100,100},{100,100}},
								{{100,100},{100,100},{100,100},{100,100},{100,100}},
								{{100,100},{100,100},{100,100},{100,100},{100,100}},
								{{100,100},{100,100},{100,100},{100,100},{100,100}} };
	int label_left_array[35][2];
	int process, i, j, check_flag;
	int counter_struct = 0, counter_label = 0, counter_process = 0;
	int index, min, process_index;
	customer_s customer[50];
	
	while(1)
	{
		printf("\n***********Banking System***********\n\n");
		printf("Current Sequence: ");
		/*----------------printing the sequence--------------------*/
		check_flag = 0;
		for(i = 0; i < 5; i++)
		{
			for(j = 0; j < 5; j++)
			{
				if(label_array[i][j][0] != 100)
				{
					printf("%d-", i);
					check_flag++;
				}
			}
		}
		for(i = counter_process; i < counter_label; i++)
		{
			printf("%d-", label_left_array[i][0]);
			check_flag++;
		}
		/*to get the option number from user*/
		printf("\n\n1-Add customer\n");
		printf("2-Process customer\n");
		printf("0-Exit\n");
		scanf("%d", &process);
		/*to perform the chosen option using switch*/
		switch(process)
		{
			case 1:
				printf("Please enter the name of the customer :");
				scanf("%s", customer[counter_struct].name);
				printf("Please enter the age of the customer :");
				scanf("%d", &customer[counter_struct].age);
				printf("Please enter the label of the customer :");
				scanf("%d", &customer[counter_struct].label);
			
				/*to check the priority limits and add the customer proper array*/
				if(customer[counter_struct].label == 0)
				{
					if(check_label(label_array, 0, &index) < 5)
					{
						label_array[0][index][0] = 0;
						label_array[0][index][1] = counter_struct;
					}
					else
					{
						label_left_array[counter_label][0] = 0;
						label_left_array[counter_label][1] = counter_struct;
						counter_label++;
					}
				}
				else if(customer[counter_struct].label == 1)
				{
					if(check_label(label_array, 1, &index) < 3)
					{
						label_array[1][index][0] = 0;
						label_array[1][index][1] = counter_struct;
					}
					else
					{
						label_left_array[counter_label][0] = 1;
						label_left_array[counter_label][1] = counter_struct;
						counter_label++;
					}
				}
				else if(customer[counter_struct].label == 2)
				{
					if(check_label(label_array, 2, &index) < 3)
					{
						label_array[2][index][0] = 0;
						label_array[2][index][1] = counter_struct;
					}
					else
					{
						label_left_array[counter_label][0] = 2;
						label_left_array[counter_label][1] = counter_struct;
						counter_label++;
					}
				}
				else if(customer[counter_struct].label == 3)
				{
					if(check_label(label_array, 3, &index) < 2)
					{
						label_array[3][index][0] = 0;
						label_array[3][index][1] = counter_struct;
					}
					else
					{
						label_left_array[counter_label][0] = 3;
						label_left_array[counter_label][1] = counter_struct;
						counter_label++;
					}
				}
				else if(customer[counter_struct].label == 4)
				{
					if(check_label(label_array, 4, &index) < 2)
					{
						label_array[4][index][0] = 0;
						label_array[4][index][1] = counter_struct;
					}
					else
					{
						label_left_array[counter_label][0] = 4;
						label_left_array[counter_label][1] = counter_struct;
						counter_label++;
					}
				}
				counter_struct++; /*to continue with a new customer*/
				break;	
			case 2:	
				if(check_flag == 0) /*to prevent if there is no one in the sequence*/
					printf("There is not any customer in bank system sequence.\n");
				else /*to let the program process the customer by giving priority*/
				{
					if(check_label(label_array, 0, &index) != 0)
					{
						min = label_array[0][0][1]; /*to find the very first customer in this label*/
						for(i = 1; i < 5; i++)
						{
							if(min > label_array[0][i][1])
							{
								min = label_array[0][i][1];
								process_index = i;
							}
						}
						label_array[0][process_index][0] = 100; /*to make free this index*/
						label_array[0][process_index][1] = 100;
					}
					else if(check_label(label_array, 1, &index) != 0)
					{
						min = label_array[1][0][1];
						for(i = 1; i < 5; i++)
						{
							if(min > label_array[1][i][1])
							{
								min = label_array[1][i][1];
								process_index = i;
							}
						}
						label_array[1][process_index][0] = 100;
						label_array[1][process_index][1] = 100;
					}
					else if(check_label(label_array, 2, &index) != 0)
					{
						min = label_array[2][0][1];
						for(i = 1; i < 5; i++)
						{
							if(min > label_array[2][i][1])
							{
								min = label_array[2][i][1];
								process_index = i;
							}
						}
						label_array[2][process_index][0] = 100;
						label_array[2][process_index][1] = 100;
					}
					else if(check_label(label_array, 3, &index) != 0)
					{
						min = label_array[3][0][1];
						for(i = 1; i < 5; i++)
						{
							if(min > label_array[3][i][1])
							{
								min = label_array[3][i][1];
								process_index = i;
							}
						}
						label_array[3][process_index][0] = 100;
						label_array[3][process_index][1] = 100;
					}
					else if(check_label(label_array, 4, &index) != 0)
					{
						min = label_array[4][0][1];
						for(i = 1; i < 5; i++)
						{
							if(min > label_array[4][i][1])
							{
								min = label_array[4][i][1];
								process_index = i;
							}
						}
						label_array[4][process_index][0] = 100;
						label_array[4][process_index][1] = 100;
					}
					else /*if there is no one in the priority sequence then continue with others*/
					{
						min = label_left_array[counter_process][1];
						counter_process++;
					}
					printf("Proceed customer is %s.\n", customer[min].name);
				}
				break;
			case 0: 
				break;	
			default:
				printf("Please enter a valid option.\n");				
		}
		if(process == 0)
			break;
	}
	return 0;
}
