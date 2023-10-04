import java.util.InputMismatchException;
import java.util.Scanner;

public class Main 
{ 
    public static void main(String[] args)
    {
        testClassTown();
        Menu();
    }

    /**Driver method to test the methods*/
    public static void testClassTown()
    {
        try
        {
            int len = 50;
            Town town = new Town(len);
            Town.House h1 = new Town.House(10, 2, 2, "black", 0, 'l', "Owner1");
            Town.Office o1 = new Town.Office(4, 10, "Dentist", 15, 'r', "Owner2");
            Town.Market m1 = new Town.Market(5, 4, 9, 20, 11, 'l', "Owner3");
            Town.Playground p1 = new Town.Playground(10, 40, 'r');
            town.EditMode(h1);
            town.EditMode(o1);
            town.EditMode(m1);
            town.EditMode(p1);
            System.out.println("\n-----------------------------------\n");
            town.ViewMode(1);
            System.out.println("\n-----------------------------------\n");
            town.ViewMode(2);
            System.out.println("\n-----------------------------------\n");
            town.ViewMode(3);
            System.out.println("\n-----------------------------------\n");
            town.ViewMode(4);
            System.out.println("\n-----------------------------------\n");
            town.ViewMode(5);

            System.out.println("Removing the house at 0");
            town.EditMode(0, 'l');
            System.out.println("\n-----------------------------------\n");
            town.ViewMode(1);
            System.out.println("\n-----------------------------------\n");
            town.ViewMode(2);
            System.out.println("\n-----------------------------------\n");
            town.ViewMode(5);

            Town.House h2 = new Town.House(10, 5, 2, "white", 45, 'l', "Owner4");
            System.out.println("\nAdding the house at 45 (error check)");
            town.EditMode(h2);

        }
        catch(Exception e)
        {
            System.out.println("\n\nSomething went wrong!\n\n");
        }
    }

    /**Interactive method for the user*/
    public static void Menu()
    {
        Town town = new Town(1); //default
        int flag = 0, check = 0;
        int decision;
        Scanner myObj, myObj1, myObj2, myObj3;
        System.out.println("\nWelcome to the small street town\n");
        
        do
        {
            myObj = new Scanner(System.in);
            try 
            {
                //Create the street town with the given input
                System.out.println("Enter the length of the street:");
                decision = myObj.nextInt();
                town = new Town(decision);
                check = 1;
            }
            catch (Exception e) 
            {
                System.out.println("\nPlease enter a valid input!\n");
            }
        }
        while(check == 0);

        do
        {
            myObj1 = new Scanner(System.in);
            try
            {
                //Get what operation the user wants to perform
                System.out.println("For edit mode, type 1 | For view mode, type 2");
                decision = myObj1.nextInt();
                CheckInput(1, decision);
                if(decision == 1) //Edit Mode
                {
                    check = 0;
                    do
                    {
                        myObj2 = new Scanner(System.in);
                        try 
                        {
                            System.out.println("For addition, type 1 | For removing, type 2");
                            decision = myObj2.nextInt();
                            CheckInput(1, decision);
                            if(decision == 1) //Addition
                            {
                                Element ele = GetElement();
                                town.EditMode(ele);   
                            }
                            else //Removing
                            {
                                int f = 0;
                                int index = 0;
                                char direction = 'r';
                                do
                                {
                                    Scanner myObjx = new Scanner(System.in);
                                    try 
                                    {
                                        System.out.print("Type the index of the element:");
                                        index = myObjx.nextInt();
                                        System.out.print("Type the direction of the element(r for right - l for left):");
                                        direction = myObjx.next().charAt(0);
                                        f = 1;
                                    } 
                                    catch (Exception e) 
                                    {
                                        System.out.println("\nPlease enter a valid input!\n");
                                    }
                                }
                                while(f == 0);
                                town.EditMode(index, direction);   
                            }
                            check = 1;
                        }
                        catch (Exception e)
                        {
                            System.out.println("\nPlease enter a valid input!\n");
                        }
                    }
                    while(check == 0);
                }
                else if(decision == 2) //View Mode
                {
                    check = 0;
                    do
                    {
                        myObj2 = new Scanner(System.in);
                        try 
                        {
                            System.out.println("To display the total remaining length of lands on the street, type 1");
                            System.out.println("To display the list of buildings on the street, type 2");
                            System.out.println("To display the number and ratio of lenth of playgrounds in the street, type 3");
                            System.out.println("To calculate the total length of street occupied by the markets, houses or offices, type 4");
                            System.out.println("To display the skyline silhouette of the street, type 5");
                            decision = myObj2.nextInt();
                            CheckInput(2, decision);
                            town.ViewMode(decision);
                            check = 1;
                        } 
                        catch (Exception e) 
                        {
                            System.out.println("\nPlease enter a valid input!\n");
                        }
                    }
                    while(check == 0);
                }

                check = 0;
                do //Flag update
                {
                    myObj3 = new Scanner(System.in);
                    try 
                    {
                        System.out.println("To continue, type 0 | To exit, type 1");
                        flag = myObj3.nextInt();  
                        CheckInput(3, flag);
                        check = 1;
                    } 
                    catch (Exception e) 
                    {
                        System.out.println("\nPlease enter a valid input!\n");
                    }
                }
                while(check == 0);
            }
            catch(Exception e)
            {
                System.out.println("\nPlease enter a valid input!\n");
            }
        }
        while(flag == 0);
    }

    /**Check the input if it is in the valid interval */
    public static void CheckInput(int type, int input)
    {
        if(type == 1)
        {
            if(!(input == 1 || input == 2))
                throw new InputMismatchException();
        }
        else if(type == 2)
        {
            if(input < 1 || input > 5)
                throw new InputMismatchException();
        }
        else if(type == 3)
        {
            if(!(input == 0 || input == 1))
                throw new InputMismatchException();
        }
        else if(type == 4)
        {
            if(input < 1 || input > 4)
                throw new InputMismatchException();
        }
    }

