/** Dec 14, 2004
    PhysicalModel.cc

    The purpose of this program is to simulate the physical self-assembly
    of NANA nodes. This begins the simulation after the nodes have been
    formed, and are about to be connected to each other.

    This simulation will eventually model this connection process in gory
    detail.

    For now, the first step is as follows
    Start with a very fine grained grid. The grid points are approximately
    20nm apart. (Yan et al, Science, Vol 301, Sept '03...nanowires). Each
    node itself is 1000x1000nm.

    For now, we'll model an area of 1x1 m = 50000x50000 grid points..
    (is this really necessary ?)

    First, we pick the total number of nodes we're gonna have. Then,
    (for now), we assume that the nodes are uniformly distributed in
    the 1x1m area. Thus, we start assigning nodes to grid points. When
    each node is assigned, it is also assigned an orientation, which
    is basically the angle by which it is rotated.

    Once each node has been placed, we start the link growth
    process. In each tiame step, the nodes send out tubules of DNA
    from designated spots on their sides. One unit length of the
    tubule occupies two squares of the grid. There is a probability
    matrix associated with each time step. This governs the
    probability that the tubule will move in a particular direction at
    each time step. Once a step is made in a direction, we fill in
    asll possible gaps up to that point, and then at the next step
    compute again. If two tubules collide, they stop growing. If a
    tubule grows a distance of zero, it stops growing permanently. As
    time increases, the probability of it growing a distance of zero
    increases.

*/
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include "stdio.h"
#include "math.h"
#include "PhysicalModel.hh"
#include "png.h"
#include "randomc.h"
#include "userintf.cpp"
#include "mersenne.cpp"
#include "Topology.hh"
#include "OptionDB.hh"
#include "Node.hh"
#include "sys/time.h"
using namespace std;
#define NODE_SIZE         250 //rch18
#define MIN_NODE_SIZE     250
#define ToRad(X)          X*M_PI/180.0
#define ToDeg(X)          X*180.0/M_PI
TRandomMersenne *R1,*R2;
unsigned char **NANANet;
Node **NodeList;
RTList *Probabilities;
int Size=100;
int AngleMatrix[19][21];
Point **Rows;
int *IndexArray;
int BitsPerEntry;
int NumberOfNodes;
//unsigned long long NumberOfNodes;
int PControl,OControl,IControl;
int Distance;
unsigned GridRowPoints;
unsigned long long RealRowSize;
unsigned long long GrowthStep;
double **ConnectMatrix;
int CDF_Count;
double GetDistance(double x1,double y1,double x2,double y2)
{
  double Dist=(x1-x2)*(x1-x2)+(y1-y2)*(y1-y2);

  Dist=sqrt(Dist);
  //  printf("\nDistance: %5.2lf",Dist);
  return Dist;
}

Point Rotate(int X,int Y,int Xr,int Yr, double cosA,double sinA)
{
  Point Rotated;
  //  double A=Angle*M_PI/180.0;
  Rotated.X=(int)(Xr+(X-Xr)*cosA-(Y-Yr)*sinA);
  Rotated.Y=(int)(Yr+(X-Xr)*sinA+(Y-Yr)*cosA);
  Rotated.Next=NULL;
  Rotated.N=NULL;
  Rotated.T=NULL;
  return Rotated;
}
void GetParameters(double *R, double *T, double Rnd,int Count)
{
  int Index=IndexArray[(int)(Size*Rnd)];
  //  printf("\nIndex: %d, Size: %d, Rnd: %lf",Index,Size,Rnd);
  //  fflush(stdout);
  if(Index+1 < Count)
    while(Probabilities[Index+1].Prob < Rnd)
      Index++;
  *R=Probabilities[Index].R/20;
  *T=Probabilities[Index].Theta;

}

void SetBit(Point Pt)
{
  if(Pt.X>=(signed)GridRowPoints || Pt.Y >= (signed)GridRowPoints || Pt.X <=0 || Pt.Y <= 0)
    return;
  NANANet[Pt.Y][Pt.X/BitsPerEntry] |= (1 << ((sizeof(unsigned char)*8-1)-(Pt.X % BitsPerEntry)));
}
void CreateBitmap()
{
  RealRowSize=GridRowPoints/BitsPerEntry;
  NANANet=new unsigned char*[GridRowPoints];
  //cout << "Allocating "<< GridRowPoints << " bitmaps, each with " << RealRowSize << " elements longs" << endl;
  for(int i=0;i<(signed)GridRowPoints;i++)
    {
      unsigned char *Temp=new unsigned char[RealRowSize];
      NANANet[i]=Temp;
      for(int j=0;j<(signed)RealRowSize;j++)
	NANANet[i][j]=0;
    }
  for(int y=0;y<(signed)GridRowPoints;y++)
    {
      Point *temp=Rows[y];

      while(temp != NULL)
	{
	  SetBit(*temp);
	  temp=temp->Next;
	}
    }
}
void DeleteBitmap()
{
  for(int i=0;i<(signed)GridRowPoints;i++)
    {
      unsigned char *Temp=NANANet[i];
      if(Temp != NULL)
	delete[] Temp;
    }
  delete[] NANANet;
}
void DestroyLink(Track *T)
{
  Link *Temp1,*Temp2;
  Temp1=T->Start;
  if(Temp1==NULL)
    return;
  while(Temp1 != T->Last)
    {
      Temp2=Temp1;
      Temp1=Temp1->Next;
      delete Temp2;
    }
  delete Temp1;
}
void DestroyList(Track *T)
{
  TrackList *Temp1,*Temp2;
  Temp1=T->ConnectedXcvrs;
  if(Temp1==NULL)
    return;
  while(Temp1->Next != NULL)
    {
      Temp2=Temp1;
      Temp1=Temp1->Next;
      delete Temp2;
    }
  delete Temp1;
}
void DestroyNodes()
{
  /* delete all links and transceivers */
  for(int i=0;i<NumberOfNodes;i++)
    {
      for(int j=0;j<NodeList[i]->NUM_TRANS;j++)
	{
	  DestroyLink(NodeList[i]->Xcvr[j]);
	  DestroyList(NodeList[i]->Xcvr[j]);
	}
    }

  /* Delete all points */
  for(unsigned i=0;i<GridRowPoints;i++)
    {
      if(Rows[i]!=NULL)
	{
	  Point *Temp=Rows[i],*Temp1;
	  while(Temp->Next != NULL)
	    {
	      Temp1=Temp;
	      Temp=Temp->Next;
	      delete Temp1;
	    }
	  delete Temp;
	}
    }
  delete[] Rows;
  /* delete all nodes */
  delete[] NodeList;


}
Track *GetClosestTrack(Point *P)
{
  Node *N=P->N;
  double minDist=500.0;
  double minDistSq=minDist*minDist;
  int Index=0;
  for(int i=0;i<N->NUM_TRANS;i++)
    {
      double Temp= (P->X - N->Xcvr[i]->X)*(P->X - N->Xcvr[i]->X) + (P->Y - N->Xcvr[i]->Y)*(P->Y - N->Xcvr[i]->Y);
      if(Temp<minDistSq)
	{
	  minDistSq=Temp;
	  Index=i;
	}
    }
  if(minDistSq < 400)
    {
      return N->Xcvr[Index];
    }
  return NULL;
}
int FindDuplicate(GraphXcvr *Start,int N,int L)
{
  GraphXcvr *Temp=Start;
  while(Temp->Next != Start)
    {
      if(Temp->NodeID==N && Temp->LinkID==L)
	return 1;
      Temp=Temp->Next;
    }
  if(Temp->NodeID==N && Temp->LinkID==L)
    return 1;
  return 0;
}
/* This procedure will convert all tracks that are connected to T
   into a list */
