import java.util.Arrays;

public class ArrayBasedBT 
{
    /**The array to store the data */
    private Item[] arr;
    private int size;
    private int capacity;
    private int INITIAL_CAPACITY = 10;
    /**This is a variable for recursion methods*/
    private static int flag = 0;

    /**Inner class Item stores the data of the nodes */
    protected static class Item implements Comparable<Item>
    {
        protected int data;

        public Item(int data)
        {
            this.data = data;
        }

        @Override
        public int compareTo(Item another)
        {
            return  data - another.getData();
        }

        public int getData() {return data;}

        @Override
        public String toString()
        {
            return "Item data: " + data;
        }
    }

    /**Constructor without parameters */
    public ArrayBasedBT()
    {
        size = 0;
        capacity = INITIAL_CAPACITY;
        arr = new Item[capacity];
    }

    /**Adds the item to only the end of the tree
     * @param item
     * @return if adding is successful then return true
     */
    public boolean add(int item)
    {
        if(size == 0)
        {
            size++;
            return addFirst(item);
        }
        else
        {
            if(contains(item))
            {
                System.out.println("This item is already in the tree!");
                return false;
            }
            if(size >= capacity)
                reallocate();

            int ind;
            if(size % 2 == 0)
            {
                ind = (size-2)/2;
                if(arr[ind].compareTo(new Item(item)) < 0)
                {
                    arr[size] = new Item(item);
                    size++;
                    return true;
                }
            }
            else
            {
                ind = (size-1)/2;
                if(arr[ind].compareTo(new Item(item)) > 0)
                {
                    arr[size] = new Item(item);
                    size++;
                    return true;
                }
            }
            return false;
        }
        
    }

    /**Adds the item to only the end of the tree
     * @param item
     * @return always returns true
     */
    private boolean addFirst(int item)
    {
        arr[0] = new Item(item);
        return true;
    }

    /**Reallocates the capacity by making it double */
    private void reallocate()
    {
        capacity *= 2;
        arr = Arrays.copyOf(arr, capacity);
    }

    /**Checks if the target is in the tree
     * @param target 
     * @return if the target is in the tree then return true
     */
    public boolean contains(int target)
    {
        flag = 0;
        return find(target);
    }

    /**Checks if the target is in the tree
     * @param target 
     * @return if the target is in the tree then return true
     */
    public boolean find(int target)
    {
        int count = 0;
        
        if(arr[0].getData() == target)
            flag = 1;
        if(arr[0].getData() > target)
            count = 1;
        else
            count = 2;

        while(count < size)
        {
            if(arr[count].getData() == target)
            {
                flag = 1;
                break;
            }
            if(arr[count].getData() < target)
                count = count*2 + 2;
            else if(arr[count].getData() > target)
                count = count*2 + 1;
        }

        if(flag == 1)
            return true;
        else
            return false;
    }

    /**Deletes the last element in the tree
     * @return if deleting is successful then return true
     */
    public boolean delete()
    {
        if(size != 0)
        {
            size--;
            return true;
        }
        return false;
    }

    /**Removes the last element in the tree
     * @return if removing is successful then return true
     */
    public boolean remove()
    {
       return delete();
    }

    @Override
    public String toString()
    {
        StringBuilder sb = new StringBuilder();
        for(int i = 0; i < size; i++)
            sb.append(arr[i].getData());
        return sb.toString();
    }
}
