public class Q2 
{
    /**Second question - finding how many numbers are there between the given numbers
     * @param arr the array of integers
     * @param first the first number
     * @param second the second number
     * @return the number of numbers between two given numbers
    */
    public static int q2(int[] arr, int first, int second)
    {
        int flag1 = 0, flag2 = 0;
        if(first >= second)
            System.out.println("The first number should be lower than the second number!");

        for(int i = 0; i < arr.length; i++)
        {
            if(arr[i] == first)
            {
                flag1 = 1;
                break;
            }
            else if(arr[i] > first)
            {
                first = arr[i];
                flag1 = 2;
                break;
            }
        }

        for(int i = 0; i < arr.length; i++)
        {
            if(arr[i] == second)
            {
                flag2 = 1;
                break;
            }
            else if(arr[i] > second)
            {
                second = arr[i];
                flag2 = 2;
                break;
            }
        }
        if(flag2 == 0)
        { 
            second = arr[arr.length-1];
            flag2 = 3;
        }

        int firstInd = binarySearch(arr, first, 0, arr.length-1); 
        int secondInd = binarySearch(arr, second, 0, arr.length-1); 
         
        if((flag1 == 1 && flag2 == 1) || (flag1 == 1 && flag2 == 2))
            return secondInd-firstInd-1;
        else if((flag1 == 2 && flag2 == 2) || (flag1 == 2 && flag2 == 1) || (flag1 == 1 && flag2 == 3))
            return secondInd-firstInd;
        else 
            return secondInd-firstInd+1;
    }

    /**Binary Search
     * @param arr the array of integers
     * @param target the number searched
     * @param start the start index
     * @param end the end index
     * @return the index of the target
    */
    private static int binarySearch(int arr[], int target, int start, int end) 
    {
        if(end >= start) 
        {
          int middle = (end+start)/2;
    
          if(arr[middle] == target)
            return middle;
    
          if(arr[middle] > target)
            return binarySearch(arr, target, start, middle-1);
    
          return binarySearch(arr, target, middle+1, end);
        }
        return -1;
      }
}
