import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Scanner;

import dataStruct.PhysicalNetwork;
import dataStruct.PhysicalNode;

import parser.UnnamedPhysicalMapParser;


/**
 * Text Based Interface for nanoMapper
 * 
 * @author Cheney Tsai
 */
public class TextConsole 
{
//TODO Extend from abstract console
	public static final String PROMPT = "-> ";
	public static final String WELCOME_MSG = "Welcome to nanoMapper v0.1!";
	public static final String DEFAULT_NETLIST = "bin/netlists/iscas/74283.isc";
	//public static final String DEFAULT_NETWORK = "bin/topos/Topology.0.0.0.80.225.0.000.20.168.150.3";
	public static final String DEFAULT_NETWORK = "bin/topos/Topology.0.0.0.10.225.0.000.17.111.150.3";
	
	public TextConsole()
	{
		System.out.println(WELCOME_MSG);
		
		Scanner netlist_In	= null;
		Scanner network_In	= null;
		try
		{
			System.out.println("Your Current Directory: " + new File(".").getCanonicalPath() + "\r");
			
			//get netlist
			String netlist_path = promptUser("Please enter netlist path:");
			
			if (netlist_path.equals(""))
			{
				System.out.println("...Using Default Path: " + DEFAULT_NETLIST + "\r");
				netlist_path = DEFAULT_NETLIST;
			}
		
			netlist_In = new Scanner(new File(netlist_path));
			
			//get physical network
			String network_path = promptUser("Please enter network path:");

			if (network_path.equals(""))
			{
				System.out.println("...Using Default Path: " + DEFAULT_NETWORK + "\r");
				netlist_path = DEFAULT_NETWORK;
			}
			
			network_In = new Scanner(new File(netlist_path));
			
		} catch (FileNotFoundException e) 
		{
			System.out.println("Error! File Not Found!");
			e.printStackTrace();
		} catch (IOException e) {
			System.out.println("Error! IOException");
			e.printStackTrace();

		}
		
		//Parse the graphs
		
		UnnamedPhysicalMapParser myPhysicalParser = new UnnamedPhysicalMapParser(network_In);
		
		PhysicalNetwork myPhysicalNetwork = myPhysicalParser.getParsedNetwork();
		PhysicalNode testNode;
		
		for (int i = 0; i < 10; i++)
		{
			testNode = myPhysicalNetwork.getRandomNode();
			System.out.println(testNode.getID());
			System.out.println(testNode.getLinks());
		}
		
	}
	
	
	
	private String promptUser(String message)
	{
		Scanner in = new Scanner(new InputStreamReader(System.in));
		System.out.println(message);
		System.out.print(PROMPT);
		String response = in.nextLine();
		return response;
		
	}
}
