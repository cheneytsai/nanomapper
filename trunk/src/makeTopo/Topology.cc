#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "Topology.hh"
/* PrintNode:
   Input: Pointer to a Node 
   Output: Prints contents of a NodeRecord
*/
void PrintNode(NodeRecord *N)
{
  if(N==NULL)
    {
      fprintf(stderr,"\nPrintNode received NULL pointer");
      return;
    }
  printf("\nNode %d @ (%d,%d)",N->ID,N->X,N->Y);
  for(int i=0;i<N->NUM_TRANS;i++)
    printf("\nTransceiver %d Connected to Node %d, Transceiver %d",i,N->Xcvr[i]->NodeID,N->Xcvr[i]->TransceiverID);
}
/* parseLine:
   Input: (1) Pointer to string that will be parsed 
          (2) Pointer to NodeRecord that will be filled
   Output: None
*/
void parseLine(char *Line,NodeRecord *N)
{
  if(Line==NULL)
    {
      printf("\nparseLine received NULL string");
      return;
    }
  char *Index=index(Line,'>');
  if(Index==NULL)
    return;
  Index++;
  sscanf(Index,"%d",&N->ID);
  Index=index(Index,',');
  Index++;
  sscanf(Index,"%d",&N->X);
  Index=index(Index,',');
  Index++;
  sscanf(Index,"%d",&N->Y);
  for(int j=0;j<N->NUM_TRANS;j++)
    {
      Index=index(Index,',');
      Index++;
      sscanf(Index,"%d.%d",&N->Xcvr[j]->NodeID,&N->Xcvr[j]->TransceiverID);
    }
}
/* Read:
   Input:    (1) Name of the file to be read
   (Output): (2) Pointer to variable holding number of nodes
   (Output): (3) Pointer to NodeRecord
   Return Value: -1 if unsuccessful, else 0
*/
int Read(char *Filename, int *NumNodes,NodeRecord **Nds)
{
  FILE *fp=fopen(Filename,"r");
  if(fp==NULL)
    {
      fprintf(stderr,"\nCannot open %s for reading",Filename);
      return -1;
    }
  fscanf(fp,"%d\n",NumNodes);
  //  fprintf(stderr,"File Has %d Nodes",*NumNodes);
  int Count=0;
  //  NodeRecord Nodes[*NumNodes];
  NodeRecord *Nodes = new NodeRecord[*NumNodes];
  char Line[100];
  while(!feof(fp) && Count < *NumNodes)
    {
      fgets(Line,100,fp);
      //      fprintf(stderr,"%d: %s",Count,Line);
      parseLine(Line,&Nodes[Count++]);
    }
  fclose(fp);
  if(Count != *NumNodes)
    {
      fprintf(stderr,"\nFile did not have %d Nodes. Read %d Nodes",*NumNodes,Count);
      return -1;
    }
  *Nds=Nodes;
  return 0;
}
/* Write:
   Input:
   (1) Name of file to be written to
   (2) Number of records to  be written to file
   (3) Pointer to records to be written to file
   Output: -1 if write is unsuccessful, else 0
*/
int Write(char *Filename, int NumNodes, NodeRecord *Nodes)
{
  FILE *fp=fopen(Filename,"w");
  if(fp==NULL)
    {
      fprintf(stderr,"\nCannot open %s for writing",Filename);
      return -1;
    } 
  fprintf(fp,"%d\n",NumNodes);
  for(int i=0;i<NumNodes;i++)
    {
      fprintf(fp,"<Nodes>%d,%d,%d",Nodes[i].ID,Nodes[i].X,Nodes[i].Y);
      for(int j=0;j<Nodes[i].NUM_TRANS;j++)
	fprintf(fp,",%d.%d",Nodes[i].Xcvr[j]->NodeID,Nodes[i].Xcvr[j]->TransceiverID);
      fprintf(fp,"</Node>\n");
    }
  fclose(fp);
  return 1;
}
