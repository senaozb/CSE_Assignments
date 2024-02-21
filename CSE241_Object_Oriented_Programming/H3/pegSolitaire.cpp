#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
using namespace std;    

enum class cell {empty, peg, out};

class PegSolitaire  
{
    public:
        class Cell
        {
            public:
                Cell(int a, int b, cell t);
                cell type;
            	int coorR;
            	int coorC;
        };
        void intro() const;
        void setType();
        void create();
        void printGame() const;
        void getInput();
        void getRandomInput();
        bool setInput();
        int getTypeBoard() const {return chosenType;}
        int getTypeGame() const {return gameType;}
        void playUser();
        void playComp();
        bool movementValid();
        bool movementValidT6();
        void update();
        int checkFull(int indR, int indC, int type, int locType);
        int checkGame();
        int checkGameT6();
        void printRes() const; 
        void getUserch();
        void fileCheck();
        bool compareGames(PegSolitaire &other);
        int getTotalPeg();
        static int flag;
        static int userch;
        int inType;
        int filecont;
    private:
    	void type1();
        void type2();
        void type3();
        void type4();
        void type5();
        void type6();
        int fileop(string fileName);
        vector <vector <PegSolitaire::Cell> > vec;
        int chosenType;
        int gameType;
        string userIn;    
};

//Default initialization
int PegSolitaire::flag = 0;
int PegSolitaire::userch = 1;

int main()
{
    PegSolitaire game;
    game.intro();
    do
    {
        game.filecont = 0;
        game.setType();
        game.create();
        game.printGame();
        if(game.getTypeGame() == 1) //User part
        	game.playUser();
        else if(game.getTypeGame() == 2) //Computer part
        	game.playComp();
        if(game.filecont != 2)
        	game.printRes();
        game.getUserch();
    }while(game.userch == 1);

    return 0;
}

PegSolitaire::Cell::Cell(int a, int b, cell t)
{
	coorR = a;
	coorC = b;
	type = t;
}

void PegSolitaire::intro() const
{
    cout << "WELCOME TO PEG SOLITAIRE" << endl;
    cout << endl;
    cout << "Please use the commands for the classic types as SAVE-FILE.TXT and LOAD-FILE.TXT" << endl;
    cout << "For arbitrary boards, use SAVE-FILE.TXT, LOAD-FILE.TXT, LOAD-FILE1/2/3.TXT(for a new game)" << endl;
    cout << "(e.g. LOAD-FILE1.TXT for 1.board)" << endl;
    cout << endl;
}

