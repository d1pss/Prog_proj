#include"cifras_method.h"

#define ALPHA_SIZE 67

int cifra_vigenere(char* senha, char input_v, int decifrar, int ffile, FILE* file_out){
    char alf[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz .,;-";
    int offset = 0;
    static size_t i = 0; //digit of "senha" we are on
    char output_v;
    int format_ = 0;

    //define the offset from the digits of the "senha"
    for(int j = 0; j < 67;j++){
        if(senha[i] == alf[j]){
            offset = j;
            if(decifrar == 2){
                offset = 67 - j;
            }
            break;
        }
    }

    //cyper using the offset defined in the previous loop
    for(int g = 0; g < 67; g++){
        if(input_v == alf[g]){
            output_v = alf[((g + offset) % 67)];
            i++;//increse the digit of "senha" (only if the input is known in the table)
            if(i == strlen(senha))
            {//loop the digit of the "senha" to the beginig
                i = 0;
            }
            break;
        }else if(g == 66)
        {//(note: this will only get to this point if we did not filter the input which is in main)
            output_v = input_v;
            break;
        }
    }
    //print output
    format_ = impress_output(decifrar, ffile, file_out, output_v);
    return format_;
}

int cifra_cesar(char* senha, char input_c, int decifrar, int ffile, FILE* file_out){
    char alf[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz .,;-";
    int offset = 0;
    char output_c;
    int format_ = 0; 
 
    //define the offset from the first digit of the "senha"
    for(int j = 0; j < 67;j++){
        if(senha[0] == alf[j]){
            offset = j;
            if(decifrar == 1)
            {//for the decyper the offset it is the oposite of the cyper so we reverse the offset
                offset = 67 - j;
            }
            break;
        }
    }

    //cyper using the offset defined in the previous loop
    for(int j = 0; j < 67; j++){
        if(input_c == alf[j]){
            output_c = alf[((j + offset) % 67)];           
            break;
        }else if(j == 66)
        {//(note: this will only get to this point if we did not filter the input which is in main)          
            output_c = input_c; 
            break;
        }
    }
    //print output
    format_ = impress_output(decifrar, ffile, file_out, output_c);
    return format_;
}

int formatar(char input_form, FILE* file_out) {
    static int k = 0; //turns to print "\n"
    static int i = 0; //turns to print "_"

    if(i == 6)
    {//i is how many char has the output after the last "_"
        if(k == 7)
        {//k is how many "_" has the output after the last "\n"
            fputc('\n', file_out);
            fputc(input_form, file_out);           
            k = 0;
            i = 1;
            return 0;
        }else{//if k != 7 print "_"
            fputc('_', file_out);
            fputc(input_form, file_out);           
            i = 1;
            k++;
            return 0;
        }
    }
    //if i != 6 print a char and i++    
    fputc(input_form, file_out);  
    i++;
    return i;
}

int impress_output(int decifrar, int ffile, FILE* file_out, char output){
    static int k = 0;
    int format_ = 0;
    //print to the assined file in '-o' or stdin if '-o' is not defined   
    if((ffile == 1) && (decifrar == 2)){
        if(k == 48){
            fputc('\n', file_out);
            fputc(output, file_out);
            k = 1;
        }else{
            fputc(output, file_out);
            k++;
        }
    }else if((ffile != 1)){
        fputc(output, file_out);
        return 0;
    }else if(ffile == 1){
        format_ = formatar(output, file_out);
        return format_;
    }
    
    return 0;    
}

void cifra_hill(FILE *file_in, FILE *file_out, char *key, int decifrar){
    char alf[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz .,;-";
    char *input = NULL, *output = NULL, *input_f = NULL, *output_f = NULL;
    int error_check = 0, curr_input_filt = 0, value_key[4] = {0}, value_1 = 0, value_2 = 0, curr_output = 0, char_ = 0, char_enter = 0;
    long int memory_input = 0;

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
        fprintf(stderr, "Error allocating memory to input\n");
        exit(1);
    }

    input_f = (char*)calloc(memory_input + 1,sizeof(char));
    if(input_f == NULL){
        fprintf(stderr, "Error allocating memory to input_f\n");
        exit(1);
    }

    //put the text in file_in to the input
    size_t bytes_read = fread(input, 1, memory_input, file_in);
    if (bytes_read != memory_input){
        fprintf(stderr, "Error writing the input\n");
        exit(1);
    }

    //filter the input
    for(int i = 0; i < memory_input; i++){
        for(int j = 0; j < ALPHA_SIZE; j++){
            if(input[i] == alf[j]){
                input_f[curr_input_filt] = input[i];
                curr_input_filt++;
                break;
            }
        }
    }
    input_f[curr_input_filt] = '\0';

    if(strlen(input_f) % 2 != 0)
    {//check if the input is a odd number if yes add a " "
        input_f[curr_input_filt] = ' ';
        curr_input_filt++;
        input_f[curr_input_filt] = '\0';
    }

    //find the value of the 4 digits of the key
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < ALPHA_SIZE; j++){
            if(key[i] == alf[j]){
                value_key[i] = j;
                break;
            }
        }
    }

    output = (char*)calloc(curr_input_filt + 1,sizeof(char));
    if(output == NULL){
        fprintf(stderr, "Error allocating memory to output\n");
        exit(1);
    }
    
    //encrypt the input
    for(int i = 0; i < curr_input_filt; i++){
        for(int j = 0; j < ALPHA_SIZE; j++)
        {//calc the value on our table for the 1 digit
            if(input_f[i] == alf[j]){
                value_1 = j;
                break;
            }
        }
        for(int j = 0; j < ALPHA_SIZE; j++)
        {//calc the value on our table for the 2 digit
            if(input_f[(i + 1)] == alf[j]){
                value_2 = j;
                break;
            }
        }
        //encrypt with key values
        output[i] = alf[(((value_1 * value_key[0]) + (value_2 * value_key[2])) % 67)];
        i++;
        output[i] = alf[(((value_1 * value_key[1]) + (value_2 * value_key[3])) % 67)];
    }
    output[curr_input_filt] = '\0';

    if(decifrar == 0)
    {//if we are encrypting filter the output with "_" and "\n"

        //formating the output
        while(output[curr_output] != '\0'){
            if(char_ == 6){
                if(char_enter == 7){
                    fprintf(file_out, "\n");
                    char_enter = 0;
                    char_ = 0;
                }else{
                    fprintf(file_out, "_");
                    char_ = 0;
                    char_enter++;
                }
            }else{
                fprintf(file_out, "%c", output[curr_output]);
                char_++;
                curr_output++;
            }
        }
        if(char_ == 6){
            fprintf(file_out, "_");
        }
        fprintf(file_out, "\n");

        free(output_f);
    }else if(decifrar == 1)
    {//if we are decrypting just print \n every 48 characters
        while(output[curr_output] != '\0'){
            if(char_enter == 48){
                fprintf(file_out, "\n");
                char_enter = 0;
            }else{
                fprintf(file_out, "%c", output[curr_output]);
                curr_output++;
                char_enter++;
            }
        }
        fprintf(file_out, "\n");
    }
    free(input);
    free(input_f);
    free(output);
    return;
}

