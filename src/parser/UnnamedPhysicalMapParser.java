package parser;

import java.util.Scanner;

import dataStruct.PhysicalNetwork;
import dataStruct.PhysicalNode;


/**
 * Parser for the physical networks of unnamed format #1
 * 
 * @author Cheney Tsai
 */
public class UnnamedPhysicalMapParser extends AbstractPhysicalMapParser
{
	//TODO make parser standalone
	public UnnamedPhysicalMapParser(Scanner input)
	{
		super(input);
		parse(input);
	}
	
	@Override
	protected void parse(Scanner input)
	{
		System.out.println("...Getting ready to parse unamed Physical Network format");
		
		//Create new Physical Network with size information
		myParsedNetwork = new PhysicalNetwork(Integer.parseInt(input.nextLine()));
		//Iterate Through Listed Nodes
		//initialize vars
		int nodeID = -1;
		int curLink = -1;
		PhysicalNode newNode =null;
		
		while (input.hasNext())
		{			
			String token = input.next();
			
			if (token.endsWith("::"))					//Sets new node when :: is defined
			{
				nodeID = Integer.parseInt(token.split("::")[0]);
				newNode = new PhysicalNode(nodeID, myParsedNetwork);			
				myParsedNetwork.addNode(newNode);
			}
			else if (token.endsWith(":"))				//Sets new link when : is defined
			{
				curLink = Integer.parseInt(token.split(":")[0]);
				
			}
			else if (token.contains("."))				//add link here
			{
				//System.out.println(token.substring(0, token.indexOf(".")));
				newNode.addLink(curLink, Integer.parseInt(token.substring(0, token.indexOf("."))));
			}
		}
		
		System.out.println("...Done Parsing unamed Physical Network format");
		return;
	}
	
	/**
	 * NOTUSED Something that recursively makes links
	 * @param input
	 * @param token
	 * @param thisNode
	 */
	private void makeLink(Scanner input, String token, PhysicalNode thisNode)
	{
		String newToken = input.next();
		if ( newToken.endsWith(":"))
		{
			thisNode.addLink(Integer.parseInt(token.split(":")[0]), -1);
			makeLink(input, newToken, thisNode);
		}
		else
		{
			thisNode.addLink(Integer.parseInt(token.split(":")[0]), 
									Integer.parseInt(newToken.split(".")[0]));
		}
	}

	
}
