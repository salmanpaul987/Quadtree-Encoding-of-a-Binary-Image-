
/* Header files that include basic functions for the program to run.
Here solution.h is the user defined header file which include the function declarations ,
the Tree node structure and the constant variables which are further used inside the code*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "solution.h"

/* The main function is the designated start of the program which calls all the following functions in the program. 
The main is passed the file which is identified when the program is called.
*/
int main ( int argc, char *argv [] )
{
    struct Treenode rootNode;
    if ( argc != 2)
    {
        puts ("Not enough arguments. Error with input file. Program exit");
        exit(-1);
    }
    
    ReadFile( argv[1], &rootNode);
    BuildQuadTree(&rootNode);
}

// createNode
/* This function is used to set information passed to the function on the node that is passed.
The first parameter is the node on which the information will be set on.
The XPosition, YPosition, Width, Value is copied to the passed Treenode. Next the four child nodes memory is allocated. 
The last parameter Pixels values are assigned to a 2d PixelArray in the given newNode .*/

void createNode(struct Treenode* newNode, int XPosition, int YPosition, int Width, int Value, int** Pixels)
{   
    newNode->positionX = XPosition;
    newNode->positionY = YPosition;
    newNode->Width = Width;
    newNode->nodeValue = Value;
    
    for (int k = 0; k < 4; k++)
        newNode->Child[k] = (struct Treenode*) malloc(sizeof(struct Treenode));
    
    newNode->PixelArray = (int**) malloc(Width*sizeof(int*));
	int i = 0;
	while(i < Width)
	{
	    int m = 0;
		newNode->PixelArray[i] = (int*) malloc(Width*sizeof(int));
		while(m < Width)
		{	
			newNode->PixelArray[i][m] = Pixels[i][m];
			m++;
		}
		i++;
	}
}

// ReadFile
/* This is the first function called by main(). 
This function is passed the input file from main.
The function starts by reading the file, make a 2D Array of image if the information is correct and than fill Pixels
according to the data provided. At the end of the function the array and buffers are freed and the file is closed.
The function makes multiples checks on the information provided to make sure the information is within boundries.*/

