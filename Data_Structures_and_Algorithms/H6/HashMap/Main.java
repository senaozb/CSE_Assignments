public class Main 
{
    public static void main(String[] args) 
    {
        testHashMap();
    }

    public static void testHashMap()
    {
        KWHashMap<Integer, Integer> obj = new KWHashMap<Integer, Integer>();
        obj.put(3,7);
        obj.put(12,4);
        obj.put(13,5);
        obj.put(25,8);
        obj.put(23,3);
        obj.put(51,6);
        System.out.println("Before deleting the key 25");
        obj.print();
        obj.remove(25);
        System.out.println("After deleting the key 25");
        obj.print();
    }
}
