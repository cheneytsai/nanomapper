package parser;

import java.util.Scanner;

import dataStruct.PhysicalNetwork;

/**
 * Abstract parser for all physical network descriptions
 * 
 * @author Cheney Tsai
 */
public abstract class AbstractPhysicalMapParser extends AbstractParser
{
	protected PhysicalNetwork myParsedNetwork;
	
	public AbstractPhysicalMapParser(Scanner input)
	{
		super(input);
		myParsedNetwork = null;
	}
	
	@Override
	protected void parse(Scanner input)
	{
		
	}
	
	public PhysicalNetwork getPhysicalNetwork()
	{
		return myParsedNetwork;
		
	}
	
}
