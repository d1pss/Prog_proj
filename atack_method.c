#include"atack_method.h"

#define ALPHA_SIZE 67
#define MAX_WORD 100
#define NOT_VALID 0
#define IS_VALID 1

struct treeType
{//struct for tree node
    unsigned int number;
    TreeType * vector[ALPHA_SIZE];
};

TreeType* create_dicionary(FILE *file_dicionary){
    char alf[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz .,;-";
    TreeType *root, *aux;
    char word[MAX_WORD] = {0}, line[MAX_WORD] = {0};
    int isvalid = 0, h = 0, char_value = 0, word_len = 0, isvalid_2 = 0, line_len = 0;

    //initialize root using the CreateLeaf function
    root = CreateLeaf();
    if (root == NULL)
    {//check if there was any error alocating memory in the leaf
        fprintf(stderr, "Error alocating memory to root\n");
        exit(1); 
    }

    //get line by line from the dicionary
    while (fgets(line, MAX_WORD, file_dicionary) != NULL){
        h = 0;
        isvalid = IS_VALID;
        line_len = (int)strlen(line);

        for(int i = 0; i < line_len - 1; i++)
        {// line_len - 1 to take off the \n
            isvalid_2 = NOT_VALID;
            //check if it is a tabled character
            for(int j = 0; j < ALPHA_SIZE; j++){              
                if(line[i] == alf[j]){
                    isvalid_2 = IS_VALID;
                    if(line[i] == ' ')
                    {//check if its a space because it has a special condition
                        break;
                    }    
                    word[h] = line[i];
                    h++;
                }        
            }
            //if is not a tabled char se if its any of the special cases otherwise ignore that line
            if(isvalid_2 == NOT_VALID){
                if(line[i] == '\''){
                    continue;
                }else if(line[i] == '\t'){
                    break;
                }else if(line[i] == '/'){
                    break;
                }else{
                    isvalid = NOT_VALID;
                    break;
                }
            }                                                              
        }
        if(isvalid == NOT_VALID)
        {//skiping the line if unknown character is found
            continue;
        }
        word[h] = '\0';

        //aux is the TreeType struct that is going to write the tree
        //root is the TreeType struct that is the begining if the tree
        aux = root;
        word_len = strlen(word);
        for(int i = 0; i < word_len; i++)
        {//start in the first leter and go down the tree creating leaf if needed to store the word for later use
            for(int j = 0; j < ALPHA_SIZE; j++)
            {//turn the character into a int code from our table
                if(word[i] == alf[j]){
                    char_value = j;
                    break;
                }
            }
            
            if (aux->vector[char_value] == NULL)
            {//check if there is allready a leaf created for that character if not create
                aux->vector[char_value] = CreateLeaf();
                if (aux->vector[char_value] == NULL)
                {//check if there was any error alocating memory in the leaf
                    fprintf(stderr, "Error creating the leaf\n");
                    exit(1);
                }                
            }
            //go to the next position in the tree                  
            aux = aux->vector[char_value];           
        }
        //after the last char of the word is read store the number 1 which means that there is a word stored in that leaf for later use
        aux->number = 1;
    }
    return root;
}

TreeType* CreateLeaf(void){
    TreeType* leaf = (TreeType *) malloc(sizeof(TreeType));
    if (leaf == NULL){
        return NULL;
    }
    for(int i = 0; i < ALPHA_SIZE; i++)
    {//initialize evey position of the array
        leaf->vector[i] = NULL;
    }
    //initialize the number
    leaf->number = 0;
    return leaf;   
}

void Free_Dicionary(TreeType *root){
    if(root == NULL){
        return;
    }
    for(int i = 0; i < ALPHA_SIZE; i++){
        if(root->vector[i] != NULL){
            Free_Dicionary(root->vector[i]);
        }
    }
    free(root);
}

void atack_method_1(FILE *file_in, FILE *file_dicionary, FILE *file_out){
    char alf[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz .,;-";
    TreeType *root, *aux;
    char *input = NULL, *input_c = NULL;
    long int memory_input = 0, error_check = 0, current_char = 0;
    int score[ALPHA_SIZE] = {0}, word_count[ALPHA_SIZE] = {0};
    int max_score = 0, max_index = 0, current_char_value = 0, curr_input_c = 0;

    //create the dicionary
    root = create_dicionary(file_dicionary);

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
    input_c = (char*)calloc(memory_input + 1,sizeof(char));
    if(input_c == NULL){
        fprintf(stderr, "Error allocating memory to input_c\n");
        exit(1);
    }
    //put the text in file_in to the input
    size_t bytes_read = fread(input, 1, memory_input, file_in);
    if (bytes_read != memory_input){
        fprintf(stderr, "Error writing the input\n");
        exit(1);
    }
    
    for(int offset = 0; offset < 67; offset++)
    {//loop through all offsets
        curr_input_c = 0;
        for(long int j = 0; j < memory_input; j++)
        {//decyper with the offset
            for(int k = 0; k < ALPHA_SIZE; k++){
                if(input[j] == alf[k]){
                    input_c[curr_input_c] = alf[((k + offset) % 67)];
                    curr_input_c++;  
                    break;
                }
            }
        }
        input_c[curr_input_c] = '\0';

        for(long int initial_char = 0; initial_char < curr_input_c; initial_char++)
        {//char_begin is the char that is going to start the chain down the tree
            current_char = initial_char;
                aux = root;
                for(long int level = 0; level < curr_input_c; level++)
                {//level is the number of chars in that current level of the tree                    
                    for(int h = 0; h < ALPHA_SIZE; h++)
                    {//turn current_char into is value in the alf[]
                        if(input_c[current_char] == alf[h]){
                            current_char_value = h;
                            break;
                        }
                    }
                    if(aux->number == 1)
                    {//if exists word in the current level add to score
                        score[offset] = score[offset] + (level * level);
                        word_count[offset]++;
                    }
                    if(aux->vector[current_char_value] == NULL)
                    {//if there is no more chars to follow, go back to the begining and use the next initial_char
                        break;
                    }
                    //give the next aux the node of the current level
                    aux = aux->vector[current_char_value];
                    current_char++;
                    if(current_char == curr_input_c)
                    {//check if next loop is going to be out of allocated memory
                       break; 
                    }
                }
        }
    }  
    for(int i = 0; i < 67; i++) {
        if(score[i] > max_score)
        {// Update max_score and max_index if a larger score is found
            max_score = score[i];
            max_index = i;
        }
    }
    for(long int j = 0; j < memory_input; j++)
    {//cyper with the max_index
        for(int k = 0; k < ALPHA_SIZE; k++){
            if(input[j] == alf[k]){
                input_c[j] = alf[((k + max_index) % 67)];        
                break;
            }else if(k == 66){
                input_c[j] = input[j];
                break;
            }
        }
    }

    if(file_out == NULL){
        fprintf(stderr, "Error Output file pointer is NULL\n");
    }
    
    fprintf(file_out,"offset máximo %d, letra '%c', cifrado com letra '%c', total %d palavras, score %d.\n", max_index, alf[max_index], alf[(67 - max_index) % 67], word_count[max_index], max_score);       
    fprintf(file_out, "%s", input_c);      
    

    Free_Dicionary(root);
    free(input);
    free(input_c);
    fclose(file_dicionary);
    return;
}

void atack_method_2(FILE *file_in, FILE *file_out){
    char alf[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz .,;-";
    const double prob_given[] = { 
        0.000160079, 0.000554985, 0.000381091, 0.000306662, 0.000263793, 0.000235223,
        0.000214441, 0.00019842, 0.000185551, 0.000174894, 0.002049944, 0.00037499,
        0.000699981, 0.00107497, 0.003174913, 0.000549985, 0.000499986, 0.001524958,
        0.001749952, 3.7499E-05, 0.000192495, 0.000999973, 0.000599984, 0.001674954,
        0.001874949, 0.000474987, 2.37493E-05, 0.001499959, 0.001574957, 0.002274938,
        0.000699981, 0.000244993, 0.000599984, 3.7499E-05, 0.000499986, 1.84995E-05,
        0.061498311, 0.011249691, 0.020999423, 0.032249114, 0.095247384, 0.016499547,
        0.014999588, 0.045748744, 0.052498558, 0.001124969, 0.005774841, 0.029999176,
        0.017999506, 0.05024862, 0.056248455, 0.014249609, 0.00071248, 0.044998764,
        0.047248702, 0.068248126, 0.020999423, 0.007349798, 0.017999506, 0.001124969,
        0.014999588, 0.000554985, 0.190494768, 0.014249609, 0.014999588, 0.000554985,
        0.001124969 };
    double prob_calc[ALPHA_SIZE] = {0};
    double erro[ALPHA_SIZE] = {0}, count_tab = 0, count_var = 0, min_erro = INFINITY;
    int error_check = 0, min_offset = 0;
    long int memory_input = 0;
    char *input = NULL, *output = NULL;

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
    
    //put the text in file_in to the input
    size_t bytes_read = fread(input, 1, memory_input, file_in);
    if (bytes_read != memory_input){
        fprintf(stderr, "Error writing the input\n");
        exit(1);
    }

    //find how many character that are in input that belong to our table
    for (int i = 0; i < memory_input; i++){
        for (int j = 0; j < ALPHA_SIZE; j++){
            if (input[i] == alf[j]){
                count_tab++;
                break;
            }
        }
    }

    //calc the frequency for all tabled character from input
    for(int i = 0; i < ALPHA_SIZE; i++){
        count_var = 0;
        for(int j = 0; j < memory_input; j++){
            if(input[j] == alf[i]){
                count_var++;
            }
        }
        prob_calc[i] = (count_var / count_tab);
    }

    //calc the error for each offset
    for(int offset = 0; offset < ALPHA_SIZE; offset++){
        for(int i = 0; i < ALPHA_SIZE; i++){
            erro[offset] += ((pow(prob_calc[i] - prob_given[(i + offset) % 67], 2))/(prob_given[(i + offset) % 67]));
        }
    }

    //find what error is the smallest
    for(int i = 0; i < 67; i++) {
        if(erro[i] < min_erro)
        {// Update min_erro if a smaller erro is found
            min_erro = erro[i];
            min_offset = i;
        }
    }

    output = (char*)calloc(memory_input + 1,sizeof(char));
    if(output == NULL){
        fprintf(stderr, "Error allocating memory to output\n");
        exit(1);
    }

    for(long int j = 0; j < memory_input; j++)
    {//cyper with the min_offset
        for(int k = 0; k < ALPHA_SIZE; k++){
            if(input[j] == alf[k])
            {//cyper table caracters and put in the output
                output[j] = alf[((k + min_offset) % 67)];          
                break;
            }else if(k == 66)
            {//put the unknown char in the output
                output[j] = input[j];
            }
        }
    }
    if(file_out == NULL)
    {//check if output is defined
        fprintf(stderr, "Error Output file pointer is NULL\n");
    }
    
    fprintf(file_out, "offset com menor erro %d, letra '%c', cifrado com letra '%c', erro quadrático médio: %.6lf.\n", min_offset, alf[min_offset], alf[(67 - min_offset) % 67], min_erro);       
    fprintf(file_out, "%s", output);      
    
    free(output);
    free(input);
    return;
}

void atack_method_3(FILE * file_in, int max_key_len, FILE *file_out){
    char alf[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz .,;-";
    const double prob_given[] = { 
        0.000160079, 0.000554985, 0.000381091, 0.000306662, 0.000263793, 0.000235223,
        0.000214441, 0.00019842, 0.000185551, 0.000174894, 0.002049944, 0.00037499,
        0.000699981, 0.00107497, 0.003174913, 0.000549985, 0.000499986, 0.001524958,
        0.001749952, 3.7499E-05, 0.000192495, 0.000999973, 0.000599984, 0.001674954,
        0.001874949, 0.000474987, 2.37493E-05, 0.001499959, 0.001574957, 0.002274938,
        0.000699981, 0.000244993, 0.000599984, 3.7499E-05, 0.000499986, 1.84995E-05,
        0.061498311, 0.011249691, 0.020999423, 0.032249114, 0.095247384, 0.016499547,
        0.014999588, 0.045748744, 0.052498558, 0.001124969, 0.005774841, 0.029999176,
        0.017999506, 0.05024862, 0.056248455, 0.014249609, 0.00071248, 0.044998764,
        0.047248702, 0.068248126, 0.020999423, 0.007349798, 0.017999506, 0.001124969,
        0.014999588, 0.000554985, 0.190494768, 0.014249609, 0.014999588, 0.000554985,
        0.001124969 };
    int error_check = 0, count_input_filt = 0, count_input_key = 0, min_offset = 0, key_offset = 0, curr_offset = 0, curr_input_ciff = 0, break_loop = 0, best_key_offset = 0, curr_best_key_offset = 0, curr_output = 0;
    long int memory_input = 0;
    char *input = NULL, *input_filt = NULL, *input_key = NULL, *key = NULL, *input_ciff = NULL, *best_key = NULL, *output = NULL;
    double erro[ALPHA_SIZE] = {0}, prob_calc[ALPHA_SIZE] = {0}, output_prob_calc[ALPHA_SIZE] = {0}, count_var = 0, min_erro = INFINITY, erro_key = 0, output_count_var = 0, erro_key_min =INFINITY, curr_input_ciff_aux = 0;
    size_t len_best_key = 0;

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

    key = (char*)calloc(max_key_len + 1,sizeof(char));
    if(key == NULL){
        fprintf(stderr, "Error allocating memory to key\n");
        exit(1);
    }

    best_key = (char*)calloc(max_key_len + 1,sizeof(char));
    if(best_key == NULL){
        fprintf(stderr, "Error allocating memory to best_key\n");
        exit(1);
    }

    output = (char*)calloc(memory_input + 1,sizeof(char));
    if(output == NULL){
        fprintf(stderr, "Error allocating memory to output\n");
        exit(1);
    }

    input_filt = (char*)calloc(memory_input + 1,sizeof(char));
    if(input_filt == NULL){
        fprintf(stderr, "Error allocating memory to input_filt\n");
        exit(1);
    }

    input_ciff = (char*)calloc(memory_input + 1,sizeof(char));
    if(input_ciff == NULL){
        fprintf(stderr, "Error allocating memory to input_ciff\n");
        exit(1);
    }
    
    //put the text in file_in to the input
    size_t bytes_read = fread(input, 1, memory_input, file_in);
    if (bytes_read != memory_input){
        fprintf(stderr, "Error writing the input\n");
        exit(1);
    }
    
    //filter the input to only have known caracters
    for(int i = 0; i < memory_input; i++){
        for(int j = 0; j < ALPHA_SIZE; j++){
            if(input[i] == alf[j]){
                input_filt[count_input_filt] = input[i];
                count_input_filt++;
                break;
            }
        }
    }
    input_filt[count_input_filt] = '\0';

    input_key = (char*)calloc(memory_input + 1,sizeof(char));
    if(input_key == NULL){
        fprintf(stderr, "Error allocating memory to input_key\n");
        exit(1);
    }

    for(int key_len = 1; key_len <= max_key_len; key_len++)
    {//loop with all key lens until we get to max key len
        for(int curr_key_len = 0; curr_key_len < key_len; curr_key_len++)
        {//loop through  curr_key_len until we hit the max key len for that loop
            count_input_key = 0;
            for(int i = 0; i < count_input_filt; i++)
            {//separate the input in current value of key_len
                if(((i + (key_len - curr_key_len)) % key_len) == 0)
                {//this if separates the input in difrents parts to find the key to that part
                    input_key[count_input_key] = input_filt[i];
                    count_input_key++;
                }
            }
            input_key[count_input_key] = '\0';      

            //do what we do in atack 2 to find the single key
            for(int i = 0; i < ALPHA_SIZE; i++){
                count_var = 0;
                for(int j = 0; j < count_input_key; j++){
                    if(input_key[j] == alf[i]){
                        count_var++;
                    }
                }
                prob_calc[i] = (count_var / (double)count_input_key);
            }
            for(int offset = 0; offset < ALPHA_SIZE; offset++){
                erro[offset] = 0;
                for(int i = 0; i < ALPHA_SIZE; i++){
                    erro[offset] += ((pow(prob_calc[i] - prob_given[(i + offset) % 67], 2))/(prob_given[(i + offset) % 67]));
                }
            }
            min_erro = INFINITY;
            min_offset = 0;
            for(int i = 0; i < 67; i++) {
                if(erro[i] < min_erro)
                {// Update min_erro if a larger erro is found
                    min_erro = erro[i];
                    min_offset = i;
                }
            }

            //add the single key calculated above and put it in a array
            key[curr_key_len] = alf[((67 - min_offset) % 67)];
        }
        key[key_len] = '\0';
        
        //cyper the input with each key
        curr_offset = 0;
        curr_input_ciff = 0;
        for(int k = 0; k < memory_input; k++){

            //check if it belongs to table if not continue
            for(int j = 0; j < ALPHA_SIZE; j++){
                if(input[k] == alf[j]){
                    break;
                }else if(j == 66){
                    break_loop = 1;
                    break;
                }
            }
            if(break_loop == 1){
                break_loop = 0;
                continue;
            }

            //find offset for curren position
            for(int j = 0; j < ALPHA_SIZE; j++){
                if(key[curr_offset] == alf[j]){
                    key_offset = j;
                    curr_offset++;
                    if(curr_offset == key_len){
                        curr_offset = 0;
                    }
                    break;
                }     
            }

            //decyper the input
            for(int j = 0; j < ALPHA_SIZE; j++){
                if(input[k] == alf[j]){
                    input_ciff[curr_input_ciff] = alf[((j + ((67 - key_offset) % 67)) % 67)];
                    curr_input_ciff++;
                    break;
                }
            }      
        }
        input_ciff[curr_input_ciff] = '\0';
        
        //count how many char the input_ciff has
        curr_input_ciff_aux = 0;
        for (int i = 0; i < curr_input_ciff; i++){
            for (int j = 0; j < ALPHA_SIZE; j++){
                if (input_ciff[i] == alf[j]){
                    curr_input_ciff_aux++;
                    break;
                }
            }
        }

        //calc the frequency for each character
        for(int i = 0; i < ALPHA_SIZE; i++){
            output_count_var = 0;
            for(int j = 0; j < curr_input_ciff; j++){
                if(input_ciff[j] == alf[i]){
                    output_count_var++;
                }
            }
            output_prob_calc[i] = (output_count_var / curr_input_ciff_aux);
        }
        
        //calc the erro
        erro_key = 0;
        for(int i = 0; i < ALPHA_SIZE; i++){
            erro_key += (pow((output_prob_calc[i] - prob_given[i]), 2)/(prob_given[i]));
        }

        
        if(erro_key < erro_key_min)
        {//checks for the minimum erro if it finds it saves the new best key
            erro_key_min = erro_key;
            strcpy(best_key, key);
            fprintf(file_out, "min \ttamanho chave %d: \"%s\" erro %lf\n", key_len, key, erro_key);
        }else{
            fprintf(file_out, "\ttamanho chave %d: \"%s\" erro %lf\n", key_len, key, erro_key);
        }
    
    }

    //decper the original input with best key
    len_best_key = strlen(best_key);
    for(int k = 0; k < memory_input; k++){

        //check if it belongs to table to decrypt or if not puts the unknown char in the output
        for(int j = 0; j < ALPHA_SIZE; j++){
            if(input[k] == alf[j]){
                break;
            }else if(j == 66){
                output[curr_output] = input[k];
                curr_output++;
                break_loop = 1;
                break;
            }
        }
        if(break_loop == 1){
            break_loop = 0;
            continue;
        }

        //find offset for curren position
        for(int j = 0; j < ALPHA_SIZE; j++){
            if(best_key[curr_best_key_offset] == alf[j]){
                best_key_offset = j;
                curr_best_key_offset++;
                if(curr_best_key_offset == (len_best_key)){
                    curr_best_key_offset = 0;
                }
                break;
            }     
        }

        //decyper the input
        for(int j = 0; j < ALPHA_SIZE; j++){
            if(input[k] == alf[j]){
                output[curr_output] = alf[((j + ((67 - best_key_offset) % 67)) % 67)];
                curr_output++;
                break;
            }
        }      
    }
    output[curr_output] = '\0';

    fprintf(file_out, "%s", output);
    
    free(output);
    free(input);
    free(key);
    free(best_key);
    free(input_filt);
    free(input_ciff);
    free(input_key);
    return;
}

