public class KWHashMap <K,V>
{
    /** The table of the hash map */
    private BinarySearchTree<Entry<K,V>>[] table;
    /** The number of keys */
    private int numKeys;

    /** The capacity */
    private static final int CAPACITY = 101;

    /** Inner class entry to store the entries */
    private static class Entry<K,V> implements Comparable<Entry<K,V>>
    {
        private K key;
        private V value;

        public Entry(K key, V value)
        {
            this.key = key;
            this.value = value;
        }
        public Entry(K key) 
        {
            this.key = key;
            this.value = null;
        }

        public K getKey(){return key;}
        public V getValue(){return value;}
        public V setValue(V value)
        {
            V oldVal = value;
            this.value = value;
            return oldVal;
        }

        @Override
        public int compareTo(Entry<K,V> another)
        {
            String s1 = this.getKey().toString();
            String s2 = another.getKey().toString();
            return s1.compareTo(s2);
        }

        @Override
        public String toString()
        {
            return "Key: " + key + " Value: " + value;
        }
    }

    /**Constructor without parameters */
    public KWHashMap()
    {
        table = new BinarySearchTree[CAPACITY];
    }

    /** This gets the value regarding the given key
     * @param key 
     * @return the value
     */
    public V get(Object key) 
    {
        int ind = key.hashCode() % table.length;
        if (ind < 0)
            ind += table.length;
        if (table[ind] == null)
            return null; 

        K keyA = (K) key;
        Entry<K,V> temp = table[ind].find(new Entry<K,V>(keyA));
        if(temp != null)
            return temp.getValue(); 

        return null;
    }

    public boolean isEmpty(){return size() == 0;}

    /** This adds the new value by finding its key's location
     * @param key
     * @param value
     * @return the value
     */
    public V put(K key, V value) 
    {
        int ind = key.hashCode() % table.length;
        if (ind < 0)
        ind += table.length;
        if (table[ind] == null) 
            table[ind] = new BinarySearchTree<Entry<K,V>>();
        
        Entry<K,V> temp = table[ind].delete(new Entry<K,V>(key));
        table[ind].add(new Entry <K,V> (key, value));
        numKeys++;
     
        if(temp != null)
            return temp.getValue(); 
        
        return null; 
    }

    /** This removes the key from the table
     * @param key
     * @return the old value
     */
    public V remove(Object key) 
    {
        int ind = key.hashCode() % table.length;
        if (ind < 0)
            ind += table.length;
        if (table[ind] == null)
            return null; 

        K keyA = (K) key;
        Entry<K,V> temp = table[ind].delete(new Entry<K,V>(keyA));
        if(temp != null)
            return temp.getValue(); 
        
        return null; 
    }
    
    public int size(){return numKeys;}

    public void print()
    {
        for (int i = 0; i < table.length; i++) 
        {
            if (table[i] != null) 
            {
                System.out.println(table[i].toString());
            }
        }
    }
}
