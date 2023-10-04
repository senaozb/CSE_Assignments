import java.util.ArrayList;
import java.util.InputMismatchException;
import java.util.ListIterator;

/**
* This program displays the general plan of a small town and allows the user to edit and view
* @author Sena Ozbelen
* @version 1.0
*/

/**This class is used to carry out main methods of the program*/
public class TownwithArrayList
{
    /**The length of the street */
    private int pathLength;
    /**Element type array list to store the objects*/
    private ArrayList<Element> arrayElements;
   
    /**Constructor with a length parameter
    * @param pathLength the length of the street
    */
    public TownwithArrayList(int pathLength) 
    {
        if(pathLength <= 0)
            throw new InputMismatchException();
        else
            this.pathLength = pathLength;

        arrayElements = new ArrayList<Element>();
    }

    /**An inner class extended from Element class and it stores the data of houses*/
    public static class House extends Element implements Cloneable
    {
        /**The number of rooms in the house*/
        private int roomNumber;
        /**The color of the house */
        private String color;
        /**The owner of house */
        private String owner;

        /**Constructor with 7 parameters 
        * @param lengthObj
        * @param heightObj
        * @param roomNumber
        * @param color
        * @param index
        * @param direction
        * @param owner
        */
        public House(int lengthObj, int heightObj, int roomNumber, String color, int index, char direction, String owner)
        {
            this.type = "House";
            
            if(lengthObj <= 0)
                throw new InputMismatchException();
            else
                this.lengthObj = lengthObj;
           
            if(heightObj <= 0)
                throw new InputMismatchException();
            else
                this.heightObj = heightObj;
            
            if(roomNumber <= 0)
                throw new InputMismatchException();
            else
                this.roomNumber = roomNumber;
            
            this.color = color;
            
            if(index < 0)
                throw new InputMismatchException();
            else
                this.index = index;
           
             if(!(direction == 'r' || direction == 'l'))
                throw new InputMismatchException();
            else
                this.direction = direction;

            this.owner = owner;
        }

        /**Getter method for the room number
        * @return the number of room
        */
        public int GetRoomNumber(){return roomNumber;}
        /**Getter method for the color
        * @return the color of the house
        */
        public String GetColor(){return color;}
        /**Getter method for the owner
        * @return the owner of the house
        */
        public String GetOwner(){return owner;}

        /**This shows the focus type of the element */
        public void FocusType()
        {
            System.out.println("House: " + GetOwner());
        }

        /**This prints the starting index of the element */
        public void PrintIndex()
        {
            System.out.println("The index of this house is" + GetIndex());
        }

        @Override
        public String toString()
        {
            return "\nHouse: \n" + "Length: " + lengthObj + "\nHeight: " + heightObj + "\nRoom Number: "
                + roomNumber + "\nColor: " + color + "\nIndex: " + index + "\nDirection: " + direction + "\nOwner: "
                + owner + "\n";
        }

        @Override
        public boolean equals(Object obj) 
        {
            if (obj == this) 
                return true;
         
            if (!(obj instanceof House)) 
                return false;
            
            House temp = (House) obj;
            
            if ((temp.GetLength() == GetLength()) && (temp.GetHeight() == GetHeight()) && (temp.GetRoomNumber() == GetRoomNumber())
                && (temp.GetIndex() == GetIndex()) && (temp.GetColor().equals(GetColor()) == true) && (temp.GetOwner().equals(GetOwner()) == true)
                && (temp.GetDirection() == GetDirection()))
                return true;

            return false;
        }

        @Override
        public Object clone() throws CloneNotSupportedException
        {  
            return super.clone();  
        }  

        @Override
        public int hashCode() 
        {
            int result = 17;
            result = 31 * result + lengthObj;
            result = 31 * result + heightObj;
            result = 31 * result + roomNumber;
            result = 31 * result + color.hashCode();
            result = 31 * result + index;
            result = 31 * result + direction;
            result = 31 * result + owner.hashCode();
            return result;
        }

    }

