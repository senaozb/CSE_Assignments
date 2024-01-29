public class BinarySearchTree<E extends Comparable<E>> 
{
    protected Node<E> root;
    protected boolean addReturn;
    protected E deleteReturn;

    /**Inner class Node stores the data of nodes */
    protected static class Node<E extends Comparable<E>> 
    {
        protected E data;
        protected Node<E> left;
        protected Node<E> right;

        public Node()
        {
            this.left = null;
            this.right = null;
        }

        public Node(E data)
        {
            this.data = data;
            this.left = null;
            this.right = null;
        }
        
        public Node(E data, Node<E> left, Node<E> right)
        {
            this.data = data;
            this.left = left; 
            this.right = right;
        }

        public E getData(){return data;}
        public E setData(E newData)
        {
            E oldData = data;
            this.data = newData;
            return oldData;
        }

        @Override
        public String toString()
        {
            return "Node data: " + data.toString();
        }

    }

    /**Constructor without parameters*/
    public BinarySearchTree()
    {
        this.root = null;
    }

    /**Constructor with one parameter
     * @param root the root of the tree
     */
    protected BinarySearchTree(Node<E> root)
    {
        this.root = root;
    }

    /**Constructor with three parameter
     * @param data the data of the root
     * @param leftTree the left subtree
     * @param rightTree the right subtree
     */
    public BinarySearchTree(E data, BinarySearchTree<E> leftTree, BinarySearchTree<E> rightTree)
    {
        this.root = new Node<E>(data, leftTree.root, rightTree.root);

    }

    /**Returns the left subtree
     * @return the left subtree
     */
    public BinarySearchTree<E> getLeftSubtree()
    {
        BinarySearchTree<E> l = new BinarySearchTree<E>(root.left);
        return l;
    }

    /**Returns the right subtree
     * @return the right subtree
     */
    public BinarySearchTree<E> getRightSubtree()
    {
        BinarySearchTree<E> r = new BinarySearchTree<E>(root.right);
        return r;
    }

    /**Returns the data in the root
     * @return the data in the root
     */
    public E getData()
    {
        return root.data;
    }

    /**Checks if the tree is a leaf or not 
     * @return if it is a leaf then true
    */
    public boolean isLeaf(Node<E> node)
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

    /**This method traverses the tree using preorder
     * @param node the current node
     * @param depth the current depth of the tree
     * @param sb the string builder for toString method
     */
    private void preOrderTraverse(Node<E> node, int depth, StringBuilder sb)
    {
        if(node == null)
            return;
        else
        {
            sb.append(depth + ".level " + node.toString());
            sb.append("\n");
            
            preOrderTraverse(node.left, depth+1, sb);
            preOrderTraverse(node.right, depth+1, sb);
        }
    }

    /**Adds the new element to the tree
     * @param data the given data
     * @return if the insertion is successful then return true
     */
    public boolean add(E data)
    {
        root = add(root, data);
        return addReturn;
    }

    /**Adds the new element to the tree
     * @param node the current node
     * @param data the given data
     * @return the current node
     */ 
    private Node<E> add(Node<E> node, E data)
    {
        if(node == null)
        {
            addReturn = true;
            return new Node<E>(data);
        }
        else if(data.compareTo(node.getData()) == 0)
        {
            addReturn = false;
            return node;
        }
        else if(data.compareTo(node.getData()) < 0)
        {
            node.left = add(node.left, data);
            return node;
        }
        else 
        {
            node.right = add(node.right, data);
            return node;
        }
    }

    /**This removes the data*/
    public E delete(E data)
    {
        root = delete(root, data);
        return deleteReturn;
    }

    /**This removes the data
     * @param node the current node
     * @param data the given data
     * @return the current node
     */
    private Node<E> delete(Node<E> node, E data)
    {
        if(node == null)
        {
            deleteReturn = null;
            return node;
        }

        int res = data.compareTo(node.getData());      
        if(res < 0)
        {
            node.left = delete(node.left, data);
            return node;
        }
        else if(res > 0)
        {
            node.right = delete(node.right, data);
            return node;
        }
        else 
        {
            deleteReturn = node.getData();
            if(node.left == null)
                return node.right;
            else if(node.right == null)
                return node.left;
            else 
            {
                if(node.left.right == null)
                {
                    node.data = node.left.getData();
                    node.left = node.left.left;
                    return node;
                }
                else
                {
                    node.data = findLargestChild(node.left);
                    return node;
                }
            }
        }
    }

    /**This finds the largest child for delete method
     * @param parent
     * @return returns the value of largest child
     */
    private E findLargestChild(Node<E> parent)
    {
        if(parent.right.right == null)
        {
            E retVal = parent.right.getData();
            parent.right = parent.right.left;
            return retVal;
        }
        else 
        {
            return findLargestChild(parent.right);
        }
    }

    /**Finds the given target
     * @param target
     * @return the given target
     */
    public E find(E target)
    {
        return find(target, root);
    }

    /**Finds the given target
     * @param target 
     * @param node the current node
     */
    private E find(E target, Node<E> node)
    {
        if(node == null)
            return null;

        int res = target.compareTo(node.getData());       
        if(res == 0)
            return node.getData();
        else if(res < 0)
            return find(target, node.left);
        else 
            return find(target, node.right);
    }

    /**Changes the data in the target
     * @param target
     * @return the given target
     */
    public E change(E target)
    {
        return change(target, root);
    }

    /**Changes the data in the target
     * @param target 
     * @param node the current node
     */
    private E change(E target, Node<E> node)
    {
        if(node == null)
            return null;

        int res = target.compareTo(node.getData());       
        if(res == 0)
        {
            return node.setData(target);
        }
        else if(res < 0)
            return change(target, node.left);
        else 
            return change(target, node.right);
    }
}
