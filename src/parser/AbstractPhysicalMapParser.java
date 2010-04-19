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
	protected PhysicalNetwork myPhysicalNetwork;
	
	public AbstractPhysicalMapParser(Scanner input)
	{
		super(input);
		myPhysicalNetwork = null;
	}
	
	@Override
	protected void parse(Scanner input)
	{
		
	}
	
	public PhysicalNetwork getPhysicalNetwork()
	{
		return myPhysicalNetwork;
		
	}
	
}
