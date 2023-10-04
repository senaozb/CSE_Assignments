import java.util.*;

/** A class for calling Dijkstra's algorithm.
 *  @author Koffman and Wolfgang
 */

public class DijkstrasAlgorithm {

  /** Dijkstra�s Shortest-Path algorithm.
      @param graph The weighted graph to be searched
      @param start The start vertex
      @param pred Output array to contain the predecessors
                  in the shortest path
      @param dist Output array to contain the distance
                  in the shortest path
   */
  public static void dijkstrasAlgorithm(MyGraph graph,
                                        Vertex start,
                                        Vertex[] pred,
                                        double[] dist) {
    int numV = graph.getNumV();
    HashSet < Vertex > vMinusS = new HashSet < Vertex > (numV);
    // Initialize V�S.
    for (int i = 0; i < numV; i++) {
      if (graph.getVertex(i) != start) {
        vMinusS.add(graph.getVertex(i));
      }
    }
    // Initialize pred and dist.
    for (Vertex v : vMinusS) {
      pred[v.getID()] = start;
      if(graph.isEdge(start, v))
        dist[v.getID()] = graph.getEdge(start, v).getWeight();
      else if(graph.isEdge(v, start))
        dist[v.getID()] = graph.getEdge(v, start).getWeight();
      else
        dist[v.getID()] = Double.POSITIVE_INFINITY;
      
      if(v.adjList != null)
        dist[v.getID()] -= v.getBoosting();

    }
  
    // Main loop
    while (vMinusS.size() != 0) {
      // Find the value u in V-S with the smallest dist[u].
      double minDist = Double.POSITIVE_INFINITY;
      int u = -1;
      for (Vertex v : vMinusS) {
        if (dist[v.getID()] < minDist) {
          minDist = dist[v.getID()];
          u = v.getID();
        }
      }
      // Remove u from vMinusS.
      vMinusS.remove(graph.getVertex(u));
    
      // Update the distances.
      for (Vertex v : vMinusS) {
        if (graph.isEdge(graph.getVertex(u), v)) {
          double weight = graph.getEdge(graph.getVertex(u), v).getWeight();
          double exact = weight;
          if(v.adjList != null)
            exact -= v.getBoosting();
          if (dist[u] + exact < dist[v.getID()]) {
            dist[v.getID()] = dist[u] + exact;
            pred[v.getID()] = graph.getVertex(u);
          }
        }
        else if (graph.isEdge(v, graph.getVertex(u))) {
          double weight = graph.getEdge(v, graph.getVertex(u)).getWeight();
          double exact = weight;
          if(v.adjList != null)
            exact -= v.getBoosting();
          if (dist[u] + exact < dist[v.getID()]) {
            dist[v.getID()] = dist[u] + exact;
            pred[v.getID()] = graph.getVertex(u);
          }
        }
      }
    }
   
    System.out.println("\nID Distance Pred");
    for (int i = 0; i < pred.length; i++) 
    {
      if(i != start.getID())
        System.out.println(i + " " + dist[i] + " " + pred[i].getID());
    }
  }
}
