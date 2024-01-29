import java.util.Arrays;
import java.util.Collections;  

public class Sorting
{
    /**100-item array */
    private static Integer[] arrS;
    /**1000-item array */
    private static Integer[] arrM;
    /**10000-item array */
    private static Integer[] arrL;
    /**100-item array */
    private static Integer[] temp_arrS;
    /**1000-item array */
    private static Integer[] temp_arrM;
    /**10000-item array */
    private static Integer[] temp_arrL;

    /**Constructor without parameter creating all the arrays */
    public Sorting()
    {
        createSmall();
        createMedium();
        createLarge();
    }

    public static void MergeSortSmall()
    {
        MergeSort obj = new MergeSort();
        temp_arrS = Arrays.copyOf(arrS, arrS.length);
        obj.sort(temp_arrS);
    }
    public static void MergeSortMedium()
    {
        MergeSort obj = new MergeSort();
        temp_arrM = Arrays.copyOf(arrM, arrM.length);
        obj.sort(temp_arrM);
    }
    public static void MergeSortLarge()
    {
        MergeSort obj = new MergeSort();
        temp_arrL = Arrays.copyOf(arrL, arrL.length);
        obj.sort(temp_arrL);
    }

    public static void QuickSortSmall()
    {
        QuickSort obj = new QuickSort();
        temp_arrS = Arrays.copyOf(arrS, arrS.length);
        obj.sort(temp_arrS);
    }
    public static void QuickSortMedium()
    {
        QuickSort obj = new QuickSort();
        temp_arrM = Arrays.copyOf(arrM, arrM.length);
        obj.sort(temp_arrM);
    }
    public static void QuickSortLarge()
    {
        QuickSort obj = new QuickSort();
        temp_arrL = Arrays.copyOf(arrL, arrL.length);
        obj.sort(temp_arrL);
    }

    public static void new_sortSmall()
    {
        new_sort obj = new new_sort();
        temp_arrS = Arrays.copyOf(arrS, arrS.length);
        obj.sort(temp_arrS);
    }
    public static void new_sortMedium()
    {
        new_sort obj = new new_sort();
        temp_arrM = Arrays.copyOf(arrM, arrM.length);
        obj.sort(temp_arrM);
    }
    public static void new_sortLarge()
    {
        new_sort obj = new new_sort();
        temp_arrL = Arrays.copyOf(arrL, arrL.length);
        obj.sort(temp_arrL);
    }

    public static class MergeSort implements SortAlgorithm
    {
        /** Merges two distinct arrays into one array
        * @param arr the main array
        * @param leftArr the left array
        * @param rightArr the right array
        */
        private void merge(Integer[] arr, Integer[] leftArr, Integer[] rightArr)
        {
            int indL = 0;
            int indR = 0;
            int indO = 0;

            while(indL < leftArr.length && indR < rightArr.length)
            {
                if(leftArr[indL].compareTo(rightArr[indR]) < 0)
                {
                    arr[indO] = leftArr[indL];
                    indO++;
                    indL++;
                }
                else
                {
                    arr[indO] = rightArr[indR];
                    indO++;
                    indR++;
                }
            }

            while(indL < leftArr.length)
            {
                arr[indO] = leftArr[indL];
                indO++;
                indL++;
            }

            while(indR < rightArr.length)
            {
                arr[indO] = rightArr[indR];
                indO++;
                indR++;
            }
        }

        @Override
        public void sort(Integer[] arr)
        {
            if(arr.length > 1)
            {
                int halfS = arr.length/2;
                Integer[] leftArr = new Integer[halfS];
                Integer[] rightArr = new Integer[arr.length-halfS];
                System.arraycopy(arr, 0, leftArr, 0, halfS);
                System.arraycopy(arr, halfS, rightArr, 0, arr.length-halfS);
                sort(leftArr);
                sort(rightArr);
                merge(arr, leftArr, rightArr);
            }
        }
    }

    public static class QuickSort implements SortAlgorithm
    {
        /**Swaps the array entries array[i] and array[j]
         * @param array the main array
         * @param i the first index
         * @param j the second index
         */
        private void swap(Integer[] array, int i, int j) 
        {
            Integer temp = array[i];
            array[i] = array[j];
            array[j] = temp;
        } 

        /**Partitions the array regarding the pivot
         * @param arr the main array
         * @param first the first index
         * @param last the second index
         */
        protected int partition(Integer[] arr, int first, int last)
        {
            Integer pivot = arr[first];
            int up = first;
            int down = last;
            do
            {
                while((up < last) && pivot.compareTo(arr[up]) >= 0)
                    up++;

                while(pivot.compareTo(arr[down]) < 0)
                    down--;

                if(up < down)
                    swap(arr, up, down);
            }
            while(up < down);

            swap(arr, first, down);
            return down;
        }

        protected void quickSort(Integer[] arr, int first, int last)
        {
            if(first < last)
            {
                int pivot = partition(arr, first, last);
                quickSort(arr, first, pivot-1);
                quickSort(arr, pivot+1, last);
            }
        }

        @Override
        public void sort(Integer[] arr)
        {
            quickSort(arr, 0, arr.length-1);
        }
    }

    public static class new_sort implements SortAlgorithm
    {
        /**Array to store the min and max values */
        private static int[] min_max;

        /** Min max finder calling binarySearch
         * @param arr the main array
         * @param head the first index
         * @param tail the second index
         */
        private void min_max_finder(Integer[] arr, int head, int tail)
        {
            binarySearch(arr, head, tail);
        }

        /** Modified binary search algorithm to find the min and max values
         * @param arr the main array
         * @param start the first index
         * @param end the second index
         */
        private void binarySearch(Integer[] arr, int start, int end) 
        {
            if(end >= start) 
            {
                int middle = (end+start)/2;
            
                if(arr[middle] < arr[min_max[0]])
                    min_max[0] = middle;
                else if(arr[middle] > arr[min_max[1]])
                    min_max[1] = middle;
            
                binarySearch(arr, start, middle-1);
                binarySearch(arr, middle+1, end);
            }
        }

        /**Swaps the array entries array[i] and array[j]
         * @param array the main array
         * @param i the first index
         * @param j the second index
         */
        private void swap(Integer[] arr, int first, int second)
        {
            int temp = arr[first];
            arr[first] = arr[second];
            arr[second] = temp;
        }

        protected void sort(Integer[] arr, int head, int tail)
        {
            if(head > tail) 
                return;
            else 
            {
                min_max[0] = head;
                min_max[1] = tail;
                min_max_finder(arr, head, tail);  
                swap(arr, head, min_max[0]); 
                swap(arr, tail, min_max[1]);
                sort(arr, head+1, tail-1);
                return;
            }
        }

        public void sort(Integer[] arr)
        {
            min_max = new int[2];
            sort(arr, 0, arr.length-1);
        }
    }

    /**Creates 100-item array */
    private void createSmall()
    {
        arrS = new Integer[100];
        for (int i = 0; i < arrS.length; i++) 
            arrS[i] = i;
        Collections.shuffle(Arrays.asList(arrS));
    }

    /**Creates 1000-item array */
    private void createMedium()
    {
        arrM = new Integer[1000];
        for (int i = 0; i < arrM.length; i++) 
            arrM[i] = i;
        Collections.shuffle(Arrays.asList(arrM));
    }

    /**Creates 10000-item array */
    private void createLarge()
    {
        arrL = new Integer[10000];
        for (int i = 0; i < arrL.length; i++) 
            arrL[i] = i;
        Collections.shuffle(Arrays.asList(arrL));
    }
}