    /**An inner class extended from Element class and it stores the data of offices*/
    public static class Office extends Element implements Cloneable
    {
        /*The type of the job */
        private String jobType;
        /**The owner of office */
        private String owner;
        
        /**Constructor with 6 parameters
        * @param lengthObj
        * @param heightObj
        * @param jobType
        * @param index
        * @param direction
        * @param owner
         */
        public Office(int lengthObj, int heightObj, String jobType, int index, char direction, String owner)
        {
            this.type = "Office";
            
            if(lengthObj <= 0)
                throw new InputMismatchException();
            else
                this.lengthObj = lengthObj;
            
            if(heightObj <= 0)
                throw new InputMismatchException();
            else
                this.heightObj = heightObj;

            this.jobType = jobType;
            
            if(index < 0)
                throw new InputMismatchException();
            else
                this.index = index;

            if(!(direction == 'r' || direction == 'l'))
                throw new InputMismatchException();
            else 
                this.direction = direction;
            
            this.owner = owner;
        }

        /**Getter method for the job type
        * @return the type of job
        */
        public String GetJobType(){return jobType;}
        /**Getter method for the owner
        * @return the owner of the office
        */
        public String GetOwner(){return owner;}

        /**This shows the focus type of the element */
        public void FocusType()
        {
            System.out.println("Office: " + GetJobType());
        }

        /**This prints the starting index of the element */
        public void PrintIndex()
        {
            System.out.println("The index of this office is" + GetIndex());
        }

        @Override
        public String toString()
        {
            return "\nOffice: \n" + "Length: " + lengthObj + "\nHeight: " + heightObj + "\nJob Type: "
                + jobType + "\nIndex: " + index + "\nDirection: " + direction + "\nOwner: " + owner + "\n";
        }

        @Override
        public boolean equals(Object obj) 
        {
            if (obj == this) 
                return true;
         
            if (!(obj instanceof Office)) 
                return false;
            
            Office temp = (Office) obj;
            
            if ((temp.GetLength() == GetLength()) && (temp.GetHeight() == GetHeight()) && (temp.GetIndex() == GetIndex()) 
                && (temp.GetJobType().equals(GetJobType()) == true) && (temp.GetOwner().equals(GetOwner()) == true)
                && (temp.GetDirection() == GetDirection()))
                return true;

            return false;
        }

        @Override
        public Object clone() throws CloneNotSupportedException
        {  
            return super.clone();  
        }  

        @Override
        public int hashCode() 
        {
            int result = 17;
            result = 31 * result + lengthObj;
            result = 31 * result + heightObj;
            result = 31 * result + jobType.hashCode();
            result = 31 * result + index;
            result = 31 * result + direction;
            result = 31 * result + owner.hashCode();
            return result;
        }
    }

    /**An inner class extended from Element class and it stores the data of markets*/
    public static class Market extends Element implements Cloneable
    {
        /**This shows the opening time of the market*/
        private int openingTime;
        /**This shows the closing time of the market*/
        private int closingTime;
        /**The owner of market */
        private String owner;

        /**Constructor with 7 parameters 
        * @param lengthObj
        * @param heightObj
        * @param openingTime
        * @param closingTime
        * @param index
        * @param direction
        * @param owner
        */
        public Market(int lengthObj, int heightObj, int openingTime, int closingTime, int index, char direction, String owner)
        {
            this.type = "Market";

            if(lengthObj <= 0)
                throw new InputMismatchException();
            else
                this.lengthObj = lengthObj;

            if(heightObj <= 0)
                throw new InputMismatchException();
            else
                this.heightObj = heightObj;

            if(openingTime < 0 || openingTime > 23)
                throw new InputMismatchException();
            else
                this.openingTime = openingTime;

            if(openingTime < 0 || openingTime > 23)
                throw new InputMismatchException();
            else
                this.closingTime = closingTime;

            if(index < 0)
                throw new InputMismatchException();
            else
                this.index = index;

            if(!(direction == 'r' || direction == 'l'))
                throw new InputMismatchException();
            else 
                this.direction = direction;
            
            this.owner = owner;
        }
        
