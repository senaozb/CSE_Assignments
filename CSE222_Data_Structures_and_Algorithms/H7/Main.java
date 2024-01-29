public class Main 
{
    public static void main(String[] args) 
    {
        testQ1();
        testQ2();
        testQ3();
    }

    public static void testQ1()
    {
        Integer[] arr = {2,5,1,8,3,11,6,14,4};
        Q1<Integer> obj = new Q1<>();
        BinaryTree<Integer> bt = createBT();
        System.out.println("The binary tree: ");
        System.out.print(bt.toString());
        BinarySearchTree<Integer> bst = obj.BTtoBST(bt, arr);
        System.out.println("The binary search tree: ");
        System.out.print(bst.toString());
    }

    public static BinaryTree<Integer> createBT()
    {
        BinaryTree<Integer> bt = new BinaryTree<>();
        bt.root = new BinaryTree.Node<Integer>(0);
        bt.root.left = new BinaryTree.Node<Integer>(0);
        bt.root.right = new BinaryTree.Node<Integer>(0);
        bt.root.left.left = new BinaryTree.Node<Integer>(0);
        bt.root.left.right = new BinaryTree.Node<Integer>(0);
        bt.root.left.right.left = new BinaryTree.Node<Integer>(0);
        bt.root.left.right.right = new BinaryTree.Node<Integer>(0);
        bt.root.right.right = new BinaryTree.Node<Integer>(0);
        bt.root.right.right.left = new BinaryTree.Node<Integer>(0);
        return bt;
    }

    public static void testQ2()
    {
        Q2<Integer> obj = new Q2<>();
        BinarySearchTree<Integer> bst = new BinarySearchTree<>();
        BinarySearchTree<Integer> avl = new BinarySearchTree<>();
        bst.add(50);
        bst.add(17);
        bst.add(76);
        bst.add(9);
        bst.add(23);
        bst.add(19);
        bst.add(14);
        bst.add(12);
        bst.add(54);
        bst.add(72);
        bst.add(67);
        System.out.println("The binary search tree:");
        System.out.print(bst.toString());

        avl = obj.BSTtoAVL(bst);
        System.out.println("The AVL tree:");
        System.out.print(avl.toString());
    }
    
    public static void testQ3()
    {
        CustomSkipList sl = new CustomSkipList();
        sl.insert(50);
        sl.insert(17);
        sl.insert(76);
        sl.insert(9);
        sl.insert(23);
        sl.insert(19);
        sl.insert(14);
        sl.insert(12);
        sl.insert(54);
        sl.insert(72);
        sl.insert(67);
        System.out.println("After adding 11 elements");
        sl.print();
        sl.insert(100);
        sl.insert(90);
        sl.insert(70);
        sl.insert(5);
        sl.insert(7);
        sl.insert(88);
        sl.insert(91);
        sl.insert(120);
        sl.insert(45);
        sl.insert(200);
        System.out.println("After adding 21 elements");
        sl.print();
    }
}