import java.util.AbstractList;

/**LDLinkedList class is a linked list implementation that extends AbstractList class */
public class LDLinkedList<E> extends AbstractList<E>
{
    /**head keeps the first node of the linked list */
    private Node<E> head = null;
    /**size is the size of the linked list */
    private int size = 0;
    /**lazyDeletion keeps the head of removed nodes */
    private Node<E> lazyDeletion = null;
    
    /**Overridden size method 
     * @return current size of the linked list
    */
    public int size(){return size;}

    /**Node class is a generic, private class that keeps the data and reference to next node */
    private static class Node<E>
    {
        private E data;
        private Node<E> next;

        /** Constructor with one parameter
         * @param data the data of the node
         */
        private Node(E data)
        {
            this.data = data;
            this.next = null;
        }
        /** Constructor with two parameters
         * @param data the data of the node
         * @param next the reference of the next node
         */
        private Node(E data, Node<E> next)
        {
            this.data = data;
            this.next = next;
        }
    }

    /**This adds a node at the beginning of the list 
     * @param entry the data of the node
    */
    private void addFirst(E entry)
    {
        if(lazyDeletion == null)
        {
            Node<E> temp = new Node<E>(entry);
            head = temp;
        }
        else
        {
            Node<E> temp = lazyDeletion;
            lazyDeletion = lazyDeletion.next;
            temp.data = entry;
            temp.next = null;
            head = temp;
        }
        size++;
    }

    /**This adds a node after the given node
     * @param node previous node
     * @param entry the data of the node
     */
    private void addAfter(Node<E> node, E entry)
    {
        if(lazyDeletion == null)
        {
            Node<E> temp = new Node<E>(entry, node.next);
            node.next = temp;
        }
        else
        {
            Node<E> temp = lazyDeletion;
            lazyDeletion = lazyDeletion.next;
            temp.data = entry;
            temp.next = node.next;
            node.next = temp;
        }
        size++;
    }

    /**This removes the first node
     * @return It returns the deleted node data
     */
    private E removeFirst()
    {
        Node<E> temp = head;
        if(head != null)
            head = head.next;
        if(temp != null)
        {
            size--;

            if(lazyDeletion == null)
                lazyDeletion = temp;
            else
            {
                Node<E> tempLazy = lazyDeletion;
                lazyDeletion = temp;
                lazyDeletion.next = tempLazy;
            }

            return temp.data;
        }
        else
            return null;
    }

    /**This removes the node after the given node
     * @return It returns the deleted node data
     */
    private E removeAfter(Node<E> node)
    {
        Node<E> temp = node.next;
        if(temp != null)
        {
            node.next = temp.next;
            size--;

            if(lazyDeletion == null)
                lazyDeletion = temp;
            else
            {
                Node<E> tempLazy = lazyDeletion;
                lazyDeletion = temp;
                lazyDeletion.next = tempLazy;
            }

            return temp.data;
        }
        else
            return null;
    }

    /**This adds an entry at the end
     * @param entry the data which is kept in the list
     * @return It returns boolean true
     */
    public boolean add(E entry)
    {
       add(size, entry);
       return true;
    }

    /**This adds an entry at the given index
     * @param index the index which the given data should be stored at
     * @param entry the data which is kept in the list
     */
    public void add(int index, E entry)
    {
        if(index < 0 || index > size)
            throw new ArrayIndexOutOfBoundsException(index);

        if(index == 0)
            addFirst(entry);
        else
        {
            Node<E> node = getNode(index-1);
            addAfter(node, entry);
        }
    }

    /**This adds an entry at the given index
     * @param index the index indicating the node 
     * @return It returns the data in the removed node
     */
    public E remove(int index)
    {
        E result;
        if(index < 0 || index > size)
            throw new ArrayIndexOutOfBoundsException(index);
        if(index == 0)
            result = removeFirst();
        else
        {
            Node<E> node = getNode(index-1);
            result = removeAfter(node);
        }

        return result;
    }

    @Override
    public String toString()
    {
        Node<E> node = head;
        StringBuilder string = new StringBuilder();
        while(node != null)
        {
            string.append(node.data);
            if(node.next != null)
                string.append(" ==> ");
            node = node.next;
        }
        return string.toString();
    }

    /**This gets the node at the given index
     * @param index the index of the node
     * @return It returns the node itself
     */
    public Node<E> getNode(int index)
    {
        Node<E> node = head;
        for(int i = 0; i < index && node != null; i++)
            node = node.next;

        return node;
    }

    /**This gets the data at the given index
     * @param index the index of the data
     * @return It returns the data in the node 
     */
    public E get(int index)
    {
        if(index < 0 || index >= size)
            throw new ArrayIndexOutOfBoundsException(index);

        Node<E> node = getNode(index);
        return node.data;
    }

     /**This replaces the data with the given data at the given index
     * @param index the index of the node
     * @param entry the data
     * @return It returns the previous data
     */
    public E set(int index, E entry)
    {
        if(index < 0 || index >= size)
            throw new ArrayIndexOutOfBoundsException(index);

        Node<E> node = getNode(index);
        E value = node.data;
        node.data = entry;
        return value;
    }

    @Override
    public int indexOf(Object entry)
    {
        Node<E> node = head;
        for(int i = 0; i < size && node != null; i++)
        {
            if(node.data == entry)
                return i;
            node = node.next;
        }

        return -1;
    }
}