void AddLinks(int NodeId,int TId,Node **N,Track *T)
{
  Node *NodePtr=N[NodeId];
  GraphXcvr *LinkPtr=NodePtr->TransceiverLinks[TId];
  GraphXcvr *Link=N[(int)T->N->ID]->TransceiverLinks[(int)T->ID];
  assert(Link != NULL && LinkPtr != NULL);
  /* Make sure we're not trying to add Track A to its own list ! */
  if(T->N->ID==NodeId && T->ID==TId)
    {
      return;
    }

  /* Ok, first, we look at the entry corresponding to [NodeId][TId] */
  if(LinkPtr->Next==NULL)
    { // Ok, this would create a new entry under LinkPtr, which means, we need
      // to examine the other fella
      if(Link->Next==NULL)
	{ // Ok, both are new...link 'em to each other
	  LinkPtr->Next=Link;
	  Link->Next=LinkPtr;
	}
      else
	{ // Ok, Link already has a next element......
	  // we *don't* have to check for duplicates, since a duplicate
	  // would imply that LinkPtr would have a NEXT entry...which would
	  // not bring us down this code path !!!!
	  // Just insert LinkPtr between Link and its next element
	  LinkPtr->Next=Link->Next;
	  Link->Next=LinkPtr;
	}
    }
  else
    {
      // Ok, LinkPtr is not NULL, which means that we're about to add LINK to an existing loop
      // Now, we'll insert LINK on the LINKPTR loop in two cases
      // a) Link is a single element *OR*
      if(Link->Next==NULL)
	{ // insert Link between LinkPtr and its next element
	  Link->Next=LinkPtr->Next;
	  LinkPtr->Next=Link;
	}
      else if(FindDuplicate(LinkPtr,Link->NodeID,Link->LinkID)==0)
	{// b) Link is part of a loop, but the LINKPTR and LINK loops are disjoint
	  GraphXcvr *Temp=Link->Next;
	  Link->Next=LinkPtr->Next;
	  LinkPtr->Next=Temp;
	}
    }
}
void PrintTrack(FILE *FP,Node *N,int LIndex,int NIndex)
{
  GraphXcvr *Start=N->TransceiverLinks[LIndex],*Temp;
  Temp=Start;
  fprintf(FP,"%d: ",LIndex);
  while(Temp != NULL)
    {
      if(Temp->NodeID != NIndex || Temp->LinkID != LIndex)
	fprintf(FP,"%d.%d ",Temp->NodeID,Temp->LinkID);
      Temp=Temp->Next;
      if(Temp==Start)
	Temp=NULL;
    }
}