void PegSolitaire::type1()
{
    vector <vector <PegSolitaire::Cell> > soli = {{Cell(0, 0, cell::out), Cell(0, 1, cell::out), Cell(0, 2, cell::peg), Cell(0, 3, cell::peg), Cell(0, 4, cell::peg), Cell(0, 5, cell::out), Cell(0, 6, cell::out)},
    										  {Cell(1, 0, cell::out), Cell(1, 1, cell::peg), Cell(1, 2, cell::peg), Cell(1, 3, cell::peg), Cell(1, 4, cell::peg), Cell(1, 5, cell::peg), Cell(1, 6, cell::out)},
			                                  {Cell(2, 0, cell::peg), Cell(2, 1, cell::peg), Cell(2, 2, cell::peg), Cell(2, 3, cell::empty), Cell(2, 4, cell::peg), Cell(2, 5, cell::peg), Cell(2, 6, cell::peg)},
			                                  {Cell(3, 0, cell::peg), Cell(3, 1, cell::peg), Cell(3, 2, cell::peg), Cell(3, 3, cell::peg), Cell(3, 4, cell::peg), Cell(3, 5, cell::peg), Cell(3, 6, cell::peg)},
			                                  {Cell(4, 0, cell::peg), Cell(4, 1, cell::peg), Cell(4, 2, cell::peg), Cell(4, 3, cell::peg), Cell(4, 4, cell::peg), Cell(4, 5, cell::peg), Cell(4, 6, cell::peg)},
			                                  {Cell(5, 0, cell::out), Cell(5, 1, cell::peg), Cell(5, 2, cell::peg), Cell(5, 3, cell::peg), Cell(5, 4, cell::peg), Cell(5, 5, cell::peg), Cell(5, 6, cell::out)},
			                                  {Cell(6, 0, cell::out), Cell(6, 1, cell::out), Cell(6, 2, cell::peg), Cell(6, 3, cell::peg), Cell(6, 4, cell::peg), Cell(6, 5, cell::out), Cell(6, 6, cell::out)}};

    vec = soli;
}
void PegSolitaire::type2()
{
    vector <vector <PegSolitaire::Cell> > soli = {{Cell(0, 0, cell::out), Cell(0, 1, cell::out), Cell(0, 2, cell::out), Cell(0, 3, cell::peg), Cell(0, 4, cell::peg), Cell(0, 5, cell::peg), Cell(0, 6, cell::out), Cell(0, 7, cell::out), Cell(0, 8, cell::out)},
											  {Cell(1, 0, cell::out), Cell(1, 1, cell::out), Cell(1, 2, cell::out), Cell(1, 3, cell::peg), Cell(1, 4, cell::peg), Cell(1, 5, cell::peg), Cell(1, 6, cell::out), Cell(1, 7, cell::out), Cell(1, 8, cell::out)},
			                                  {Cell(2, 0, cell::out), Cell(2, 1, cell::out), Cell(2, 2, cell::out), Cell(2, 3, cell::peg), Cell(2, 4, cell::peg), Cell(2, 5, cell::peg), Cell(2, 6, cell::out), Cell(2, 7, cell::out), Cell(2, 8, cell::out)},
			                                  {Cell(3, 0, cell::peg), Cell(3, 1, cell::peg), Cell(3, 2, cell::peg), Cell(3, 3, cell::peg), Cell(3, 4, cell::peg), Cell(3, 5, cell::peg), Cell(3, 6, cell::peg), Cell(3, 7, cell::peg), Cell(3, 8, cell::peg)},
			                                  {Cell(4, 0, cell::peg), Cell(4, 1, cell::peg), Cell(4, 2, cell::peg), Cell(4, 3, cell::peg), Cell(4, 4, cell::empty), Cell(4, 5, cell::peg), Cell(4, 6, cell::peg), Cell(4, 7, cell::peg), Cell(4, 8, cell::peg)},
			                                  {Cell(5, 0, cell::peg), Cell(5, 1, cell::peg), Cell(5, 2, cell::peg), Cell(5, 3, cell::peg), Cell(5, 4, cell::peg), Cell(5, 5, cell::peg), Cell(5, 6, cell::peg), Cell(5, 7, cell::peg), Cell(5, 8, cell::peg)},
			                                  {Cell(6, 0, cell::out), Cell(6, 1, cell::out), Cell(6, 2, cell::out), Cell(6, 3, cell::peg), Cell(6, 4, cell::peg), Cell(6, 5, cell::peg), Cell(6, 6, cell::out), Cell(6, 7, cell::out), Cell(6, 8, cell::out)},
			                                  {Cell(7, 0, cell::out), Cell(7, 1, cell::out), Cell(7, 2, cell::out), Cell(7, 3, cell::peg), Cell(7, 4, cell::peg), Cell(7, 5, cell::peg), Cell(7, 6, cell::out), Cell(7, 7, cell::out), Cell(7, 8, cell::out)},
			                                  {Cell(8, 0, cell::out), Cell(8, 1, cell::out), Cell(8, 2, cell::out), Cell(8, 3, cell::peg), Cell(8, 4, cell::peg), Cell(8, 5, cell::peg), Cell(8, 6, cell::out), Cell(8, 7, cell::out), Cell(8, 8, cell::out)}};

    vec = soli;
}
void PegSolitaire::type3()
{
    vector <vector <PegSolitaire::Cell> > soli = {{Cell(0, 0, cell::out), Cell(0, 1, cell::out), Cell(0, 2, cell::peg), Cell(0, 3, cell::peg), Cell(0, 4, cell::peg), Cell(0, 5, cell::out), Cell(0, 6, cell::out), Cell(0, 7, cell::out)},
											  {Cell(1, 0, cell::out), Cell(1, 1, cell::out), Cell(1, 2, cell::peg), Cell(1, 3, cell::peg), Cell(1, 4, cell::peg), Cell(1, 5, cell::out), Cell(1, 6, cell::out), Cell(1, 7, cell::out)},
			                                  {Cell(2, 0, cell::out), Cell(2, 1, cell::out), Cell(2, 2, cell::peg), Cell(2, 3, cell::peg), Cell(2, 4, cell::peg), Cell(2, 5, cell::out), Cell(2, 6, cell::out), Cell(2, 7, cell::out)},
			                                  {Cell(3, 0, cell::peg), Cell(3, 1, cell::peg), Cell(3, 2, cell::peg), Cell(3, 3, cell::peg), Cell(3, 4, cell::peg), Cell(3, 5, cell::peg), Cell(3, 6, cell::peg), Cell(3, 7, cell::peg)},
			                                  {Cell(4, 0, cell::peg), Cell(4, 1, cell::peg), Cell(4, 2, cell::peg), Cell(4, 3, cell::empty), Cell(4, 4, cell::peg), Cell(4, 5, cell::peg), Cell(4, 6, cell::peg), Cell(4, 7, cell::peg)},
			                                  {Cell(5, 0, cell::peg), Cell(5, 1, cell::peg), Cell(5, 2, cell::peg), Cell(5, 3, cell::peg), Cell(5, 4, cell::peg), Cell(5, 5, cell::peg), Cell(5, 6, cell::peg), Cell(5, 7, cell::peg)},
			                                  {Cell(6, 0, cell::out), Cell(6, 1, cell::out), Cell(6, 2, cell::peg), Cell(6, 3, cell::peg), Cell(6, 4, cell::peg), Cell(6, 5, cell::out), Cell(6, 6, cell::out), Cell(6, 7, cell::out)},
			                                  {Cell(7, 0, cell::out), Cell(7, 1, cell::out), Cell(7, 2, cell::peg), Cell(7, 3, cell::peg), Cell(7, 4, cell::peg), Cell(7, 5, cell::out), Cell(7, 6, cell::out), Cell(7, 7, cell::out)}};

    vec = soli;
}
void PegSolitaire::type4()
{
    vector <vector <PegSolitaire::Cell> > soli = {{Cell(0, 0, cell::out), Cell(0, 1, cell::out), Cell(0, 2, cell::peg), Cell(0, 3, cell::peg), Cell(0, 4, cell::peg), Cell(0, 5, cell::out), Cell(0, 6, cell::out)},
											  {Cell(1, 0, cell::out), Cell(1, 1, cell::out), Cell(1, 2, cell::peg), Cell(1, 3, cell::peg), Cell(1, 4, cell::peg), Cell(1, 5, cell::out), Cell(1, 6, cell::out)},
			                                  {Cell(2, 0, cell::peg), Cell(2, 1, cell::peg), Cell(2, 2, cell::peg), Cell(2, 3, cell::peg), Cell(2, 4, cell::peg), Cell(2, 5, cell::peg), Cell(2, 6, cell::peg)}, 
			                                  {Cell(3, 0, cell::peg), Cell(3, 1, cell::peg), Cell(3, 2, cell::peg), Cell(3, 3, cell::empty), Cell(3, 4, cell::peg), Cell(3, 5, cell::peg), Cell(3, 6, cell::peg)},
			                                  {Cell(4, 0, cell::peg), Cell(4, 1, cell::peg), Cell(4, 2, cell::peg), Cell(4, 3, cell::peg), Cell(4, 4, cell::peg), Cell(4, 5, cell::peg), Cell(4, 6, cell::peg)},
			                                  {Cell(5, 0, cell::out), Cell(5, 1, cell::out), Cell(5, 2, cell::peg), Cell(5, 3, cell::peg), Cell(5, 4, cell::peg), Cell(5, 5, cell::out), Cell(5, 6, cell::out)},
			                                  {Cell(6, 0, cell::out), Cell(6, 1, cell::out), Cell(6, 2, cell::peg), Cell(6, 3, cell::peg), Cell(6, 4, cell::peg), Cell(6, 5, cell::out), Cell(6, 6, cell::out)}};

    vec = soli;
}
void PegSolitaire::type5()
{
    vector <vector <PegSolitaire::Cell> > soli = {{Cell(0, 0, cell::out), Cell(0, 1, cell::out), Cell(0, 2, cell::out), Cell(0, 3, cell::out), Cell(0, 4, cell::peg), Cell(0, 5, cell::out), Cell(0, 6, cell::out), Cell(0, 7, cell::out), Cell(0, 8, cell::out)},
											  {Cell(1, 0, cell::out), Cell(1, 1, cell::out), Cell(1, 2, cell::out), Cell(1, 3, cell::peg), Cell(1, 4, cell::peg), Cell(1, 5, cell::peg), Cell(1, 6, cell::out), Cell(1, 7, cell::out), Cell(1, 8, cell::out)},
			                                  {Cell(2, 0, cell::out), Cell(2, 1, cell::out), Cell(2, 2, cell::peg), Cell(2, 3, cell::peg), Cell(2, 4, cell::peg), Cell(2, 5, cell::peg), Cell(2, 6, cell::peg), Cell(2, 7, cell::out), Cell(2, 8, cell::out)},
			                                  {Cell(3, 0, cell::out), Cell(3, 1, cell::peg), Cell(3, 2, cell::peg), Cell(3, 3, cell::peg), Cell(3, 4, cell::peg), Cell(3, 5, cell::peg), Cell(3, 6, cell::peg), Cell(3, 7, cell::peg), Cell(3, 8, cell::out)},
			                                  {Cell(4, 0, cell::peg), Cell(4, 1, cell::peg), Cell(4, 2, cell::peg), Cell(4, 3, cell::peg), Cell(4, 4, cell::empty), Cell(4, 5, cell::peg), Cell(4, 6, cell::peg), Cell(4, 7, cell::peg), Cell(4, 8, cell::peg)}, 
			                                  {Cell(5, 0, cell::out), Cell(5, 1, cell::peg), Cell(5, 2, cell::peg), Cell(5, 3, cell::peg), Cell(5, 4, cell::peg), Cell(5, 5, cell::peg), Cell(5, 6, cell::peg), Cell(5, 7, cell::peg), Cell(5, 8, cell::out)},
			                                  {Cell(6, 0, cell::out), Cell(6, 1, cell::out), Cell(6, 2, cell::peg), Cell(6, 3, cell::peg), Cell(6, 4, cell::peg), Cell(6, 5, cell::peg), Cell(6, 6, cell::peg), Cell(6, 7, cell::out), Cell(6, 8, cell::out)},
			                                  {Cell(7, 0, cell::out), Cell(7, 1, cell::out), Cell(7, 2, cell::out), Cell(7, 3, cell::peg), Cell(7, 4, cell::peg), Cell(7, 5, cell::peg), Cell(7, 6, cell::out), Cell(7, 7, cell::out), Cell(7, 8, cell::out)},
			                                  {Cell(8, 0, cell::out), Cell(8, 1, cell::out), Cell(8, 2, cell::out), Cell(8, 3, cell::out), Cell(8, 4, cell::peg), Cell(8, 5, cell::out), Cell(8, 6, cell::out), Cell(8, 7, cell::out), Cell(8, 8, cell::out)}};

    vec = soli;
}
void PegSolitaire::type6()
{
    vector <vector <PegSolitaire::Cell> > soli = {{Cell(0, 0, cell::out), Cell(0, 1, cell::out), Cell(0, 2, cell::out), Cell(0, 3, cell::out), Cell(0, 4, cell::empty), Cell(0, 5, cell::out), Cell(0, 6, cell::out), Cell(0, 7, cell::out), Cell(0, 8, cell::out)},
											  {Cell(1, 0, cell::out), Cell(1, 1, cell::out), Cell(1, 2, cell::out), Cell(1, 3, cell::peg), Cell(1, 4, cell::out), Cell(1, 5, cell::peg), Cell(1, 6, cell::out), Cell(1, 7, cell::out), Cell(1, 8, cell::out)},
			                                  {Cell(2, 0, cell::out), Cell(2, 1, cell::out), Cell(2, 2, cell::peg), Cell(2, 3, cell::out), Cell(2, 4, cell::peg), Cell(2, 5, cell::out), Cell(2, 6, cell::peg), Cell(2, 7, cell::out), Cell(2, 8, cell::out)},
			                                  {Cell(3, 0, cell::out), Cell(3, 1, cell::peg), Cell(3, 2, cell::out), Cell(3, 3, cell::peg), Cell(3, 4, cell::out), Cell(3, 5, cell::peg), Cell(3, 6, cell::out), Cell(3, 7, cell::peg), Cell(3, 8, cell::out)},
			                                  {Cell(4, 0, cell::peg), Cell(4, 1, cell::out), Cell(4, 2, cell::peg), Cell(4, 3, cell::out), Cell(4, 4, cell::peg), Cell(4, 5, cell::out), Cell(4, 6, cell::peg), Cell(4, 7, cell::out), Cell(4, 8, cell::peg)}};

    vec = soli;
}

