#ifndef NODE_H
#define NODE_H

//#include "PhysicalModel.hh"

typedef struct Track;
/* The GraphXcvr struct is used to keep track of which
   Node/Transceiver pairs are connected to the link
*/
class GraphXcvr
{
public: 
  int NodeID,LinkID;
  unsigned Counter;
  GraphXcvr *Next;
};

class Node
{
public: 
  Node();
  Node(int, int);
  int ID;
  double X;
  double Y;
  double Angle;
  Track** Xcvr;
  int NUM_TRANS;
  int MY_NODE_SIZE; //Supposed to replace NODE_SIZE
  //Graph Node
  int GID; //ID of the node
  int Links; // How many links are connected somewhere
  int Visited;
  // const static int NUM_TRANS = 4;
  GraphXcvr **TransceiverLinks; // Pointers to the links... hwt
  int ClusterID;
  int ClusterCount;
  int VisitedCluster;
  //This is an idealistic design ideas
 
  //const static int NUMBER_TRANS = 4;
};

class NodeA : public Node
{
public:
  NodeA()
  {
    //    this();
  }
  NodeA(int size, int trans) : Node(size, trans)
  {
    // this(size, trans);
  }
  //const static int MY_NODE_SIZE = 250;
};

/*
class GraphNode
{
public:
  int ID; //ID of the node
  int Links; // How many links are connected somewhere
  int Visited;
  const static int NUM_TRANS = 4;
  GraphXcvr *TransceiverLinks[NUM_TRANS]; // Pointers to the links... hwt
  int ClusterID;
  int ClusterCount;
  int VisitedCluster;
};
*/

#endif