void VisitNeighbors(Node **Nodes,int Index,int ClusterID)
{
  Nodes[Index]->VisitedCluster=1;
  if(ClusterID != Index)
    {
      Nodes[ClusterID]->ClusterCount++;
      Nodes[Index]->ClusterCount=0;
    }
  Nodes[Index]->ClusterID=ClusterID;
  for(int j=0;j<Nodes[Index]->NUM_TRANS;j++)
    {
      GraphXcvr *Temp=Nodes[Index]->TransceiverLinks[j];
      GraphXcvr *Start=Temp;
      while(Temp != NULL)
	{
	  if(Nodes[Temp->NodeID]->VisitedCluster!=1)
	    VisitNeighbors(Nodes,Temp->NodeID,ClusterID);
	  Temp=Temp->Next;
	  if(Temp==Start)
	    Temp=NULL;
	}
    }
}
void Analyze(double Decay,int Seed1,int Seed2)
{
  /*  Node* Nodes[NumberOfNodes];
  for(int i = 0; i<NumberOfNodes; i++)
    { //Need to allow for types here
      Nodes[i]=new Node();
    }
  */
  unsigned Count=0;
  for(int i=0;i<NumberOfNodes;i++)
    {
      //NodeList[i]->ID=i;
      NodeList[i]->Links=0;
      NodeList[i]->Visited=0;
      NodeList[i]->ClusterID=i;
      NodeList[i]->ClusterCount=1;
      NodeList[i]->VisitedCluster=0;
      for(int j=0;j<NodeList[i]->NUM_TRANS;j++)
	{
	  NodeList[i]->TransceiverLinks[j]=new GraphXcvr;
	  NodeList[i]->TransceiverLinks[j]->Next=NULL;
	  NodeList[i]->TransceiverLinks[j]->NodeID=i;
	  NodeList[i]->TransceiverLinks[j]->LinkID=j;
	  NodeList[i]->TransceiverLinks[j]->Counter=Count++;
	}
    }
  for(int i=0;i<NumberOfNodes;i++)
    {
      for(int j=0;j<NodeList[i]->NUM_TRANS;j++)
	{
	  int X,Y;
	  X=(int)NodeList[i]->Xcvr[j]->Last->X;
	  Y=(int)NodeList[i]->Xcvr[j]->Last->Y;
	  Link *S=NodeList[i]->Xcvr[j]->Start;
	  Link *E=NodeList[i]->Xcvr[j]->Last;
	  Point *P=Find(X,Y);
	  if(S != E && P != NULL)
	    { // Link is longer than one point, and the endpoint overlaps with something !
	      /* Since the track stopped growing at E, the point corresponding to E should be owned by someone else ! */
	      /* We need to identify the owner */
	      /* Now, to find out what other tracks and nodes this track intersects, consider this:
		 a) the primary "intersection" occurs only at the end of the track, where it stops growing. The
		    track/node at the end can be determined by examining the end-point and computing its owner
		 b) there may be secondary intersections which occur when *other* tracks intersect with this track.
		    However, note that those intersections *must* occur at the end of those other tracks, and will be
		    detected during the analysis of those tracks...so we can ignore them for now..*assuming* that there
		    are multiple passes to iron out all links
	      */

	      /* Ok, with that out of the way, how do we determine details of the primary intersection ?
		 We need to do two things
		 a) Determine if 'P' belongs to a node...if it does, compute which track on the node it is closest to
		 b) Determine the track corresponding to P, and add all the tracks connected to that track
		 c) Process all the tracks that have been stored as being connected to the current track ! <-- !!!
	      */
	      if(P->N != NULL)
		{ // Handle any node
		  Track *T=GetClosestTrack(P);
		  //		  printf("\nAdding (N) Track %x(%d.%d) to track %x (%d.%d)",T,T->N->ID,T->ID,&NodeList[i]->Xcvr[j],i,j);
		  if(T != NULL) // If the track wasn't close enough to the transceiver, it is not considered connected
		    {
		      AddLinks(i,j,NodeList,T);
		    }

		}
	      if(P->T != NULL)
		{ // Handle track
		  //		  printf("\nAdding Track (PT %d,%d) %x (%d.%d) to track %x (%d.%d)",P->X,P->Y,P->T,P->T->N->ID,P->T->ID,&NodeList[i]->Xcvr[j],i,j);
		  AddLinks(i,j,NodeList,P->T);
		  /* also need to add all links listed in P->T->ConnectedXcvrs */
		  TrackList *TL=P->T->ConnectedXcvrs;
		  while(TL != NULL)
		    {
		      //		      printf("\nAdding Track %x(%d.%d) to track %x (%d.%d)",TL->T,TL->T->N->ID,TL->T->ID,&NodeList[i]->Xcvr[j],i,j);
		      AddLinks(i,j,NodeList,TL->T);
		      TL=TL->Next;
		    }
		}
	    }
	  else if(P==NULL)
	    {
	      //	      printf("\n%d,%d is NULL, %x!",X,Y,Find(X,Y-1));
	      if(NodeList[i]->Xcvr[j]->ConnectedXcvrs != NULL)
		{
		  TrackList *TL=NodeList[i]->Xcvr[j]->ConnectedXcvrs;
		  while(TL != NULL)
		    {
		      //		      printf("\nAdding Track %x(%d.%d) to track %x (%d.%d)",TL->T,TL->T->N->ID,TL->T->ID,&NodeList[i]->Xcvr[j],i,j);
		      AddLinks(i,j,NodeList,TL->T);
		      TL=TL->Next;
		    }
		}
	    }
	}
    }
  char TopFile[30];
  sprintf(TopFile,"Topology/Topology.%d.%d.%d.%d.%d.%3.3lf.%d.%d.%d.%d",PControl,OControl,IControl,NumberOfNodes,Distance,Decay,Seed1,Seed2, NodeList[0]->MY_NODE_SIZE, NodeList[0]->NUM_TRANS);
  FILE *FP=fopen(TopFile,"w");
  if(FP==NULL)
    {
      fprintf(stderr,"\nFAILED TO WRITE TOPOLOGY FILE !");
    }
  else
    {
      fprintf(FP,"%ld",NumberOfNodes);
      for(int i=0;i<NumberOfNodes;i++)
	{
	  fprintf(FP,"\n%d:: ",i);
	  for(int j = 0; j<NodeList[i]->NUM_TRANS; j++)
	    {
	      PrintTrack(FP,NodeList[i],j,i);
	      //PrintTrack(FP,NodeList[i],1,i);
	      //PrintTrack(FP,NodeList[i],2,i);
	      //PrintTrack(FP,NodeList[i],3,i);
	    }
	}
      fprintf(FP,"\n");
      fclose(FP);

    }
  double Links=0,Loops=0;
  for(int i=0;i<NumberOfNodes;i++)
    {
      for(int j=0;j<NodeList[i]->NUM_TRANS;j++)
	{
	  if(NodeList[i]->TransceiverLinks[j]->Next != NULL)
	    NodeList[i]->Links++;
	  for(int k=j+1;k<NodeList[i]->NUM_TRANS;k++)
	    if(NodeList[i]->TransceiverLinks[j]->Next != NULL)
	      if(FindDuplicate(NodeList[i]->TransceiverLinks[j],i,k)==1)
		{
		  fprintf(stderr,"Loop at Node %d, X%d to X%d\n",i,j,k);
		  Loops++;
		}
	}
      Links+=NodeList[i]->Links;
    }
  double LinkCount=0,NodesPerLink=0;
  for(int i=0;i<NumberOfNodes;i++)
    for(int j=0;j<NodeList[i]->NUM_TRANS;j++)
      {
	if(NodeList[i]->Visited==0)
	  {
	    LinkCount++;
	    GraphXcvr *Temp=NodeList[i]->TransceiverLinks[j];
	    GraphXcvr *Start=Temp;
	    while(Temp != NULL)
	      {
		if(NodeList[Temp->NodeID]->Visited==0)
		  NodesPerLink++;
		NodeList[Temp->NodeID]->Visited=1;
		Temp=Temp->Next;
		if(Temp==Start)
		  Temp=NULL;
	      }
	  }
      }
  /* Figure out number of clusters */
  for(int i=0;i<NumberOfNodes;i++)
    {
      if(NodeList[i]->VisitedCluster==0)
	{
	  VisitNeighbors(NodeList,i,i);
	}
    }
  /*  for(int i=0;i<NumberOfNodes;i++)
    {
            if(i != Nodes[i].ClusterID)
	{
	  Nodes[Nodes[i].ClusterID].ClusterCount+=Nodes[i].ClusterCount;
	  Nodes[i].ClusterCount=0;
	  printf("\nCluster %d, Count: %d, i: %d",Nodes[i].ClusterID,Nodes[Nodes[i].ClusterID].ClusterCount,i);
	}
	}*/
  /*
  printf("\nAverage # of Links Per Node: %lf",Links/NumberOfNodes);
  double SideN=sqrt((double)NumberOfNodes);
  printf("\nAverage # of Links Per Node for perfect mesh with %ld Nodes: %lf",NumberOfNodes,(4*(SideN-2)*(SideN-2)+12.0*(SideN-2)+8)/NumberOfNodes);
  printf("\n# of Links Looping Back to same node: %lf",Loops);
  printf("\nAverage # of Nodes Per Link: %lf",NodesPerLink/LinkCount);
  */
  printf("%d:%u:%d:%lf:%lf:",NumberOfNodes,GridRowPoints,Distance,Decay,Links/NumberOfNodes);
  double SideN=sqrt((double)NumberOfNodes);
  printf("%lf:",(4*(SideN-2)*(SideN-2)+12.0*(SideN-2)+8)/NumberOfNodes);
  printf("%lf:",Loops);
  printf("%lf::",NodesPerLink/LinkCount);
  for(int i=0;i<NumberOfNodes;i++)
    if(NodeList[i]->ClusterCount!=0)
      printf("%d:%d::",NodeList[i]->ClusterID,NodeList[i]->ClusterCount);
  printf("\n");
  /* Now, free up memory ! */
  for(int i=0;i<NumberOfNodes;i++)
    {
      for(int j=0;j<NodeList[i]->NUM_TRANS;j++)
	{
	  GraphXcvr *T1;
	  T1=NodeList[i]->TransceiverLinks[j];
	  if(T1!=NULL)
	  {
	    delete T1;
	  }
	}
    }
  //  printf("\nLinks: %lf,Average Links: %lf",Links,Links/NumberOfNodes);
}
int main(int argc, char **argv)
{

  OptionDB odb;//create an option database
  int Seed1=0,Seed2=1;
  Distance=150; //rch18
  double DecayGrowth=0;
  int PNGDump, T1Size, T1Trans, T2Size, T2Trans;
  /* void reg_int(char *name,			 option name
	    char *desc,			 option description
	    int *var,			 pointer to option variable (location where the variable is stored)
	    int def_val);		 default value of option variable
	    -NumberOfNodes is an option defined in simconfig.cfg
	    -MaximumNodeNeighbors is an option defined in simconfig.cfg
	    -InternalBufferSize is an option defined in simconfig.cfg
	    -ReadPhysicalNetworkFromFile is an option defined in simconfig.cfg
  */
  odb.reg_int("NumberOfNodes", "Number of Nodes in the Network",&NumberOfNodes, 10000);
  odb.reg_uint("AreaSize","Size of Area (length of side only)",&GridRowPoints,20000);
  odb.reg_int("Seed1","Random Seed 1",&Seed1,0);
  odb.reg_int("Seed2","Random Seed 2",&Seed2,0);
  odb.reg_int("PlacementControl","Do we have control over node placement ? (0=No, 1=Yes)",&PControl,0);
  odb.reg_int("OrientationControl","Do we have control over node orientation ? (0=No, 1=Yes)",&OControl,0);
  odb.reg_int("InterconnectControl","Do we have control over Interconnect Growth ? (0=No, 1=Yes)",&IControl,0);
  odb.reg_int("MinDist","Minimum Distance between Nodes",&Distance,100);
  odb.reg_double("DecayRate","Decay Growth ?",&DecayGrowth,0.0);
  odb.reg_int("PngDump","Dump PNG Files at end of growth ?",&PNGDump,0);
  odb.reg_int("T1Size", "Size of type one node", &T1Size, 150);
  odb.reg_int("T1Trans", "Number of T1 trans", &T1Trans, 4);
  odb.reg_int("T2Size", "Size of type one node", &T2Size, 150);
  odb.reg_int("T2Trans", "Number of T2 trans", &T2Trans, 4);

  odb.opt_process_options(argc, argv);


  BitsPerEntry=sizeof(unsigned char)*8;
  //  if(Distance < NODE_SIZE) //rch18
  //  Distance=NODE_SIZE;
  if(PControl==1)
    {//rch18 no support
      double SC=sqrt(NumberOfNodes);
      if(Distance < NODE_SIZE*sqrt(2))
	GridRowPoints=(int)SC*(int)((double)NODE_SIZE*sqrt(2)+2);
      else
	GridRowPoints=(int)SC*(Distance);
    }
  else	// yang - assign a slightly larger area for simulation with the same number of nodes but without placement control
    {
      double SC=sqrt(NumberOfNodes);
      if(Distance < NODE_SIZE*sqrt(2))
      	{
      		GridRowPoints=(int)SC*(int)((double)NODE_SIZE*sqrt(2)+2);
      	}
      else
      	{
      		GridRowPoints=(int)SC*(Distance);
      	}
//      	GridRowPoints=(int)(GridRowPoints*1.5);
    }
  TRandomMersenne rg(Seed1);
  TRandomMersenne rg1(Seed2);
  R1=&rg;
  R2=&rg1;
  Rows=new Point*[GridRowPoints];
  for(int i=0;i<(signed)GridRowPoints;i++)
    Rows[i]=NULL;
  ReadCDF("CDF_0_0");
  NodeList=new Node*[NumberOfNodes](); //Here is the Node listing
  for(int i = 0; i<NumberOfNodes; i++)
    {
      if(i<NumberOfNodes/2)
	NodeList[i]=new NodeA(T1Size, T1Trans);
      else
	NodeList[i]=new Node(T2Size, T2Trans);
    }
  fprintf(stderr,"Number of Nodes: %ld\n",NumberOfNodes);
  fprintf(stderr,"Min. Distance: %d\n",Distance);
  struct timeval Start,End;
  unsigned long long microsecs;
  unsigned long long microsecs2;
  unsigned long long CheckReg;
  gettimeofday(&Start,NULL);
  //cout<<"HELLO L571"<<endl; segfault in the below

  CheckReg=CreateNodes(PControl,OControl);
  gettimeofday(&End,NULL);
  microsecs=((long long)1000000)*Start.tv_sec+Start.tv_usec;
  microsecs2=((long long)1000000)*End.tv_sec+End.tv_usec;
  fprintf(stderr,"Time Creating Nodes: %Ld\n",(microsecs2-microsecs)/1000);
  fprintf(stderr,"Time in Check Region: %Ld\n",CheckReg/1000);

//  fprintf(stderr,"\nBeginning Growth !");

  Grow(DecayGrowth,IControl);

  printf("%d\n", (int)GridRowPoints);
  printf("%d\n", NumberOfNodes);

  for(int i=0; i<NumberOfNodes; i++)
  {
//	printf("Node %d - X: %f, Y: %f\n", NodeList[i]->ID, NodeList[i]->X, NodeList[i]->Y);
	printf("%d,%d\n", (int)NodeList[i]->X, (int)NodeList[i]->Y);
  }

#ifndef NO_PNG
  if(PNGDump==1)
    {
      CreateBitmap();
      char FName[50];
      sprintf(FName,"Growth_%d_%d_%u_%d_%d_%lf.png",NumberOfNodes,Distance,GridRowPoints,Seed1,Seed2,DecayGrowth);
      WritePNG(FName,GridRowPoints,GridRowPoints,1,(char **)NANANet);
      DeleteBitmap();
    }
#endif
  delete[] IndexArray;
  delete[] Probabilities;

  Analyze(DecayGrowth,Seed1,Seed2);

  DestroyNodes();

  return 1;
}
int CheckLine(int Y,int minX,int maxX)
{
  Point *Temp=Rows[Y];
  while(Temp != NULL)
    {
      if(Temp->X < minX)
	Temp=Temp->Next;
      else if(Temp->X > maxX)
	return 0;
      else
	return 1;
    }
  return 0;
}
void AddToXList(int *Loc1,int *Loc2,int *Count,int Value)
{
  switch(*Count)
    {
    case 0: // No value in either location
      *Loc1=Value;
      (*Count)++;
      break;
    case 1: // One value
      if(*Loc1 > Value)
	{
	  *Loc2=*Loc1;
	  *Loc1=Value;
	}
      else
	*Loc2=Value;
      (*Count)++;
      break;
    case 2: // Both values occupied....
      if(Value > *Loc1 && Value > *Loc2)
	*Loc2=Value;
      else if(Value < *Loc1 && Value < *Loc2)
	*Loc1=Value;
    }
}
void Adjust(int Index,int *X0,int *X1,int *XC,int Size)
{
  if(Index==0 || Index==Size-1)
    {
      X1[Index]=X0[Index];
      XC[Index]++;
      return;
    }
  if(XC[Index+1]==2)
    {
      if(X0[Index+1] > X0[Index] && X1[Index+1] > X0[Index])
	X1[Index]=X1[Index+1];
      if(X0[Index+1] < X0[Index] && X1[Index+1]< X0[Index])
	{
	  X1[Index]=X0[Index];
	  X0[Index]=X0[Index+1];
	}
      if(X0[Index+1]< X0[Index] && X1[Index+1]> X0[Index])
	{
	  X1[Index]=X0[Index+1];
	  X0[Index]=X0[Index+1];
	}
    }
  else if(XC[Index-1]==2)
    {
      if(X0[Index-1] > X0[Index] && X1[Index-1] > X0[Index])
	X1[Index]=X1[Index-1];
      if(X0[Index-1] < X0[Index] && X1[Index-1]< X0[Index])
	{
	  X1[Index]=X0[Index];
	  X0[Index]=X0[Index-1];
	}
      if(X0[Index-1]< X0[Index] && X1[Index-1]> X0[Index])
	{
	  X1[Index]=X0[Index-1];
	  X0[Index]=X0[Index-1];
	}
    }
  else
    {
      X1[Index]=X0[Index];
    }
  XC[Index]++;

}
int NewCheckRegion(int X,int Y,double Angle)
{
  int startX=0,startY=0;
  int endX,endY;
  startX=X-Distance/2-1;
  startY=Y-Distance/2-1;
  endX=X+Distance/2+1;
  endY=Y+Distance/2+1;
  Point P[4];
  double cosA=cos(Angle),sinA=sin(Angle);
  P[0]=Rotate(startX,startY,X,Y,cosA,sinA);
  P[1]=Rotate(startX,endY,X,Y,cosA,sinA);
  P[2]=Rotate(endX,startY,X,Y,cosA,sinA);
  P[3]=Rotate(endX,endY,X,Y,cosA,sinA);
  int minY=P[0].Y,maxY=P[0].Y;

  /* Make sure that the extremeties are within the borders */
  for(int i=0;i<4;i++)
    if(P[i].X <=0 || P[i].Y <=0 || P[i].X >=(signed)GridRowPoints || P[i].Y >= (signed)GridRowPoints)
	return 1;
  /*  if(P[0].X <=0 || P[1].X <=0 || P[2].X <=0 || P[3].X <=0)
    return 1;
  if(P[0].Y <=0 || P[1].Y <=0 || P[2].Y <=0 || P[3].Y <=0)
    return 1;
  if(P[0].X >=(signed)GridRowPoints || P[1].X >=(signed)GridRowPoints || P[2].X >=(signed)GridRowPoints || P[3].X >=(signed)GridRowPoints)
    return 1;
  if(P[0].Y >=(signed)GridRowPoints || P[1].Y >=(signed)GridRowPoints || P[2].Y >=(signed)GridRowPoints || P[3].Y >=(signed)GridRowPoints)
  return 1;*/

  /* Ok, now compute min and max Y */
  for(int i=1;i<4;i++)
    {
      if(P[i].Y < minY)
	minY=P[i].Y;
      if(P[i].Y > maxY)
	maxY=P[i].Y;
    }
  /* Create X0 and X1 */
  int X0[maxY-minY+1];
  int X1[maxY-minY+1];
  int XCount[maxY-minY+1];
  /* Ok, now compute perimeter points */
  double Exp1=X*(1-cosA)+Y*sinA;
  double Exp2=Y*(1-cosA)-X*sinA;

  double sYX=startY*sinA;
  double sYY=startY*cosA;
  double eYX=endY*sinA;
  double eYY=endY*cosA;

  double sXX=startX*cosA;
  double sXY=startX*sinA;
  double eXX=endX*cosA;
  double eXY=endX*sinA;


  int XRot,YRot;
  for(int y=0;y<=maxY-minY;y++)
    {
      X0[y]=X1[y]=0;
      XCount[y]=0;
    }
  for(int x=startX;x<=endX;x++)
    {
      double Xt=Exp1+x*cosA;
      double Yt=Exp2+x*sinA;

      XRot=(int)(Xt-sYX);
      YRot=(int)(Yt+sYY);
      AddToXList(&X0[YRot-minY],&X1[YRot-minY],&XCount[YRot-minY],XRot);

      XRot=(int)(Xt-eYX);
      YRot=(int)(Yt+eYY);
      AddToXList(&X0[YRot-minY],&X1[YRot-minY],&XCount[YRot-minY],XRot);
    }

  for(int y=startY;y<=endY;y++)
    {
      double Xt=Exp1-y*sinA;
      double Yt=Exp2+y*cosA;

      XRot=(int)(Xt+sXX);
      YRot=(int)(Yt+sXY);
      AddToXList(&X0[YRot-minY],&X1[YRot-minY],&XCount[YRot-minY],XRot);

      XRot=(int)(Xt+eXX);
      YRot=(int)(Yt+eXY);
      AddToXList(&X0[YRot-minY],&X1[YRot-minY],&XCount[YRot-minY],XRot);
    }

  /* Now, the checking code */
  for(int y=minY;y<=maxY;y++)
    {
      if(XCount[y-minY]==1)
	Adjust(y-minY,X0,X1,XCount,maxY-minY);
      if(CheckLine(y,X0[y-minY],X1[y-minY])==1)
	return 1;
    }
  return 0;
}
int CheckRegion(int X,int Y,double Angle)
{
  int startX=0,startY=0;
  int endX=GridRowPoints,endY=GridRowPoints;
  startX=X-Distance;
  startY=Y-Distance;
  endX=X+Distance;
  endY=Y+Distance;
  double DistSq=Distance*Distance;
  double cosA=cos(Angle);
  double sinA=sin(Angle);
  double Exp1=X*(1-cosA)+Y*sinA;
  double Exp2=Y*(1-cosA)-X*sinA;
  for(int y=startY;y<endY+1;y++)
    {
      for(int x=startX;x<endX+1;x++)
        {
	  Point Pt;
	  Pt.Next=NULL;
	  Pt.N=NULL;
	  Pt.T=NULL;
	  Pt.X=(int)(Exp1+x*cosA-y*sinA);
	  Pt.Y=(int)(Exp2+x*sinA+y*cosA);
	  if(Pt.X<=0 || Pt.X>=(signed)GridRowPoints || Pt.Y<=0 || Pt.Y>=(signed)GridRowPoints)
	    return 1;
	  if(Rows[Pt.Y]==NULL)
	    continue;
	  Point *temp=Rows[Pt.Y];
	  while(temp != NULL)
	    {
	      if(temp->X < Pt.X)
		{
		  temp=temp->Next;
		}
	      else if(temp->X > Pt.X)
		break;
	      else if(temp->X == Pt.X)
		{
		  if(temp->N != NULL && ( ((temp->X-X)*(temp->X-X)+(temp->Y-Y)*(temp->Y-Y))<DistSq || abs(temp->X-X)<Distance || abs(temp->Y-Y)<Distance))
		    return 1;
		  temp=NULL;
		}
	    }
	}
    }
  return 0;
}
void FillRegion(int X,int Y,double Angle,Node *Node)
{
  int startX=0,startY=0;
  int endX=GridRowPoints,endY=GridRowPoints;
  if(X>Node->MY_NODE_SIZE/2)//rch18
    startX=X-Node->MY_NODE_SIZE/2;
  if(Y>Node->MY_NODE_SIZE/2)
    startY=Y-Node->MY_NODE_SIZE/2;
  if(GridRowPoints-X > Node->MY_NODE_SIZE/2)
    endX=X+Node->MY_NODE_SIZE/2;
  if(GridRowPoints-Y > Node->MY_NODE_SIZE/2)
    endY=Y+Node->MY_NODE_SIZE/2;
  double cosA=cos(Angle);
  double sinA=sin(Angle);
  double Exp1=X*(1-cosA)+Y*sinA;
  double Exp2=Y*(1-cosA)-X*sinA;

  for(int y=startY;y<endY;y++)
    {
      for(int x=startX;x<endX;x++)
	{
	  if(y>startY+5 && y<endY-5 && x>startX+5 && x < endX-5 )
	    continue;
	  Point *Pt=new Point;
	  Pt->X=(int)(Exp1+x*cosA-y*sinA);
	  Pt->Y=(int)(Exp2+x*sinA+y*cosA);

	  Pt->N=Node;
	  Pt->T=NULL;
	  Pt->Next=NULL;
	  Point *Temp=Find(Pt->X,Pt->Y);
	  //	  printf("\nInserting %d, %d %x",Pt->X,Pt->Y,Temp);
	  //	  fflush(stdout);
	  if(Temp != NULL)
	    if(Temp->N==Node)
	      {
		delete Pt;
		continue;
	      }
	  InsertPoint(Pt);
	}
    }
}

