#include "packing.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    // CHECKING IF ALL INPUTS ARE AVAILABLE
    if(argc != 3)
    {
        fprintf(stderr,"Insufficient number of inputs\n");
        return EXIT_FAILURE;
    }
    Node *mallocSpace = NULL;   // STORES THE ARRAY OF NODE* ELEMENTS RETURNED BY THE LOADING FUNCTION
    Node* tree = Load_From_File(argv[1], &mallocSpace); // LOADING THE FILE
    if(tree == NULL)
    {
        fprintf(stderr, "Loading from file failed\n");
        free(mallocSpace);
        return EXIT_FAILURE;
    }
    Node *packed = Packing_Data(tree);
    if(packed == NULL)
    {
        fprintf(stderr, "Packing algorithm failed\n");
        free(mallocSpace);
        return EXIT_FAILURE;
    }
    Save_To_File(argv[2],packed);
    printf("Width: %le\n", packed->width);
    printf("Height: %le\n", packed->height);
    printf("X-coordinate: %le\n",packed->xcoord);
    printf("Y-coordinate: %le\n",packed->ycoord);
    free(mallocSpace);
    return EXIT_SUCCESS;
}