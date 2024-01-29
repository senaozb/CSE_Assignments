/**
* This program stores the dates in a set and it allows to conduct the set operations.
* @author Sena Ozbelen
* @version 1.0
*/

import java.util.ArrayList;
import java.io.FileWriter; 
import java.io.IOException;
import java.io.File;

/**
* Every DayofYearSet object contains DayofYear objects which have day and month fields.
* DayofYearSet class includes set operations, constructors and overridden toString and equals methods
 */
public class DayofYearSet 
{
    /**
    * DayofYear class is a inner class of DayofYearSet.
    * It contains day and month fields as integers.
    * It has own constructors, getters, setters and toString overridden method.
    */
    public static class DayofYear
    {
        private int day;
        private int month;

        /** Constructor with no parameters */
        public DayofYear()
        {
            day = 1;
            month = 1;
        }
        /** Constructor with two parameters */
        public DayofYear(int d, int m)
        {
            day = d;
            month = m;
            ++totalDOY;
        }
        /** 
        * Getter for day field 
        * @return value of day as int
        */
        public int getDay(){return day;}
        /** 
        * Getter for month field 
        * @return value of month as int
        */
        public int getMonth(){return month;}
        /** Setter for day field */
        public void setDay(int d){day = d;}
        /** Setter for month field */
        public void setMonth(int m){month = m;}
        /** 
        * inputValid method checks if the given date is valid or not
        * @return true if the date is valid
        */
        public boolean inputValid()
        {
            if(getMonth() > 12 || getMonth() < 1)
                return false;
            if((getMonth() == 1 || getMonth() == 3 || getMonth() == 5 || getMonth() == 7 ||
            getMonth() == 8 || getMonth() == 10 || getMonth() == 12) && (getDay() > 31))
                return false;
            if((getMonth() == 4 || getMonth() == 6 || getMonth() == 9 || 
            getMonth() == 11) && (getDay() > 30))
                return false;
            if(getMonth() == 2 && (getDay() > 28))
                return false;
            
            return true;
        }
        /** 
        * Overridden toString method to display the dates 
        * @return the date as string
        */
        public String toString()
        {
            return this.day + "." + this.month;
        }
    }

    /** arr is used to store the elements of the set */
    private DayofYear[] arr;
    private int size;
    /** totalDOY is the number of total DayofYear objects */
    public static int totalDOY = 0;
    
    /** 
    * Getter for size field 
    * @return value of size as int
    */
    public int size(){return size;}
    
    /** Constructor with no parameters */
    public DayofYearSet()
    {
        size = 0;
        arr = new DayofYear[size];
    }
    /** Constructor with one integer parameter */
    public DayofYearSet(int s)
    {
        size = s;
        arr = new DayofYear[size];
    }
    /** Constructor with one ArrayList parameter */
    public DayofYearSet(ArrayList<DayofYear> arrList)
    {
        int index = 0;
        for(DayofYear i : arrList)
        {
            if(i.inputValid() == true)
                ++index;
            else
            {
                System.out.printf("Invalid Date for ");
                System.out.println(i.toString());
            }
        }
        size = index;
        arr = new DayofYear[size];
        index = 0;
        for(DayofYear i : arrList)
        {
            if(i.inputValid() == true)
            {
                DayofYear obj = new DayofYear(i.getDay(), i.getMonth());
                arr[index] = obj;
                ++index;
                --totalDOY;
            }
        }
    }

    /** 
    * add method is used to add new element to the set creating new array with new size 
    */
    public void add(DayofYear obj)
    {
        if(doesExist(obj) == false)
        {
            int s = size();
            DayofYear[] temp = new DayofYear[s+1];

            for(int i = 0; i < s; i++)
                temp[i] = arr[i];
            temp[s] = obj;

            arr = temp;
            ++size;
        }
        else
            System.out.println("Warning: Duplicated element!");
    }

    /** 
    * remove method is used to remove new element from the set creating new array with new size 
    */
    public void remove(DayofYear obj)
    {
        if(doesExist(obj) == true)
        {
            int s = size();
            DayofYear[] temp = new DayofYear[s-1];
            
            int index = 0;
            for(int i = 0; i < s; i++)
            {
                if(!(arr[i].getMonth() == obj.getMonth() && arr[i].getDay() == obj.getDay()))
                {
                    temp[index] = arr[i];
                    ++index;
                }
                else
                    --totalDOY;
            }

            arr = temp;
            --size;
            --totalDOY;
        }
        else
            System.out.println("Warning: Element does not exist!");
    }

    /** 
    * doesExist is a private method to check is the given date is in the set already
    * @param obj the given date
    * @return true if the date is already in the set
    */
    private boolean doesExist(DayofYear obj) 
    {
        //Order : Day, Month
        int d = obj.getDay();
        int m = obj.getMonth();
        for(int i = 0; i < size(); i++)
        {
            if(arr[i].getMonth() == m && arr[i].getDay() == d)
                return true;
        }
        return false;
    }
    /** 
    * union method returning union set 
    * @param obj another set to create an union set
    * @return an union set as DayofYearSet
    */
    public DayofYearSet union(DayofYearSet obj)
    {
        int size1 = size();
        int size2 = obj.size();
        int counter = size1;
        int index = size1;

        for(int i = 0; i < size2; i++)
        {
            if(doesExist(obj.arr[i]) == false)
                ++counter;
        }

        DayofYearSet n = new DayofYearSet(counter);
        for (int j = 0; j < size1; j++)
        {
            n.arr[j] = arr[j];
            ++totalDOY;
        }
        for(int k = 0; k < size2; k++)
        {
            if(doesExist(obj.arr[k]) == false)
            {
                n.arr[index] = obj.arr[k];
                ++index;
                ++totalDOY;
            }
        }

        return n;
    }

