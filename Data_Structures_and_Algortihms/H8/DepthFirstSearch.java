import java.util.*;

/** Class to implement the depth-first search algorithm.
*   @author Koffman and Wolfgang
* */

public class DepthFirstSearch {

  private static int path = 0;
  // Data Fields
  /** A reference to the graph being searched. */
  private MyGraph graph;

  /** Flag to indicate whether this vertex has been visited. */
  private static boolean[] visited;

  /** The array that contains each vertex in discovery order. */
  private int[] discoveryOrder;

  /** The array that contains each vertex in finish order. */
  private int[] finishOrder;

  /** The index that indicates the discovery order. */
  private int discoverIndex = 0;

  /** The index that indicates the finish order. */
  private int finishIndex = 0;

  public int getFinishIndex(){return finishIndex;}
  public int getPath(){return path;}

  // Constructors
  /** Construct the depth-first search of a Graph
      starting at vertex 0 and visiting the start vertices in
      ascending order.
      @param graph The graph
   */
  public DepthFirstSearch(MyGraph graph, int start) {
    this.graph = graph;
    int n = graph.getNumV();
    visited = new boolean[n];
    discoveryOrder = new int[n];
    finishOrder = new int[n];
    
    depthFirstSearch(start);
  }

  /** Construct the depth-first search of a Graph
      selecting the start vertices in the specified order.
      The first vertex visited is order[0].
      @param graph The graph
      @param order The array giving the order
                   in which the start vertices should be selected
   */
  public DepthFirstSearch(Graph graph, int[] order) {
    // Same as constructor above except for the if statement.
  }

  /** Recursively depth-first search the graph
      starting at vertex current.
      @param current The start vertex
   */
  public void depthFirstSearch(int current) 
  {
    /* Mark the current vertex visited. */
    visited[current] = true;
    discoveryOrder[discoverIndex++] = current;
  
    Iterator<Vertex> i = graph.getVertex(current).adjList.iterator();
    while (i.hasNext()) 
    {
      Vertex n = i.next();
      path += graph.getEdge(graph.getVertex(current), n).getWeight();
      if (!visited[n.getID()])   
          depthFirstSearch(n.getID());
    }
    /* Mark current finished. */
    finishOrder[finishIndex++] = current;
  }

  /** Get the finish order
      @return finish order
   */
  public int[] getFinishOrder() {
    return finishOrder;
  }

}
