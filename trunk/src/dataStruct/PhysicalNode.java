package dataStruct;

import java.awt.Dimension;
import java.awt.Point;
import java.util.HashMap;

/**
 * A node to represent a physical spot on a physical network
 * 
 * @author Cheney Tsai
 */
public class PhysicalNode extends Node
{
	private int nodeID;
	private HashMap<Integer, Integer> myLinks;
	private PhysicalNetwork myParentNetwork;
	
	//unused
	boolean isDefective;
	Dimension mySize;
	Point myLocation;
	
	/**
	 * Creates a Node and associates(NOT ADD) to a PhysicalNetwork if possible
	 * 
	 * @param newID
	 * @param parent
	 */
	public PhysicalNode(int newID, PhysicalNetwork parent)
	{
		nodeID = newID;						
		myLinks = new HashMap<Integer, Integer>();
		myParentNetwork = parent;
		
		//unused
		isDefective = false;
		mySize = new Dimension();
		myLocation = new Point();
	}
	
	/**
	 * Tries to add Node to Network. Checking if spot is taken, full network, or conflicting links
	 * 
	 * @return
	 */
	public int addToNetwork(HashMap<Integer,Integer> linksToTry)
	{
		myLinks = linksToTry;
		return myParentNetwork.addNode(this);
	}
	
	/**
	 * Adds a link to the map..
	 * TODO should contain which link of node connected to
	 * @param id
	 * @param node
	 */
	public void addLink(int id, int node)
	{
		myLinks.put(id, node);
	}
	
	/**
	 * Cuts Link to Other Nodes, making sure to sever both connections
	 * 
	 * @param linkID
	 * @return int -1 if method call fails
	 */
	public int cutLink(int linkID)
	{
		myLinks.remove(linkID);
		return 0;
	}
	
	/**
	 * Gets this Physical Nodes ID (currently only integer values)
	 * 
	 * @return
	 */
	public int getID()
	{
		return nodeID;
	}
	
	public String getLinks()
	{
		String stuff = "";
		for( int i = 0; i < myLinks.size(); i++)
		{
			if (myLinks.containsKey(i))
			{
				System.out.println("asdfasdf");
				stuff.concat(myLinks.get(i) + " ");
			}
			else
				stuff.concat("x ");
		}
		
		return stuff;
		
	}
	
	public void printNode()
	{
		System.out.print(nodeID + ":: " );
		for(int i = 0; i < 3; i++) //tentative
		{
			System.out.print(i + ": ");
			if (myLinks.containsKey(i))
			System.out.print(myLinks.get(i) + " ");
			else
			System.out.print(" ");
		}
		System.out.println("");
	}
}