        /**Getter method for the opening time 
        * @return the time of opening
        */
        public int GetOpeningTime(){return openingTime;}
        /**Getter method for the closing time
        * @return the time of closing
        */
        public int GetClosingTime(){return closingTime;}
        /**Getter method for the owner
        * @return the owner of the market
        */
        public String GetOwner(){return owner;}

        /**This shows the focus type of the element */
        public void FocusType()
        {
            System.out.println("Market: " + GetClosingTime());
        }

        /**This prints the starting index of the element */
        public void PrintIndex()
        {
            System.out.println("The index of this market is" + GetIndex());
        }

        @Override
        public String toString()
        {
            return "\nMarket: \n" + "Length: " + lengthObj + "\nHeight: " + heightObj + "\nOpening Time: "
                + openingTime + "\nClosing Time: " + closingTime + "\nIndex: " + index + "\nDirection: " + direction + "\nOwner: "
                + owner + "\n";
        }
        
        @Override
        public boolean equals(Object obj) 
        {
            if (obj == this) 
                return true;
         
            if (!(obj instanceof Market)) 
                return false;
            
            Market temp = (Market) obj;
            
            if ((temp.GetLength() == GetLength()) && (temp.GetHeight() == GetHeight()) && (temp.GetOpeningTime() == GetOpeningTime())
                && (temp.GetClosingTime() == GetClosingTime()) && (temp.GetIndex() == GetIndex()) && (temp.GetOwner().equals(GetOwner()) == true)
                && (temp.GetDirection() == GetDirection()))
                return true;

            return false;
        }

        @Override
        public Object clone() throws CloneNotSupportedException
        {  
            return super.clone();  
        }  

        @Override
        public int hashCode() 
        {
            int result = 17;
            result = 31 * result + lengthObj;
            result = 31 * result + heightObj;
            result = 31 * result + openingTime;
            result = 31 * result + closingTime;
            result = 31 * result + index;
            result = 31 * result + direction;
            result = 31 * result + owner.hashCode();
            return result;
        }
    }

    /**An inner class extended from Element class and it stores the data of playground*/
    public static class Playground extends Element implements Cloneable
    {
        /**Constructor with 3 parameters
        * @param lengthObj
        * @param index
        * @param direction
        */
        public Playground(int lengthObj, int index, char direction)
        {
            this.type = "Playground";
            this.heightObj = 1;

            if(lengthObj <= 0)
                throw new InputMismatchException();
            else
                this.lengthObj = lengthObj;

            if(index < 0)
                throw new InputMismatchException();
            else
            this.index = index;

            if(!(direction == 'r' || direction == 'l'))
                throw new InputMismatchException();
            else 
                this.direction = direction;
        }

        /**This shows the focus type of the element */
        public void FocusType()
        {
            System.out.println("Playground: " + GetLength());
        }

        /**This prints the starting index of the element */
        public void PrintIndex()
        {
            System.out.println("The index of this playground is" + GetIndex());
        }
        
        @Override
        public String toString()
        {
            return "\nPlayground: \n" + "Length: " + lengthObj + "\nIndex: " + index + "\nDirection: " + direction + "\n";
        }

        @Override
        public boolean equals(Object obj) 
        {
            if (obj == this) 
                return true;
         
            if (!(obj instanceof Playground)) 
                return false;
            
            Playground temp = (Playground) obj;
            
            if ((temp.GetLength() == GetLength()) && (temp.GetHeight() == GetHeight()) && (temp.GetIndex() == GetIndex())
                && (temp.GetDirection() == GetDirection()))
                return true;

            return false;
        }

