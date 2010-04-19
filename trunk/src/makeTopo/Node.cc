#include "Node.hh"
#include "PhysicalModel.hh"

Node::Node()
{ //hwt
  MY_NODE_SIZE = 250;
  NUM_TRANS = 10;
  Xcvr = new Track*[NUM_TRANS]();
  TransceiverLinks = new GraphXcvr*[NUM_TRANS]();
  for(int i= 0; i<NUM_TRANS; i++)
   {
     Xcvr[i] = new Track();
     TransceiverLinks[i] = new GraphXcvr();
   }
}

Node::Node(int size, int trans)
{
  // this();
  MY_NODE_SIZE = size;
  NUM_TRANS = trans;
  Xcvr = new Track*[NUM_TRANS]();
  TransceiverLinks = new GraphXcvr*[NUM_TRANS]();
  for(int i= 0; i<NUM_TRANS; i++)
   {
     Xcvr[i] = new Track();
     TransceiverLinks[i] = new GraphXcvr();
   }
}
/*
NodeA::NodeA()
{
  //  MY_NODE_SIZE = 150;
  //NUM_TRANS = 6;
  Xcvr = new Track*[NUM_TRANS]();
  TransceiverLinks = new GraphXcvr*[NUM_TRANS]();
  for(int i= 0; i<NUM_TRANS; i++)
    {
      Xcvr[i] = new Track();
      TransceiverLinks[i] = new GraphXcvr();
    }
}*/