    /**Get the desired element's information from the user by calling the right method */
    public static Element GetElement()
    {
        Element inp = null;
        int flag = 0;

        do 
        {
            Scanner myObj = new Scanner(System.in);
            try 
            {
                System.out.println("To add a house, type 1");
                System.out.println("To add an office, type 2");
                System.out.println("To add a market, type 3");
                System.out.println("To add a playground, type 4");
                int input = myObj.nextInt();
                CheckInput(4, input);
                switch(input)
                {
                    case 1:
                        inp = GetHouse();
                        flag = 1;
                        break;
                    case 2:
                        inp = GetOffice();
                        flag = 1;
                        break;
                    case 3:
                        inp = GetMarket();
                        flag = 1;
                        break;
                    case 4:
                        inp = GetPlayground();
                        flag = 1;
                        break;
                }

            } 
            catch (Exception e) 
            {
                System.out.println("\nPlease enter a valid input!\n");
            }
        }
        while (flag == 0);
        return inp;
    }

    /**Get a house object specifically
     * @return A house object as element
     */
    public static Element GetHouse()
    {
        Town.House obj;
        int flag = 0;
        int lengthObj = 0;
        int heightObj = 0;
        int roomNumber = 0;
        String color = "black";
        int index = 0;
        char direction = 'r';
        String owner = "owner";

        do
        {
            Scanner myObj = new Scanner(System.in);
            try 
            {
                System.out.print("Length of the house:");
                lengthObj = myObj.nextInt();
                System.out.print("Height of the house:");
                heightObj = myObj.nextInt();
                System.out.print("Number of the rooms of the house:");
                roomNumber = myObj.nextInt();
                System.out.print("Color of the house:");
                color = myObj.next();
                System.out.print("Index of the house:");
                index = myObj.nextInt();
                System.out.print("Direction of the house (r for right - l for left):");
                direction = myObj.next().charAt(0);
                System.out.print("Owner of the house:");
                owner = myObj.next();
                flag = 1;
            } 
            catch (Exception e) 
            {
                System.out.println("\nPlease enter a valid input!\n");
            }
        }
        while(flag == 0);
        obj = new Town.House(lengthObj, heightObj, roomNumber, color, index, direction, owner);
        return obj;
    }

    /**Get an office object specifically
     * @return An office object as element
     */
    public static Element GetOffice()
    {
        Town.Office obj;
        int flag = 0;
        int lengthObj = 0;
        int heightObj = 0;
        String jobType = "Dentist";
        int index = 0;
        char direction = 'r';
        String owner = "owner";

        do
        {
            Scanner myObj = new Scanner(System.in);
            try 
            {
                System.out.print("Length of the office:");
                lengthObj = myObj.nextInt();
                System.out.print("Height of the office:");
                heightObj = myObj.nextInt();
                System.out.print("Job type of the office:");
                jobType = myObj.next();
                System.out.print("Index of the office:");
                index = myObj.nextInt();
                System.out.print("Direction of the office (r for right - l for left):");
                direction = myObj.next().charAt(0);
                System.out.print("Owner of the office:");
                owner = myObj.next();
                flag = 1;
            } 
            catch (Exception e) 
            {
                System.out.println("\nPlease enter a valid input!\n");
            }
        }
        while(flag == 0);
        obj = new Town.Office(lengthObj, heightObj, jobType, index, direction, owner);
        return obj;
    }

    /**Get a market object specifically
     * @return A market object as element
     */
    public static Element GetMarket()
    {
        Town.Market obj;
        int flag = 0;
        int lengthObj = 0;
        int heightObj = 0;
        int openingTime = 9;
        int closingTime = 20;
        int index = 0;
        char direction = 'r';
        String owner = "owner";

        do
        {
            Scanner myObj = new Scanner(System.in);
            try 
            {
                System.out.print("Length of the market:");
                lengthObj = myObj.nextInt();
                System.out.print("Height of the market:");
                heightObj = myObj.nextInt();
                System.out.print("Opening time of the market as 24 hours e.g 9:");
                openingTime = myObj.nextInt();
                System.out.print("Closing time of the market as 24 hours e.g 20:");
                closingTime = myObj.nextInt();
                System.out.print("Index of the market:");
                index = myObj.nextInt();
                System.out.print("Direction of the market (r for right - l for left):");
                direction = myObj.next().charAt(0);
                System.out.print("Owner of the market:");
                owner = myObj.next();   
                flag = 1;
            } 
            catch (Exception e) 
            {
                System.out.println("\nPlease enter a valid input!\n");
            }
        }
        while(flag == 0);

        obj = new Town.Market(lengthObj, heightObj, openingTime, closingTime, index, direction, owner);
        return obj;
    }

    /**Get a playground object specifically
     * @return A playground object as element
     */
    public static Element GetPlayground()
    {
        Town.Playground obj;
        int flag = 0;
        int lengthObj = 0;
        int index = 0;
        char direction = 'r';

        do
        {
            Scanner myObj = new Scanner(System.in);
            try 
            {
                System.out.print("Length of the playground:");
                lengthObj = myObj.nextInt();
                System.out.print("Index of the playground:");
                index = myObj.nextInt();
                System.out.print("Direction of the playground (r for right - l for left):");
                direction = myObj.next().charAt(0);
                flag = 1;
            } 
            catch (Exception e) 
            {
                System.out.println("\nPlease enter a valid input!\n");
            }
        }
        while(flag == 0);

        obj = new Town.Playground(lengthObj, index, direction);
        return obj;
    }
}
