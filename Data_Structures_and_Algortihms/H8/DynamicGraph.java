public interface DynamicGraph extends Graph
{
    public Vertex newVertex (String label, double weight, double boosting);
    
    public void addVertex (Vertex new_vertex);
    
    public void addEdge (int vertexID1, int vertexID2, double weight);
    
    public void removeEdge (int vertexID1, int vertexID2);
    
    public void removeVertex (int vertexID);
    
    public void removeVertex (String label);
    
    public int[][] exportMatrix();
    
    public void printGraph();
}