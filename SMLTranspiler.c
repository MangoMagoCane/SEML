#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int processor(FILE* f_output, FILE* f_input);
int checkExtension(char* filename, char** filename_extension_p, char* extension);

char* void_elements[] = {
    "area",
    "base",
    "br",
    "col",
    "embedj",
    "hr",
    "img",
    "input",
    "link",
    "meta",
    "source",
    "track",
    "wbr",
};

enum err {
    INVALID_ARGUMENT_COUNT,
    INVALID_FILE,
    INVALID_FILE_EXTENSION,
    FAILED_MALLOC
};

#define INDENT_WIDTH 2
#define EXTENSION ".sml"
#define INPUT_BUFSIZE 1024
#define ELEMENT_STACK_SIZE 1024

int main(int argc, char* argv[])
{
    int retval = 0;
    FILE* f_input;
    FILE* f_output;
    char* output_name;
    char* extension_p;

    if (argc != 2) {
        fprintf(stderr, "usage: ./SMLTranspiler file.sml");
        retval = INVALID_ARGUMENT_COUNT;
        goto exit;
    }
    if (checkExtension(argv[1], &extension_p, EXTENSION) == false) {
        fprintf(stderr, "file: %s has invalid extension: %s, expected: %s\n", argv[1], extension_p, EXTENSION);
        retval = INVALID_FILE;
        goto exit;
    }
    if ((f_input = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "cannot open: %s\n", argv[1]);
        retval = INVALID_FILE_EXTENSION;
        goto exit;
    }

    extension_p[0] = '\0';
    output_name = malloc(strlen(argv[1]) + 6);
    sprintf(output_name, "%s%s", argv[1], ".html");
    f_output = fopen(output_name, "w");
    free(output_name);

    if (f_output == NULL) {
        fprintf(stderr, "cannot create output file: %s\n", f_output);
        retval = INVALID_FILE_EXTENSION;
        goto close_input;
    }

close_output:
    fclose(f_input);
close_input:
    fclose(f_input);
exit:
    exit(retval);
}

int processor(FILE* f_output, FILE* f_input)
{
    char input_buf[INPUT_BUFSIZE];
    int indent_depth = 0;

    for (int line_num = 1; fgets(input_buf, INPUT_BUFSIZE, f_input) != NULL; line_num++) {
    }
}

int checkExtension(char* filename, char** filename_extension_p, char* extension)
{
    char* dot = strrchr(filename, '.');
    *filename_extension_p = dot;
    if (!dot || dot == filename || strcmp(dot, extension) != 0) {
        return false;
    }
    return true;
}