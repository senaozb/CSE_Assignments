import random

#---------- QUESTION 1 -------------#   
max_val = 0
max_route = []

#This function tries every route according to current position
def find_path(i, j, array, sum, route):
    global max_val, max_route
    route.append("A"+str(i+1)+"B"+str(j+1))
    
    if (i < len(array)-1) and (j < len(array[0])-1):
        find_path(i, j+1, array, sum+array[i][j], route)
        find_path(i+1, j, array, sum+array[i][j], route)
    elif (i < len(array)-1) and (j == len(array[0])-1):
        find_path(i+1, j, array, sum+array[i][j], route)
    elif (i == len(array)-1) and (j < len(array[0])-1):
        find_path(i, j+1, array, sum+array[i][j], route)
    elif (i == len(array)-1) and (j == len(array[0])-1):
        #If the max value is smaller than current one, update it
        if max_val < sum + array[i][j]:
            max_val = sum + array[i][j]
            max_route[:] = route
            
    route.pop()

def q1(array):
    route = []
    find_path(0, 0, array, 0, route)
    print("The max value = " + str(max_val))
    print("The route = ", end="")
    print(max_route)
 
#---------- QUESTION 2 -------------#   
def insertion_sort(array):
    for i in range(1, len(array)):
 
        key = array[i]
        j = i-1
        while j >=0 and key < array[j] :
                array[j+1] = array[j]
                j -= 1
        array[j+1] = key   
        
    return array 
    
def q2(array):
    new_array = insertion_sort(array)
    median = 0
    
    #Find the average of median elements if it is even
    #Find the median's index and assign to median variable if it is odd
    if len(new_array) % 2 == 0:
        ind = int(len(new_array)/2)
        median = (new_array[ind-1] + new_array[ind])/2
    else:
        median = new_array[int(len(new_array)/2)]
   
    return median
    
#---------- QUESTION 3A -------------#  

#Circular Linked List Structure
class Node:
    def __init__(self, val, next_node = None):
      self.val = val
      self.next = next_node

class CircularLL:
   def __init__(self, head):
      self.head = head 

#Create a CLL according to the given number
def create_cll(n):
    head = Node("P1")
    cll = CircularLL(head)
    temp = head
    
    for i in range(2, n+1):
        temp.next = Node("P" + str(i))
        temp = temp.next

    temp.next = head
    
    return head
      
#Traverse the list while deleting the nodes 
def traverse_list(head):
    temp = head
 
    if head != None:
        while(True):
            print(temp.val + " eliminates " + temp.next.val)
            temp.next = temp.next.next
            temp = temp.next
            if (temp == temp.next):
                break
            
    return temp
      
def q3a(n):   
    head = create_cll(n)
    winner = traverse_list(head)
    print(winner.val + " is the winner.")

#---------- QUESTION 3B -------------#  

#Calculate the biggest power of 2 and find the rest part 
#Use 2*rest + 1 to find the winner
def eliminate(n):
    power = 1
    while power <= n:
        power *= 2
    
    power //= 2
  
    rest = n - power
    return (2 * rest) + 1

        

def q3b(n):
    print("The winner is " + "P" + str(eliminate(n)))
       
#---------- DRIVERS -------------# 
print("QUESTION 1: ")  
def driver_q1():
    array = [[random.randint(1, 99), random.randint(1, 99), random.randint(1, 99)],
             [random.randint(1, 99), random.randint(1, 99), random.randint(1, 99)],
             [random.randint(1, 99), random.randint(1, 99), random.randint(1, 99)],
             [random.randint(1, 99), random.randint(1, 99), random.randint(1, 99)]]
    
    for i in range(4):
        for j in range(3):
            print("|" + str(array[i][j]), end="")
        print("|\n----------")
    
    q1(array)
    
driver_q1()

print()
print("QUESTION 2: ") 
def driver_q2():
    array = [random.randint(1, 99), random.randint(1, 99), random.randint(1, 99), 
             random.randint(1, 99), random.randint(1, 99), random.randint(1, 99)]
    print("Size 6 - even: " + str(array))
    print("The median: " + str(q2(array)))
    
    array = [random.randint(1, 99), random.randint(1, 99), random.randint(1, 99), 
             random.randint(1, 99), random.randint(1, 99), random.randint(1, 99), random.randint(1, 99)]
    print("Size 7 - odd: " + str(array))
    print("The median: " + str(q2(array)))
    
driver_q2()

print()
print("QUESTION 3: ") 
def driver_q3():
    print()
    n = random.randint(1, 15)
    print("The number of player is (1-15) " + str(n));
    print()
    print("QUESTION 3A: ")
    q3a(n)
    print()
    print("QUESTION 3B: ") 
    q3b(n)
    
driver_q3()