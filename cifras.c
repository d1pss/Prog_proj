#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<ctype.h>
#include<string.h>

#include"cifras_method.h"
#include"atack_method.h"

#define ALPHA_SIZE 67
#define HELP_MENU "-i filename |nome do ficheiro de entrada, em alternativa a stdin\n \
                   -o filename |nome do ficheiro de saida, em alternativa a stdout\n\
                   -s senha    |senha a usar para cifrar/decifrar\n \
                   -f          |filtra o ficheiro de entrada e formata o ficheiro de saida\n\
                   -c nn       |a operacao a realizar deve ser cifrar, com o metodo nn\n\
                   -d nn       |a operacao a realizar deve ser decifrar, com o metodo nn\n\
                   -e          |a operacao a realizar deve ser calcular estatisticas\n\
                   -a nn       |a operacao a realizar deve ser atacar, com o metodo nn\n\
                   -n nn       |para o ataque método 3, a dimensao maxima da chave deve ser nn\n\
                   -w filename |nome do ficheiro de dicionario a usar\n"

/******************************************************************************************************************
 * @brief calculates the statistics of the input.
 * 
 * @param file_in Input file from '-i'.
 * @param file_out_use Check if is to write in the output file or in stdout (1 or 0).
 * @param file_out Output file from '-o'.
 ******************************************************************************************************************/
