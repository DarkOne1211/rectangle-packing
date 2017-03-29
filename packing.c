// PACKING OF RECTANGLES STORED IN A BINARY TREE 
#include <stdio.h>
#include <stdlib.h>
#include "packing.h"
#include <string.h>

//HELPER FUNCTION DECLARATION
Node *createBinaryTree(Node* element, int counter);

//STACK OPERATION 
int push(Stack *s, Node *element);
void pop(Stack *s);

// END OF FUNCTION DECLARATION

// FUNCTION DEFINITION

// THIS FUNCTION LOADS THE FILE AND CREATES A BINARY TREE AND RETURN THE ROOT NODE TO THE CALLER FUCTION
Node *Load_From_File(char *Filename, Node **packingArray)
{
    FILE *readptr;
    readptr = fopen(Filename,"r");
    if(readptr == NULL)
    {
        fprintf(stderr, "Opening the file\n");
        return NULL;
    }
    char *input = NULL;
    size_t lineSize;
    ssize_t readSize;
    int numberOfNodes = 0;
    int counter = 0;
    while((readSize = getline(&input, &lineSize, readptr))!= -1)
    {
        numberOfNodes++;
    }
    rewind(readptr);
    Node *packingNode = malloc(sizeof(Node)*numberOfNodes);;
    if(packingNode == NULL)
    {
        fprintf(stderr, "Creation of Nodes elements failed\n");
        return NULL;
    }
    // COPYING THE ELEMENTS LINE BY LINE INTO A ARRAY OF NODES

    while((readSize = getline(&input, &lineSize, readptr))!= -1)
    {   
        if(sscanf(input,"%d(%le,%le)",&(packingNode[counter].label),&(packingNode[counter].width),&(packingNode[counter].height)) == 3)
        {
            packingNode[counter].orientation = 0;
            packingNode[counter].xcoord = 0;
            packingNode[counter].ycoord = 0;
            packingNode[counter].left = NULL;
            packingNode[counter].right = NULL;
        }
        else
        {
            packingNode[counter].label = 0;
            packingNode[counter].orientation = input[0];
            packingNode[counter].width = 0;
            packingNode[counter].height = 0;
            packingNode[counter].xcoord = 0;
            packingNode[counter].ycoord = 0;
            packingNode[counter].left = NULL;
            packingNode[counter].right = NULL;
        }
        free(input);
        input = NULL;
        counter++;
    }
    free(input);
    Node *root = createBinaryTree(packingNode,numberOfNodes); // Converts the array into a binary tree and retuns the root;
    if(root == NULL)
    {
        fprintf(stderr, "Creating a binary tree failed");
        return NULL;
    }
    *packingArray = packingNode;
    fclose(readptr);
    return root;
}

// RECONSTRUCTS BINARY TREE BY USING STACKS. PUSH THE NODE INTO STACK IF ITS A LEAF NODE.
// IF ITS NOT A LEAF NODE IT PUSHES THE NODE AND CHANGES THE LEFT AND RIGHT VALUE TO THE 
// PREDECESSORS OF THE STACK AND THEN POPS THE NODES AND PUSH THE CREATED TREE. REPEASTS 
// PROCESS TILL THE ENITRE ARRAY IS TRAVERSED.

Node* createBinaryTree(Node* element, int numberOfNodes)
{
    int counter = 0;
    Stack s;
    s.maxSize = numberOfNodes;
    s.stk = malloc(sizeof(Node*)*s.maxSize);
    if(s.stk == NULL)
    {
        fprintf(stderr, "Stack creation failed. Insufficient space");
        return NULL;
    }
    s.top = -1;
    for(counter = 0; counter < numberOfNodes; counter++)
    {
        if(element[counter].label != 0)
        {
            if(!push(&s,&element[counter]))
            {
                return NULL;
            }
        }
        else
        {
            if(!push(&s,&element[counter]))
            {
                return NULL;
            }
            s.stk[s.top]->left = s.stk[s.top - 2];
            s.stk[s.top]->right = s.stk[s.top - 1];
            Node *temp = s.stk[s.top];
            pop(&s);
            pop(&s);
            pop(&s);
            if(!push(&s,temp))
            {
                return NULL;
            }
        }
    }
    Node *root = s.stk[s.top];
    free(s.stk);
    return root;
}