int isInside(double cx, double cy, double sideLength, double angle, Point test)
{
  test.X -= cx;
  test.Y -= cy;
  double mag = sqrt(test.X*test.X + test.Y*test.Y);
  angle = atan2(test.Y,test.X)-angle;
  test.X = mag*cos(-angle);
  test.Y = mag*sin(-angle);
  return (abs(test.X) < sideLength/2) && (abs(test.Y) < sideLength/2);
}

void ComputeTransceiverLocation(Node *N,int X,int Y)
{
  int x=0,y=0;
  double Angle=0;
  double angleStep = 2*M_PI/N->NUM_TRANS;
  double ctNodeSize = N->MY_NODE_SIZE/2;
  for(int i=0;i<N->NUM_TRANS;i++)
    {//hwt FIX THIS

      Angle = i*angleStep;
      x = X + ctNodeSize * cos(Angle);
      y = Y + ctNodeSize * sin(Angle);

      Angle+=N->Angle;
      while(Angle>=2*M_PI)
	Angle-=2*M_PI;
      Point TP=Rotate(x,y,X,Y,cos(N->Angle),sin(N->Angle));
      // if(isInside(N->X, N->Y, N->MY_NODE_SIZE, N->Angle, TP))
      //	printf("INSIDER: %i \n", i);
      double xdir = TP.X - N->X;
      double ydir = TP.Y - N->Y;
      xdir = (xdir==0)?0:(xdir>0?1:-1);
      ydir = (ydir == 0)?0:(ydir>0?1:-1);

      while( isInside(N->X, N->Y, N->MY_NODE_SIZE, N->Angle, TP) )
	{
	  if(xdir>0)
	    {
	      TP.X+=1;
	    }
	  else
	    {
	      TP.X-=1;
	    }
	  if(ydir>0)
	    {
	      TP.Y+=1;
	    }
	  else
	    {
	      TP.Y-=1;
	    }
	}


      Track *NewTrack=N->Xcvr[i];
      NewTrack->X=TP.X;
      NewTrack->Y=TP.Y;
      NewTrack->Angle=Angle;
      NewTrack->NoGrow=0;
      NewTrack->ID=i;
      NewTrack->ConnectedXcvrs=NULL;
      NewTrack->N=N;
      Link *NewLink=new Link;
      NewLink->X=TP.X;
      NewLink->Y=TP.Y;
      NewLink->Angle=Angle;
      NewLink->Next=NULL;
      NewLink->T=NewTrack;
      NewTrack->Start=NewLink;
      NewTrack->Last=NewLink;

      // Now, a point corresponding to X and Y must already exist
      // Confirm that, and once it is confirmed, put ina  link to the track for the point
      Point *P=Find(TP.X,TP.Y);
      if(P==NULL)
	{
	  // Try perturbing TP.X and TP.Y +1 or -1 to see if we find anything
	  // Ok, perturn TP.X and TP.Y by 1 (+ or -) and confirm that we find a point on the node...
	  /* rch18  if(Find(TP.X+1,TP.Y)==NULL && Find(TP.X-1,TP.Y) == NULL && Find(TP.X,TP.Y-1) == NULL && Find(TP.X,TP.Y+1)==NULL)
	    {
	      // Something went wrong !
	      printf("\nTP.X: %d, TP.Y: %d, Angle: %lf",TP.X,TP.Y,Angle);
	      assert(P!=NULL);
	      }
	      else*/
	  {
	    // printf("Failed point on T: %i", i);
	    P=new Point;
	    P->Next=NULL;
	    P->X=TP.X;
	    P->Y=TP.Y;
	    P->N=NULL;
	    P->T=NULL;
	    InsertPoint(P);
	  }
	}

      // yang - Make sure MinDist is not less than any NodeSize, otherwise there will be an assertion error here.
      if(P->T != NULL)
	assert(P->T==NULL);

      P->T=NewTrack;
    }

}
/* Add a pointer to the new track, to the list of tracks being stored by the old track */
void AddToTrackList(Track *New,Track *Old)
{
  /* First, some simple checks */
  if(New==NULL || Old==NULL)
    {
      assert(New != NULL);
      assert(Old != NULL);
    }
  TrackList *NewList=new TrackList;
  NewList->Next=NULL;
  NewList->T=New;
  if(Old->ConnectedXcvrs==NULL)
    {
      Old->ConnectedXcvrs=NewList;
      return;
    }
  TrackList *Temp=Old->ConnectedXcvrs;
  while(Temp->Next != NULL)
    {
      Temp=Temp->Next;
      if(Temp->T->ID==New->ID && Temp->T->N==New->N)
	{
	  delete NewList;
	  return;
	}
    }
  if(Temp->T->ID==New->ID && Temp->T->N==New->N)
    {
      delete NewList;
      return;
    }

  Temp->Next=NewList;

}
Point *Find(int X, int Y)
{
  if(Y<0 || Y >= (signed)GridRowPoints)
    return NULL;

  Point *Temp=Rows[Y];
  if(Temp==NULL)
    return NULL;
  if(Temp->X==X)
    return Temp;
  while(Temp->Next != NULL)
    {
      if(Temp->X==X)
	return Temp;
      Temp=Temp->Next;
    }
  if(Temp->X==X)
    return Temp;

  return NULL;
}
void InsertPoint(Point *P)
{
  int X=P->X;
  int Y=P->Y;
  // yang - check whether the value of Y will cause array access overflow
//  if(Y>=GridRowPoints)
//  	{
//  		return;
//  	}
  Point *Temp=Rows[Y];
  //  printf("\nInserting %d,%d",X,Y);
  if(Temp==NULL)
    {
      Rows[Y]=P;
      return;
    }
  if(Temp->X > P->X)
    { // We have only one 'X' value, and its higher than the one we're inserting...
      P->Next=Temp;
      Rows[Y]=P;
      return;
    }
  while(Temp->Next != NULL)
    {
      if(Temp->Next->X < X)
	Temp=Temp->Next;
      else
	{
	  if(Temp->Next->X==X)
	    { // something went wrong....the point already exists and we're trying to re-insert it !
	      printf("\nFATAL: Tried to insert a point that already exists...");
	      assert(Temp->Next->X != X);
	    }
	  break;
	}
    }
  P->Next=Temp->Next;
  Temp->Next=P;
}
int PointNearLinkOnTrack(Point *P,Link *L,int LinkDistance)
{
  Track *T=L->T;
  Link *Temp=T->Start;
  if(Temp==L)
    {
      //The first link on the track is the current link...do the check !
      if(P->X==(int)Temp->X && P->Y==(int)Temp->Y)
	return 1;
      return 0;
    }
  // Ok, this is not the first link on track....
  while(Temp->Next != L)
    Temp=Temp->Next;

  // Now, Temp points to the entry before 'L'...verify that !
  assert(Temp->Next==L);

  // Now, check !
  if(P->X==(int)Temp->X && P->Y==(int)Temp->Y)
    return 1;
  else if(LinkDistance>1) // we still have some distance to cover !
    return PointNearLinkOnTrack(P,Temp,LinkDistance-1);

  return 0;
}
void CreateNewPoint(Link *L,Track *T)
{
  /* First, see if the point already exists */
  int X=(int)L->X,Y=(int)L->Y;
  Point *P=Find(X,Y);
  /* Check the neighborhood of the point (+/- 1) */
  /* Make sure that if the neighborhood is occupied, its not the same track
     if it is, ignore it, allow growth, else stop growth */
  if(P==NULL)
    {
      Point *P1;
      for(int dY=-1;dY<=1;dY++)
	{
	  for(int dX=-1;dX<=1;dX++)
	    {
	      P1=Find(X+dX,Y+dY);
	      if(P1 != NULL)
		{
		  if(PointNearLinkOnTrack(P1,L,5)==1)
		    continue;
		  if(P1->T != NULL)
		    { // Other tracks overlap the point
		      AddToTrackList(T,P1->T);
		      T->NoGrow=1;
		    }
		  else
		    {
		      P1->T=T;
		    }
		  //		  T->NoGrow=1;
		  break;
		}
	    }
	}
    }

  if(P==NULL)
    { // Point does not exist
      /* Now, to be a little more rigorous in our testing...*/
      /* Create New Point */
      P=new Point;
      P->Next=NULL;
      P->T=T;
      P->N=NULL;
      P->X=X;
      P->Y=Y;
      InsertPoint(P);
    }
  else
    { // Point exists
      /* Since point exists, we must add the current track to the list of
	 tracks/nodes that overlap this point.
      */
      if(P->T != NULL)
	{ // Other tracks overlap the point
	  AddToTrackList(T,P->T);
	}
      else
	P->T=T;

      /* Since point exists, we must stop growth as well */
      T->NoGrow=1;
    }

}
int FindClosestNode(unsigned X,unsigned Y, int Index,double Angle)
{
  double DiagonalSq=2*Distance*Distance;
  double SideSq=Distance*Distance;
  double DistSq=0;
  int RetVal=0;
  double T1=X*X+Y*Y;
  double T3=X*2.0;
  double T4=Y*2.0;
  //rch18 This is where the node size is no longer the maximum
  if(X<MIN_NODE_SIZE/2 || Y<MIN_NODE_SIZE/2 || X>(GridRowPoints-MIN_NODE_SIZE/2) || Y>(GridRowPoints-MIN_NODE_SIZE/2))
   return -1;
  //rch18
  if(X<NODE_SIZE/sqrt(2) || Y<NODE_SIZE/sqrt(2) || X>(GridRowPoints-NODE_SIZE/sqrt(2)) || Y>(GridRowPoints-NODE_SIZE/sqrt(2)))
   RetVal=1;

  for(int i=0;i<Index;i++)
    {
      DistSq=T1+NodeList[i]->X*NodeList[i]->X+NodeList[i]->Y*NodeList[i]->Y-T3*NodeList[i]->X-T4*NodeList[i]->Y;
      if(DistSq < SideSq)
	return -1;
      if(RetVal==0)
      if(DistSq < DiagonalSq)
	RetVal=1;
      //      if(DistSq < DiagonalSq)
      //	return 1;
    }
  return RetVal;
}
unsigned long long CreateNodes(int PlacementControl,int OrientationControl)
{ //rch18: This should be interesting...
  int NodeCount=0;
  if(PlacementControl==0)
    fprintf(stderr,"Distributing %d Nodes uniformly on %Ld Points\n",NumberOfNodes,(long long)(GridRowPoints)*(long long)GridRowPoints);
  else
    fprintf(stderr,"Distributing %d Nodes in a grid on %Ld Points\n",NumberOfNodes,(long long)(GridRowPoints)*(long long)GridRowPoints);

  //  struct timeval Start,End;
  unsigned long long CheckReg=0;
  double SideCount=sqrt(NumberOfNodes);
  //rch18
  int NodeSize = 0;
  for(int i=0;i<NumberOfNodes;i++)
    {
      NodeSize=(NodeList[i]->MY_NODE_SIZE*sqrt(2)>Distance)?((int)(NodeList[i]->MY_NODE_SIZE*sqrt(2))+2):Distance;
      int Done=0;
      long long X,Y;
      double Angle=0;
      long long Attempts=0;
      NodeList[i]->ID=i;
      int TestRegion=0;
      while(Done==0)
	{
	  if(OrientationControl==0)
	    { // If we dont have control over node orientation, generate a random orientation for the node
	      Angle=(360*R1->Random());
	      Angle=ToRad(Angle);
	    }
	  if(PlacementControl==0)
	    {
	      X=(long long)(GridRowPoints*R1->Random());
	      Y=(long long)(GridRowPoints*R1->Random());
	      int ValidPoint=FindClosestNode(X,Y,i,Angle);
	      switch(ValidPoint)
		{
		case 0: TestRegion=0;
		  break;
		case 1:
		  TestRegion=NewCheckRegion(X,Y,Angle);
		  break;
		default:
		  TestRegion=1;
		}
	    }
	  else
	    { // If we have placement control, each node has a pre-defined location
	      X=(long long)((i % (int)(SideCount))*(NodeSize))+NodeSize/2;
	      Y=(long long)((i / (int)(SideCount))*NodeSize)+NodeSize/2;
	      TestRegion=0;
	      // We don't need to check region occupancy, since each node has a pre-defined location !
	    }
	  if(TestRegion==0)
	    {
	      NodeList[i]->X=X;
	      NodeList[i]->Y=Y;
	      NodeList[i]->Angle=Angle;
	      FillRegion(X,Y,Angle,NodeList[i]);
	      ComputeTransceiverLocation(NodeList[i],X,Y);
      	      Done=1;
	      NodeCount++;
	      if(NodeCount % 100==0)
		fprintf(stderr,"\nPlaced %d Nodes !",NodeCount);
	    }
	  if(Attempts < 1000000)
	    {
	      Attempts++;
	      if(Attempts % 10000==0)
		fprintf(stderr,"\nCould not place in %d Node in %Ld attempts....will keep trying",i,Attempts);
	    }
	  else
	    {
	      fprintf(stderr,"\nCould not place node in %Ld attempts\n%d Nodes placed",Attempts,i-1);
	      i=NumberOfNodes;
	      break;
	    }
	}
    }
  NumberOfNodes=NodeCount;
  return CheckReg;
}
void ReadCDF(char *FName)
{
  int Count=0;
  FILE *fp=fopen(FName,"r");
  double Temp;
  int IndexIndex=0;
  IndexArray=new int[Size];
  if(fp==NULL)
    {
      printf("\nCould not open %s",FName);
      exit(0);
    }
  while(!feof(fp))
    {
      fscanf(fp,"%lf,%lf,%lf\n",&Temp,&Temp,&Temp);
      Count++;
    }
  //printf("\nFound %d Entries in %s\n",Count,FName);
  Probabilities=new RTList[Count];
  rewind(fp);
  Count=0;
  while(!feof(fp))
    {
      fscanf(fp,"%lf,%lf,%lf\n",&Probabilities[Count].R,&Probabilities[Count].Theta,&Probabilities[Count].Prob);
      if(Probabilities[Count].Prob*Size > IndexIndex)
	{
	  IndexArray[IndexIndex]=Count;
	  IndexIndex++;
	}
      Count++;
    }
  fclose(fp);
  CDF_Count=Count;

}
void Grow(double D,int IControl)
{
  int GrowFlag=-1;
  double DecayFactor=0;
  int MaxLength=10;
  for(GrowthStep=0;GrowthStep<5000;GrowthStep++)
    { // Growth Steps
      /* PNG File Dump
	 if(D!=0) // Do we want to dump png files per growth step ?
	 {
	 char FN[35];
	 sprintf(FN,"IMG/IMG%03Ld.png",GrowthStep);
	 CreateBitmap();
	 if(WritePNG(FN,GridRowPoints,GridRowPoints,1,(char **)NANANet)==-1)
	 {
	 printf("\nError Encountered while writing PNG file");
	 exit(0);
	 }
	 DeleteBitmap();
	 }
       End PNG File Dump */
      /* Main Growth Loops... */
      double R,Theta;
      DecayFactor+=D;
      if(GrowFlag==0 || DecayFactor >= 1.00) // This can only happen if no link grew in the last iteration
	{
	  fprintf(stderr,"\nGrowth terminated after %d steps",GrowthStep);
	  return;
	}
      //      fprintf(stderr,"\nGrowth Step :%Ld: %d",GrowthStep,GrowFlag);
      GrowFlag=0;
      for(int n=0;n<NumberOfNodes;n++)
	{ // Per Node
	  for(int T=0;T<NodeList[n]->NUM_TRANS;T++)
	    { // Per transceiver
	      double Rnd=R2->Random();
	      // We are decaying the rate of growth....limit the max value of R by D% per growth step
	      Rnd*=(1-DecayFactor);

	      /* If this transceiver link is done growing...skip the growth procedure.. */
	      if(NodeList[n]->Xcvr[T]->NoGrow==1)
		continue;

	      if(IControl==0)
		{
		  /* Get R and Theta value based on generated random number */
		  GetParameters(&R,&Theta,Rnd,CDF_Count);
		}
	      else
		{
		  R=MaxLength*Rnd;
		  Theta=0;
		}
	      Track *CurrTrack=NodeList[n]->Xcvr[T];
	      /* Get co-ordinates of point, and growth orientation from last point on track */
	      double X=CurrTrack->Last->X;
	      double Y=CurrTrack->Last->Y;
	      int pX=(int)X,pY=(int)Y;
	      double Angle=CurrTrack->Last->Angle+Theta;
	      GrowFlag++;
	      double cosA=cos(Angle);
	      double sinA=sin(Angle);
	      /* For every unit distance in R, we add a point to the track */
	      for(int i=1;i<=ceil(R);i++)
		{
		  Link *NewLink=new Link;
		  NewLink->X=(X+i*cosA);
		  NewLink->Y=(Y+i*sinA);
		  NewLink->Angle=Angle;
		  NewLink->Next=NULL;
		  NewLink->T=CurrTrack;
		  CurrTrack->Last->Next=NewLink;
		  CurrTrack->Last=NewLink;
		  if(NewLink->X < 0 || NewLink->X >= (signed)GridRowPoints || NewLink->Y <0 || NewLink->Y >= (signed)GridRowPoints)
		    {
		      CurrTrack->NoGrow=1;
		      break;
		    }
		  else
		    {
		      if(pX!=(int)NewLink->X || pY !=(int)NewLink->Y)
			CreateNewPoint(NewLink,CurrTrack);
		    }
		  if(CurrTrack->NoGrow==1 && GrowthStep < 2)
		    {
		      //		      printf("\nNo Grow Set at sub-step %d, (N:%d, T:%d)",i,n,T);
		      CurrTrack->NoGrow=0;
		      //		      break;
		    }
		  pX=(int)NewLink->X;
		  pY=(int)NewLink->Y;
		  if(CurrTrack->NoGrow==1)
		    break;
		}
	    }
	}
    }
}