void ReadFile ( char *file_name, struct Treenode* node ){
    
    int countedNumberofBlackPixels = 1, Counter=0, a, givenImageWidth, XValue, YValue, b, val, PowerofTwo, numofBlackPixels;
    long  fileReadSize, fileSize;
    int** OriginalArray;
    char *buffer = NULL;
    FILE *fp;
    
    fp = fopen(file_name,"r");
    
    if (fp == NULL){
        puts("Error reading file. Program exit");
        exit (-1);
    }
    if (fp)
    {
        fseek(fp , 0 , SEEK_END);
        fileSize = ftell(fp);
        rewind(fp);
        buffer = (char*) malloc (sizeof(char) * (fileSize + 1) );
        fileReadSize = fread(buffer, sizeof(char), fileSize, fp);
        buffer[fileSize] = '\0';
        if(buffer[fileSize - 1] == '\n')
        {
            buffer[fileSize - 1] = '\0';
        }
        if (fileSize != fileReadSize) {
            free(buffer);
            buffer = NULL;
            puts("File not Read Properly.Error storing file. Program exit");
            exit(-1);
        }
    }
    char *str = buffer;
	char *p = str;
    while (*p) {
        if (*p == '\n') {
            countedNumberofBlackPixels++;
            p++;
        }
        else if (isdigit(*p)) {
			/*strtol() function
			Parses the C-string given as 1st argument interpreting its content as an integral number of the specified basegiven in 3rd argument,
			which is returned as a long int value. If 2nd argument is not a null pointer, the function also sets the value of 2nd argument to point to the first character after the number.*/
            val = strtol(p, &p, 10);
            if (Counter == 0) {
                givenImageWidth = val;
                //Calling to function to check whether the width given is a power of two or not
                PowerofTwo = PowerOfTwo(givenImageWidth);
                if (PowerofTwo != 1){
                    puts("The image width is not a power of 2. Program exit");
                    exit (-1);
                }
                if (givenImageWidth < MinImageWidth) {
                    puts("The image is too small: it is less than the minimum image width. Program exit");
                    exit (-1);
                }
                Counter++;
                OriginalArray = (int**) malloc(givenImageWidth * sizeof(int*));
				//Initializing the Image Array With White Pixels 
				int a1 = 0;
                while(a1 < givenImageWidth) {
                    OriginalArray[a1] = (int*) malloc(givenImageWidth * sizeof(int));
					int a2 = 0;
                    while (a2 < givenImageWidth)
                    {
                        OriginalArray[a1][a2] = White;
						a2++;
                    }
					a1++;
                }
            }
            else if (Counter == 1) {
                numofBlackPixels = val;
                if (numofBlackPixels > (givenImageWidth * givenImageWidth)) {
                    puts("There are more black pixels than the size of the image. Program exit");
                    exit (-1);
                }
                if (numofBlackPixels == (givenImageWidth * givenImageWidth)) {
                    puts("The image is only black pixels");
                    exit (-1);
                }
                if (numofBlackPixels == 0) {
                    puts("The image is only white pixels");
                    exit (-1);
                }
                if (numofBlackPixels < 0) {
                    puts("There are less than 0 black pixels. Program exit");
                    exit (-1);
                }
                Counter++;
            }
            else if (Counter%2 == 0) {
                XValue = val;
                Counter++;
            }
            else {
                YValue=val;
                if (XValue < 0 || XValue >= givenImageWidth || YValue < 0 || YValue >= givenImageWidth) {
                    printf("Coordinate (%i,%i) is outside of the bounds of the image width. Program exit\n", XValue, YValue);
                    exit (-1);
                }
                OriginalArray[XValue][YValue] = Black;
                Counter++;
            }
        }
        else {
            p++;
        }
    }
    countedNumberofBlackPixels = countedNumberofBlackPixels - 2;
    if (numofBlackPixels != countedNumberofBlackPixels) {
        puts("The given number of black pixels is not the same as the counted number of black pixels. Program exit ");
        exit (0);
    }
    b = givenImageWidth - 1;
    createNode(node, 0, b, givenImageWidth, 2, OriginalArray);
    free(buffer);
	a = 0;
    while (a < givenImageWidth) {
        free(OriginalArray[a]);
		a++;
    }
    free(OriginalArray);
    fclose(fp);
    return;
}

// PowerOfTwo
//The power of two function is used to check if width which is passed to the function is a power of two.
int PowerOfTwo (int width)
{
    while (((width % 2) == 0) && width > 1) /* While width is even and > 1 */
        width /= 2;
    return width;
}

