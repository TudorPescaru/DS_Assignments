// Copyright 2020 Pescaru Tudor-Mihai 315CA
#ifndef DLLIST_H_
#define DLLIST_H_

#include "./header.h"

// Structure that defines all data a node contains
struct Node{
    struct Header *header;
    struct Node *next;
    struct Node *prev;
};

// Structure that defines the elements of a list
struct DLinkedList{
    struct Node *head;
    struct Node *tail;
    struct Node *cursor;
    int size;
};

// Function that initialises the list
void init_list(struct DLinkedList *list);

// Function used to search for a song whithin the playlist
struct Node* search_dup(struct DLinkedList *list, char song[]);

// Fucntion that adds a song on the first postion of the list
void add_first(struct DLinkedList *list, char song[]);
// Function that adds an element at the end of the list
void add_last(struct DLinkedList *list, char song[]);
// Function that adds a song on the next position relative to cursor
void add_after(struct DLinkedList *list, char song[]);

// Remove the first element in the list
int del_first(struct DLinkedList *list);
// Remove the last element in the list
int del_last(struct DLinkedList *list);
// Delete the element the cursor points to
int del_curr(struct DLinkedList *list);
// Delete a particular song
int del_song(struct DLinkedList *list, char song[]);

// Move cursor to the next postion on the right
void move_next(struct DLinkedList *list, FILE *out);
// Move cursor to the next position on the left
void move_prev(struct DLinkedList *list, FILE *out);

// Display information about the first song in the playlist
void show_first(struct DLinkedList *list, FILE *out);
// Display information about the last song in the playlist
void show_last(struct DLinkedList *list, FILE *out);
// Display information about the song the cursor points to
void show_curr(struct DLinkedList *list, FILE *out);
// Display the titles of all songs in the playlist
void show_playlist(struct DLinkedList *list, FILE *out);

// Free all memory allocated to list nodes and node headers
void free_list(struct DLinkedList *list);

#endif  // DLLIST_H_