void PegSolitaire::setType()
{
        //Get which types the user choose
        int fl = 0;
        do //Check for invalid inputs
        {
            if(fl != 0)
                cerr << "Invalid Type" <<endl;
            cout << "Please choose one type for the game between 6 types:";
            if(!(cin >> chosenType)) 
			{
		      cin.clear();
		      cin.ignore(10000,'\n');
		    }
		    ++fl;
        }while(chosenType < 1 || chosenType > 6);
    
        
        fl = 0;
        do //Check for invalid inputs
        {
            if(fl != 0) 
                cerr << "Invalid Type" <<endl;
            cout << "If it is a human player game, type 1. If it is a computer game, type 2:";
            if(!(cin >> gameType)) 
			{
		      cin.clear();
		      cin.ignore(10000,'\n');
		    }
			++fl;
        }while(!(gameType == 1 || gameType == 2));
}

void PegSolitaire::create()
{
    switch(chosenType)
        {
            //Create the game board
            case 1:
                type1();
                break;
            case 2:
                type2();
                break;
            case 3:
                type3();
                break;
            case 4:
                type4();
                break;
            case 5:
                type5();
                break;
            case 6:
                type6();
                break;
        }
}

void PegSolitaire::printGame() const
{
	int col = vec[0].size();
	int row = vec.size();
    //Print the letters for the first row
    for (int k = 0; k < col+2; k++)
    {
        if (k == 0 || k == 1)
            cout << " ";
        else 
            cout << static_cast <char> (95+k);
    }
    cout << endl;

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (j == 0) //Print the numbers to order for each row
                cout << i+1 << " ";
            if (vec[i][j].type == cell::empty)
                cout << ".";
            else if (vec[i][j].type == cell::peg)
                cout << "P";
            else if (vec[i][j].type == cell::out)
                cout << " ";
        }
        cout << endl;    
    }

    if(gameType == 2)
    {
        cout << userIn << " is the movement done randomly." << endl;
        cout << endl;
    }
}

