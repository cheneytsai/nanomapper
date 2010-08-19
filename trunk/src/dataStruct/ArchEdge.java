package dataStruct;

import java.util.ArrayList;

/**
 * A node to represent a element on a circuit architecture
 * 
 * @author Cheney Tsai
 */
public class ArchEdge
{
	String id;
	ArrayList<ArchNode> connectedNodes;
	int type;

	public ArchEdge()
	{
		id = "";
		connectedNodes = new ArrayList<ArchNode>();
		type = -1;
	}
	
	
	public ArchEdge(ArrayList<ArchNode> nodes, int newType, String newID)
	{
		id = newID;
		connectedNodes = nodes;
		type = newType;
	}
	
	public void addLink(ArchNode newNode)
	{
		connectedNodes.add(newNode);
	}
	
	public String getId()
	{
		return id;
	}
	
	
}
