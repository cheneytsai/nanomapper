package fitter;

import dataStruct.MappedNetwork;
import dataStruct.NetSet;
import dataStruct.PhysicalNetwork;

/**
 * Implements a fitter following a greedy Algorithm
 * 
 * @author cct8
 *
 */
public class GreedyAlgo 
{
	static MappedNetwork myMappedNetwork;
	
	public static MappedNetwork runFit(PhysicalNetwork aPhysicalNetwork, NetSet aNetSet)
	{
		myMappedNetwork = new MappedNetwork(aNetSet, aPhysicalNetwork);
		
		runAlgo();
		
		return myMappedNetwork;
	}
	
	private static void runAlgo()
	{
		//TODO Code the Algo
	}
}
