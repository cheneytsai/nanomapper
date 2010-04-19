package parser;

import java.util.Scanner;

import dataStruct.NetSet;

/**
 * Abstract parser for all architecture netlists
 * 
 * @author Cheney Tsai
 */
public abstract class AbstractNetlistParser extends AbstractParser
{
	protected NetSet myNetSet;
	
	public AbstractNetlistParser(Scanner input)
	{
		super(input);
		myNetSet = new NetSet();
	}
	
	@Override
	protected void parse(Scanner input)
	{
		
	}
	
	public NetSet getNodeSet()
	{
		return myNetSet;
		
	}
	
}