void statistics_calc(FILE *file_in, FILE *file_out){
  char alf[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz .,;-";
  long int error_check = 0, memory_input = 0;
  char *input;
  double count_tab = 0, count_notab = 0, count_var = 0, count_total = 0;
  double percentage = 0, percentage_notab = 0;

  //find how many bytes we have to alloc to read the whole file
  error_check = fseek(file_in, 0, SEEK_END);
  if(error_check == -1){
    fprintf(stderr, "Error using fseek\n");
    exit(1);
  }
  memory_input = ftell(file_in);
  rewind(file_in);

  input = (char*)calloc(memory_input + 1,sizeof(char));
  if(input == NULL){
    fprintf(stderr, "Error allocating memorie to input\n");
    exit(1);
  }

  //put the text in file_in to the input
  size_t bytes_read = fread(input, 1, memory_input, file_in);
  if ((int)bytes_read != memory_input){
    fprintf(stderr, "Error writing the input\n");
    exit(1);
  }

  //count the number of known and unkown characters
  for (int i = 0; i < memory_input; i++){
    for (int j = 0; j < ALPHA_SIZE; j++){
      if (input[i] == alf[j]){
        count_tab++;
        break;
      }else if (j == (ALPHA_SIZE - 1)){
        count_notab++;
      }
    }
  }
  count_total = count_notab + count_tab;

  //calc the percentage of each character in the table
  for (int i = 0; i < ALPHA_SIZE; i++){
    count_var = 0;
    for (int j = 0; j < memory_input; j++){
      if (input[j] == alf[i]){
        count_var++;
      }
    }
    percentage = (count_var / (count_tab)) * 100;
    fprintf(file_out, "conta('%c')=%.0lf\t%.6lf%%\n", alf[i], count_var, percentage);  
  }

  //print total know chars, percentage of unknowm char and total chars
  percentage_notab = (count_notab / (count_total)) * 100;
    fprintf(file_out, "Total: %.0lf caracteres\n", count_tab);
    fprintf(file_out, "conta(outros)=%.0lf\t%.6lf%%\n",count_notab, percentage_notab);
    fprintf(file_out, "Total do ficheiro: %.0lf caracteres\n", count_total); 
  free(input);
  return;
}

/******************************************************************************************************************
 * @brief The main function is responsible for handling command-line arguments.  
 *        The first part loops through each argument provided when running the program and performs 
 *        the action based on the flag selected (line 1 to 109). After this the second part begins,
 *        it uses the functions in the other files in this menu (line 110 to 149).
 * 
 * @param argc arguments from getopt.
 * @param argv arguments from getopt.
 * @return 0 if sucsess or 1 if error.
 ******************************************************************************************************************/
int main(int argc, char *argv[]){
  char flag, input;
  char *senha = "Programacao2024";
  int ffile = 0, cifrar = 0, decifrar = 0, format_ = 0, atack = 0, statistics = 0, max_key_len = 20;
  char alf[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz .,;-";
  FILE *file_in = stdin, *file_dicionary = NULL, *file_out = stdout;

  while ((flag = getopt(argc,argv, "hi:o:s:fc:d:ea:n:w:")) != -1){
    switch (flag) {
      case'h':
        printf(HELP_MENU);
        return 0;
        break;
      case'i':
        if(optarg ==NULL){//checks if it has an argument
          fprintf(stderr, "Error '-i' needs a argument\n");
          return 1;
        }
        file_in = fopen(optarg, "r");
        if(file_in == NULL){
          fprintf(stderr, "Error opening the input file\n");
          return 1;
        }
        break;
      case'o':
        if(optarg ==NULL){//checks if it has an argument
          fprintf(stderr, "Error '-o' needs a argument\n");
          return 1;
        }
        file_out = fopen(optarg, "w");
        if(file_out == NULL){
          fprintf(stderr, "Error opening the output file\n");
          return 1;
        }
        break;
      case's':
        if(optarg == NULL){//checks if it has an argument
          fprintf(stderr, "Error '-s' needs a argument\n");
          return 1;
        }
        senha = optarg;
        for(size_t i = 0; i < strlen(senha); i++)
        {//checks for unknown chars is "senha"
          for(int j = 0; j < 67; j++){
            if(senha[i] == alf[j]){
              break;
            }else if(j == 66){
              fprintf(stderr,"Error unknown caracters in password\n");
              return 1;
            }
          }
        }
        break;
      case'f':
        ffile = 1;
        break;
      case'c':
        if(optarg ==NULL){//checks if it has an argument
          fprintf(stderr, "Error '-c' needs a argument\n");
          return 1;
        }
        sscanf(optarg, "%d", &cifrar);
        if((cifrar < 1) || (cifrar > 3)){
          fprintf(stderr,"Error the number you wrote in -c must be betwen 1 and 3\n");
          return 1;
        }
        break;
      case'd':
        if(optarg ==NULL){//checks if it has an argument
          fprintf(stderr, "Error '-d' needs a argument\n");
          return 1;
        }
        sscanf(optarg, "%d", &decifrar);
        if((decifrar < 1) || (decifrar > 3)){
          fprintf(stderr,"Error the number you wrote in -d must be betwen 1 and 3\n");
          return 1;
        }
        break;
      case'e':
        statistics = 1;
        break;
      case'a':
        if(optarg ==NULL){//checks if it has an argument
          fprintf(stderr, "Error '-a' needs a argument\n");
          return 1;
        }
        sscanf(optarg, "%d", &atack);
        if((atack < 1) || (atack > 3)){
          fprintf(stderr,"Error the number you wrote in -a must be betwen 1 and 3\n");
          return 1;
        }
        break;
      case'n':
        if(optarg ==NULL){//checks if it has an argument
          fprintf(stderr, "Error '-n' needs a argument\n");
          return 1;
        }
        sscanf(optarg, "%d", &max_key_len);
        if(max_key_len < 1){
          fprintf(stderr,"Error the number you wrote in -a must be higher than or equal to 1\n");
          return 1;
        }
        break;
      case'w':
        if(optarg ==NULL){//checks if it has an argument
          fprintf(stderr, "Error '-w' needs a argument\n");
          return 1;
        }
        file_dicionary = fopen(optarg, "r");
        if (file_dicionary == NULL) {
          fprintf(stderr, "Error opening the dicionary file\n");
          return 1;
        }
        break;          
      default:
        fprintf(stderr,"Error unknown input\n");
        printf(HELP_MENU);
        return 1;
        break;
    }
  }

  //1 fase part
  if(cifrar != 0 || decifrar != 0 || ffile != 0){
    if(cifrar == 3 || decifrar == 3)
    {//checks for hill method if "senha" is to short
      if(strlen(senha) < 4){
        fprintf(stderr, "Error the key is to small it needs to be at lest 4 characters long\n");
        fclose(file_in);
        fclose(file_out);
        return 1;
      }
    }
    if(cifrar == 3)
    {//cyper with hill method
      cifra_hill(file_in, file_out, senha, 0);
    }else if(decifrar == 3)
    {//decyper with hill method
      decifra_hill(file_in, file_out, senha);
    }else{
      while((input = getc(file_in)) != EOF)
      {//loop that goes char by char to cyper or decyper the input
        if(ffile == 1)
        {//filter the input from unknown char
          int found = 0;
          for (size_t j = 0; j < strlen(alf); j++) {
            if (input == alf[j]) {
              found = 1;
              break;
            }
          }
          if(!found) { 
            continue;
          }
        }
        if((cifrar == 1) || (decifrar == 1))
        {//cyper or decyper with cesar method
          format_ = cifra_cesar(senha, input, decifrar, ffile, file_out);
        }else if((cifrar == 2) || (decifrar == 2))
        {//cyper or decyper with vigenere method
          format_ = cifra_vigenere(senha, input, decifrar, ffile, file_out);
        }
      }
      if(format_ == 6)
      {//print "_" if there was 6 char at the end of the output without "_"
        fputc('_', file_out);
      }
      if(ffile == 1)
      {//print "\n" because when we filter the input we lose the "\n" and in the end it needs to have a "\n"
        fputc('\n', file_out);
      }    
    }  
  }else if(atack != 0){
    if(atack == 1){
      if(file_dicionary == NULL)
      {//checks if dicionary is given in -w
        fprintf(stderr, "Error no dicionary selected or dicionary = NULL\n");
        fclose(file_in);
        fclose(file_out);
        return 1;
      }
      atack_method_1(file_in, file_dicionary, file_out);
    }else if(atack == 2){
      atack_method_2(file_in, file_out);
    }else if(atack == 3){
      atack_method_3(file_in, max_key_len, file_out);
    }   
  }
  if(statistics == 1){
    statistics_calc(file_in, file_out);
  } 
  fclose(file_in);
  fclose(file_out);
  return 0;
}
