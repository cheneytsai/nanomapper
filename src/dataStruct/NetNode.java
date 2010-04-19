package dataStruct;

/**
 * A node to represent a physical spot on a physical network
 * 
 * @author Cheney Tsai
 */
public class NetNode extends Node
{
	NetSet myNetSet;
	
	int myAddress;			//unique id
	String myName;			//string description
	String myType;			//function of gate driving node
							//inpt, and, nand, or, nor, xor, xnor, buff, not, from
	int fanout;				//number of gates driven by this node
	int fanin;				//# of nodes driving gate that drives this node
	boolean stuckAtZero;	//>sa0
	boolean stuckAtOne;		//>sa1
	
	public NetNode()
	{
		myNetSet = null;
		myAddress = -1;
		myName = "";
		myType = "";
		fanout = -1;
		fanin = -1;
		stuckAtZero = false;
		stuckAtOne = false;
	}
	
	public NetNode(NetSet myNetSet, int id, String name, String type, int fin, int fout, boolean sa0, boolean sa1)
	{
		myNetSet = myNetSet;
		myAddress = id;
		myName = name;
		myType = type;
		fanout = fin;
		fanin = fout;
		stuckAtZero = sa0;
		stuckAtOne = sa1;
	}

	public void printNodeInfo() {
		System.out.println(myAddress + " " + myName + " " + myType + " " + fanout +	" " + 
				fanin + " " + stuckAtZero + " " + stuckAtOne);
		
	}
	
	
}
