#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<unistd.h>
#include <math.h>

#ifndef ATACK_METHOD
#define ATACK_METHOD

/******************************************************************************************************************
 * @brief The structure has a vector with 67 positions and an unsined int 
 * 
 ******************************************************************************************************************/
typedef struct treeType TreeType;

/******************************************************************************************************************
 * @brief Creates a dictionary from a file specified by '-w', extracting one word per line. Utilizing a tree-based
 *        structure, it generates various leaf nodes, each storing a character in allocated memory alongside a
 *        number. The number represents whether a word is stored there or not (0 or 1), and the character signifies
 *        the letter of the alphabet assigned to the node.
 *   
 * @param file_dicionary input file from '-w'.
 * @return The root that is the begining for the dictionary for use in atack 1
 ******************************************************************************************************************/
TreeType* create_dicionary(FILE *file_dicionary);

/******************************************************************************************************************
 * @brief Create a Leaf for the tree initializing the vector and the unsined int as NULL.
 * 
 * @return The TreeType *leaf or NULL if there is any error while processing.
 ******************************************************************************************************************/
TreeType* CreateLeaf(void);

/******************************************************************************************************************
 * @brief The function goes through every leaf pointing it to NULL (freeing the memory alocated to that leaf) and
 *        after all leafs point to NULL it frees the root.
 * 
 * @param root The first node (root of the tree). 
 ******************************************************************************************************************/
void Free_Dicionary(TreeType *root);

/******************************************************************************************************************
 * @brief This function uses the dicionary previous defined and check all 67 offsets and sees what offset has a
 *        higher score (score = (nºchar)^2) and uses the given offset to decrypt the input.
 * 
 * @param file_in input file from '-i'.
 * @param file_dicionary input file from '-w'.
 * @param file_out The output file from '-o'.
 ******************************************************************************************************************/
void atack_method_1(FILE *file_in, FILE *file_dicionary, FILE *file_out);

/******************************************************************************************************************
 * @brief This function uses defined average statistics and compares to the calculated statistics to see what offset
 *        is the closest to the average use of leters in the english alphabet printing the decyperd output for cesar 
 *        cypered input.
 * 
 * @param file_in input file from '-i'.
 * @param file_out The output file from '-o'.
 ******************************************************************************************************************/
void atack_method_2(FILE *file_in, FILE *file_out);

/******************************************************************************************************************
 * @brief This function uses defined average statistics and compares to the calculated statistics to see what offset
 *        is the closest to the average use of leters in the english alphabet printing the decyperd output for vigenere
 *        cypered input.
 * 
 * @param file_in input file from '-i'.
 * @param max_key_len max key len defined form '-n'.
 * @param file_out he output file from '-o'.
 ******************************************************************************************************************/
void atack_method_3(FILE * file_in, int max_key_len, FILE *file_out);

#endif