void PegSolitaire::getInput()
{
    userIn.clear();
    cout << "Please enter row, column and direction (as RC-D, e.g. B4-R):";
    cin >> userIn;
}

void PegSolitaire::getRandomInput()
{
    bool res;
    int col = vec[0].size();
	int row = vec.size();
    do
    {
        //Get the random inputs
        userIn.clear();
        int random1 = rand()%col + 65;
        int random2 = rand()%row + 49;
        int ranFlag = rand()%4;
        char random;  

        switch(ranFlag)
        {
            case 0:
                random = 'R';
                break;
            case 1:
                random = 'L';
                break;
            case 2:
                random = 'U';
                break;
            case 3:
                random = 'D';
                break;
        }
            
        userIn = static_cast <char> (random1);
        userIn += static_cast <char> (random2);
        userIn += '-';
        userIn += random;
                        
        if(chosenType != 6)
            res = movementValid();
        else
            res = movementValidT6();
    }while(res == false);
}

bool PegSolitaire::setInput()
{
    int col = vec[0].size();
	int row = vec.size();
    int fl = 0;
    string str1 = "LOAD-FILE.TXT";
    string str2 = "SAVE-FILE.TXT";
    string str3 = "LOAD-FILE1.TXT";
    string str4 = "SAVE-FILE1.TXT";
    string str5 = "LOAD-FILE2.TXT";
    string str6 = "SAVE-FILE2.TXT";
    string str7 = "LOAD-FILE3.TXT";
    string str8 = "SAVE-FILE3.TXT";

    if (userIn == str1)
    { 
        if(chosenType != 6)
        {
            inType = 1;
            return true;
        }
        else
            fl = 1;
    }
    else if (userIn == str2)
    { 
        if(chosenType != 6)
        {
            inType = 2;
            return true;
        }
        else
            fl = 1;
    }
    else if (userIn == str3)
    {
        inType = 3;
        return true;
    }
    else if (userIn == str4)
    {
        inType = 4;
        return true;
    }
    else if (userIn == str5)
    {
        inType = 5;
        return true;
    }
    else if (userIn == str6)
    {
        inType = 6;
        return true;
    }
    else if (userIn == str7)
    {
        inType = 7;
        return true;
    }
    else if (userIn == str8)
    {
        inType = 8;
        return true;
    }
    
    /*These operations are not compatible with Type 6*/
    if(fl == 1)   
        cerr << "File operations are not available for Type 6" << endl;
        
	//Checks the validation of input string digit by digit
    if (userIn.size() != 4)
        return false;
    if (!(static_cast <int> (userIn[0]) >= 65 && static_cast <int> (userIn[0]) < col+65))
        return false;
    if (!(static_cast <int> (userIn[1]) >= 49 && static_cast <int> (userIn[1]) < row+49))
        return false;
    if (userIn[2] != '-')
        return false;
    if (!(userIn[3] == 'R' || userIn[3] == 'L' || userIn[3] == 'U' || userIn[3] == 'D'))
        return false;
      
    return true;
}

void PegSolitaire::playUser()
{
	do
        {
            bool fl1;
            int count1 = 0, count2 = 0;
            inType = 0;
            flag = 0;
            filecont = 0;
            do 
            {
                if(count2 != 0)
                    cerr << "Invalid Movement" << endl;
                do
                {
                    if(count1 != 0)
                        cerr << "Invalid Input" << endl;
                    getInput();
                    ++count1;
                }while(setInput() == false);
                count1 = 0;
                    
                if(inType != 0)
                {
                    fileCheck();
                    break;
                }
                else
                {
                    if(chosenType != 6)
                        fl1 = movementValid();
                    else
                        fl1 = movementValidT6();
                }
                ++count2;
            }while(fl1 == false);

            if(inType == 2 || inType == 4 || inType == 6 || inType == 8)
            {
                do
                {
                    cout << "If you want to continue to play in this board, type 1, otherwise go to the menu typing 2:";
                    if(!(cin >> filecont)) 
                    {
                        cin.clear();
                        cin.ignore(10000,'\n');
                    }
                }while(!(filecont == 1 || filecont == 2));
            }
            else if(inType == 1 || inType == 3 || inType == 5 || inType == 7)
                continue; //Continue with the next step skipping update part after loading

            if(filecont == 1 || filecont == 0)
            {
                update();
                printGame();
                if(chosenType != 6)
                    flag = checkGame();
                else
                    flag = checkGameT6();
            }
            else    
                break;

        }while(flag == 0);
}
void PegSolitaire::playComp()
{
	 do
        {
            getRandomInput();
        	update();
            printGame();
                
            if(chosenType != 6)
                flag = checkGame();
            else
                flag = checkGameT6();

        }while(flag == 0);
}