        @Override
        public Object clone() throws CloneNotSupportedException
        {  
            return super.clone();  
        }  

        @Override
        public int hashCode() 
        {
            int result = 17;
            result = 31 * result + lengthObj;
            result = 31 * result + heightObj;
            result = 31 * result + index;
            result = 31 * result + direction;
            return result;
        }
    }
    
    /**This performs the addition operations by overloading the method
     * @param obj the element which the user wants to add
    */
    public void EditMode(Element obj) 
    {
        String type = obj.GetType();
        switch(type)
        {
            case "House":
                House house = (House)obj;
                Add(house);
                break;
            case "Office":
                Office office = (Office)obj;
                Add(office);
                break;
            case "Market":
                Market market = (Market)obj;
                Add(market);
                break;
            case "Playground":
                Playground playground = (Playground)obj;
                Add(playground);
                break;
        }
    }

    /**This performs the removing operations by overloading the method
     * @param index the index of the element which the user wants to remove
     * @param direction the direction of the element
    */
    public void EditMode(int index, char direction) 
    {
        Remove(index, direction);
    }

    /**This performs the view mode operations
     * @param decision the user's decision
    */
    public void ViewMode(int decision)
    {
        switch(decision)
        {
            case 1:
                TotalRemaining();
                break;
            case 2:
                TotalBuildings();
                break;
            case 3:
                TotalPlaygrounds();
                break;
            case 4:
                TotalLengthOfBuildings();
                break;
            case 5:
                SkylineSilhouette();
                break;
        }
    }

    /**Add method for houses
     * @param house the object with a type of house
    */
    private void Add(House house)
    {
        if(CheckEmpty(house.GetIndex(), house.GetDirection(), house.GetLength()) == false)
            throw new ArrayIndexOutOfBoundsException(house.GetIndex());
        else
        {
            //Add the element to the Element array list
            arrayElements.add((Element)house);
        }
    }

    /**Add method for markets
     * @param market the object with a type of market
    */
    private void Add(Market market)
    {
        if(CheckEmpty(market.GetIndex(), market.GetDirection(), market.GetLength()) == false)
            throw new ArrayIndexOutOfBoundsException(market.GetIndex());
        else
        {
            //Add the element to the Element array list
            arrayElements.add((Element)market);
        }
    }

    /**Add method for offices
     * @param office the object with a type of office
    */
    private void Add(Office office)
    {
        if(CheckEmpty(office.GetIndex(), office.GetDirection(), office.GetLength()) == false)
            throw new ArrayIndexOutOfBoundsException(office.GetIndex());
        else
        {
            //Add the element to the Element array list
            arrayElements.add((Element)office);
        }
    }

    /**Add method for playgrounds
     * @param playground the object with a type of playground
    */
    private void Add(Playground playground)
    {
        if(CheckEmpty(playground.GetIndex(), playground.GetDirection(), playground.GetLength()) == false)
            throw new ArrayIndexOutOfBoundsException(playground.GetIndex());
        else
        {
            //Add the element to the Element array list
            arrayElements.add((Element)playground);
        }
    }

    /**Remove method based on the index of element
     * @param ind the index of the element
     * @param dir the direction of the element
    */
    private void Remove(int ind, char dir)
    {
        if(ind < 0 || ind >= pathLength)
            throw new ArrayIndexOutOfBoundsException(ind);
        else if(!(dir == 'r' || dir == 'l'))
            throw new ArrayIndexOutOfBoundsException();
        else
        {
            ListIterator<Element> i = arrayElements.listIterator();
            while(i.hasNext())
            {
                Element element = i.next();
                if((element.GetIndex() <= ind && ind < element.GetIndex()+element.GetLength()) && element.GetDirection() == dir)
                {
                    i.remove();
                    break;
                }
            }
        }
    }

