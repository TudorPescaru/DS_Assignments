// Copyright 2020 Pescaru Tudor-Mihai 315CA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./header.h"

// Read all header information from mp3 file
void read_header(FILE *songfile, struct Header *header) {
    char tostr[4];
    memset(header, 0, 101);
    fseek(songfile, -97, SEEK_END);
    fread(header->tag, 3, 1, songfile);
    fread(header->title, 30, 1, songfile);
    fread(header->artist, 30, 1, songfile);
    fread(header->album, 30, 1, songfile);
    fread(tostr, 4, 1, songfile);
    header->year = atoi(tostr);
}

// Only read the title from the mp3 file
void read_title(FILE *songfile, char title[]) {
    memset(title, 0, 31);
    fseek(songfile, -94, SEEK_END);
    fread(title, 30, 1, songfile);
}
