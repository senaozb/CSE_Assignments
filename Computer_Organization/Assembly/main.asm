# This program takes 3 arguments: divisor, length of the array and array elements respectively
# Give divisor and length individually when program asks
# Give the elements of the array as a string with spaces e.g.
# divisor = 6 
# length = 4
# array elements = 1 34 5 30


# This program takes the string and evaluates the digits one by one. 
# Converts them into integers with respect to their digit numbers ("12 2" => 12 2) and store them in an array
# In nested for loops, it takes two integers and sum and divide the result by divisor
# If it is divisible (by checking the remainder), then increments the counter by 1
# After traversing the array, prints the result


# This part allocates enough space for both string and int array
.data
buffer: .space 500 # space for the string
arr: .space 500    # space for the integer array

# Strings to print during execution
str1: .asciiz "Type the divisor (1- 100): "
str2: .asciiz "Type the array length at first (2-100): "
str3: .asciiz "Type the array elements (1-100): "
str4: .asciiz "Result = "


.text
.globl main


main:
	addi $sp, $sp, -8  # stack pointer for 2 stack registers
	sw $s1, 4($sp) 
	sw $s0, 0($sp)

	#------------GET THE INPUT----------#


	li $v0, 4 	# print the string to ask the user to type the divisor
	la $a0, str1 
	syscall	
	li $v0, 5 	# get the divisor
	syscall	
	move $s0, $v0   # move into stack
	
	
	
	li $v0, 4 	# print the string to ask the user to type the length of the array
	la $a0, str2 
	syscall	
	li $v0, 5 	# get the length
	syscall	
	move $s1, $v0   # move into stack
	
	
	li $v0, 4 	# print the string to ask the user to type the array elements
	la $a0, str3 
	syscall	
	li $v0, 8 	# get the string
	la $a0, buffer 	
	li $a1, 500	# the allocated space is 500
	syscall 
	
	#--------------------------------#
	
	li $t0, 0 		# counter is 0 to fill int array 
	li $t1, 0		# counter for string
    	addi $t2, $zero, 0  	# the sum for the integer evaluation 
    	
    	j string_parser
    
	
	
	
#-------------STRING PARSER PART------------#

string_parser:    
  	lbu $t3, buffer($t1)      # load char into t3
  	beq $t3, 10, finish       # if new-line char is encountered, then finish the string parser
 	bne $t3, 32, converter    # if it is not a space, then convert char into int
  	beq $t3, 32, store_reset  # if it is a space, then store the evaluated number and reset the sum
  	
string_parser_cont:
  	addi $t1, $t1, 1     # increment the address of the string
    	j string_parser	     # jump to the beginning 
    	
# This part converts the char to int with respect to its digit numbers
converter:
	addi $t3, $t3, -48   # convert char to int as a digit
	mul $t2, $t2, 10     # multiply sum by 10 to evaluate the digits properly
  	add $t2, $t2, $t3    # add the new digit to sum
  	j string_parser_cont
  	
store_reset: 
    	sw $t2, arr($t0)     # store int to the integer array
    	addi $t0, $t0, 4     # increment the counter by 4 
    	
    	addi $t2, $zero, 0   # reset the sum 
    	
	j string_parser_cont
	
finish:
    	sw $t2, arr($t0)     # store the int to the integer array
    	addi $t0, $t0, 4     # increment the counter by 4 
	j calculate          # jump to the calculate branch
 
#-----------------------------------------#

# Set up all variables to find pairs
calculate: 
	addi $t0, $zero, 0 	# i for the loop1
	addi $t1, $zero, 0 	# j for the loop2
	addi $t2, $zero, 0	# counter
	la $t3, ($s1)
	mul $t3, $t3, 4		# get the length multiplying by 4 for integers
	
	addi $t0, $t0, -4 	
	
	j loop1


#--------------NESTED LOOPS---------------#

loop1:
	addi $t0, $t0, 4 	# increase i by 4 
	move $t1, $t0		# assign i to j
	addi $t1, $t1, 4        # satisfy the condition i < j 
	beq $t0, $t3, exit	# exit from the loop by checking the boundary(length)
	bne $t0, $t3, loop2	# go to the inner loop if it is not equal



loop2:
	beq $t1, $t3, loop1	# exit from the inner loop
	lw $t4, arr($t0)	# get arr[i]
	lw $t5, arr($t1)	# get arr[j]
	
	add $t6, $t4, $t5	# arr[i]+arr[j]
	div $t6, $s0		# divide it by k 
	mfhi $t7		# get the remainder
	beq $t7, 0, counter	# if it is divisible, then go to counter branch
	bne $t7, 0, loop2_cont
	



loop2_cont:		
	addi $t1, $t1, 4 	# keep going in the inner loop by incrementing j by 4
	j loop2
	

#--------------------------------------#

counter:
	addi $t2, $t2, 1        # counter++
	j loop2_cont



exit:	
	addi $sp,$sp, 8		# bring the stack pointer back to the beginning
	
	li $v0, 4 
	la $a0, str4 
	syscall	
	
	move $a0, $t2		
	li $v0 1		# print the result
	syscall
	
	li $v0, 10		# exit from the program
    	syscall



