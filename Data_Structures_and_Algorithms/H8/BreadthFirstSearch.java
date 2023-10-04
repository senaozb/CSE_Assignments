import java.util.*;

/** Class to implement the breadth-first search algorithm.
 *  @author Koffman and Wolfgang
 * */

public class BreadthFirstSearch {

  private static int path = 0;

  /** Perform a breadth-first search of a graph.
      post: The array parent will contain the predecessor
            of each vertex in the breadth-first
            search tree.
      @param graph The graph to be searched
      @param start The start vertex ID
      @return The array of parents
   */
  public static int breadthFirstSearch(MyGraph graph, int start) {
    Queue <Vertex> theQueue = new LinkedList <Vertex> ();
    Queue <Vertex> visitSeq = new LinkedList <Vertex> ();
  
    // Declare array identified and
    // initialize its elements to false.
    boolean[] identified = new boolean[graph.getNumV()];
    boolean[] queueContains = new boolean[graph.getNumV()];
    /* Mark the start vertex as identified and insert it
       into the queue */
    identified[start] = true;
    visitSeq.offer(graph.getVertex(start));

    for(Vertex v : graph.getVertex(start).adjList)
    {
        theQueue.offer(v);
        path += graph.getEdge(graph.getVertex(start), v).getWeight();
        queueContains[v.getID()] = true;
    }
    /* While the queue is not empty */
    while (!theQueue.isEmpty() && visitSeq.size() != graph.getNumV()) 
    {  
        Vertex current = theQueue.remove();
        identified[current.getID()] = true;
        visitSeq.offer(current);

        for(Vertex v : current.adjList)
        {
            if(!identified[v.getID()] && !queueContains[v.getID()])
            {
                theQueue.offer(v);
                path += graph.getEdge(current, v).getWeight();
                queueContains[v.getID()] = true;
            }
        }
    }

    for(Vertex v : visitSeq)
      System.out.print(v.getLabel());

    System.out.println();

    return path;
  }
}