// BuildQuadTree
/* Called recursively
This functions only parameter is node being passed to it.
The main part of the function is the while loop which switches between 4 cases; each case is for a different quadrant of the pixel array.
The case calls the CheckColour function to find out if that quadrant is Black, White or Mixed and copies that to Value.
If value is mixed the BuildQuadTree function is called recursively, this time with the child of the node as the root node. 
*/
void BuildQuadTree(struct Treenode *node)
{
    int  mid, uppermid, edge, previousXPosition, previousYPosition, Xposition, Yposition, Value, x;
    int** TempArray;
    previousXPosition = node->positionX;
    previousYPosition = node->positionY;
    uppermid = ((node->Width)/2);
    mid = uppermid-1;
    edge = node->Width-1;
	int check = 0;
    while(check < 4)
    {
        switch(check){
            case 0: //NW
                Value= CheckColour(node, 0, mid, 0, mid);
                TempArray = createTemporaryArray(node, 0, mid, 0, mid, uppermid);
                Xposition = previousXPosition;
                Yposition = previousYPosition - uppermid;
                createNode(node->Child[0], Xposition, Yposition, uppermid, Value, TempArray);
                x = 0;
				while(x < uppermid)
				{
					free(TempArray[x]);
					x++;
				}
                free(TempArray);
                if (Value == Black)
                    printf("Position (%i,%i), Width %i, Black Terminal\n", Xposition, Yposition, uppermid);
                if (Value == Mixed)
                    BuildQuadTree(node->Child[0]);
                break;
                
            case 1: // NE
                Value= CheckColour(node, 0, mid, uppermid, edge);
                TempArray = createTemporaryArray(node, 0, mid, uppermid, edge, uppermid);
                Xposition = previousXPosition + uppermid;
                Yposition = previousYPosition - uppermid;
                createNode(node->Child[1], Xposition , Yposition , uppermid, Value, TempArray);
				x = 0;
				while(x < uppermid)
				{
					free(TempArray[x]);
					x++;
				}
                free(TempArray);
                if (Value == Black)
                    printf("Position (%i,%i), Width %i, is all black\n", Xposition, Yposition, uppermid);
                if (Value == Mixed)
                    BuildQuadTree(node->Child[1]);
                break;
                
            case 2: //SW
                Value= CheckColour(node, uppermid, edge, 0, mid);
                TempArray = createTemporaryArray(node, uppermid, edge, 0, mid, uppermid);
                Xposition = previousXPosition;
                Yposition = previousYPosition;
                createNode(node->Child[2], Xposition, Yposition, uppermid, Value, TempArray);
                x = 0;
				while(x < uppermid)
				{
					free(TempArray[x]);
					x++;
				}
                free(TempArray);
                if (Value == Black)
                    printf("Position (%i,%i), Width %i, is all black\n", Xposition, Yposition, uppermid);
                if (Value == Mixed)
                    BuildQuadTree(node->Child[2]);
                break;
                
            case 3: //SE
                Value= CheckColour(node, uppermid, edge, uppermid, edge);
                TempArray = createTemporaryArray(node, uppermid, edge, uppermid, edge, uppermid);
                Xposition = previousXPosition + uppermid;
                Yposition = previousYPosition;
                createNode(node->Child[3], Xposition, Yposition, uppermid, Value, TempArray);
                x = 0;
				while(x < uppermid)
				{
					free(TempArray[x]);
					x++;
				}
                free(TempArray);
                if (Value == Black)
                    printf("Position (%i,%i), Width %i, is all black\n", Xposition, Yposition, uppermid);
                if (Value == Mixed)
                    BuildQuadTree(node->Child[3]);
                break;
        }
		check++;
    }
}

// CheckColour
/* This function takes the node that is being passed to it and between top, bottom, left and right, 
it counts the number of black pixels and white pixels,
then if there are no white pixels it returns black,
if there are no black pixels it returns white and if there are both black and white pixels it returns mixed.*/

int CheckColour (struct Treenode *node, int Top, int Bottom, int Left, int Right)
{
    int countOfBlack = 0, countOfWhite = 0;
	int a2 = Top;
	while(a2 <= Bottom){
		int a1 = Left;
		while(a1 <= Right){
			if (node->PixelArray[a1][a2] == Black){
                countOfBlack++;
            }
            else if (node->PixelArray[a1][a2] == White){
                countOfWhite++;
            }
			a1++;
		}
		a2++;
	}
    if (countOfBlack == 0){
        return White;
    }
    if (countOfWhite == 0){
        return Black;
    }
    return Mixed;
}

// createTemporaryArray
/* This function creates a temporary array for the quadrant that has called the function and copies the array values from that quadrant into the temporary array.
It takes first argument as the node from which the data is to read and than top , bottom, left, and right which Xposition and Yposition of the previous and the new temp Array.
And at last mid which is the width of the temporaryArr*/

int** createTemporaryArray (struct Treenode *node, int Top, int Bottom, int Left, int Right, int mid)
{
    int i = 0 ,j = 0;
    int** temporaryArr = (int**) malloc(mid*sizeof(int*));
	int a1 = 0;
    while(a1 < mid) {
        temporaryArr[a1] = (int*) malloc(mid*sizeof(int));
		a1++;
    }
	int a2 = Top;
    while ( a2 <= Bottom ) {
		int a3  = Left;
        while(a3 <= Right ) {
            temporaryArr[i][j] = node->PixelArray[a3][a2];
            i = i + 1;
            if (i == mid)
            { 
                j = j + 1;
                i = 0;
            }
			a3++;
        }
		a2++;
    }
    return temporaryArr;
}