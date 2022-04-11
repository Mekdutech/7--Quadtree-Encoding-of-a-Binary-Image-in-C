
#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>

#define BLACK   0
#define WHITE   1
#define GRAY    2
#define MAX_SIZE   64
#define MIN_SIZE   1

//Node Structure
//consisted of 9 elements: 1 pointer to parent, 4 pointers to children, x position, y position, size, and value - it represents the node.
typedef struct node {
    int x_pos;
    int y_pos;
    int size;
    int value;
    struct node* parent;
    struct node* child[4];
} Node;

//Delaring functions used in the code.
int** ReadImageFile(Node*, char *);
void SetNode(Node *, int, int, int, int, Node*);
void BuildQuadtree(Node*, int**);
int colourCheck(int, int, int, int, int**);
int PowerOfTwo(int);



//The main is passed the file which is identified when the program is called.
//The main function has two arguments, first is the number of arguments, the second is a char array of the calls written in terminal.
int main(int argc, char *argv[]) {
    
    
    if (argc != 2) {
        printf("Error with input file. Program exit\n");
        exit(-1);
    }
    
    
    // Array which will store binary image
    int** imageArray;
    
    Node rootNode;
    int i;
    
    imageArray = ReadImageFile(&rootNode, argv[1]);
    //imageArray = ReadImageFile(&rootNode, "input.txt");
    BuildQuadtree(&rootNode, imageArray);
    
    //release image array
    for (i = 0; i < sizeof(imageArray); i++)
        free(imageArray[i]);
    free(imageArray);
    
    return 0;
}


/*This function reads the input file and constructs and returns an image based on the information read.
 We also use the SetNode function to create the root node of the quadtree tree.*/

int**  ReadImageFile(Node* rootNode, char *fileName) {
    
    FILE *fp;
    int** imagePixels = NULL;
    int imageSize;
    int blackPixels;
    int i, j;
    int power_check = 0;
    int blackCheck = 0;
    int powerOfTwo;
    
    fp = fopen(fileName, "r");
    if(fp == NULL){
        printf("Can not read the input file.\n");
        exit(-1);
    }
    
    //read image size
    fscanf(fp, "%d", &imageSize);
    
    if (imageSize < MIN_SIZE || imageSize > MAX_SIZE) {
        printf("Size of image error: must be 1 - 64.\n");
        exit(-1);
    }
    
    //check the image size
    powerOfTwo = PowerOfTwo(imageSize);
    if (powerOfTwo != 1) {
        printf("Size of image must be a power of two.\n");
        exit(-1);
    }
    
    //Initialize image pixels, set all pixels to white pixel
    imagePixels = (int**)malloc(imageSize * sizeof(int*));
    for (i = 0; i < imageSize; i++)
    {
        imagePixels[i] = (int*)malloc(imageSize * sizeof(int));
        for (j = 0; j < imageSize; j++)
        {
            imagePixels[i][j] = WHITE;
        }
    }
    
    //read number of black pixels
    fscanf(fp, "%d", &blackPixels);
    
    //read the position-(x, y) of all black pixels
    while (fscanf(fp, "%d %d", &i, &j) == 2) {
        if (i >= 0 && j >= 0) {
            if (i < imageSize && j < imageSize) {
                imagePixels[i][j] = BLACK;
                blackCheck++;
            }
            else {
                printf("Position-(x, y) must be smaller than size of image.\n");
                exit(-1);
            }
        }
        else {
            printf("position-(x, y) must be positive.\n");
            exit(-1);
        }
    }
    
    //validity check of black pixels read
    if (blackCheck != blackPixels) {
        printf("Number of black squares does not match number of given coordinates.\n");
        exit(-1);
    }
    
    fclose(fp);
    
    if (blackPixels == 0) {
        printf("Image is only white pixels.\n");
        exit(0);
    }
    
    else if (blackCheck == (imageSize * imageSize)) {
        printf("Image is only black pixels.\n");
        exit(EXIT_SUCCESS);
    }
    
    //construct the root node of the quadtree
    SetNode(rootNode, 0, 0, GRAY, imageSize, NULL);  /* The root node is created. */
    
    return imagePixels;
}

int PowerOfTwo(int x)
{
    while (((x % 2) == 0) && x > 1) /* While x is even and > 1 */
        x /= 2;
    return x;
}