bool PegSolitaire::movementValid()
{
    //Check the validation of given movement looking at the game board
    int row = static_cast <int> (userIn[1]) - 49;
    int col = static_cast <int> (userIn[0]) - 65;
    char dir = userIn[3];
    int max_r = vec.size() - 1;
    int max_c = vec[0].size() - 1;
    
    if (vec[row][col].type == cell::out || vec[row][col].type == cell::empty)
        return false;
    
    //If there is no peg and empty cell after the chosen peg then return 0
    if (dir == 'R')
    {
        if (max_c - col < 2)
            return false;
        if (!(vec[row][col+1].type == cell::peg && vec[row][col+2].type == cell::empty))
            return false;
    }
    else if (dir == 'L')
    {
        if (col < 2)
            return false;
        if (!(vec[row][col-1].type == cell::peg && vec[row][col-2].type == cell::empty))
            return false;
    }
    else if (dir == 'U')
    {
        if (row < 2)
            return false;
        if (!(vec[row-1][col].type == cell::peg && vec[row-2][col].type == cell::empty))
            return false;
    }
    else if (dir == 'D')
    {
        if (max_r - row < 2)
            return false;
        if (!(vec[row+1][col].type == cell::peg && vec[row+2][col].type == cell::empty))
            return false;
    }
    
    return true;
}

bool PegSolitaire::movementValidT6()
{
    //Check the validation of given movement for Type 6
    int row = static_cast <int> (userIn[1]) - 49;
    int col = static_cast <int> (userIn[0]) - 65;
    char dir = userIn[3];
    int max_r = vec.size() - 1;
    int max_c = vec[0].size() - 1;
    
    if (vec[row][col].type == cell::out || vec[row][col].type == cell::empty)
        return false;
    
    //If there is no peg and empty cell after the chosen peg then return 0
    if (dir == 'R')
    {
        if (max_c - col < 4)
            return false;
        if (!(vec[row][col+2].type == cell::peg && vec[row][col+4].type == cell::empty))
            return false;
    }
    else if (dir == 'L')
    {
        if (col < 4)
            return false;
        if (!(vec[row][col-2].type == cell::peg && vec[row][col-4].type == cell::empty))
            return false;
    }
    else if (dir == 'U')
    {
        if (row < 2)
            return false;
        if(col != 0 && col != max_c)
        {
	        if (!((vec[row-1][col+1].type == cell::peg && vec[row-2][col+2].type == cell::empty) 
	        || (vec[row-1][col-1].type == cell::peg && vec[row-2][col-2].type == cell::empty)))
	            return false;
		}
		else if(col == 0)
		{
			if (!(vec[row-1][col+1].type == cell::peg && vec[row-2][col+2].type == cell::empty))
	            return false;
		}
		else if(col == max_c)
		{
			if (!(vec[row-1][col-1].type == cell::peg && vec[row-2][col-2].type == cell::empty))
	            return false;
		}
    }
    else if (dir == 'D')
    {
        if (max_r - row < 2)
            return false;
        if(col != 0 && col != max_c)
        {
	        if (!((vec[row+1][col+1].type == cell::peg && vec[row+2][col+2].type == cell::empty) 
	        || (vec[row+1][col-1].type == cell::peg && vec[row+2][col-2].type == cell::empty)))
	            return false;
		}
		else if(col == 0)
		{
			if (!(vec[row+1][col+1].type == cell::peg && vec[row+2][col+2].type == cell::empty))
	            return false;
		}
		else if(col == max_c)
		{
			if (!(vec[row+1][col-1].type == cell::peg && vec[row+2][col-2].type == cell::empty))
	            return false;
		}
    }
    return true;
}

void PegSolitaire::update()
{
    int row = static_cast <int> (userIn[1]) - 49;
    int col = static_cast <int> (userIn[0]) - 65;
    char dir = userIn[3];

    //Change the position of the peg and remove one peg
    if(chosenType != 6)
    {
        switch (dir)
        {
            case 'R':
                vec[row][col].type = cell::empty;
                vec[row][col+1].type = cell::empty;
                vec[row][col+2].type = cell::peg;
                break;
            case 'L':
                vec[row][col].type = cell::empty;
                vec[row][col-1].type = cell::empty;
                vec[row][col-2].type = cell::peg;
                break;
            case 'U':
                vec[row][col].type = cell::empty;
                vec[row-1][col].type = cell::empty;
                vec[row-2][col].type = cell::peg;
                break;
            case 'D':
                vec[row][col].type = cell::empty;
                vec[row+1][col].type = cell::empty;
                vec[row+2][col].type = cell::peg;
                break;
        }
    }
    else 
    {
    	/*For Type 6, if down and up are chosen then the program checks both two ways
		and chooses one of them if both are available*/
        switch (dir)
        {
            case 'R':
                vec[row][col].type = cell::empty;
                vec[row][col+2].type = cell::empty;
                vec[row][col+4].type = cell::peg;
                break;
            case 'L':
                vec[row][col].type = cell::empty;
                vec[row][col-2].type = cell::empty;
                vec[row][col-4].type = cell::peg;
                break;
            case 'U':
                if (vec[row-1][col+1].type == cell::peg && vec[row-2][col+2].type == cell::empty)
                {
                    vec[row][col].type = cell::empty;
                    vec[row-1][col+1].type = cell::empty;
                    vec[row-2][col+2].type = cell::peg;
                }
                else if (vec[row-1][col-1].type == cell::peg && vec[row-2][col-2].type == cell::empty)
                {
                    vec[row][col].type = cell::empty;
                    vec[row-1][col-1].type = cell::empty;
                    vec[row-2][col-2].type = cell::peg;
                }
                break;
            case 'D':
                if (vec[row+1][col+1].type == cell::peg && vec[row+2][col+2].type == cell::empty)
                {
                    vec[row][col].type = cell::empty;
                    vec[row+1][col+1].type = cell::empty;
                    vec[row+2][col+2].type = cell::peg;
                }
                else if (vec[row+1][col-1].type == cell::peg && vec[row+2][col-2].type == cell::empty)
                {
                    vec[row][col].type = cell::empty;
                    vec[row+1][col-1].type = cell::empty;
                    vec[row+2][col-2].type = cell::peg;
                }
                break;
        }
    }
}