void decifra_hill(FILE *file_in, FILE *file_out, char *key){
    char alf[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz .,;-";
    int inverter_det[] = { 0, 1, 34, 45, 17, 27, 56, 48, 42, 15, 47, 61, 28, 31, 24, 9, 21, 4, 41, 60, 57, 16, 64, 35, 14, 59, 49, 5, 12,
                          37, 38, 13, 44, 65, 2, 23, 54, 29, 30, 55, 62, 18, 8, 53, 32, 3, 51, 10, 7, 26, 63, 46, 58, 43, 36, 39, 6, 20,
                          52, 25, 19, 11, 40, 50, 22, 33, 66 };
    int value_key[4] = {0}, det_key = 0, det_key_invert = 0;
    char new_key[4] = {0};

    //find the value of the 4 digits of the key
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < ALPHA_SIZE; j++){
            if(key[i] == alf[j]){
                value_key[i] = j;
                break;
            }
        }
    }

    //calculate det and invert it
    det_key = (((67 * 67) + ((value_key[0] * value_key[3]) - (value_key[1] * value_key[2]))) % 67);
    det_key_invert = inverter_det[det_key];

    //calc the new key with inverted det and adjoint matrix
    new_key[0] = alf[((det_key_invert * value_key[3]) % 67)];
    new_key[1] = alf[((det_key_invert * (67 - value_key[1])) % 67)];
    new_key[2] = alf[((det_key_invert * (67 - value_key[2])) % 67)];
    new_key[3] = alf[((det_key_invert * value_key[0]) % 67)];

    //use hill cyper with new key to decyper the code
    cifra_hill(file_in, file_out, new_key, 1);
    return;
}