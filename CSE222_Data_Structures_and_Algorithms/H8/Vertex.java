import java.util.ArrayList;

public class Vertex 
{
    private int ID;
    private String label;
    private double weight;
    private double boosting;
    protected ArrayList<Vertex> adjList;
  
    public int getID(){return ID;}
    public String getLabel(){return label;}
    public double getWeight(){return weight;}
    public double getBoosting(){return boosting;}

    /**Constructor with 4 parameters
     * @param ID
     * @param label
     * @param weight
     * @param boosting
     */
    public Vertex(int ID, String label, double weight, double boosting)
    {
        this.ID = ID;
        this.label = label;
        this.weight = weight;
        this.boosting = boosting;
        adjList = new ArrayList<Vertex>();
    }    

    /**Add an adjacent vertex 
     * @param another
     */
    public void addAdj(Vertex another)
    {
        adjList.add(another);
    }

    /**Remove an adjacent vertex
     *  @param another
     */
    public void removeAdj(Vertex another)
    {
        adjList.remove(another);
    }

    /**Find an adjacent vertex
     *  @param another
     */
    public boolean findAdj(Vertex another)
    {
        return adjList.contains(another);
    }

    @Override
    public String toString()
    {
        return "Vertex: " + ID + " " + label + " " + weight;
    }
}
