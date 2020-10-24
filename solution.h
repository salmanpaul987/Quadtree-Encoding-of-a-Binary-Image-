/* Defines key values and gives them a name for easy reference in the code. 
Also in the future makes it easy to change the value throughtout the code.
*/
#define MinImageWidth   1
#define Black   0
#define White   1
#define Mixed   2

// Structure
/*The structure for the node on the quadtree. Contains the coordinates, width,
value (if the node is black, white or mixed), pointers to the four child nodes which also have the same structure,
and the pixel array contained within the node.*/

struct Treenode
{
	struct Treenode* Child[4];
    int nodeValue;
	int Width;
    int positionX;
    int positionY;
    int** PixelArray;
};

//Defining functions used in the code.

void ReadFile ( char *,struct Treenode *);
void createNode(struct Treenode* ,int , int , int , int , int**);
int PowerOfTwo (int);
void BuildQuadTree(struct Treenode *);
int CheckColour (struct Treenode *, int , int , int , int);
int** createTemporaryArray (struct Treenode *, int , int , int , int , int);