    /**This method shows the total remaining lands for both sides*/
    private void TotalRemaining()
    {
        int right = 0;
        int left = 0;
        
        ListIterator<Element> i = arrayElements.listIterator();
        while(i.hasNext())
        {
            Element element = i.next();
            if(element.GetDirection() == 'r')
                right += element.GetLength();
            else
                left += element.GetLength();
        }

        right = pathLength-right;
        left = pathLength-left;

        System.out.println("\nTotal Remaining Lands on Right Side:" + right);
        System.out.println("Total Remaining Lands on Left Side:" + left);
    }
   
    /**This method shows all the buildings with their info*/
    private void TotalBuildings()
    {
        System.out.println("\nList of the buildings: \n");
        
        ListIterator<Element> i = arrayElements.listIterator();
        while(i.hasNext())
        {
            Element element = i.next();
            if(element.GetType().equals("Playground") == false)
                System.out.print(element.toString());
        }
    }

    /**This method shows the ratio of playgrounds and the number of playgrounds*/
    private void TotalPlaygrounds()
    {
        int lenRight = 0;
        int lenLeft = 0;
        int num = 0;

        ListIterator<Element> i = arrayElements.listIterator();
        while(i.hasNext())
        {
            Element element = i.next();
            if(element.GetType().equals("Playground") == true)
            {
                num++;
                if(element.GetDirection() == 'r')
                    lenRight += element.GetLength();
                else
                    lenLeft += element.GetLength();
            }
        }

        System.out.println("The ratio of total length of playgrounds on right side is " + lenRight + "/" + pathLength);
        System.out.println("The ratio of total length of playgrounds on left side is " + lenLeft + "/" + pathLength);

        System.out.println("The number of total playgrounds is " + num);
    }

    /**This method shows the total length of the buildings*/
    private void TotalLengthOfBuildings()
    {
        int len = 0;

        ListIterator<Element> i = arrayElements.listIterator();
        while(i.hasNext())
        {
            Element element = i.next();
            if(element.GetType().equals("Playground") == false)
                len += element.GetLength();
        }
        
        System.out.println("The total length of markets, offices and houses is " + len);
    }

    /**This method shows the skyline silhouette of the town*/
    private void SkylineSilhouette()
    {
        System.out.println("\nThe Skyline Silhouette\n");
        int maxHeight = 0;
        int flag = 0;

        ListIterator<Element> iter = arrayElements.listIterator();
        while(iter.hasNext())
        {
            Element element = iter.next();
            if(element.GetHeight() > maxHeight)
                maxHeight = element.GetHeight();
        }
       
        for (int i = maxHeight; i > 0; i--)
        {
            for (int j = 0; j < pathLength; j++)
            {
                ListIterator<Element> iterInner = arrayElements.listIterator();
                while(iterInner.hasNext()) 
                {
                    Element element = iterInner.next();
                    if((element.GetHeight() >= i) && (element.GetIndex() <= j && j < element.GetIndex()+element.GetLength()))
                    {
                        System.out.print("*");
                        flag = 1;
                        break;
                    }
                }
                if(flag == 0)
                    System.out.print(" ");
                else
                    flag = 0;
            }
            System.out.print('\n');
        }

        for (int i = 0; i < pathLength; i++)
        {
            System.out.print("-");
        }
        System.out.print('\n');
    }

    /**This method checks if the desired area is empty and suitable
     * @param index the index of the object
     * @param direction the direction of the object
     * @param length the length of the obejct
     * @return if it is empty then return true
    */
    private boolean CheckEmpty(int index, char direction, int length)
    {
        if(index+length > pathLength)
            return false;
        else
        {
            ListIterator<Element> iter = arrayElements.listIterator();
            while(iter.hasNext())
            {
                Element element = iter.next();
                if(element.GetDirection() == direction)
                {
                    for(int i = index; i < index+length; i++)
                    {
                        if(element.GetIndex() <= i && i < element.GetIndex()+element.GetLength())
                            return false;
                    }
                }
            }
            return true;
        }
    }
}