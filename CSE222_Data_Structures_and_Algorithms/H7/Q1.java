public class Q1<E extends Comparable<E>>
{
    /** Convert the array to a BST using the structure of given BT
    * @param bt The binary tree
    * @param arr the given array 
    */
    public BinarySearchTree<E> BTtoBST(BinaryTree<E> bt, E[] arr)
    {
        //Sort the array at first using Merge Sort and store it in BST
        MergeSort.<E>sort(arr);
        bt.store(arr);
        BinarySearchTree<E> bst = new BinarySearchTree<>(bt);

        return bst;
    }
}
