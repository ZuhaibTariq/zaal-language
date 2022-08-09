#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"
#include "scanner.c"
#include "source.c"
#include <string.h>

void print_usage(){
    printf("\nUsage: interpreter [file]\n");
}

Source* read_source_file(char* file)
{
    // open an existing file for reading
    FILE* fptr = fopen(file, "r");

    // exit if the file does not exist
    if(fptr == NULL) exit(1);

    // Get the number of bytes
    fseek(fptr, 0L, SEEK_END);
    long numbytes = ftell(fptr);

    // reset the file position indicator to the beginning of the file
    fseek(fptr, 0L, SEEK_SET);

    // grab sufficient memory for the buffer to hold the text
    char* buffer = (char*)calloc(numbytes, sizeof(char));
    
    // memory error
    if (buffer == NULL) exit(2);
    
    // copy all the text into the buffer
    fread(buffer, sizeof(char), numbytes, fptr);
    fclose(fptr);

    //return a source object with buffer as data and size as its length
    return new_source(numbytes, buffer);

}

int run_code(Source* source)
{
    TokensList* tokens = scan_tokens(source);

    //print tokens for now
    //TODO: find a way to remove x array for printing
    char x[200] = {'\0'};
    for(int i=0; tokens->arr[i].type == EOFI; i++)
    {
        int start_index = tokens->arr[i].s_offset;
        int end_index = tokens->arr[i].e_offset;
        int size = end_index - start_index;
        strncpy(x, source->data+start_index, size);
        printf("\"%s\"\n", x);
    }

    //free tokens
    free(tokens);
    // return status code
    return 0;
}

int run_interactive(){

    //create a source object with line and its size
    Source* source = new_source(0, "");
    int status = 0;
    while (true) {

        //display a prompt
        printf("%s", "> ");

        //read a line from stdin and store its size
        size_t size;
        source->size = getline(&source->data, &size, stdin);

        //scanf returns NULL when Ctrl+D (EOF) is pressed
        if (source->size == -1) break;
        
        //initialize source current to 0
        source->current = 0;
        
        // run the line
        status = run_code(source);

    }

    // free source from heap
    free(source);

    return status;

}

int run_file(char* file_path)
{
     // read file contents from file passed as argument in argv[1] and return a source instance
        Source* code = read_source_file(file_path);
        
        // run the source and get its exit status 
        int status = run_code(code);
        
        // free source memory from heap
        free_source(code);
        
        // return the status to calling program
        return status;
}

int main(int argc, char** argv)
{
    if (argc == 1) return run_interactive();
    else if (argc == 2) return run_file(argv[1]);
    else print_usage(); return 0;
}