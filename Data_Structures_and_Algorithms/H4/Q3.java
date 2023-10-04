import java.util.ArrayList;

public class Q3 
{
    /**Wrapper method for q3
     * @param arr the array of integers
     * @param total the given sum
     */
    public static void q3(int[] arr, int total)
    {
        ArrayList<Integer> arrList = new ArrayList<Integer>();
        q3(arr, 0, 0, total, arrList);
    }

    /**Third question-finding the subarrays whose elements' sum is equal to the given value
     * @param arr the array of integers
     * @param count the current sum
     * @param index the current index
     * @param total the given sum
     * @param arrList an array list to store the items
     */
    private static void q3(int[] arr, int count, int index, int total, ArrayList<Integer> arrList)
    {
        for(int i = index; i < arr.length; i++)
        {
            count += arr[i];
            arrList.add(arr[i]);
            if(count == total)
            {
                printArr(arrList);
                arrList.clear();
                count = 0;
                break;
            }
            else if(count > total)
            {
                arrList.clear();
                count = 0; 
                break;
            }
        }
        
        if(index+1 != arr.length)
            q3(arr, count, index+1, total, arrList);
        else
            return;
    }

    private static void printArr(ArrayList<Integer> arrList)
    {
        System.out.print("The subarray: ");
        for(int item : arrList)
            System.out.print(item + " ");
        System.out.println();
    }
    
}
