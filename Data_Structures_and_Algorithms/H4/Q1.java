public class Q1 
{
    /**Wrapper method for q1
     * @param str the main string
     * @param subStr the substring which is searched
     * @param i indicating the number of occurrence
     * @return the index of ith occurrence
     */
    public static int q1(String str, String subStr, int i)
    {
        if(i <= 0)
        {
            System.out.println("The number of occurrence should be higher than 0");
            return -1;
        }
        else
            return q1(str, subStr, i, 0, 0, 0);
    }

    /**First question - string search in another string
     * @param str the main string
     * @param subStr the substring which is searched
     * @param i indicating the number of occurrence
     * @param index1 the current index of str
     * @param index2 the current index of subStr
     * @param count the current number of occurrences of subStr
     * @return the index of ith occurrence
    */
    private static int q1(String str, String subStr, int i, int index1, int index2, int count)
    {
        if(index1 == str.length())
            return -1;
        else
        {
            if(str.charAt(index1) == subStr.charAt(index2))
            {
                if(index2 == subStr.length()-1)
                {
                    count++;
                    if(count == i)
                        return index1-index2;
                    else
                        index2 = 0;
                }
                else
                    index2++;
            }
            index1++;
            return q1(str, subStr, i, index1, index2, count);
        }
    }  
}
