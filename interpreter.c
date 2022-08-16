#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "scanner.h"
#include "source.h"


void print_usage(){
    printf("\nUsage: interpreter [file]\n");
}

long read_source_file(char* file, char* buffer)
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
    buffer = (char*)calloc(numbytes, sizeof(char)); //responsibility of the caller to free this memory
    
    // memory error
    if (buffer == NULL) exit(2);
    
    // copy all the text into the buffer
    fread(buffer, sizeof(char), numbytes, fptr);
    fclose(fptr);

    //return a source object with buffer as data and size as its length
    return numbytes;

}

int run_code(Source* source)
{
    TokensList tokens;
    scan_tokens(source, &tokens); //alloc tokens.arr[0-size].value, tokens->arr

    //print tokens for now
    for(int i=0; tokens.arr[i].type != EOFI; i++)
    {
        printf("\"%s\"\n", tokens.arr[i].value);
        free(tokens.arr[i].value); // free tokens.arr[0-size].value
    }

    // NOTE: Check whether all tokens.arr[0-size].value are freed
    free(tokens.arr); // free-2 token->arr
    // return status code
    return 0;
}

int run_interactive(){

    //create a source object with line and its size
    Source source;
    int status = 0;
    char* buffer;
    while (true) {

        //display a prompt
        printf("%s", "> ");

        //read a line from stdin and store its size
        size_t size;
        source.size = getline(&buffer, &size, stdin);   // alloc-0 buffer

        //scanf returns NULL when Ctrl+D (EOF) is pressed
        if (source.size == -1) break;
        
        //initialize source
        source.current = 0;

        // save char* to a const char* to avoid unwanted modification
        source.data = buffer;

        // run the line
        status = run_code(&source);

    }

    free(buffer); // free-0 buffer

    return status;

}

int run_file(char* file_path)
{
    Source source;
    // read file contents from file passed as argument in argv[1] and return a source instance
    char* buffer;
    source.size = read_source_file(file_path, buffer); // alloc-1 buffer
    
    // save char* to a const char* to avoid unwanted modification
    source.data = buffer;
    
    // run the source and get its exit status 
    int status = run_code(&source);
    
    free(buffer); // free-1 buffer
    
    // return the status to calling program
    return status;
}

int main(int argc, char** argv)
{
    if (argc == 1) return run_interactive();
    else if (argc == 2) return run_file(argv[1]);
    else print_usage(); return 0;
}