//This function is used to set information passed to the function on the node that is passed.The first parameter is the node that the information will be set on.The PosX, PosY, Width, Value is copied to the passed node.
//x_pos and y_pos is position of NW
void SetNode(Node* newNode, int x_pos, int y_pos, int color, int size, Node *parent) {
    
    newNode->x_pos = x_pos;
    newNode->y_pos = y_pos;
    newNode->size = size;
    newNode->value = color;
    newNode->parent = parent;
    
    // Memory is allocated for 4 child nodes
    int i;
    for (i = 0; i < 4; i++) {
        newNode->child[i] = (Node*)malloc(sizeof(Node));
    }
    
}



//This function is called recursively
/*The following operations are recursively performed on the NW, NE, SW, and SE of the image represented by the node.
 Prints a black terminal node if all pixels in a given image are black. Also,
 if all pixels in the image are mixed, the 4-minute tree is constructed with the current node as the last node.*/

void BuildQuadtree(Node* node, int** image) {
    
    int i, x_left, y_top, x_right, y_bottom;
    int mid, end, result;
    
    mid = (node->size) / 2;
    end = (node->size) - 1;
    
    for (i = 0; i < 4; i++) {
        switch (i) {
            case 0: // NW
                x_left = node->x_pos;
                y_top = node->y_pos;
                x_right = x_left + (mid - 1);
                y_bottom = y_top + (mid - 1);
                
                result = colourCheck(x_left, x_right, y_top, y_bottom, image);
                
                SetNode(node->child[0], x_left, y_top, result, mid, node);
                
                if (result == BLACK) {
                    printf("Black terminal node at position (%d,%d) with size %d\n", x_left, y_top, mid);
                }
                
                if (result == GRAY) {
                    BuildQuadtree(node->child[0], image);
                }
                break;
                
            case 1: // NE
                x_left = node->x_pos + mid;
                y_top = node->y_pos;
                x_right = x_left + (mid - 1);
                y_bottom = y_top + (mid - 1);
                
                result = colourCheck(x_left, x_right, y_top, y_bottom, image);
                
                SetNode(node->child[1], x_left, y_top, result, mid, node);
                
                if (result == BLACK) {
                    printf("Black terminal node at position (%d,%d) with size %d\n", x_left, y_top, mid);
                }
                
                if (result == GRAY) {
                    BuildQuadtree(node->child[1], image);
                }
                break;
                
            case 2: // SE
                x_left = node->x_pos + mid;
                y_top = node->y_pos + mid;
                x_right = x_left + (mid - 1);
                y_bottom = y_top + (mid - 1);
                
                result = colourCheck(x_left, x_right, y_top, y_bottom, image);
                
                SetNode(node->child[2], x_left, y_top, result, mid, node);
                
                if (result == BLACK) {
                    printf("Black terminal node at position (%d,%d) with size %d\n", x_left, y_top, mid);
                }
                
                if (result == GRAY) {
                    BuildQuadtree(node->child[2], image);
                }
                break;
                
            case 3: // SW
                x_left = node->x_pos;
                y_top = node->y_pos + mid;
                x_right = x_left + (mid - 1);
                y_bottom = y_top + (mid - 1);
                
                result = colourCheck(x_left, x_right, y_top, y_bottom, image);
                
                SetNode(node->child[3], x_left, y_top, result, mid, node);
                
                if (result == BLACK) {
                    printf("Black terminal node at position (%d,%d) with size %d\n", x_left, y_top, mid);
                }
                
                if (result == GRAY) {
                    BuildQuadtree(node->child[3], image);
                }
                break;
                
        }
    }
    
}

/* Parameters : the left and right x, top and bottom y coordinates of the image part to check the color, and the original image pointer.
 Returns BLACK if all checked pixels are black, WHITE if all pixels are white, and GRAY if they are mixed.
 This is done by calculating the number of black and white pixels calculated while scanning the image. */

int colourCheck(int x_left, int x_right, int y_top, int y_bottom, int** image) {
    
    int i, j;
    int blackPixels = 0;
    int whitePixels = 0;
    
    for (i = x_left; i <= x_right; i++) {
        for (j = y_top; j <= y_bottom; j++) {
            if (image[i][j] == 0) blackPixels++;
            if (image[i][j] == 1) whitePixels++;
            
            if (blackPixels != 0 && whitePixels != 0)
                return GRAY;                             
        }
    }
    
    if (whitePixels == 0) return BLACK;
    if (blackPixels == 0) return WHITE;
    return GRAY;
}