// BASIC STACK OPERATIONS
int  push(Stack *s, Node *element)
{
    if(s->top == s->maxSize)
    {      
        return 0;
    }
    else
    {
        s->top = s->top + 1;
        s->stk[s->top] = element;
        return 1;
    }

}

void pop(Stack *s)
{
    if(s->top != -1)
    {
        s->top = s->top - 1;
    }
}

void postOrderPacking(Node *root)
{
    if(root->orientation == 0)
    {
        return;
    }
    postOrderPacking(root->left);
    postOrderPacking(root->right);
    if(root->orientation == 'V')
    {
        root->width = root->left->width + root->right->width;
        root->height = root->left->height > root->right->height ? root->left->height : root->right->height;
        root->left->xcoord = 0;
        root->left->ycoord = 0;
        root->right->xcoord = root->left->width;
        root->right->ycoord = 0;
    }
    if(root->orientation == 'H')
    {
        root->height = root->left->height + root->right->height;
        root->width = root->left->width > root->right->width ? root->left->width : root->right->width;
        root->left->xcoord = 0;
        root->left->ycoord = root->right->height;
        root->right->xcoord = 0;
        root->right->ycoord = 0;
    }
}

void shiftingYPacking(Node *node)
{
    if(node->orientation == 0)
    {
        return;
    }
    node->left->ycoord += node->ycoord;
    node->right->ycoord += node->ycoord;
    shiftingYPacking(node->left);
    shiftingYPacking(node->right);
}

void shiftingXPacking(Node *node)
{
    if(node->orientation == 0)
    {
        return;
    }
    node->right->xcoord += node->xcoord;
    node->left->xcoord += node->xcoord;
    shiftingXPacking(node->left);
    shiftingXPacking(node->right);
}

// FInds the largest label

Node* findLargestLabel(Node *node, Node *largest)
{
    if(largest->label < node->label)
    {
        largest = node;
        return largest;
    }
    if(node->left)
        largest = findLargestLabel(node->left, largest);
    if(node->right)
        largest = findLargestLabel(node->right, largest);
    return largest;
}

// PERFORMS THE PACKING

Node *Packing_Data(Node *root)
{
    postOrderPacking(root);
    if(root->orientation == 'H')
    {
        shiftingYPacking(root);
        shiftingXPacking(root);
    }
    if(root->orientation == 'V')
    {
        shiftingXPacking(root);
        shiftingYPacking(root);
    }
    Node *largest = root;
    largest = findLargestLabel(root, largest);
    root->xcoord = largest->xcoord;
    root->ycoord = largest->ycoord;
    return root;
}

//SAVE THE PACKING IN THE GIVEN FILE NAME

int recursive_Save_To_File(FILE *Filename, Node* node)
{
    if(node == NULL)
    {
        return 0;
    }
    int NumberNodes = 0;
    NumberNodes += recursive_Save_To_File(Filename, node->left);
    NumberNodes += recursive_Save_To_File(Filename, node->right);
    NumberNodes += 1;
    if(node->label != 0)
    {
        fprintf(Filename, "%d %le %le %le %le\n",node->label, node->width, node->height, node->xcoord, node->ycoord);
    }
    return NumberNodes;
}

int Save_To_File(char *Filename, Node* root)
{
    FILE *writeptr;
    writeptr = fopen(Filename,"w");
    int numberofNodes = recursive_Save_To_File(writeptr,root);
    fclose(writeptr);
    return numberofNodes;
}
