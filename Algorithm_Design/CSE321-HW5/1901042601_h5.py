import random

# ----------- QUESTION 1 -------------- #
def q1(arr):
    print(q1Rec(arr, 0, len(arr)-1))

#Binary search based function to find the longest common substring
#If input size is 1 in the array, then return word itself
#If more than one, get common substrings from both halves and compare
#Return their longest substring
def q1Rec(arr, start, end):
    if start == end:
        return arr[start]
    
    mid = (end + start) // 2
    
    commonStr1 = q1Rec(arr, start, mid)
    commonStr2 = q1Rec(arr, mid+1, end)
    
    commonStr = ""
    minLen = min(len(commonStr1), len(commonStr2))
    for i in range(0,minLen):
        if commonStr1[i] == commonStr2[i]:
            commonStr += str(commonStr1[i])
        else:
            break
   
    return commonStr

# ----------- QUESTION 2 -------------- #

def q2a(arr):
    minTemp, maxTemp, minValInd, maxValInd = q2aRec(arr, 0, len(arr)-1)
    print("Buy on Day" + str(minValInd) + " for " + str(arr[minValInd]))
    print("Sell on Day" + str(maxValInd) + " for " + str(arr[maxValInd]))
    print()
    
#Binary search based function to find the max profit
#If input size is 1, then the only price is the current price so it returns its index
#If more than one, it gets the index of minimum price/maximum price and 
#indices that gives us the max profit in the half
#Compare the profits from the first and second halves 
#to the profit of the min price from the first half and max price from the second half
def q2aRec(arr, start, end):    
    if start == end:
        return start, end, start, end
        
    mid = (end + start) // 2
    
    min1, max1, profitMin1, profitMax1 = q2aRec(arr, start, mid)
    min2, max2, profitMin2, profitMax2 = q2aRec(arr, mid+1, end)
    
    profit1 = arr[profitMax1] - arr[profitMin1]
    profit2 = arr[profitMax2] - arr[profitMin2]
    profit3 = arr[max2] - arr[min1]
    
    newMin = min2 if arr[min1] > arr[min2] else min1
    newMax = max1 if arr[max1] > arr[max2] else max2
        
    if max(profit1, profit2, profit3) == profit1:
        return newMin, newMax, profitMin1, profitMax1
    elif max(profit1, profit2, profit3) == profit2:
        return newMin, newMax, profitMin2, profitMax2
    
    return newMin, newMax, min1, max2
        
  
#Go through the list once by storing max-min prices and max profit
#Use current value as max value and calculate the profit with min value
def q2b(arr):
    minValInd = 0
    maxValInd = 0
    maxProfit = 0
    
    minValTempInd = 0
   
    for i in range(0, len(arr)):
        if arr[minValTempInd] > arr[i]:
            minValTempInd = i
        
        profit = arr[i] - arr[minValTempInd]
        
        if maxProfit < profit:
            maxProfit = profit
            maxValInd = i
            minValInd = minValTempInd
                
    print("Buy on Day" + str(minValInd) + " for " + str(arr[minValInd]))
    print("Sell on Day" + str(maxValInd) + " for " + str(arr[maxValInd]))
    print()
    
# ----------- QUESTION 3 -------------- #

#While traversing the list, store the current situation of increasing counter
#If there exists a case which breaks the rule then reset counter and keep storing
def q3(arr):
  
    dp = [0 for i in range(len(arr))]
    dp[0] = 1
    
    for i in range(1, len(arr)):
        if arr[i] > arr[i-1]:
            dp[i] = dp[i-1] + 1
        else:
            dp[i] = 1
            
    maxVal, maxInd = 0, 0
 
    for i in range(len(dp)):
        if dp[i] > maxVal:
            maxVal = dp[i]
            maxInd = i
    
    print("The size of subarray is " + str(maxVal))
    print("The subarray is : ", end = '')
    print(arr[(maxInd-maxVal+1):(maxInd+1)])
    
# ----------- QUESTION 4 -------------- #

#Store the max point of the cell which can reach at most
#For first column and first row, there is only one way, so store them 
#For other cells, choose the max point between two options.
def q4a(arr):
    col = len(arr[0])
    row = len(arr)
    dp = [[0 for i in range(col)] for i in range(row)]
 
    dp[0][0] = arr[0][0]
    
    for i in range(1, row):
        dp[i][0] = dp[i-1][0] + arr[i][0]
 
    for j in range(1, col):
        dp[0][j] = dp[0][j-1] + arr[0][j]
        
    
 
    for i in range(1, row):
        for j in range(1, col):
            dp[i][j] = max(dp[i-1][j], dp[i][j-1]) + arr[i][j]
            
        
    
    print("The max point = " + str(dp[row-1][col-1]))
    
    
#Choose the cell having the max point and reach the end
def q4b(arr):
    point = arr[0][0]
    i, j = 0, 0
    
    while True:
        if i < len(arr)-1 and j < len(arr[0])-1:
            if arr[i][j+1] > arr[i+1][j]:
                point += arr[i][j+1]
                j += 1
            else:
                point += arr[i+1][j]
                i += 1
        elif i < len(arr)-1:
            point += arr[i+1][j]
            i += 1
        elif j < len(arr[0])-1:
            point += arr[i][j+1]
            j += 1
        else:
            break
    
    print("The max point = " + str(point))
    

# ------------ Drivers ---------------- #
print("\n\n")
print("----------- QUESTION 1 -------------")
def driver1():
    array = []
    while True:
        print("Enter the words to append (type \'*\' to stop): ")
        word = input()
        if word != "*":
            array.append(word)
        else:
            break
        
    print()
    print("The longest common substring: ")
    q1(array)
             
driver1()

print("\n\n")
def driver2():
    array = [random.randint(1, 99), random.randint(1, 99), random.randint(1, 99)
             ,random.randint(1, 99), random.randint(1, 99), random.randint(1, 99)]
    print("----------- QUESTION 2A -------------")
    print(array)
    print()
    q2a(array)
    print("\n----------- QUESTION 2B -------------")
    print(array)
    print()
    q2b(array)

driver2()

print("\n\n")
print("----------- QUESTION 3 -------------")
def driver3():
    array = [random.randint(1, 99), random.randint(1, 99), random.randint(1, 99)
             ,random.randint(1, 99), random.randint(1, 99), random.randint(1, 99)
             ,random.randint(1, 99), random.randint(1, 99), random.randint(1, 99)]
    print(array)
    print()
    q3(array)
    
driver3()

print("\n\n")
def driver4():
    array = [[random.randint(1, 99), random.randint(1, 99), random.randint(1, 99)],
             [random.randint(1, 99), random.randint(1, 99), random.randint(1, 99)],
             [random.randint(1, 99), random.randint(1, 99), random.randint(1, 99)],
             [random.randint(1, 99), random.randint(1, 99), random.randint(1, 99)]]
    
    print("----------- QUESTION 4A -------------") 
    for i in range(4):
        for j in range(3):
            print("|" + str(array[i][j]), end="")
        print("|\n----------")
    
    print()
    q4a(array)
    
    print("\n----------- QUESTION 4B -------------")
    for i in range(4):
        for j in range(3):
            print("|" + str(array[i][j]), end="")
        print("|\n----------")
    
    print()
    q4b(array)
    
driver4()