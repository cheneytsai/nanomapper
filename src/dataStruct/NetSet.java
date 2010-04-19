package dataStruct;

import java.util.ArrayList;

/**
 * A container for a architecture and it's components
 * 
 * @author Cheney Tsai
 */
public class NetSet 
{
	public ArrayList<NetNode> myNodes;
	
	public NetSet()
	{
		myNodes = new ArrayList<NetNode>();
	}
	
	public void addNode(NetNode newNode)
	{
		myNodes.add(newNode);
	}
	
	public void printNodes()
	{
		for(int i = 0; i <myNodes.size(); i++)
		{
			NetNode tempNode = myNodes.get(i);
			tempNode.printNodeInfo();
		}
	}
}
