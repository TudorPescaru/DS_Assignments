// Copyright 2020 Pescaru Tudor-Mihai 315CA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./dllist.h"
#include "./header.h"

#define INPLEN 128
#define CMDLEN 100
#define SLEN 100

int main(int argc, char* argv[]) {
    struct DLinkedList *list;
    int q, i, len, err;
    char input[INPLEN], cmd[CMDLEN], song[SLEN];
    // Check if command was used correctly
    if (argc != 3) {
        fprintf(stderr, "Correct usage is: ./tema1 <in_file> <out_file>\n");
        return -1;
    }
    // Initialise list used for playlist
    list = (struct DLinkedList*)malloc(sizeof(struct DLinkedList));
    init_list(list);
    // Open the input and output files
    FILE *in = fopen(argv[1], "rt");
    if (!in) {
        printf("Error: could not open file\n");
        return -1;
    }
    FILE *out = fopen(argv[2], "wt");
    if (!out) {
        printf("Error: could not open file\n");
        return -1;
    }
    // Read number of inputs and start reading each command
    fscanf(in, "%d\n", &q);
    for (i = 0; i < q; i++) {
        fgets(input, INPLEN, in);
        input[strlen(input) - 1] = '\0';
        // Check if file name is given alongside command
        if (strlen(input) > 13) {
            // Separate command and file name
            sscanf(input, "%s", cmd);
            len = strlen(input) - strlen(cmd);
            snprintf(song, sizeof("songs/"), "%s", "songs/");
            snprintf(song + 6, len, "%s", input + strlen(cmd) + 1);
            // Determine whether it was an add or delete command
            if (cmd[0] == 'A') {
                // Check what add command was given
                if (cmd[4] == 'F') {
                    add_first(list, song);
                }
                if (cmd[4] == 'L') {
                    add_last(list, song);
                }
                if (cmd[4] == 'A') {
                    add_after(list, song);
                }
            } else {
                // Only del_song command contains file name
                err = del_song(list, song);
                if (err < 0) {
                    fprintf(out, "Error: no song found to delete\n");
                }
            }
        } else {
            // Read command
            sscanf(input, "%s", cmd);
            if (cmd[0] == 'D') {
                // Check what delete command was given
                if (cmd[4] == 'F') {
                    err = del_first(list);
                    if (err < 0) {
                        fprintf(out, "Error: delete from empty playlist\n");
                    }
                }
                if (cmd[4] == 'L') {
                    err = del_last(list);
                    if (err < 0) {
                        fprintf(out, "Error: delete from empty playlist\n");
                    }
                }
                if (cmd[4] == 'C') {
                    err = del_curr(list);
                    if (err < 0) {
                        fprintf(out, "Error: no track playing\n");
                    }
                }
            }
            // Check what move command was given
            if (cmd[0] == 'M') {
                if (cmd[5] == 'N') {
                    move_next(list, out);
                }
                if (cmd[5] == 'P') {
                    move_prev(list, out);
                }
            }
            // Check what show command was given
            if (cmd[0] == 'S') {
                if (cmd[5] == 'F') {
                    show_first(list, out);
                }
                if (cmd[5] == 'L') {
                    show_last(list, out);
                }
                if (cmd[5] == 'C') {
                    show_curr(list, out);
                }
                if (cmd[5] == 'P') {
                    show_playlist(list, out);
                }
            }
        }
    }
    // Clean up by freeing all alocated memory and closing files
    free_list(list);
    fclose(in);
    fclose(out);
    return 0;
}
