public class Q2<E extends Comparable<E>> 
{
    /** Convert the BST to an AVL tree
    * @param bst The binary search tree
    * @return the balanced AVL tree as Binary Search Tree
    */
    public BinarySearchTree<E> BSTtoAVL(BinarySearchTree<E> bst)
    {
        BinarySearchTree<E> obj = new BinarySearchTree<E>(BSTtoAVLBalancer(bst));
        return obj;
    }

    /** Convert the array to a BST using the structure of given BT
    * @param bt The binary tree
    * @return The current node of the tree
    */
    private BinaryTree.Node<E> BSTtoAVLBalancer(BinarySearchTree<E> bst)
    {
        if(bst.root.left == null && bst.root.right == null)
            return bst.root;
        else
        {
            //Recursive Calls
            if(bst.getLeftSubtree() != null && bst.getRightSubtree() != null)
            {
                bst.root.left = BSTtoAVLBalancer(bst.getLeftSubtree());
                bst.root.right = BSTtoAVLBalancer(bst.getRightSubtree());
            }
            else if(bst.getLeftSubtree() == null)
                bst.root.right = BSTtoAVLBalancer(bst.getRightSubtree());
            else
                bst.root.left = BSTtoAVLBalancer(bst.getLeftSubtree());
            
            
            int height = bst.heightDiff(bst.root);
            if(-1 <= height && height <= 1)
                return bst.root;
            else if(height == -2) //left tree
            {
                if(bst.heightDiff(bst.getLeftSubtree().root) == -1 || bst.heightDiff(bst.getLeftSubtree().root) == 0)
                {
                    bst.root = bst.rotateRight(bst.root);
                }
                else if(bst.heightDiff(bst.getLeftSubtree().root) == 1)
                {
                    bst.root.left = bst.rotateLeft(bst.getLeftSubtree().root);
                    bst.root = bst.rotateRight(bst.root);
                }
            }
            else if(height == 2) //right tree
            {
                if(bst.heightDiff(bst.getRightSubtree().root) == -1)
                {
                    bst.root.right = bst.rotateRight(bst.getRightSubtree().root);
                    bst.root = bst.rotateLeft(bst.root);
                }
                else if(bst.heightDiff(bst.getRightSubtree().root) == 1 || bst.heightDiff(bst.getRightSubtree().root) == 0)
                {
                    bst.root = bst.rotateLeft(bst.root);
                }
            }
            return bst.root;
        }
    }
}