int PegSolitaire::checkFull(int indR, int indC, int type, int locType)
{
    //This function checks the immovable situations for being stuck
    //locType indicates the location of peg to prevent the memory errors(e.g. 1.column or the last row)
    int count = 0, fl = 0;
    int col = vec[0].size();
	int row = vec.size();

    for (int j = 0; j < col; j++) //Full row control
    {
        if(vec[indR][j].type == cell::peg)
            ++count;
        else if(vec[indR][j].type == cell::empty) //If there is an empty area then reset counter
        {
            count = 0;
            fl = 1;
            break;
        }
    }
        
    if(fl == 0)
    {
        //Checks if up and down rows are empty, if not so returns 0
        if(type == 0)
        {
            for (int k = 0; k < col; k++)
            {
                if(locType == 0 || locType == 3 || locType == 4)
                {
                    if(vec[indR][k].type == cell::peg && (vec[indR-1][k].type == cell::peg || vec[indR+1][k].type == cell::peg))
                        return 0;  
                }
                else if(locType == 1)
                {
                    if(vec[indR][k].type == cell::peg && vec[indR+1][k].type == cell::peg)
                        return 0;  
                }
                else if(locType == 2)
                {
                    if(vec[indR][k].type == cell::peg && vec[indR-1][k].type == cell::peg)
                        return 0;  
                }
            }
            return count;
        }
        else if(indR != 0) //For Type 6
        {
            for (int k = 0; k < col; k++)
            {
                if(k != 0 && k != col-1 && indR != 0 && indR != row-1)
                {
                    if(vec[indR][k].type == cell::peg && (vec[indR-1][k-1].type == cell::peg || vec[indR-1][k+1].type == cell::peg 
                    || vec[indR+1][k-1].type == cell::peg || vec[indR+1][k+1].type == cell::peg))
                        return 0;  
                }
                else if(k != 0 && k != col-1 && indR == row-1)
                {
                    if(vec[indR][k].type == cell::peg && (vec[indR-1][k-1].type == cell::peg || vec[indR-1][k+1].type == cell::peg))
                        return 0;  
                }
                else if(k == 0 && indR == row-1)
                {
                    if(vec[indR][k].type == cell::peg && vec[indR-1][k+1].type == cell::peg)
                        return 0;
                }
                else if(k == col-1 && indR == row-1)
                {
                    if(vec[indR][k].type == cell::peg && vec[indR-1][k-1].type == cell::peg) 
                        return 0;
                }
            }
            return count;
        }
    }
        
    count = 0;
    if(type == 0)
    {
        for (int i = 0; i < row; i++) //Full column control
        {
            if(vec[i][indC].type == cell::peg)
                ++count;
            else if(vec[i][indC].type == cell::empty)
                return 0;
        }
        for (int l = 0; l < row; l++) //Checks if the right and left columns are empty
        {
            if(locType == 0 || locType == 1 || locType == 2)
            {
                if(vec[l][indC].type == cell::peg && (vec[l][indC-1].type == cell::peg || vec[l][indC+1].type == cell::peg))
                    return 0;  
            } 
            else if(locType == 3)
            {
                if(vec[l][indC].type == cell::peg && vec[l][indC+1].type == cell::peg)
                    return 0;  
            }
            else if(locType == 4)
            {
                if(vec[l][indC].type == cell::peg && vec[l][indC-1].type == cell::peg)
                    return 0;  
            }

        }
    }
    else //For Type 6
    {
        int i = indR, index = indC;
        int directPeg = 0;
        
        if(i != row-1 && i != 0) //Find the direction of pegs being stuck
        {
            if(vec[i-1][index-1].type == cell::peg || vec[i+1][index+1].type == cell::peg)
                directPeg = 0;
            else if(vec[i-1][index+1].type == cell::peg || vec[i+1][index-1].type == cell::peg)
                directPeg = 1;
        }
        else if(i == 0)
        {
            if(vec[i+1][index+1].type == cell::peg)
                directPeg = 0;
            else if(vec[i+1][index-1].type == cell::peg)
                directPeg = 1;
        }
        else if(i == row-1 && index == col-1)
            directPeg = 0;
        else if(i == row-1 && index == 0)
            directPeg = 1;
        else if(i == row-1)
        {
            if(vec[i-1][index-1].type == cell::peg)
                directPeg = 0;
            else if(vec[i-1][index+1].type == cell::peg)
                directPeg = 1;
        }

        if(directPeg == 0)
        {
            while (i < row && index < col) //Full column control from right bottom to left top for Type 6
            {
                if(vec[i][index].type == cell::peg)
                {
                    if(i != row-1 && i != 0)
                    {
                        if(vec[i][index-2].type == cell::peg || vec[i][index+2].type == cell::peg 
                        || vec[i-1][index+1].type == cell::peg || vec[i+1][index-1].type == cell::peg)
                            return 0;
                    }
                    else if(i == row-1 && index != col-1)
                    {
                        if(vec[i][index-2].type == cell::peg || vec[i][index+2].type == cell::peg || vec[i-1][index+1].type == cell::peg)
                            return 0;
                    }
                    else if(i == row-1 && index == col-1)
                    {
                        if(vec[i][index-2].type == cell::peg)
                            return 0;
                    }
                    else if(i == 0)
                    {
                        if(vec[i+1][index-1].type == cell::peg)
                            return 0;
                    }
                    ++count;
                }
                else if(vec[i][index].type == cell::empty)
                    return 0;
                ++i;
                ++index;
            }
            
            index = indC;
            i = indR;
            while (i >= 0 && index >= 0) 
            {
                if(vec[i][index].type == cell::peg)
                {
                    if(i != 0 && i != row-1)
                    {
                        if(vec[i][index-2].type == cell::peg || vec[i][index+2].type == cell::peg 
                        || vec[i-1][index+1].type == cell::peg || vec[i+1][index-1].type == cell::peg)
                            return 0;
                    }
                    else if(i == row-1 && index != col-1)
                    {
                        if(vec[i][index-2].type == cell::peg || vec[i][index+2].type == cell::peg || vec[i-1][index+1].type == cell::peg)
                            return 0;
                    }
                    else if(i == row-1 && index == col-1)
                    {
                        if(vec[i][index-2].type == cell::peg)
                            return 0;
                    }
                    else if(i == 0)
                    {
                        if(vec[i+1][index-1].type == cell::peg)
                            return 0;
                    }
                    ++count;
                }
                else if(vec[i][index].type == cell::empty)
                    return 0;
                --i;
                --index;
            }
        }
        else
        {
            while (i < row && index >= 0) //Full column control from left bottom to right top for Type 6
            {
                if(vec[i][index].type == cell::peg)
                {
                    if(i != row-1 && i != 0)
                    {
                        if(vec[i][index-2].type == cell::peg || vec[i][index+2].type == cell::peg 
                        || vec[i-1][index-1].type == cell::peg || vec[i+1][index+1].type == cell::peg)
                            return 0;
                    }
                    else if(i == row-1 && index != 0)
                    {
                        if(vec[i][index-2].type == cell::peg || vec[i][index+2].type == cell::peg || vec[i-1][index-1].type == cell::peg)
                            return 0;
                    }
                    else if(i == row-1 && index == 0)
                    {
                        if(vec[i][index+2].type == cell::peg)
                            return 0;
                    }
                    else if(i == 0)
                    {
                        if(vec[i+1][index+1].type == cell::peg)
                            return 0;
                    }
                    ++count;
                }
                else if(vec[i][index].type == cell::empty)
                    return 0;
                ++i;
                --index;
            }
            
            index = indC;
            i = indR;
            while (i >= 0 && index < col) 
            {
                if(vec[i][index].type == cell::peg)
                {
                    if(i != 0 && i != row-1)
                    {
                        if(vec[i][index-2].type == cell::peg || vec[i][index+2].type == cell::peg 
                        || vec[i-1][index-1].type == cell::peg || vec[i+1][index+1].type == cell::peg)
                            return 0;
                    }
                    else if(i == row-1 && index != 0)
                    {
                        if(vec[i][index-2].type == cell::peg || vec[i][index+2].type == cell::peg || vec[i-1][index-1].type == cell::peg)
                            return 0;
                    }
                    else if(i == row-1 && index == 0)
                    {
                        if(vec[i][index+2].type == cell::peg)
                            return 0;
                    }
                    else if(i == 0)
                    {
                        if(vec[i+1][index+1].type == cell::peg)
                            return 0;
                    }
                    ++count;
                }
                else if(vec[i][index].type == cell::empty)
                    return 0;
                --i;
                ++index;
            }
        }
    }
    return count;
}

