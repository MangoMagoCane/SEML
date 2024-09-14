// cc -g -o SEML SMLTranspiler.c
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int processor(FILE* f_output, FILE* f_input);
int parser(FILE* f_output, int indent_depth);
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
#define EXTENSION ".seml"
#define INPUT_BUFSIZE 1024
#define PROGRAM_BUFSIZE 1000000
#define TAG_NAME_LEN 20

int main(int argc, char* argv[])
{
    int retval = 0;
    FILE* f_input;
    FILE* f_output;
    char* output_name;
    char* extension_p;

    if (argc != 2) {
        fprintf(stderr, "usage: ./SEMLT file%s", EXTENSION);
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
    processor(f_output, f_input);

close_output:
    fclose(f_input);
close_input:
    fclose(f_input);
exit:
    exit(retval);
}

static char program_buf[PROGRAM_BUFSIZE];
static char* p_buf_p = program_buf;

int processor(FILE* f_output, FILE* f_input)
{
    char input_buf[INPUT_BUFSIZE];
    char* buf_start_p;
    char* buf_p;
    char c;

    for (int line_num = 1; fgets(input_buf, INPUT_BUFSIZE, f_input) != NULL; ++line_num) {
        buf_start_p = input_buf;
        while ((c = *buf_start_p++) == ' ' || c == '\t') { };
        if (c == '\n' || c == '\0') {
            continue;
        }
        buf_p = --buf_start_p;
        while (*buf_p++ != '\0') { }
        while ((c = *--buf_p) == ' ' || c == '\t' || c == '\n' || c == '\0') { }
        *++buf_p = '\0';
        if ((p_buf_p - program_buf) + (buf_p - buf_start_p) >= PROGRAM_BUFSIZE) {
            break; // return out of buffer size
        }
        p_buf_p += sprintf(p_buf_p, "%s ", buf_start_p);
    }
    fprintf(f_output, "%s", "<!DOCTYPE html>");
    p_buf_p = program_buf;
    while (*p_buf_p++ != '(') { };
    parser(f_output, 0);
}

int parser(FILE* f_output, int indent_depth)
{
    int indent_len = indent_depth * INDENT_WIDTH;
    int sub_indent_len = indent_len + INDENT_WIDTH;
    char tag_buf[TAG_NAME_LEN] = { 0 };
    char print_buf[INPUT_BUFSIZE] = { 0 };
    char c;
    int i;

    for (i = 0; (c = *p_buf_p++) != ' ' && c != '\t' && c != '\0' && i < TAG_NAME_LEN; ++i) {
        tag_buf[i] = c;
    }
    fprintf(f_output, "\n%*s<%s>", indent_len, "", tag_buf);
    i = 0;
    while ((c = *(p_buf_p)++) != ')' && c != '\0') {
        if (c == '(') {
            print_buf[i] = '\0';
            if (strlen(print_buf)) {
                fprintf(f_output, "\n%*s%s", sub_indent_len, "", print_buf);
            }
            parser(f_output, indent_depth + 1);
            while (*p_buf_p++ == ' ') { }
            p_buf_p--;
            i = 0;
        } else {
            print_buf[i++] = c;
        }
    }
    if (i) {
        fprintf(f_output, "\n%*s%.*s", sub_indent_len, "", i, print_buf);
    }
    fprintf(f_output, "\n%*s</%s>", indent_len, "", tag_buf);
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