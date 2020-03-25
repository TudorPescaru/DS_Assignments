// Copyright 2020 Pescaru Tudor-Mihai 315CA
#ifndef HEADER_H_
#define HEADER_H_

// Structure that defines how header elements are stored
struct Header{
    char tag[4];
    char title[31];
    char artist[31];
    char album[31];
    int year;
};

// Read all header information from mp3 file
void read_header(FILE *songfile, struct Header *header);

// Only read the title from the mp3 file
void read_title(FILE *songfile, char title[]);

#endif  // HEADER_H_