int PegSolitaire::checkGame()
{
    //Check if the game is finished or not
    int count = 0;
    int col = vec[0].size();
	int row = vec.size();

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (vec[i][j].type == cell::peg) //Check around the peg if there are other pegs
            {   
                if (i != 0 && j != 0 && i != row-1 && j != col-1)
                {
                    if (!(vec[i][j+1].type != cell::peg && vec[i][j-1].type != cell::peg 
                        && vec[i+1][j].type != cell::peg && vec[i-1][j].type != cell::peg))
                    {
                        if (checkFull(i, j, 0, 0) == 0)
                            return 0;
                        else
                            ++count;
                    }
                    else    
                        ++count;
                }
                else if (i == 0)
                {
                    if (!(vec[i][j+1].type != cell::peg && vec[i][j-1].type != cell::peg 
                        && vec[i+1][j].type != cell::peg))
                    {
                        if (checkFull(i, j, 0, 1) == 0)
                            return 0;
                        else
                            ++count;
                    }
                    else    
                        ++count;
                }
                else if (i == row-1)
                {
                    if (!(vec[i][j+1].type != cell::peg && vec[i][j-1].type != cell::peg 
                        && vec[i-1][j].type != cell::peg))
                    {
                        if (checkFull(i, j, 0, 2) == 0)
                            return 0;
                        else
                            ++count;
                    }
                    else    
                        ++count;
                }
                else if (j == 0)
                {
                    if (!(vec[i][j+1].type != cell::peg && vec[i+1][j].type != cell::peg 
                        && vec[i-1][j].type != cell::peg))
                    {
                        if (checkFull(i, j, 0, 3) == 0)
                            return 0;
                        else
                            ++count;
                    }
                    else    
                        ++count;
                }
                else if (j == col-1)
                {
                    if (!(vec[i][j-1].type != cell::peg && vec[i+1][j].type != cell::peg 
                        && vec[i-1][j].type != cell::peg))
                    {
                        if (checkFull(i, j, 0, 4) == 0)
                            return 0;
                        else
                            ++count;
                    }
                    else    
                        ++count;
                }
            }
        }  
    }
    return count;
}

