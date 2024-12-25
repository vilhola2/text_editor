#include "text_editor.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void main_loop(char *filename);
void print_help();
int32_t edit_line(char *buffer, int32_t current_line);
int32_t get_current_line(int32_t line_count);

int main(int argc, char **argv) {
    if(argc != 2) {
        printf("Usage: %s 'file_name'\n", argv[0]);
        return EXIT_FAILURE;
    }
    main_loop(argv[1]);
    return EXIT_SUCCESS;
}

void main_loop(char *filename) {
    char *buffer = file_read(filename);
    if(buffer == NULL) return;

    int32_t line_count = 0;
    int32_t current_line;

    str_print(buffer, &line_count);
    printf("Enter 'h' to see a list of commands or 'q' to quit\n");
    char opt;
    while(opt != 'q') {
        printf("> ");
        scanf(" %c", &opt);
        clear_input_buffer();
        switch (opt) {
            case 'q':
                break;
            case 'h':
                print_help();
                break;
            case 'w':
                file_write(filename, buffer);
                break;
            case 'p':
                str_print(buffer, &line_count);
                break;
            case 'e': current_line = get_current_line(line_count);
                if(edit_line(buffer, current_line) == ERR) return;
                line_count = str_get_line_count(buffer);
                break;
            default:
                printf("Invalid option! Try again:\n");
        }
    }
    free(buffer);
}

void print_help() {
    printf( "'p' -- print the contents of the buffer\n"
            "'e' -- edit a single line\n"
            // todo "'a' -- append a single line to the end of the buffer"
            "'h' -- print this list\n"
            "'q' -- quit\n");
}

int32_t edit_line(char *buffer, int32_t current_line) {
    char *buffer_start = strdup(buffer);
    char *buffer_end = buffer;

    for(int i = 1; i < current_line; ++i) {
        buffer_end = strchr(buffer_end, '\n') + 1;
    }

    buffer_start[buffer_end - buffer] = '\0';
    buffer_end = strchr(buffer_end, '\n');

    size_t newline_size = 16;
    char *newline = str_input(&newline_size);
    if(!newline) {
        printf("Function str_input failed!\n");
        free(buffer);
        free(buffer_start);
        free(newline);
        return ERR;
    }

    buffer = realloc(buffer, strlen(buffer_start) + newline_size + strlen(buffer_end));
    strcpy(buffer, buffer_start);
    strcat(buffer, newline);
    strcat(buffer, buffer_end);

    free(newline);
    free(buffer_start);
    return NOERR;
}

int32_t get_current_line(int32_t line_count) {
    int current_line;
    printf("Enter line number: ");
    while(1) {
        if(!scanf("%d", &current_line)) {
            printf("Invalid line number!\n");
            printf("Try again: ");
            clear_input_buffer();
            continue;
        }
        clear_input_buffer();
        if(current_line < 1 || current_line > line_count) {
            printf("Error: Non-existent line!\n");
            printf("Try again: ");
            continue;
        }
        break;
    }
    return current_line;
}