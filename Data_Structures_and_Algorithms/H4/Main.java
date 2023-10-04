public class Main 
{
    public static void main(String[] args) 
    {
        //Question 1
        System.out.println("-------------------------------");
        Driver1();
        //Question 2
        System.out.println("-------------------------------");
        Driver2();
        //Question 3
        System.out.println("-------------------------------");
        Driver3(); 
        System.out.println("-------------------------------");
    }

    public static void Driver1()
    {
        long startTime = System.nanoTime();
        System.out.println("The index of 2. occurrence of 'hey' is " + Q1.q1("hey hello hey hi", "hey", 2) + " in 'hey hello hey hi'");
        long endTime   = System.nanoTime();
        long totalTime = endTime - startTime;
        System.out.println("Time: " + totalTime);
       
        System.out.println("The index of 3. occurrence of 'hey' is " + Q1.q1("hey hello hey hi", "hey", 3) + " in 'hey hello hey hi'");
    }
    public static void Driver2()
    {
        int[] arr = new int[] {1,2,4,5,7,9,11};

        long startTime = System.nanoTime();
        System.out.println("The number of items between 1 and 5 is " + Q2.q2(arr, 1, 5));
        long endTime   = System.nanoTime();
        long totalTime = endTime - startTime;
        System.out.println("Time: " + totalTime);

        System.out.println("The number of items between 0 and 8 is " + Q2.q2(arr, 0, 8));
        System.out.println("The number of items between 4 and 12 is " + Q2.q2(arr, 4, 12));
        System.out.println("The number of items between 0 and 12 is " + Q2.q2(arr, 0, 12));
    }
    public static void Driver3()
    {
        System.out.println("The sum is 7");
        int[] arr2 = new int[] {1,2,4,5,7,2,5};

        long startTime = System.nanoTime();
        Q3.q3(arr2, 7);
        long endTime   = System.nanoTime();
        long totalTime = endTime - startTime;
        System.out.println("Time: " + totalTime);

        System.out.println("The sum is 30");
        Q3.q3(arr2, 30);
        System.out.println("--Empty--");
    }
}
