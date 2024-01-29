public class BinaryHeap 
{
    protected Node root;
    private int size = 0;
    /**This is a variable for recursion methods*/
    private static int flag = 0;

    /**Inner class Node stores the data of nodes */
    protected static class Node implements Comparable<Node>
    {
        protected int data;
        protected Node left;
        protected Node right;

        public Node()
        {
            this.left = null;
            this.right = null;
        }

        public Node(int data)
        {
            this.data = data;
            this.left = null;
            this.right = null;
        }
        
        public Node(int data, Node left, Node right)
        {
            this.data = data;
            this.left = left;
            this.right = right;
        }

        public int getData(){return data;}

        @Override
        public int compareTo(Node another)
        {
            return data - another.getData();
        }

        @Override
        public String toString()
        {
            return "Node data: " + data;
        }

    }

    /**Constructor without parameters*/
    public BinaryHeap()
    {
        this.root = new Node();
    }

    /**Constructor with one parameter
     * @param node the root of the heap
     */
    protected BinaryHeap(Node root)
    {
        this.root = root;
    }

    /**Constructor with three parameter
     * @param leftHeap the left subheap
     * @param rightHeap the right subheap
     */
    public BinaryHeap(int data, BinaryHeap leftHeap, BinaryHeap rightHeap)
    {
        this.root = new Node(data, leftHeap.root, rightHeap.root);

    }

    /**Returns the left subheap
     * @return the left subheap
     */
    public BinaryHeap getLeftSubtree()
    {
        BinaryHeap l = new BinaryHeap(root.left);
        return l;
    }

    /**Returns the right subheap
     * @return the right subheap
     */
    public BinaryHeap getRightSubtree()
    {
        BinaryHeap r = new BinaryHeap(root.right);
        return r;
    }

    /**Returns the data in the root
     * @return the data in the root
     */
    public int getData()
    {
        return root.data;
    }

    /**Checks if the heap is a leaf or not 
     * @return if it is a leaf then true
    */
    public boolean isLeaf(Node node)
    {
        if(node.left == null && node.right == null)
            return true;
        return false;
    }

    @Override
    public String toString()
    {
        StringBuilder sb = new StringBuilder();
        preOrderTraverse(root, 1, sb);
        return sb.toString();
    }

    /**This method traverses the heap using preorder
     * @param node the current node
     * @param depth the current depth of the heap
     * @param sb the string builder for toString method
     */
    private void preOrderTraverse(Node node, int depth, StringBuilder sb)
    {
        if(node == null)
            return;
        else
        {
            sb.append(depth + ".level " + node.getData());
            sb.append("\n");
            
            preOrderTraverse(node.left, depth+1, sb);
            preOrderTraverse(node.right, depth+1, sb);
        }
    }

    /**Adds the new element to the heap
     * @param data the given data
     */
    public void add(int data)
    {
        if(size == 0)
            addFirst(data);
        else
        {
            if(find(data) == false)
            {
                int max = findMaxLevel(root, 1);
                flag = 0;
                addLast(root, max, 1, data);
                heapOrderAdd(root, root, data);
            }
            else
                System.out.println("This item is already in the heap!");
        }
    }

    /**Adds the first new element to the heap
     * @param data the given data
     */
    private void addFirst(int data)
    {
        root.data = data;
        size++;
    }

    /**Adds the new element to the heap to the end
     * @param node the current node
     * @param maxLevel the max level of the heap
     * @param level the current level of the heap
     * @param data the given data
     */
    private void addLast(Node node, int maxLevel, int level, int data)
    {
        if(node == null)
            return;

        if(isLeaf(node) == false && maxLevel-1 == level && node.right == null && flag == 0)
        {
            node.right = new Node(data);
            size++;
            flag++;
        }
        else if(isLeaf(node) == true && maxLevel-1 == level && flag == 0)
        {
            node.left = new Node(data);
            size++;
            flag++;
        }
        else if(isLeaf(node) == true && maxLevel == level && flag == 0)
        {
            int numOfnodes = (int) Math.pow(2,level);
            numOfnodes -= 1;
            if(size == numOfnodes)
            {
                node.left = new Node(data);
                size++;
                flag++;
            }
        }
        else if(isLeaf(node) == false)
        {
            addLast(node.left, maxLevel, level+1, data);
            addLast(node.right, maxLevel, level+1, data);
        }
    }

    /**This method finds the max level of the heap
     * @param node the current node
     * @param level the current level
     * @return the max level
     */
    private int findMaxLevel(Node node, int level)
    {
        if(isLeaf(node) == false)
            return findMaxLevel(node.left, level+1);
        else
            return level;
    }

    /**This provides the heap order properly for adding
     * @param node the current node
     * @param prev the previous node
     * @param data the given data
     */
    private void heapOrderAdd(Node node, Node prev, int data)
    {
        if(node == null)
            return;

        if(isLeaf(node) == false)
        {
            heapOrderAdd(node.left, node, data);
            heapOrderAdd(node.right, node, data);
        }
         
        if(node.getData() == data)
        {
            if(data < prev.getData())
            {
                node.data = prev.getData();
                prev.data = data;
            }
        }
    }

    /**This removes the data at root */
    public void remove()
    {
        int max = findMaxLevel(root, 1);
        flag = 0;
        int nodeNum = (int) Math.pow(2,max-1);
        nodeNum -= 1;
        nodeNum = size - nodeNum;

        remove(root, max, 1, nodeNum);
        toString();
        heapOrderRemove(root, root.left, root.right, getData());
    }

    /**This finds the last element in the heap and stores it at root
     * @param node the current node
     * @param maxLevel the max level of the heap
     * @param level the current level
     * @param nodeNum the number of nodes in the max level
     * @return returns the node
     */
    private Node remove(Node node, int maxLevel, int level, int nodeNum)
    {
        if(node == null)
            return null;

        if(maxLevel == level)
        {
            flag++;
            if(flag == nodeNum)
            {
                root.data = node.getData();
                size--;
                return null;
            }
        }

        if(isLeaf(node) == false)
        {
            node.left = remove(node.left, maxLevel, level+1, nodeNum);
            node.right = remove(node.right, maxLevel, level+1, nodeNum);
        }

        return node;
    }

    /**This provides the heap order properly for removing
     * @param node the current node
     * @param nextL the next left node
     * @param nextR the next right node
     * @param data the given data
     */
    private void heapOrderRemove(Node node, Node nextL, Node nextR, int data)
    {
        if(nextR == null && nextL == null)
            return;

        if(node.getData() == data && nextR != null && nextL != null)
        {
            if(data > nextL.getData() && data < nextR.getData())
            {
                node.data = nextL.getData();
                nextL.data = data;
                if(isLeaf(node.left) == false)
                    heapOrderRemove(node.left, node.left.left, node.left.right, data);
            }
            else if(data < nextL.getData() && data > nextR.getData())
            {
                node.data = nextR.getData();
                nextR.data = data;
                if(isLeaf(node.right) == false)
                    heapOrderRemove(node.right, node.right.left, node.right.right, data);
            }
            else if(data > nextL.getData() && data > nextR.getData())
            {
                if(nextL.getData() > nextR.getData())
                {
                    node.data = nextR.getData();
                    nextR.data = data;
                    if(isLeaf(node.right) == false)
                        heapOrderRemove(node.right, node.right.left, node.right.right, data);
                }
                else
                {
                    node.data = nextL.getData();
                    nextL.data = data;
                    if(isLeaf(node.left) == false)
                        heapOrderRemove(node.left, node.left.left, node.left.right, data);
                }
            }
        }
        if(node.getData() == data && nextL == null)
        {
            if(data > nextR.getData())
            {
                node.data = nextR.getData();
                nextR.data = data;
            }
        }
        if(node.getData() == data && nextR == null)
        {
            if(data > nextL.getData())
            {
                node.data = nextL.getData();
                nextL.data = data;
            }
        }
    }

    /**Finds the given target
     * @param target
     * @return if target is in heap then return true
     */
    public boolean find(int target)
    {
        flag = 0;
        find(target, root, 1);
        if(flag == 0)
            return false;
        else
            return true;

    }

    /**Finds the given target
     * @param target 
     * @param node the current node
     * @param depth the current depth
     */
    private void find(int target, Node node, int depth)
    {
        if(node != null)
        {
            if(target == node.getData())
                flag++;
            
            find(target, node.left, depth+1);
            find(target, node.right, depth+1);
        }
    }
}
