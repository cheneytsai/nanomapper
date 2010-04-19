package parser;

import java.util.Scanner;

import dataStruct.NetSet;

/**
 * Parser for the ISCAS-85 benchmark format
 * 
 * @author Cheney Tsai
 */
public abstract class IscasParser extends AbstractNetlistParser
{
	
	public IscasParser(Scanner input)
	{
		super(input);
		myNetSet = new NetSet();
		parse(input);
	}
	
	@Override
	protected void parse(Scanner input)
	{
		System.out.println("...Getting ready to parse ISCAS netlist format");
	}
	
	
}
