package dataStruct;

import java.util.ArrayList;
import java.util.HashMap;

/**
 * A container for a circuit architecture and it's components
 * 
 * @author Cheney Tsai
 */
public class CircuitArch 
{
	ArrayList<ArchNode> myNodes;
	HashMap<String,ArchEdge> myEdges;
	
	//private boolean needExpansion = false; //not needed
	
	public CircuitArch()
	{
		myNodes = new ArrayList<ArchNode>();
		myEdges = new HashMap<String, ArchEdge>();
	}
	
	public void addNode(String nodeExpression)
	{
		String nodeType = nodeExpression.substring(nodeExpression.indexOf('=')+2, nodeExpression.indexOf('('));
		ArchNode newNode = new ArchNode(nodeType);
		myNodes.add(newNode);
		
		String newOut = nodeExpression.substring(0, nodeExpression.indexOf(' '));
		ArchEdge outEdge = null;
		if (myEdges.containsKey(newOut))
			outEdge = myEdges.get(newOut);
		else
		{
			outEdge = new ArchEdge(new ArrayList<ArchNode>(), 0, newOut);
			myEdges.put(newOut, outEdge);
		}
		outEdge.addLink(newNode);			//Add Node to Link
		newNode.addOutput(outEdge);			//Add Link to Node
		
		String[] inputs = nodeExpression.substring(nodeExpression.indexOf('(')+1, nodeExpression.indexOf(')')).split(", ");
		for (int i =0; i<inputs.length;i++)
		{
			ArchEdge inEdge = null;
			if (myEdges.containsKey(inputs[i]))	//Get Link or Create New One
				inEdge = myEdges.get(inputs[i]);
			else
			{
				inEdge = new ArchEdge(new ArrayList<ArchNode>(), 0, inputs[i]);
				myEdges.put(inputs[i], inEdge);
			}
			inEdge.addLink(newNode);		//Add Node to Link
			newNode.addInputs(inEdge);		//Add Link to Node
		}
		
	}
	
	//0 = bounded, 1 = input, 2 = output
	public void addEdge(String newEdge, int type)
	{
		if (myEdges.containsKey(newEdge))
		{}
		else
		myEdges.put(newEdge, new ArchEdge(new ArrayList<ArchNode>(), type, newEdge));
	}
	
	
	//test methods
	public void printNumNodes()
	{
		System.out.println("Number of Nodes: " + myNodes.size());
	}
	
	public void printEdges()
	{
		System.out.println(myEdges.toString());
	}
	
	public void printNodeLinks(int index)
	{
		System.out.println(myNodes.get(index).getEdges());
	}
	

}
