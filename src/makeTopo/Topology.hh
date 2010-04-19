/* */
enum directions {Left,Right,Top,Down};

typedef struct Transceiver
{
  int NodeID;
  int TransceiverID;
};
typedef struct NodeRecord
{
  int ID,X,Y;
  int NUM_TRANS;
  Transceiver** Xcvr; //hwt
};
#define REVERSE_DIRECTION(DIRECTION) (DIRECTION ^ 0x1)
int Read(char *,int *,NodeRecord **);
int Write(char *,int,NodeRecord *);
void parseLine(char *,NodeRecord *);
void PrintNode(NodeRecord *);
