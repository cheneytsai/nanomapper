package parser;

import java.util.Scanner;

import dataStruct.NetNode;
import dataStruct.NetSet;


/**
 * Parser for the ISCAS-85 benchmark format
 * 
 * @author Cheney Tsai
 */
public class IscasParser extends AbstractNetlistParser
{
	public IscasParser(Scanner input)
	{
		super(input);
		parse(input);
	}
	
	@Override
	protected void parse(Scanner input)
	{
		//TODO write REALLLLL parser that actually will work better
		System.out.println("...Getting ready to parse ISCAS netlist format");
		myNetSet = new NetSet();
		int currentID = 0;
		while(input.hasNextLine())
		{
			int id = Integer.parseInt(input.next());
			if(id == currentID+1)			//skipping the lines I don't understand
			{
			String name = input.next();
			String type = input.next();
			int fin	= Integer.parseInt(input.next());
			int fout = Integer.parseInt(input.next());
			boolean sa0;
			boolean sa1;
			if (input.next().equals(">sa0"))
			sa0 = true;
			else
			sa0 = false;
			if (input.next().equals(">sa1"))
			sa1 = true;
			else
			sa1 = false;
			
			NetNode newNode = new NetNode(myNetSet, id, name, type, fin, fout, sa0, sa1);
			currentID++;
			}
			else
			input.nextLine(); //skip line
					
		}
		System.out.println("...Finised parsing ISCAS netlist format");
	}
	
	
}
