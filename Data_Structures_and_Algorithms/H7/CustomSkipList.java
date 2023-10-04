import java.util.Random;

public class CustomSkipList 
{
    /**The head of the skip node*/
    private SkipNode head; 
    /**The max level of the SkipList */
    private int level;     
        
    static private Random ran = new Random();
    
    private int size;
    
    /**The inner class to store the data and links */
    private class SkipNode 
    {
        /**The key*/
        private Integer data;
        /**The links */
        private SkipNode[] links;    
      
        /**Constructor with 2 parameters
         * @param key
         * @param level
         */
        public SkipNode(Integer key, int level) 
        {
            this.data = key;
            links = new SkipNode[level+1];
            for (int i = 0; i < level; i++) 
              links[i] = null;    
        }
  
        public Integer getData() {return data;}

        @Override
        public String toString() {return data + "";}

    }
      
    /**Constructor without parameters */
    public CustomSkipList() 
    {   
        head = new SkipNode(null, 1); 
        level = 1;       
        size = 0;
    }
     
    /**Adjust the head again regarding new level 
     * @param newLevel
    */
    private void AdjustHead(int newLevel) 
    { 
        SkipNode temp = head;
        head = new SkipNode(null, newLevel);
        for (int i=0; i<=level; i++)
            head.links[i] = temp.links[i];
        level = newLevel;
    }
      
    /**Print the skip list
    */
    public void print() 
    {    
        SkipNode temp = head;
        while (temp != null) 
        {
            System.out.print(temp.getData() + ": length is " + temp.links.length + ":");
            for (int i=0; i<temp.links.length; i++)
                if (temp.links[i] == null)
                    System.out.print("null ");
                else
                    System.out.print(temp.links[i].getData() + " ");
            System.out.println();
            temp = temp.links[0];
        }
        System.out.println();
    }

    /** Get the random level regarding the given percentage
     * @param p
     * @return the level
    */
    private int randomLevel(int p) 
    {
        int lev;
        int[] arr = new int[10];

        for(int i = 0; i < p; i++)
          arr[i] = 1;
        for(int i = p; i < 10; i++)
          arr[i] = 0;
        int rand = Math.abs(ran.nextInt(10));

        for (lev=0; arr[rand] == 1 && lev < level; lev++); // Do nothing
        return lev;
    }

    /**Insert the new item 
     * @param newValue
    */
    public void insert(Integer newValue) 
    { 

        if (size % 10 == 0 && size != 0) 
            AdjustHead(level+1); 
        
        int percentage = getPercentage(newValue);
        int newLevel = randomLevel(percentage); 
  
        SkipNode[] update = new SkipNode[level+1]; 
        SkipNode x = head;                
        for (int i=level; i>=0; i--) 
        {    
          while((x.links[i] != null) && (x.links[i].getData() < newValue))
              x = x.links[i];
          update[i] = x;               
        }
        x = new SkipNode(newValue, newLevel);  
        for (int i=0; i<=newLevel; i++) 
        {    
            x.links[i] = update[i].links[i];
            update[i].links[i] = x;            
        }

        size++;
    }

    /**Searches for the given key
     * @param searchKey
     * @return the key
     */
    public Integer search(int searchKey) 
    { 
        SkipNode x = head;                
        for (int i=level; i>=0; i--)      
          while ((x.links[i] != null) && 
                 (x.links[i].getData() < searchKey))
            x = x.links[i];            
        x = x.links[0];  
        if ((x != null) && x.getData() == searchKey)
          return x.getData();              
        else return null;                
    }
      

    /**Get the probabilty of the random level 
     * @param key
     * @return the probabilty
     */
    private int getPercentage(Integer key)
    {
        int maxl = 0;
        int maxr = 0;
        SkipNode x = head; 

        while ((x.links[0] != null) && (x.links[0].getData().compareTo(key) < 0))
        {
            if(x.links.length == 1)
                maxl++;
            else
                maxl = 0;
            x = x.links[0]; 
        }  

        while ((x.links[0] != null) && (x.links[0].getData().compareTo(key) > 0))
        {
            if(x.links.length == 1)
                maxr++;
            else
                break;
            x = x.links[0]; 
        }  

        return maxl+maxr;
    }
}
