package dataStruct;

import java.util.HashMap;

/**
 * A container for a physical network and its nodes
 * 
 * @author Cheney Tsai
 */
public class PhysicalNetwork 
{
	//TODO Node ID should be more generic
	private HashMap<Integer, PhysicalNode> myNodes;
	private int nodeLimit;
	
	/**
	 * Instantiates a PhysicalNetwork of some size
	 * 
	 * @param size
	 */
	public PhysicalNetwork(int size)
	{
		myNodes = new HashMap<Integer, PhysicalNode>();
		nodeLimit = size;
	}
	
	/**
	 * Adds a node to the Physical Network
	 * 
	 * @return int if method call fails, return -1
	 */
	public int addNode(PhysicalNode rushee)
	{
		//TODO Add check for link conflictsprintPhysicalNetwork(myPhysicalParser);
		if(myNodes.containsValue(rushee.getID()) || myNodes.size() >= nodeLimit)
		{
			return -1;			
		}
		else
		{
			myNodes.put(rushee.getID(), rushee);
		}
		return 0;
	}
	
	/**
	 * Gets a random node for the caller to use
	 * 
	 * @return PhysicalNode a node randomly chose from inside this network
	 */
	public PhysicalNode getRandomNode()
	{
		if (myNodes.isEmpty())
		{
			System.out.println("Warning: Physical Network is empty!");
			return null;
		}
		else 
		{
			Integer randID = (int)(Math.random()*(myNodes.size()));
			return myNodes.get(randID);
		}
		
	}
	
}
