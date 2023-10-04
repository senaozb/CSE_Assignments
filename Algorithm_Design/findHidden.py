# This program is a game in which the user tries to find the hidden fruit
# Level: Easy

import random

# This function checks if the given character is a letter
def correct(letter):
    if letter >= 'a' and letter <= 'z':  
        return True 
    else:
        return False   

# This function prints the letters of hidden fruit
def print_func(word, word_hidden):
    counter = 0 # Checks if the word is already found
    for l in word:
        if l in word_hidden:
            print(l, end='') 
        else:
            counter += 1
            print("*", end='')   
    print("\n")   
    return counter             

def main():
    words = ["apple", "carrot", "pear", "mandarin", "orange", "fig", "pineapple", "mango"]
    word_hidden = []
    count = 7 # the number of lives
    random_num = random.randint(0,len(words)-1) # random index number for the hidden fruit
    while 1:
        letter = input("Please type a letter for the hidden fruit:")
        letter = letter.lower()
        if len(letter) == 1 and correct(letter) == True:
            if letter in words[random_num]:
                word_hidden.append(letter)
                flag = print_func(words[random_num], word_hidden)
                
                # Checks if the game finished
                if flag == 0: 
                    print("Congrats! You won.")
                    break
            else:
                count -= 1
                if count != 0:
                    print(count ,"left\n")
                else:
                    print("Game Over")
                    break   
        else:
            print("Enter a proper letter!\n")           

main()
