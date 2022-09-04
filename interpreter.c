#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "Includes/scanner.h"
#include "Includes/source.h"
#include "Includes/utils.h"


void print_usage(){
    printf("\nUsage: interpreter [file]\n");
}

void free_tokens(TokensList* tokens)
{
    //Free each token's value
    for(int i=0; tokens->arr[i].type != EOFI; i++)
    {
        free(tokens->arr[i].value); // free tokens.arr[0-size].value
    }

    //free all tokens
    free(tokens->arr); // free-2 token->arr
}

int run_code(Source* source)
{
    //initialize source
    source->current = 0;
    source->line = 1;

    //Get Tokens
    TokensList tokens;
    scan_tokens(source, &tokens); //alloc tokens.arr[0-size].value, tokens->arr

    //print tokens for now
    for(int i=0; tokens.arr[i].type != EOFI; i++)
    {
        printf("\"%s\"\n", tokens.arr[i].value);
    }

    free_tokens(&tokens); //free-2 tokens->arr,  tokens.arr[0-size].value, 
    // return status code
    return 0;
}

int run_interactive(){

    //create a source object with line and its size
    Source source;
    int status = 0;
    char* buffer;
    size_t size=0;
    while (true) {

        //display a prompt
        printf("%s", "> ");

        //read a line from stdin and store its size

        source.size = getline(&buffer, &size, stdin);   // alloc-0 buffer

        //scanf returns NULL when Ctrl+D (EOF) is pressed
        if (source.size == -1) break;

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
    // read file contents from file passed as argument in argv[1]
    char* buffer;
    source.size = read_source_file(file_path, &buffer); // alloc-1 buffer
    
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