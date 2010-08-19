package dataStruct;

import java.util.ArrayList;

/**
 * A node to represent a element on a circuit architecture
 * 
 * @author Cheney Tsai
 */
public class ArchNode
{
	String myType;			//function of gate driving node
	ArchEdge outputEdge;
	ArrayList<ArchEdge> inputEdges;
	
	public ArchNode()
	{
		myType = "null";
		outputEdge = null;
		inputEdges = new ArrayList<ArchEdge>();
	}
	
	public ArchNode(String whatType)
	{
		myType = whatType;
		outputEdge = null;
		inputEdges = new ArrayList<ArchEdge>();
	}
	
	public ArchNode(String whatType, ArchEdge output, ArrayList<ArchEdge> inputs)
	{
		myType = whatType;
		outputEdge = output;
		inputEdges = inputs;
	}
	
	public void addInputs(ArchEdge inEdge)
	{
		inputEdges.add(inEdge);
	}
	
	public void addOutput(ArchEdge outEdge)
	{
		outputEdge = outEdge;
	}
	
	public String getEdges()
	{
		String edges = "Inputs: ";
		for(ArchEdge edge: inputEdges)
		{
			edges = edges.concat(edge.getId()).concat(" ");
		}
		return edges;
	}
}
