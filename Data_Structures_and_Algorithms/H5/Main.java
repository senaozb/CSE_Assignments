public class Main {
    public static void main(String[] args) 
    {
        testQuad();
        testBinaryHeap();
        testArrayBasedBT();
    }

    public static void testQuad()
    {
        QuadTree qd = new QuadTree();
        QuadTree.Point p1 = new QuadTree.Point(30, 30);
        QuadTree.Point p2 = new QuadTree.Point(20, 15);
        QuadTree.Point p3 = new QuadTree.Point(50, 40);
        QuadTree.Point p4 = new QuadTree.Point(10, 12);
        QuadTree.Point p5 = new QuadTree.Point(40, 20);
        QuadTree.Point p6 = new QuadTree.Point(25, 60);
        QuadTree.Point p7 = new QuadTree.Point(15, 25);

        System.out.println("---------  Quad Tree  ---------");
        qd.add(p1);
        System.out.println(qd.toString());
        System.out.println("-------------------------------");
        qd.add(p2);
        System.out.println(qd.toString());
        System.out.println("-------------------------------");
        qd.add(p3);
        System.out.println(qd.toString());
        System.out.println("-------------------------------");
        qd.add(p4);
        System.out.println(qd.toString());
        System.out.println("-------------------------------");
        qd.add(p5);
        System.out.println(qd.toString());
        System.out.println("-------------------------------");
        qd.add(p6);
        System.out.println(qd.toString());
        System.out.println("-------------------------------");
        qd.add(p7);
        System.out.println(qd.toString());
        System.out.println("-------------------------------");
    }

    public static void testBinaryHeap()
    {
        BinaryHeap bHeap = new BinaryHeap();
        System.out.println("--------  Binary Heap  --------");
        System.out.println("Add 3: ");
        bHeap.add(3);
        System.out.println(bHeap.toString());
        System.out.println("-------------------------------");
        System.out.println("Add 5: ");
        bHeap.add(5);
        System.out.println(bHeap.toString());
        System.out.println("-------------------------------");
        System.out.println("Add 2: ");
        bHeap.add(2);
        System.out.println(bHeap.toString());
        System.out.println("-------------------------------");
        System.out.println("Add 1: ");
        bHeap.add(1);
        System.out.println(bHeap.toString());
        System.out.println("-------------------------------");
        System.out.println("Add 7: ");
        bHeap.add(7);
        System.out.println(bHeap.toString());
        System.out.println("-------------------------------");
        System.out.println("Add 8: ");
        bHeap.add(8);
        System.out.println(bHeap.toString());
        System.out.println("-------------------------------");
        System.out.println("Add 0: ");
        bHeap.add(0);
        System.out.println(bHeap.toString());
        System.out.println("-------------------------------");
        System.out.print("Is 7 in the heap: ");
        System.out.println(bHeap.find(7));
        System.out.println("-------------------------------");
        System.out.print("Is 10 in the heap: ");
        System.out.println(bHeap.find(10));
        System.out.println("-------------------------------");
        System.out.println("Remove the top element: ");
        bHeap.remove();
        System.out.println(bHeap.toString());
        System.out.println("-------------------------------");
    }

    public static void testArrayBasedBT()
    {
        ArrayBasedBT obj = new ArrayBasedBT();
        System.out.println("------- Array Based BST  ------");
        System.out.println("Add 4: ");
        obj.add(4);
        System.out.println(obj.toString());
        System.out.println("-------------------------------");
        System.out.println("Add 2: ");
        obj.add(2);
        System.out.println(obj.toString());
        System.out.println("-------------------------------");
        System.out.println("Add 6: ");
        obj.add(6);
        System.out.println(obj.toString());
        System.out.println("-------------------------------");
        System.out.println("Add 1: ");
        obj.add(1);
        System.out.println(obj.toString());
        System.out.println("-------------------------------");
        System.out.println("Add 3: ");
        obj.add(3);
        System.out.println(obj.toString());
        System.out.println("-------------------------------");
        System.out.println("Add 5: ");
        obj.add(5);
        System.out.println(obj.toString());
        System.out.println("-------------------------------");
        System.out.println("Add 7: ");
        obj.add(7);
        System.out.println(obj.toString());
        System.out.println("-------------------------------");
        System.out.println("Remove the last element: ");
        obj.remove();
        System.out.println(obj.toString());
        System.out.println("-------------------------------");
        System.out.println("Is 3 in the tree: ");
        System.out.println(obj.contains(3));
        System.out.println("-------------------------------");
        System.out.println("Is 10 in the tree: ");
        System.out.println(obj.contains(10));
    }
}
