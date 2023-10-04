public class Main 
{
    public static void main(String[] args) 
    {
        testSorting();
    }

    public static void testSorting()
    {
        System.out.println("------------- Average Runtime Calculation -----------");
        long count1S = 0;
        long count1M = 0;
        long count1L = 0;
        long count2S = 0;
        long count2M = 0;
        long count2L = 0;
        long count3S = 0;
        long count3M = 0;
        long count3L = 0;
        for(int i = 0; i < 10; i++)
        {
            Sorting sorting = new Sorting();
            long startTime = System.nanoTime();
            Sorting.MergeSortSmall();
            long endTime   = System.nanoTime();
            long totalTime = endTime - startTime;
            count1S += totalTime;
            
            startTime = System.nanoTime();
            Sorting.MergeSortMedium();
            endTime   = System.nanoTime();
            totalTime = endTime - startTime;
            count1M += totalTime;
            
            startTime = System.nanoTime();
            Sorting.MergeSortLarge();
            endTime   = System.nanoTime();
            totalTime = endTime - startTime;
            count1L += totalTime;

            startTime = System.nanoTime();
            Sorting.QuickSortSmall();
            endTime   = System.nanoTime();
            totalTime = endTime - startTime;
            count2S += totalTime;
            
            startTime = System.nanoTime();
            Sorting.QuickSortMedium();
            endTime   = System.nanoTime();
            totalTime = endTime - startTime;
            count2M += totalTime;
            
            startTime = System.nanoTime();
            Sorting.QuickSortLarge();
            endTime   = System.nanoTime();
            totalTime = endTime - startTime;
            count2L += totalTime;

            startTime = System.nanoTime();
            Sorting.new_sortSmall();
            endTime   = System.nanoTime();
            totalTime = endTime - startTime;
            count3S += totalTime;
            
            startTime = System.nanoTime();
            Sorting.new_sortMedium();
            endTime   = System.nanoTime();
            totalTime = endTime - startTime;
            count3M += totalTime;
            
            startTime = System.nanoTime();
            Sorting.new_sortLarge();
            endTime   = System.nanoTime();
            totalTime = endTime - startTime;
            count3L += totalTime;
        }
         System.out.println("Average Time for Merge Sort - 100: " + count1S/10);
         System.out.println("Average Time for Merge Sort - 1000: " + count1M/10);
         System.out.println("Average Time for Merge Sort - 10000: " + count1L/10);
         System.out.println("Average Time for Quick Sort - 100: " + count2S/10);
         System.out.println("Average Time for Quick Sort - 1000: " + count2M/10);
         System.out.println("Average Time for Quick Sort - 10000: " + count2L/10);
         System.out.println("Average Time for New Sort - 100: " + count3S/10);
         System.out.println("Average Time for New Sort - 1000: " + count3M/10);
         System.out.println("Average Time for New Sort - 10000: " + count3L/10);

    }
}
