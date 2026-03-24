#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<ctype.h>
#include<string.h>

#ifndef _CIFRA_METHOD_
#define _CIFRA_METHOD_

/******************************************************************************************************************
 * @brief Cypers and Decypers using the cesar method.
 * 
 * @param senha String that is used to cypers or decyper using the first character in the string as offset.
 * @param input_c The character that is going to be processed from '-i' or stdin.
 * @param decifrar Check if is to decyper or not (1 or 0).
 * @param ffile Check if is to filter the file or not (1 or 0).
 * @param file_out The output file from '-o'.
 * @return int format_ is a int that goes from (0 -> 6) and at the end of the function if format_ == 6 it prints a "_".
 */
int cifra_cesar(char* senha, char input_c, int decifrar, int ffile, FILE* file_out);

/******************************************************************************************************************
 * @brief Cypers and Decypers using the vigenere method.
 * 
 * @param senha String that is used to cypers or decyper using all the character in the string as offset.
 * @param input_v The character that is going to be processed from '-i' or stdin.
 * @param decifrar Check if is to decyper or not (1 or 0).
 * @param ffile Check if is to filter the file or not (1 or 0).
 * @param file_out The output file from '-o'.
 * @return int format_ is a int that goes from (0 -> 6) and at the end of the function if format_ == 6 it prints a "_". 
 ******************************************************************************************************************/
int cifra_vigenere(char* senha, char input_v, int decifrar, int ffile, FILE* file_out);

/******************************************************************************************************************
 * @brief Filters the file using "_" and "\\n".
 * 
 * @param input_form This input is the output from previous cypers or decypers.
 * @param file_out The output file from '-o'.
 * @return int format_ is a int that goes from (0 -> 6) and at the end of the function if format_ == 6 it prints a "_". 
 ******************************************************************************************************************/
int formatar(char input_form, FILE* file_out);

/******************************************************************************************************************
 * @brief Prints the output in stdout or in the output file defined by '-o'.
 * 
 * @param decifrar Check if is to decyper or not (1 or 0).
 * @param ffile Check if is to filter the file or not (1 or 0).
 * @param file_out The output file from '-o'.
 * @param output Output from previous cypers or decypers.
 * @return int format_ is a int that goes from (0 -> 6) and at the end of the function if format_ == 6 it prints a "_". 
 ******************************************************************************************************************/
int impress_output(int decifrar, int ffile, FILE* file_out, char output);

/******************************************************************************************************************
 * @brief Cypers using the hill method.
 * 
 * @param file_in input file from '-i'.
 * @param file_out The output file from '-o'.
 * @param key it is the "senha" that comes form '-s'
 * @param decifrar Check if is to decyper or not (1 or 0).
 ******************************************************************************************************************/
void cifra_hill(FILE *file_in, FILE *file_out, char *key, int decifrar);

/******************************************************************************************************************
 * @brief Decypers using the hill method.
 * 
 * @param file_in input file from '-i'.
 * @param file_out The output file from '-o'.
 * @param key it is the "senha" that comes form '-s'
 ******************************************************************************************************************/
void decifra_hill(FILE *file_in, FILE *file_out, char *key);

#endif