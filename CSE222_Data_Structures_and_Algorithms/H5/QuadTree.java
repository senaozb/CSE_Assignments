public class QuadTree 
{
    protected Node root;
    private int length = 100;
    private int size = 0;

    /**Inner class storing the coordinates of the point */
    protected static class Point
    {
        protected int x;
        protected int y;
    
        public Point(int x, int y) 
        {
            this.x = x;
            this.y = y;
        }

        public int getX(){return x;}

        public int getY(){return y;}

        public String toString()
        {
            return "X: " + x + " Y: " + y + "\n";
        }
    }
    
    /**Inner class storing the nodes */
    protected static class Node
    {
        protected Node left;
        protected Node right;
        protected Point data;

        public Node() 
        {
            this.left = null;
            this.right = null;
            this.data = null;
        }

        public Node(Node left, Node right) 
        {
            this.left = left;
            this.right = right;
            this.data = null;
        }

        public Point getData(){return data;}

        public String toString()
        {
            return data.toString();
        }
    }

    /**Constructor without parameters*/
    public QuadTree() 
    {
        this.root = new Node();
    }

    /**Constructor with one parameter
     * @param node the root of the quad tree
     */
    protected QuadTree(Node node) 
    {
        this.root = node;
    }
    
    /**Constructor with two parameter
     * @param left the left sub - quad tree
     * @param right the right sub - quad tree
     */
    public QuadTree(QuadTree left, QuadTree right) 
    {
        this.root = new Node(left.root, right.root);

    }

    public int getSize(){return size;}

    /**Returns the left subtree
     * @return the left subtree
     */
    public QuadTree getLeftSubtree()
    {
        QuadTree l = new QuadTree(root.left);
        return l;
    }

    /**Returns the right subtree
     * @return the right subtree
     */
    public QuadTree getRightSubtree()
    {
        QuadTree r = new QuadTree(root.right);
        return r;
    }

    /**Returns the data in the root
     * @return the data in the root
     */
    public Point getData()
    {
        return root.getData();
    }

    /**Adds the new element to the tree
     * @param point the given point
     */
    public void add(Point point)
    {
        if(point.getX() < 0 || point.getY() < 0)
            System.out.println("Coordinates should be positive!");
        else
        {
            if(size == 0)
                addFirst(point);
            else
                add(root, 0, length, point, 0, 1, 1);
        }
    }

    /**Adds the first new element to the tree
     * @param point the given point
     */
    private void addFirst(Point point)
    {
        root.data = point;
        size++;
    }

    /**Adds the new element to the tree
     * @param node the current node
     * @param start the start index
     * @param end the end index
     * @param p the point 
     * @param d the direction of the divided area
     * @param regionX the region of the point in X
     * @param regionY the region of the point in Y
     */
    private void add(Node node, float start, float end, Point p, int d, int regionX, int regionY)
    {
        if(d == 0) // horizontal comparison
        {
            float middle = (end-start)/2;
            if(middle <= p.getX())
            {
                if(node.right != null)
                    add(node.right, start, end, p, 1, 2, regionY);
                else if(node.left == null)
                {
                    Point temp = node.getData();
                    node.left = new Node();
                    node.right = new Node();
                    if(temp.getX() < middle)
                    {
                        node.left.data = temp;
                        node.right.data = p;
                    }
                    else if(temp.getX() >= middle)
                    {
                        node.left = null;
                        node.right.left = new Node();
                        node.right.right = new Node();
                        if(temp.getY() >= middle && p.getY() >= middle)
                        {
                            node.right.left = null;
                            node.right.right.left= new Node();
                            node.right.right.right = new Node();
                            if(temp.getX() >= p.getX())
                            {
                                node.right.right.left.data = p;
                                node.right.right.right.data = temp;
                            }
                            else
                            {
                                node.right.right.left.data = temp;
                                node.right.right.right.data = p;
                            }
                        }
                        else if(temp.getY() < middle && p.getY() < middle)
                        {
                            node.right.right = null;
                            node.right.left.left= new Node();
                            node.right.left.right = new Node();
                            if(temp.getX() >= p.getX())
                            {
                                node.right.left.left.data = p;
                                node.right.left.right.data = temp;
                            }
                            else
                            {
                                node.right.left.left.data = temp;
                                node.right.left.right.data = p;
                            }
                        }
                        else
                        {
                            if(temp.getY() >= p.getY())
                            {
                                node.right.left.data = p;
                                node.right.right.data = temp;
                            }
                            else
                            {
                                node.right.left.data = temp;
                                node.right.right.data = p;
                            }
                        }
                    }
                    size++;
                    return;
                }
                else
                {
                    node.right = new Node();
                    node.right.data = p;
                    size++;
                    return;
                }
            }      
            else
            {
                if(node.left != null)
                    add(node.left, start, end, p, 1, 1, regionY);
                else if(node.right == null)
                {
                    Point temp = node.getData();
                    node.left = new Node();
                    node.right = new Node();
                    if(temp.getX() >= middle)
                    {
                        node.left.data = p;
                        node.right.data = temp;
                    }
                    else if(temp.getX() < middle)
                    {
                        node.right = null;
                        node.left.left = new Node();
                        node.left.right = new Node();
                        if(temp.getY() >= middle && p.getY() >= middle)
                        {
                            node.left.left = null;
                            node.left.right.left = new Node();
                            node.left.right.right = new Node();
                            if(temp.getX() >= p.getX())
                            {
                                node.left.right.left.data = p;
                                node.left.right.right.data = temp;
                            }
                            else
                            {
                                node.left.right.left.data = temp;
                                node.left.right.right.data = p;
                            }
                        }
                        else if(temp.getY() < middle && p.getY() < middle)
                        {
                            node.left.right = null;
                            node.left.left.left = new Node();
                            node.left.left.right = new Node();
                            if(temp.getX() >= p.getX())
                            {
                                node.left.left.left.data = p;
                                node.left.left.right.data = temp;
                            }
                            else
                            {
                                node.left.left.left.data = temp;
                                node.left.left.right.data = p;
                            }
                        }
                        else
                        {
                            if(temp.getY() >= p.getY())
                            {
                                node.left.left.data = p;
                                node.left.right.data = temp;
                            }
                            else
                            {
                                node.left.left.data = temp;
                                node.left.right.data = p;
                            }
                        }
                    }
                    size++;
                    return;
                }
                else
                {
                    node.left = new Node();
                    node.left.data = p;
                    size++;
                    return;
                }
            }
        }
        else // vertical comparison
        {
            float middle = (end-start)/2;
            if(middle <= p.getY())
            {
                if(node.right != null)
                {
                    if(regionX == 1)
                        add(node.right, start, middle, p, 0, regionX, 2);
                    else    
                        add(node.right, middle, end, p, 0, regionX, 2);
                }
                else if(node.left == null)
                {
                    Point temp = node.getData();
                    node.left = new Node();
                    node.right = new Node();
                    if(temp.getX() < middle)
                    {
                        node.left.data = temp;
                        node.right.data = p;
                    }
                    else if(temp.getX() >= middle)
                    {
                        node.left = null;
                        node.right.left = new Node();
                        node.right.right = new Node();
                        if(regionX == 1)
                            middle = (middle-start)/2;
                        else
                            middle = (end-middle)/2;
                            
                        if(temp.getX() >= middle && p.getX() >= middle)
                        {
                            node.right.left = null;
                            node.right.right.left= new Node();
                            node.right.right.right = new Node();
                            if(temp.getY() >= p.getY())
                            {
                                node.right.right.left.data = p;
                                node.right.right.right.data = temp;
                            }
                            else
                            {
                                node.right.right.left.data = temp;
                                node.right.right.right.data = p;
                            }
                        }
                        else if(temp.getX() < middle && p.getX() < middle)
                        {
                            node.right.right = null;
                            node.right.left.left= new Node();
                            node.right.left.right = new Node();
                            if(temp.getY() >= p.getY())
                            {
                                node.right.left.left.data = p;
                                node.right.left.right.data = temp;
                            }
                            else
                            {
                                node.right.left.left.data = temp;
                                node.right.left.right.data = p;
                            }
                        }
                        else
                        {
                            if(temp.getX() >= p.getX())
                            {
                                node.right.left.data = p;
                                node.right.right.data = temp;
                            }
                            else
                            {
                                node.right.left.data = temp;
                                node.right.right.data = p;
                            }
                        }
                    }
                    size++;
                    return;
                }
                else
                {
                    node.right = new Node();
                    node.right.data = p;
                    size++;
                    return;
                }
            }      
            else
            {
                if(node.left != null)
                {
                    if(regionX == 1)
                        add(node.left, start, middle, p, 0, regionX, 1);
                    else
                        add(node.left, middle, end, p, 0, regionX, 1);
                }
                else if(node.right == null)
                {
                    Point temp = node.getData();
                    node.left = new Node();
                    node.right = new Node();
                    if(temp.getY() >= middle)
                    {
                        node.left.data = p;
                        node.right.data = temp;
                    }
                    else if(temp.getY() < middle)
                    {
                        node.right = null;
                        node.left.left = new Node();
                        node.left.right = new Node();
                        if(regionX == 1)
                            middle = (middle-start)/2;
                        else
                            middle = (end-middle)/2;

                        if(temp.getX() >= middle && p.getX() >= middle)
                        {
                            node.left.left = null;
                            node.left.right.left = new Node();
                            node.left.right.right = new Node();
                            if(temp.getY() >= p.getY())
                            {
                                node.left.right.left.data = p;
                                node.left.right.right.data = temp;
                            }
                            else
                            {
                                node.left.right.left.data = temp;
                                node.left.right.right.data = p;
                            }
                        }
                        else if(temp.getX() < middle && p.getX() < middle)
                        {
                            node.left.right = null;
                            node.left.left.left = new Node();
                            node.left.left.right = new Node();
                            if(temp.getY() >= p.getY())
                            {
                                node.left.left.left.data = p;
                                node.left.left.right.data = temp;
                            }
                            else
                            {
                                node.left.left.left.data = temp;
                                node.left.left.right.data = p;
                            }
                        }
                        else
                        {
                            if(temp.getX() >= p.getX())
                            {
                                node.left.left.data = p;
                                node.left.right.data = temp;
                            }
                            else
                            {
                                node.left.left.data = temp;
                                node.left.right.data = p;
                            }
                        }
                    }
                    size++;
                    return;
                }
                else
                {
                    node.left = new Node();
                    node.left.data = p;
                    size++;
                    return;
                }
            }
        }
    }

    /**Checks if the tree is a leaf or not 
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

    /** This method traverses the tree using preorder traverse
     * @param node the current node
     * @param depth the current depth of the tree
     * @param sb string builder for toString method
     */
    private void preOrderTraverse(Node node, int depth, StringBuilder sb)
    {
        if(node == null)
            return;
        else
        {
            if(isLeaf(node) == true && node.getData() != null)
            {
                sb.append(depth + ".level " + node.toString());
                sb.append("\n");
            }
            preOrderTraverse(node.left, depth+1, sb);
            preOrderTraverse(node.right, depth+1, sb);
        }
    }
}