    /** 
    * intersection method returning intersection of the sets
    * @param obj another set to create an intersection of sets
    * @return intersection of sets as DayofYearSet
    */
    public DayofYearSet intersection(DayofYearSet obj)
    {
        int size1 = size();
        int index = 0;
        int counter = 0;

        for(int i = 0; i < size1; i++)
        {
            if(obj.doesExist(arr[i]) == true)
                ++counter;
        }

        DayofYearSet n = new DayofYearSet(counter);
        for(int j = 0; j < size1; j++)
        {
            if(obj.doesExist(arr[j]) == true)
            {
                n.arr[index] = arr[j];
                ++index;
                ++totalDOY;
            }
        }
        return n;
    }

    /** 
    * difference method returning difference of the sets
    * @param obj another set to create a difference set
    * @return a difference set as DayofYearSet
    */
    public DayofYearSet difference(DayofYearSet obj)
    {
        int size1 = size();
        int index = 0;
        int counter = 0;

        for(int i = 0; i < size1; i++)
        {
            if(obj.doesExist(arr[i]) == true)
                ++counter;
        }

        DayofYearSet n = new DayofYearSet(size1-counter);
        for(int j = 0; j < size1; j++)
        {
            if(obj.doesExist(arr[j]) == false)
            {
                n.arr[index] = arr[j];
                ++index;
                ++totalDOY;
            }
        }
        return n;
    }

    /** 
    * complement method returning complement of the set
    * @return a complement set as DayofYearSet
    */
    public DayofYearSet complement()
    {
        int capacity = 365-size();
        DayofYearSet n = new DayofYearSet(capacity);
        int counter = 0;

        for(int i = 1; i <= 12; i++)
        {
            if(i == 1 || i == 3 || i == 5 || i == 7 || i == 8 || i == 10 || i == 12)
            {
                for(int j = 1; j <= 31; j++)
                {
                    DayofYear obj = new DayofYear(j, i);
                    if(doesExist(obj) == false)
                    {
                        n.arr[counter] = obj;
                        ++counter;
                    }
                    else
                        --totalDOY;
                }
            }
            else if(i == 4 || i == 6 || i == 9 || i == 11)
            {
                for(int j = 1; j <= 30; j++)
                {
                    DayofYear obj = new DayofYear(j, i);
                    if(doesExist(obj) == false)
                    {
                        n.arr[counter] = obj;
                        ++counter;
                    }
                    else
                        --totalDOY;
                }
            }
            else
            {
                for(int j = 1; j <= 28; j++)
                {
                    DayofYear obj = new DayofYear(j, i);
                    if(doesExist(obj) == false)
                    {
                        n.arr[counter] = obj;
                        ++counter;
                    }
                    else
                        --totalDOY;
                }
            }
        }
        return n;
    }
    /** 
    * toString is an overridden method to display all the dates in the set returning a string 
    * @return a string of all dates in the set
    */
    public String toString()
    {
        String setS = "";
        for(int i = 0; i < size(); i++)
        {
            String temp = arr[i].toString();
            setS = setS.concat(temp);
            setS = setS.concat(", ");
        }
        return setS;
    }

    /** 
    * equals is an overridden method to compare two sets 
    * @param obj another set to compare
    * @return true if sets are the same 
    */
    public boolean equals(DayofYearSet obj)
    {
        //Overridden equals method
        int size1 = size();
        int size2 = obj.size();
        
        if(size1 != size2)
            return false;
        
        for(int i = 0; i < size1; i++)
        {
            if(doesExist(obj.arr[i]) == false)
                return false;
        }

        return true;
    }
    /** printSet is used to print the set details */
    public void printSet()
    {
        System.out.println("Days in the set: ");
        for(DayofYear i : arr)
        {
            int month = i.getMonth();
            switch (month)
            {
                case 1:
                    System.out.printf("January "); 
                    break;
                case 2:
                    System.out.printf("February "); 
                    break;
                case 3:
                    System.out.printf("March "); 
                    break;
                case 4:
                    System.out.printf("April ");
                    break;
                case 5:
                    System.out.printf("May ");
                    break;
                case 6:
                    System.out.printf("June ");
                    break;
                case 7:
                    System.out.printf("July ");
                    break;
                case 8:
                    System.out.printf("August ");
                    break;
                case 9:
                    System.out.printf("September ");
                    break;
                case 10:
                    System.out.printf("October ");
                    break;
                case 11:
                    System.out.printf("November ");
                    break;
                case 12:
                    System.out.printf("December ");
                    break;
            }
            System.out.println(i.getDay());
        } 
    }

    /** 
    * totalDayofYear is a static method to return the total number of the DayofYear objects 
    * @return value of totalDOY as int
    */
    public static int totalDayofYear() {return totalDOY;}

    /** writeFile method is used to write the set to a text file */
    public void writeFile()
    {
        try 
        {
            File obj = new File("sets.txt");
            FileWriter ioFile;
            if (obj.exists() == false) 
            {
                ioFile = new FileWriter("sets.txt");
            }
            else 
            {
                ioFile = new FileWriter("sets.txt", true);
            }

            String s = toString();
            ioFile.write("Set: ");
            ioFile.write(s);
            ioFile.write("\n");
            ioFile.close();
            System.out.println("Successfully wrote to the file.");
        }
        catch (IOException e) 
        {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }
    }
}