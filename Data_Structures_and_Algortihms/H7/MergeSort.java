public class MergeSort 
{
    public static < T extends Comparable < T >> void sort(T[] table) 
    {
        if (table.length > 1) 
        {
            int halfSize = table.length / 2;
            T[] leftTable = (T[])new Comparable[halfSize];
            T[] rightTable = (T[])new Comparable[table.length - halfSize];
            System.arraycopy(table, 0, leftTable, 0, halfSize);
            System.arraycopy(table, halfSize, rightTable, 0, table.length - halfSize);

            //Sort the the right and left parts.
            sort(leftTable);
            sort(rightTable);

            //Merge them
            merge(table, leftTable, rightTable);
        }
    }

    /** Merge two sequences.
         pre: leftSequence and rightSequence are sorted.
        post: outputSequence is the merged result and is sorted.
        @param outputSequence The destination
        @param leftSequence The left input
        @param rightSequence The right input
    */
    private static < T extends Comparable < T >> void merge(T[] outputSequence,
                                            T[] leftSequence,
                                            T[] rightSequence) 
    {
        int i = 0; // Index into the left input sequence.
        int j = 0; // Index into the right input sequence.
        int k = 0; // Index into the output sequence.

        // While there is data in both input sequences
        while (i < leftSequence.length && j < rightSequence.length) 
        {
            // Find the smaller and
            // insert it into the output sequence.
            if (leftSequence[i].compareTo(rightSequence[j]) < 0) 
            {
              outputSequence[k++] = leftSequence[i++];
            }
            else 
            {
              outputSequence[k++] = rightSequence[j++];
            }
        }
        // assert: one of the sequences has more items to copy.
        // Copy remaining input from left sequence into the output.
        while (i < leftSequence.length) {
          outputSequence[k++] = leftSequence[i++];
        }
        // Copy remaining input from right sequence into output.
        while (j < rightSequence.length) {
          outputSequence[k++] = rightSequence[j++];
        }
    }   
}
