public class Main 
{
    public static void main(String[] args) 
    {
        MyGraph graph = new MyGraph(true);
        Q1(graph);
        Q2(graph);
        Q3(graph);
    }

    public static void Q1(MyGraph graph)
    {
        System.out.println("\nThe graph: ");
        graph.addVertex(graph.newVertex("0", 1, 1)); 
        graph.addVertex(graph.newVertex("1", 1, 2));
        graph.addVertex(graph.newVertex("2", 1, 3));
        graph.addVertex(graph.newVertex("3", 1, 4));
        graph.addVertex(graph.newVertex("4", 1, 5));
        graph.addEdge(0, 1, 10);
        graph.addEdge(0, 2, 15);
        graph.addEdge(0, 3, 20);
        graph.addEdge(1, 3, 10);
        graph.addEdge(1, 4, 15);
        graph.addEdge(2, 3, 20);
        graph.addEdge(3, 4, 10);
        graph.printGraph();
    }

    public static void Q2(MyGraph graph)
    {
        System.out.println("\nBreadth First Search Order:");
        int distance = BreadthFirstSearch.breadthFirstSearch(graph, 0);
        System.out.println("\nDepth First Search Order:");
        DepthFirstSearch d = new DepthFirstSearch(graph, 0);
        int[] res = d.getFinishOrder();

        for(int i = 0; i < d.getFinishIndex(); i++)
            System.out.print(res[i]);

        System.out.println("\n");
        System.out.println("The BFS: " + distance);
        System.out.println("The DFS: " + d.getPath());
    }

    public static void Q3(MyGraph graph)
    {
        Vertex[] pred = new Vertex[graph.getNumV()];
        for(int i = 0; i < pred.length; i++)
            pred[i] = null;
        double[] dist = new double[graph.getNumV()];
        DijkstrasAlgorithm.dijkstrasAlgorithm(graph, graph.getVertex(0), pred, dist);
    }
}
