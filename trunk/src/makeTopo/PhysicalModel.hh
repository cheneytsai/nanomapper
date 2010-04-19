#ifndef PM_HH
#define PM_HH
#include "Node.hh"

// PhysicalMode.hh
// December 15, 2004
// This file will hold class, structure and function definitions 
// for the simulation of the self-assembly process 
/* Struct Link:
   This is a linked list of points that represent a DNA tube
   The X,Y co-ordinates represent the point occupied by this link in space
   the Angle represents the orientation of the link in space wrt XXX
*/
typedef class  Node;
typedef struct Link;
typedef struct TrackList;
typedef struct Track
{
  double X,Y; // Starting point of Growth
  double Angle; // Initial Growth Angle
  char NoGrow,ID; // Are we still growing ? what is the id of this link ?
  Link *Last; // Pointer to end of link point chain
  Link *Start; // Pointer to start of link point chain
  TrackList *ConnectedXcvrs; // What transceivers are connected to this Track ?
  Node *N; // Pointer to parent node
};
typedef struct Link
{
  double X,Y; // Co-ordinate of current link point
  double Angle; // Growth orientation from current link point
  Link *Next; // Pointer to next link point
  Track *T; // Pointer to Track that this link point belongs to..
};
/* Struct Node:
   This is the representation of a Node
   The points in the node can be computed simply using the center (X,Y) and
   the orientation of the Node
   The node also holds four "link" structs corresponding to each transceiver
*/


/*typedef struct Node
{
  int ID;
  double X,Y,Angle;
  Track Xcvr[4]; //hwt 
};
*/
/* struct TrackList: Auxiliary structure used to hold a list of links
   Used by a link to keep track of other links it is connected to
   When an intersection with another link is discovered, an entry is added to this structure
*/
typedef struct TrackList
{
  Track *T;
  TrackList *Next;
};
typedef struct RTList
{
  double R,Theta,Prob;
};
/* struct Point
   This struct represents a point on the large space grid
   Each point has an X and Y co-ordinate
   It also has a Node owner or a Link owner. There is at least one
   link or node owner, there can be multiple link owners, but not more than 
   one node owner. There can also be one node owner and one or more link owners
*/
typedef struct Point
{
  int X,Y;
  Node *N;
  Track *T;
  Point *Next;
};

//int AngleMatrix[19][21];
//Point **Rows;
unsigned long long CreateNodes(int,int);
int CheckRegion();
void MarkRegion();
void InsertPoint(Point *P);

void Grow(double,int);
void ReadCDF(char *);
Track* AddLinkToNode(int,int,Track*,int);
Track* FindLink(int,int,Track*,int);
int WritePNG(char *FName,int H,int W,int BPP, char **Buff);
void AddLink(TrackList *,TrackList *);
void CreateNewPoint(Link *L, Track *T);
Point *Find(int,int);
void MoveLink(GraphXcvr *,GraphXcvr *);
void VisitNeighbors(Node *,int,int);

#endif
