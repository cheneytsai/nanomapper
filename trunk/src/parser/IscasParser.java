package parser;

import java.util.Scanner;

import dataStruct.CircuitArch;


/**
 * Parser for the ISCAS-85 benchmark format
 * 
 * @author Cheney Tsai
 */
public class IscasParser
{
	private static CircuitArch myCircuitArch;
	public static CircuitArch parse(Scanner input)
	{
		System.out.println("...Getting ready to parse ISCAS netlist format");
		myCircuitArch = new CircuitArch();
		String currentLine = "";
		while(input.hasNextLine())
		{
			currentLine = input.nextLine();
			if (currentLine.isEmpty()) { }
			else if (currentLine.charAt(0) == '#'){ }
			else if (currentLine.substring(0, 5).equalsIgnoreCase("INPUT"))
			{
				myCircuitArch.addEdge(currentLine.substring(currentLine.indexOf('(')+1, currentLine.lastIndexOf(')')),1);  			//Assumes I/O is before Node Declarations
			}
			else if (currentLine.substring(0, 6).equalsIgnoreCase("OUTPUT"))
			{
				myCircuitArch.addEdge(currentLine.substring(currentLine.indexOf('(')+1, currentLine.lastIndexOf(')')),2);  			//Assumes I/O is before Node Declarations
			}
			else if (currentLine.contains(" = "))
			{
				myCircuitArch.addNode(currentLine);
			}
			else
			System.out.println("Unknown Syntax in Line: " + currentLine);
			
					
		}
		System.out.println("...Finished parsing ISCAS netlist format");

		myCircuitArch.printNodeLinks(0);

		return myCircuitArch;
	}
	
	
}
