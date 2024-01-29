# This program performs the game "Eight Puzzle" and the number of tiles can be increased changing N
# Level: Hard  

import numpy as np

class Board:
    #Constructor to build the game
    def __init__(self, N):
        self.N = N
        tempBoard = np.arange(1, (self.N)*(self.N) + 1)
        np.random.shuffle(tempBoard)
        self.board = np.reshape(tempBoard, ((self.N), (self.N)))
        self.input = " "
        self.stx, self.sty = np.where(self.board == (self.N)*(self.N))
        self.stx, self.sty = self.stx[0], self.sty[0]
        
    #Special print function to print the board
    def printBoard(self):
        k,l = 0,0
        
        for i in range(-1, 2*(self.N)):
            for j in range(-1, 2*(self.N)):
                if i == -1 or i % 2 == 1:
                    print("*  ", end = "")
                elif j == -1 or j % 2 == 1:
                    print("* ", end = "")	
                else:
                    if self.board[k][l] == (self.N)*(self.N):
                        print("    ", end = "")
                    else:
                        print('{:2d}'.format(self.board[k][l]) + "  ", end = "")
                    l += 1
            print("\n")
            if i % 2 == 0:
                k += 1
                l = 0
              
    #Calls the right function to perform move operation 
    def move(self):
        direction = str(self.input[3])
        
        if direction == 'r':
            self.moveRight()
        elif direction == 'l':
           self.moveLeft()
        elif direction == 'u':
            self.moveUp()
        elif direction == 'd':
            self.moveDown()
            
    #Performs the right movement
    def moveRight(self):
        X = int(self.input[0])
        Y = int(self.input[1])
        
        temp = self.board[self.stx][self.sty]
        for i in range (self.sty, Y, -1):
            self.board[self.stx][i] = self.board[self.stx][i-1]
		
        self.board[X][Y] = temp
        
    #Performs the left movement
    def moveLeft(self):
        X = int(self.input[0])
        Y = int(self.input[1])
        
        temp = self.board[self.stx][self.sty]
        for i in range (self.sty, Y):
            self.board[self.stx][i] = self.board[self.stx][i+1]
		
        self.board[X][Y] = temp
        
    #Performs the up movement
    def moveUp(self):
        X = int(self.input[0])
        Y = int(self.input[1])
        
        temp = self.board[self.stx][self.sty]
        for i in range (self.stx, X):
            self.board[i][self.sty] = self.board[i+1][self.sty]
		
        self.board[X][Y] = temp
     
    #Performs the down movement   
    def moveDown(self):
        X = int(self.input[0])
        Y = int(self.input[1])
        
        temp = self.board[self.stx][self.sty]
        for i in range (self.stx, X, -1):
            self.board[i][self.sty] = self.board[i-1][self.sty]
		
        self.board[X][Y] = temp
            
     
    #Gets the input from the user
    def getInput(self):
        ctrl = False
        while ctrl != True:
            self.input = input("Please enter the coordinates as index of array and the direction as a char (e.g 00-r):").lower()
            if self.moveCheck() == True:
                ctrl = self.moveValid()
                if ctrl == False:
                    print("\nThis movement is not valid.\n\n")
            
           
    #Checks whether the given input is valid 
    def moveCheck(self):
        x, y, d = self.input[0], self.input[1], self.input[3]
        flag = True
        
        if x.isalpha() == True or y.isalpha() == True:
            print("\nThese coordinates are not valid.\n")
            flag = False
            return flag
        
        if d.isalpha() == False:
            print("\nThis direction is not valid.\n")
            flag = False
            return flag
        
        x, y = int(x), int(y)
        
        if x < 0 or x >= self.N or y < 0 or y >= self.N:
            print("\nThese coordinates are not valid.\n")
            flag = False
        
        dirCheck = (d == 'r' or d == 'l' or d == 'u' or d == 'd')
        if dirCheck == False:
            print("\nThis direction is not valid.\n")
            flag = False
        
        return flag 
         
    #Checks whether the given input is valid 
    def moveValid(self):
        x, y, d = int(self.input[0]), int(self.input[1]), self.input[3]
        flag = True
        
        if d == 'r':
            validBool = (self.stx == x and y != self.N - 1 and y <= self.sty)
            if validBool == False:
                flag = False
        elif d == 'l':
            validBool = (self.stx == x and y != 0 and y >= self.sty)
            if validBool == False:
                flag = False
        elif d == 'u':
            validBool = (self.sty == y and x != 0 and x >= self.stx)
            if validBool == False:
                flag = False
        elif d == 'd':
            validBool = (self.sty == y and x != self.N - 1 and x <= self.stx)
            if validBool == False:
                flag = False
            
        return flag     
    
    #Checks whether the game is finished
    def checkGame(self):
        correctBoard = np.arange(1, (self.N)*(self.N) + 1)
        correctBoard = np.reshape(correctBoard, ((self.N), (self.N)))
        return np.array_equal(correctBoard, self.board)
     
    #Plays the game until it is finished
    def play(self):
        finish = False
        while finish != True:
            self.printBoard()
            self.getInput()
            self.move()
            self.updateStartPoints()
            finish = self.checkGame()
        
        self.printBoard()
        print("Game Over")
            
    #Updates the start points (stx and sty) for other functions         
    def updateStartPoints(self):
        self.stx, self.sty = np.where(self.board == (self.N)*(self.N))
        self.stx, self.sty = self.stx[0], self.sty[0]
     
game = Board(3)
game.play() 