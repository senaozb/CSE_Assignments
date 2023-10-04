import java.util.*;

public class MyGraph implements DynamicGraph
{
    /**Counter to create ID*/
    private static int counterID = 0;
    private boolean directed;
    /**Array list to store the vertices*/
    private ArrayList<Vertex> vertices;
    /**Array list to store the edges*/
    private ArrayList<Edge> edges;

    /**Constructor with one parameter
     * @param directed
     */
    public MyGraph(boolean directed)
    {
        vertices = new ArrayList<Vertex>();
        edges = new ArrayList<Edge>();
        this.directed = directed;
    }

    /**Create a vertex with given parameters
     * @param label
     * @param weight
     * @param boosting
     */
    public Vertex newVertex (String label, double weight, double boosting)
    {
        Vertex v = new Vertex(counterID++ ,label, weight, boosting);
        return v;
    }

    /**Add the new vertex
     * @param new_vertex
     */
    public void addVertex (Vertex new_vertex)
    {
        vertices.add(new_vertex);
    }

    /**Add a new edge
     * @param vertexID1
     * @param vertexID2
     * @param weight
     */
    public void addEdge (int vertexID1, int vertexID2, double weight)
    {
        Vertex first = null;
        Vertex second = null;
        for (Vertex obj : vertices)
        {
            if(obj.getID() == vertexID1)
                first = obj;
            if(obj.getID() == vertexID2)
                second = obj;
        }

        first.addAdj(second);
        if(!isDirected())
            second.addAdj(first);

        Edge edge = new Edge(first, second, weight);
        edges.add(edge);
    }

    /**Remove the edge
     * @param vertexID1
     * @param vertexID2
     */
    public void removeEdge (int vertexID1, int vertexID2)
    {
        Vertex first = null;
        Vertex second = null;
        for (Vertex obj : vertices)
        {
            if(obj.getID() == vertexID1)
                first = obj;
            if(obj.getID() == vertexID2)
                second = obj;
        }

        if(isDirected())
        {
            Edge edge = null;
            for (Edge obj : edges)
            {
                if(obj.getSource().equals(first) && obj.getDest().equals(second))
                    edge = obj;
            }
            edges.remove(edge);
        }
        else
        {
            Edge edge = null;
            for (Edge obj : edges)
            {
                if(obj.getSource().equals(first) && obj.getDest().equals(second)
                    && obj.getSource().equals(second) && obj.getDest().equals(first))
                    edge = obj;
            }
            edges.remove(edge);
        }

        first.removeAdj(second);
        if(!isDirected())
            second.removeAdj(first);

    }

    /**Remove the vertex
     * @param vertexID
     */
    public void removeVertex (int vertexID)
    {
        Vertex vertex = null;
        for (Vertex obj : vertices)
        {
            if(obj.getID() == vertexID)
                vertex = obj;
        }

        vertices.remove(vertex);
        
        if(!isDirected())
        {
            for (Vertex obj : vertex.adjList)
            {
                obj.removeAdj(vertex);
                if(isEdge(obj, vertex))
                    edges.remove(getEdge(obj, vertex));
                else if(isEdge(vertex, obj))
                    edges.remove(getEdge(vertex, obj));
            }
        }
        else
        {
            for (Vertex obj : vertex.adjList)
                edges.remove(getEdge(vertex, obj));

            for (Vertex obj : vertices)
            {
                if(obj.findAdj(vertex))
                {
                    obj.removeAdj(vertex);
                    edges.remove(getEdge(obj, vertex));
                }
            }
        }
        
    }

    /**Remove the vertex
     * @param label
     */
    public void removeVertex (String label)
    {
        for (Vertex obj : vertices)
        {
            if(obj.getLabel().equals(label))
                removeVertex(obj.getID());
        }
    }

    /**Export the matrix
     * @return 2D integer array
     */
    public int[][] exportMatrix()
    {
        int size = vertices.size();
        int [][] matrix = new int[size][size];

        for(int i = 0; i < size; i++)
        {
            for(int j = 0; j < size; j++)
            {
                if(vertices.get(i).findAdj(vertices.get(j)) == true)
                    matrix[i][j] = 1;
                else 
                    matrix[i][j] = 0;
            }
        }

        return matrix;
    }

    /**Print the graph using adjacent list 
     */
    public void printGraph()
    {
        for(int i = 0; i < getNumV(); i++)
        {
            Vertex v = vertices.get(i);
            System.out.print(v.toString());
            for(int j = 0; j < v.adjList.size(); j++)
            {
                System.out.print(" -> ");
                System.out.print(v.adjList.get(j).toString());
            }
            System.out.println();
        }
    }

    /**Print the graph using adjacent matrix
     * @param matrix 2D array
     */
    public void printMatrix(int[][] matrix)
    {
        for(int i = -2; i < getNumV(); i++)
        {
            for(int j = -2; j < getNumV(); j++)
            {
                if(i == -2 && j == -2)
                    System.out.print(" ");
                else if(i == -2 && j != -2 && j != -1) 
                    System.out.print(vertices.get(i).getID());
                else if(i == -1)
                    System.out.print(" ");
                else if(j == -2)
                    System.out.print(vertices.get(i).getID());
                else if(j == -1)
                    System.out.print(" ");
                else
                    System.out.print(matrix[i][j]);
            }
            System.out.println();
        }
    }

    /**Get the number of the vertices
     * @return the number of vertices
     */
    public int getNumV()
    {
        return vertices.size();
    }

    /**Get the info of being directed
     * @return true if it is directed
     */
    public boolean isDirected()
    {
        return directed;
    }

    /** Insert the given edge
     * @param edge
     */
    public void insert(Edge edge)
    {
        int vertexID1 = edge.getSource().getID();
        int vertexID2 = edge.getDest().getID();
        double weight = edge.getWeight();
        addEdge(vertexID1, vertexID2, weight);
    }

    /**Check if the given vertices has a edge between them 
     * @param source
     * @param dest
     * @return true if there is an edge
     */
    public boolean isEdge(Vertex source, Vertex dest)
    {
        if(isDirected())
        {
            if(source.findAdj(dest) == true)
                return true;
        }
        else
        {
            if(source.findAdj(dest) == true || dest.findAdj(source) == true)
                return true;
        }
        return false;
    }

    /**Get the edge between the given vertices
     * @param source 
     * @param dest
     * @return the edge between them 
     */
    public Edge getEdge(Vertex source, Vertex dest)
    {
        if(isDirected())
        {
            if(source.findAdj(dest) == true)
            {
                Edge edge = null;
                for (Edge obj : edges)
                {
                    if(obj.getSource().equals(source) && 
                        obj.getDest().equals(dest))
                        edge = obj;
                }
                return edge;
            }
        }
        else
        {
            if(source.findAdj(dest) == true)
            {
                Edge edge = null;
                for (Edge obj : edges)
                {
                    if(obj.getSource().equals(source) && 
                        obj.getDest().equals(dest))
                        edge = obj;
                }
                return edge;
            }
            else if(dest.findAdj(source) == true)
            {
                Edge edge = null;
                for (Edge obj : edges)
                {
                    if(obj.getSource().equals(dest) && 
                        obj.getDest().equals(source))
                        edge = obj;
                }
                return edge;
            }
        }

        return new Edge(source, dest, Double.POSITIVE_INFINITY);

    }

    /**Get the vertex with the given index
     * @param index
     * @return the vertex
     */
    public Vertex getVertex(int index)
    {
        for (Vertex obj : vertices)
        {
            if(obj.getID() == index)
                return obj;
        }

        return null;
    }
}