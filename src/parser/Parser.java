package parser;

import java.util.Scanner;

/*
 * 
 * @author Cheney Tsai
 */
public abstract class Parser 
{
	private NodeSet myNodeSet;
	
	public Parser(Scanner input)
	{
		myNodeSet = new NodeSet();
	}
	
	protected void setup(Scanner input)
	{
		
	}
	
	public NodeSet getNodeSet()
	{
		return myNodeSet;
		
	}
	
}
