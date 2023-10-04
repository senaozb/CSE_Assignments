/**
* @author Sena Ozbelen
* @version 1.0
*/

/**Element is an abstract class and it includes common aspects of four types of elements in the town*/
public abstract class Element
{
    /**This is the type of element */
    protected String type;
    /**This is the length of element */
    protected int lengthObj;
    /**This is the starting index of element */
    protected int index;
    /**This indicates in which side element is */
    protected char direction;
    /**This is the height of element */
    protected int heightObj;
    
    /**Getter method for the type*/
    public String GetType(){return type;}
    /**Getter method for the length*/
    public int GetLength(){return lengthObj;}
    /**Getter method for the index*/
    public int GetIndex(){return index;}
    /**Getter method for the direction*/
    public char GetDirection(){return direction;}
    /**Getter method for the height*/
    public int GetHeight(){return heightObj;}
    public abstract void FocusType();
    public abstract void PrintIndex();

}