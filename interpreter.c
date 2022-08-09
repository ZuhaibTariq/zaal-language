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
    scan_tokens(source);

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

        //read a line from stdin
        scanf("%s", source->data);
        
        //scanf returns NULL when Ctrl+D (EOF) is pressed
        if (source->data == NULL) break;

        // store line and its size in source instance
        source->size = strlen(source->data);
        
        // run the line
        status = run_code(source);

    }

    // free source from heap
    free_source(source);

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