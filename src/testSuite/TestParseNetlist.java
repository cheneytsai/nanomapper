package testSuite;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Scanner;

import parser.IscasParser;

/**
 * Test for Netlist Parser
 * @author Cheney Tsai 
 */
public class TestParseNetlist 
{

	//Constants
	public static final String DEFAULT_NETLIST = "bin/netlists/iscas/s298.isc";

	public static void main(String[] args) 
	{
		System.out.println("Running TestParseNetlist...");
		Scanner netlist_In	= null;
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
		}
		catch(FileNotFoundException e) 
		{
			System.out.println("Error! File Not Found!");
			e.printStackTrace();
		} 
		catch (IOException e) 
		{
			System.out.println("Error! IOException");
			e.printStackTrace();
		}
		//Parse Netlist
		IscasParser.parse(netlist_In);
		
	}
	
	/**
	 * Prompt User for Input
	 * @param message
	 * @return
	 */
	private static String promptUser(String message)
	{
		Scanner in = new Scanner(new InputStreamReader(System.in));
		System.out.println(message);
		System.out.print("->");
		String response = in.nextLine();
		return response;
		
	}
}
