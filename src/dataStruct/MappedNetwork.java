package dataStruct;

import java.util.HashMap;
import java.util.List;

/**
 * A container for mapped network
 * 
 * @author Cheney Tsai
 */
public class MappedNetwork 
{
	private NetSet myNetSet;
	private PhysicalNetwork myPhysicalNetwork;
	private HashMap<PhysicalNode, NetNode> myMapping;
	// add mapping of links too
	
	/**
	 * Instantiates a Mapped Network
	 * 
	 * @param size
	 */
	public MappedNetwork(NetSet tobeMapped, PhysicalNetwork tobeMappedOn)
	{
		myNetSet = tobeMapped;
		myPhysicalNetwork = tobeMappedOn;
		myMapping = new HashMap<PhysicalNode, NetNode>();
	}
	
	/**
	 * Adds a Mapping to the network
	 * 
	 * @return int if method call fails, return -1
	 */
	public int addMapping(PhysicalNode rushee)
	{
		return -1;
		
	}
	
	/**
	 * Returns a List of possible Nodes from current Node
	 * 
	 * @param currentNode
	 * @return
	 */
	public List<PhysicalNode> getPossibleNextNode(PhysicalNode currentNode)
	{
		return null;
	}
	
}