int PegSolitaire::checkGameT6()
{
    //Check if the game is finished or not for Type 6
    int count = 0;
    int col = vec[0].size();
	int row = vec.size();

    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            if (vec[i][j].type == cell::peg) //Check around the peg if there are other pegs
            {
                if (i != 0 && j != 0 && i != row-1 && j != col-1)
                {
                    if (!(vec[i-1][j+1].type != cell::peg && vec[i-1][j-1].type != cell::peg 
                        && vec[i+1][j+1].type != cell::peg && vec[i+1][j-1].type != cell::peg
                        && vec[i][j+2].type != cell::peg && vec[i][j-2].type != cell::peg))
                    {
                        if(checkFull(i, j, 1, 0) == 0)
                            return 0;
                        else
                            ++count;
                    }
                    else    
                        ++count;
                }
                else if (i == row-1 && j == col-1)
                {
                    if (!(vec[i][j-2].type != cell::peg && vec[i-1][j-1].type != cell::peg))
                    {
                        if(checkFull(i, j, 1, 3) == 0)
                            return 0;
                        else
                            ++count;
                    }
                    else    
                        ++count;
                }
                else if (i == row-1 && j == 0)
                {
                    if (!(vec[i][j+2].type != cell::peg && vec[i-1][j+1].type != cell::peg))
                    {
                        if(checkFull(i, j, 1, 4) == 0)
                            return 0;
                        else
                            ++count;
                    }
                    else    
                        ++count;
                }
                else if (i == 0)
                {
                    if (!(vec[i+1][j-1].type != cell::peg && vec[i+1][j+1].type != cell::peg))
                    {
                        if(checkFull(i, j, 1, 1) == 0)
                            return 0;
                        else
                            ++count;
                    }
                    else    
                        ++count;
                }
                else if (i == row-1)
                {
                    if (!(vec[i][j+2].type != cell::peg && vec[i][j-2].type != cell::peg 
                        && vec[i-1][j-1].type != cell::peg && vec[i-1][j+1].type != cell::peg))
                    {
                        if(checkFull(i, j, 1, 2) == 0)
                            return 0;
                        else
                            ++count;
                    }
                    else    
                        ++count;
                }
            }
        }
    }
    return count;
}

void PegSolitaire::printRes() const
{
	if (flag == 1)
        cout << "Final Score: 1. You won!" << endl;
    else
        cout << "Final Score: " << flag << endl;
}

void PegSolitaire::getUserch()
{
    int flag = 0;
    do //Check for invalid inputs
    {
        if(flag != 0)
            cerr << "Invalid Type" <<endl;
        cout << "Type 1 to continue to the game, Type 2 to exit:";
        if(!(cin >> userch)) 
		{
		    cin.clear();
		    cin.ignore(10000,'\n');
		}
	    ++flag;
    }while(!(userch == 1 || userch == 2));
}

int PegSolitaire::fileop(string fileName)
{
    int col = vec[0].size();
	int row = vec.size();

    if(inType == 1 || inType == 3 || inType == 5 || inType == 7) //Load the game board
    {  
        ifstream fileio;
        fileio.open(fileName);
        string str;
        int sizeStr = 0, l = 0, counter = 0;
        while(l <= sizeStr)
        {
            getline(fileio, str);
            if(counter == 0 && str == "") //Check if the file is empty or not
            {
                cerr << "File is empty!" << endl;
                fileio.close();
                return -1;
            }
            sizeStr = str.size();
            if(counter == 0) //Create a N*N vector
            {
            	vec.resize(sizeStr);
            	for(int m = 0; m < sizeStr; m++)
            	{
            		vec[m].resize(sizeStr, Cell(0, 0, cell::out));
            		for(int n = 0; n < sizeStr; n++)
					{
						vec[m][n].coorR = m;
						vec[m][n].coorC = n;
					}
				}
			}
			
            for(int k = 0; k < sizeStr; k++)
            {
                if(str[k] == ' ')
                    vec[l][k].type = cell::out;
                else if(str[k] == '.')
                    vec[l][k].type = cell::empty;
                else if(str[k] == 'P')
                    vec[l][k].type = cell::peg;
            }
            ++l;
            ++counter;
            str.clear();
        }
        printGame();
        fileio.close();
    }
    else //Save the current game board
    {      
        ofstream ofs; //Clear the file
        ofs.open(fileName, ofstream::out | ofstream::trunc);
        ofs.close();
        
        fstream fileio;
        fileio.open(fileName);

        for(int i = 0; i < row; i++)
        {
            for(int j = 0; j < col; j++)
            {
                if(vec[i][j].type == cell::out)
                    fileio << " ";
                else if(vec[i][j].type == cell::empty)
                    fileio << ".";
                else if(vec[i][j].type == cell::peg)
                    fileio << "P";
            }
            fileio << endl;
        }
        fileio.close();
    }
    return 0;
}

void PegSolitaire::fileCheck()
{
	//Check if file operation is done and print the message
    if(inType == 1)
    {
        auto x = fileop("saved_game.txt");
        if(x == 0)
            cout << "Loaded successfully!" << endl;
    }
    else if(inType == 2)
    {
        fileop("saved_game.txt");
        cout << "Saved successfully!" << endl;
    }
    else if(inType == 3)
    {
        fileop("board_1.txt");;
        chosenType = 1;
        cout << "Loaded successfully!" << endl;
    }
    else if(inType == 4)
    {
        fileop("saved_game.txt");
        cout << "Saved successfully!" << endl;
    }
    else if(inType == 5)
    {
        fileop("board_2.txt");
        chosenType = 1;
        cout << "Loaded successfully!" << endl;
    }
    else if(inType == 6)
    {
        fileop("saved_game.txt");
        cout << "Saved successfully!" << endl;
    }
    else if(inType == 7)
    {
        fileop("board_3.txt");
        chosenType = 1;
        cout << "Loaded successfully!" << endl;
    }
    else if(inType == 8)
    {
        fileop("saved_game.txt");
        cout << "Saved successfully!" << endl;
    }
}

bool PegSolitaire::compareGames(PegSolitaire &other) 
{
	//If first board has more pegs then return true, otherwise false
	int total1 = getTotalPeg();
	int total2 = other.getTotalPeg();
	
    if(total1 > total2)
    	return true;
	else
		return false;
}
                
int PegSolitaire::getTotalPeg()
{
	int pegN = 0;
	int col = vec[0].size();
	int row = vec.size();
	
	for(int i = 0; i < row; i++)
    {
        for(int j = 0; j < col; j++)
        {
            if(vec[i][j].type == cell::peg)
           		++pegN;
        }
    }
    return pegN;
}
