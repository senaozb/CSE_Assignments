# ----------- QUESTION 1 -------------- #
#These functions are for both a and b
#These are for checking vertex coming before the current vertex
def check(graph, vertex, checklist, visited):
    for v in graph:
        if v is not vertex and v not in visited:
            if vertex in graph[v]:
                checklist.append(v)
                check(graph, v, checklist, visited)
                
def appendcheck(checklist, visited):
    for i in range(len(checklist)):
        x = checklist.pop()
        visited.append(x)
        print(x, end = " -> ")

# ----------- QUESTION 1 A -------------- #
#This algorithm uses DFS 
def q1a(graph, vertex):
    stack = []
    visited = []
    checklist = []
    
    stack.append(vertex)
    
    while len(stack) != 0: 
        newvertex = stack.pop()
        if newvertex not in visited:
            check(graph, newvertex, checklist, visited)
            appendcheck(checklist, visited)
            print(newvertex, end = " -> ")
            visited.append(newvertex)
            for adjacent in graph[newvertex]:
                if adjacent not in visited:
                    stack.append(adjacent)
    
        
# ----------- QUESTION 1 B -------------- #
#This algorithm uses BFS
def q1b(graph, vertex):
    queue = []
    visited = []
    checklist = []
    
    queue.append(vertex)
    visited.append(vertex)
 
    while queue:
        newvertex = queue.pop(0) 
        check(graph, newvertex, checklist, visited)
        appendcheck(checklist, visited)
        print (newvertex, end = " -> ")
        
        for adjacent in graph[newvertex]:
            if adjacent not in visited:
                visited.append(adjacent)
                queue.append(adjacent)


# ------------------------------------- #

# ----------- QUESTION 2 -------------- #

def q2(a, n):
    if n == 0:
        result = 1
    else:
        result = divide(a, 1, n)
    
    print(result)
    
'''If the power is even, then divide the multiplication sequence into 2 and call both 
sides recursively and multiply the results
   If it is odd, then divide it into 2 and call recursively and multiply the results 
but in addition to results, multiply it by the number itself'''
def divide(a, start, end):
    if start == end:
        return a
    
    evenflag = False
    if end % 2 == 0:
        evenflag = True

    mid = int((start + end) / 2)
    
    if evenflag == True:
        res = divide(a, start, mid)
        return res * res
    else:
        res = divide(a, start, mid-1) 
        return a * res * res

# ------------------------------------- #

# ----------- QUESTION 3 -------------- #

 
#Print the sudoku game
def print_arr(arr):
    for i in range(9):
        for j in range(9):
            print(arr[i][j], end = " ")
        print()
                
#Try all possible numbers and verify if it is the right number with recursive calls
def find_num(arr, r, c):
    for i in range(1,10):
        if valid_location(arr, r, c, i):
           arr[r][c] = i 
           if solve(arr):
                return True
        #If it is not the right number, try with another number
        arr[r][c] = 0
            
    return False

#Call validation functions for row, column and 3x3 squares
def valid_location(arr, r, c, num):
    row = valid_row(arr, r, num)
    col = valid_col(arr, c, num)
    
    #Find the boundary of 3x3 square
    if r < 3 : 
        sq_r = 3
    elif r < 6:
        sq_r = 6
    else: 
        sq_r = 9
        
    if c < 3 : 
        sq_c = 3
    elif c < 6:
        sq_c = 6
    else: 
        sq_c = 9
        
    square = valid_square(arr, sq_r, sq_c, num)
    
    if row and col and square:
        return True

    return False

#Check if the row already has the given number
def valid_row(arr, r, num):
    for i in range(9):
        if arr[r][i] == num:
            return False
    return True
    
#Check if the column already has the given number
def valid_col(arr, c, num):
    for i in range(9):
        if arr[i][c] == num:
            return False
    return True
    
#Check if 3x3 square already has the given number
def valid_square(arr, r, c, num):
    start_r = r - 3
    start_c = c - 3
    for i in range(3):
        for j in range(3):
            if arr[start_r + i][start_c + j] == num:
                return False   
    return True

#If the given coordinates are still valid, then check if it is zero (empty)
#If so, then find the correct number for that cell
#If not zero, then continue with the next cell
def solve(arr):
    for r in range(9):
        for c in range(9):
            if arr[r][c] == 0:
                return find_num(arr, r, c)
            
    return True
                

def q3(arr):
    if solve(arr):
        print_arr(arr)
    else:
        print("No Solution")


#Drivers-----------------------------

print("\n\n")
print("----------- QUESTION 1 A -------------")
print("The order: ")
def driver1a():
    graph = {   "CSE102" : ["CSE241"],
                "CSE241" : ["CSE222"],
                "CSE222" : ["CSE321"],
                "CSE321" : ["CSE422"],
                "CSE211" : ["CSE321"],
                "CSE422" : []}
    q1a(graph, "CSE102")
    
driver1a()
        
print("\n\n")
print("----------- QUESTION 1 B -------------")
print("The order: ")
def driver1b():
    graph = {   "CSE102" : ["CSE241"],
                "CSE241" : ["CSE222"],
                "CSE222" : ["CSE321"],
                "CSE321" : ["CSE422"],
                "CSE211" : ["CSE321"],
                "CSE422" : []}
    q1b(graph, "CSE102")
 
driver1b()

print("\n\n")
print("----------- QUESTION 2 -------------")
def driver2():
    print("The result of 10^3: ", end = "")
    q2(10,3)
    print("The result of 2^10: ", end = "")
    q2(2,10)

driver2()

print("\n\n")
print("----------- QUESTION 3 -------------")
def driver3():
    sudoku = [[3, 0, 6, 5, 0, 8, 4, 0, 0],
            [5, 2, 0, 0, 0, 0, 0, 0, 0],
            [0, 8, 7, 0, 0, 0, 0, 3, 1],
            [0, 0, 3, 0, 1, 0, 0, 8, 0],
            [9, 0, 0, 8, 6, 3, 0, 0, 5],
            [0, 5, 0, 0, 9, 0, 6, 0, 0],
            [1, 3, 0, 0, 0, 0, 2, 5, 0],
            [0, 0, 0, 0, 0, 0, 0, 7, 4],
            [0, 0, 5, 2, 0, 6, 3, 0, 0]]
    
    q3(sudoku)
    
    
driver3()