// Copyright 2020 Pescaru Tudor-Mihai 315CA
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./dllist.h"
#include "./header.h"


// Function that initialises the list
void init_list(struct DLinkedList *list) {
    list->head = NULL;
    list->tail = NULL;
    list->cursor = NULL;
    list->size = 0;
}

// Function used to search for a song whithin the playlist
struct Node* search_dup(struct DLinkedList *list, char song[]) {
    if (list == NULL || list->head == NULL) {
        return NULL;
    }
    struct Node* curr;
    int ok = 0;
    curr = list->head;
    while (curr != NULL) {
        if (strcmp(curr->header->title, song) == 0) {
            ok = 1;
            break;
        }
        curr = curr->next;
    }
    // A pointer to the node conatining the song is returned if found
    if (ok) {
        return curr;
    } else {
        return NULL;
    }
}

// Fucntion that adds a song on the first postion of the list
void add_first(struct DLinkedList *list, char song[]) {
    if (list == NULL) {
        return;
    }
    struct Node *new;
    // Finds and deletes the song if it was already present
    del_song(list, song);
    // Reading all header data
    FILE *songfile = fopen(song, "rb");
    if (!songfile) {
        printf("Error: could not open file\n");
        return;
    }
    new = (struct Node*)malloc(sizeof(struct Node));
    new->header = (struct Header*)malloc(sizeof(struct Header));
    read_header(songfile, new->header);
    if (list->size == 0) {
        // This is the first node added
        list->head = new;
        list->tail = new;
        list->cursor = new;
        new->next = NULL;
        new->prev = NULL;
        list->size++;
    } else {
        // The list already conatins one or more elements
        new->next = list->head;
        list->head->prev = new;
        new->prev = NULL;
        list->head = new;
        list->size++;
    }
    fclose(songfile);
}

// Function that adds an element at the end of the list
void add_last(struct DLinkedList *list, char song[]) {
    if (list == NULL) {
        return;
    }
    struct Node *new;
    // Finds and deletes the song if it was already present
    del_song(list, song);
    // Reading all header data
    FILE *songfile = fopen(song, "rb");
    if (!songfile) {
        printf("Error: could not open file\n");
        return;
    }
    new = (struct Node*)malloc(sizeof(struct Node));
    new->header = (struct Header*)malloc(sizeof(struct Header));
    read_header(songfile, new->header);
    if (list->size == 0) {
        // This is the first node added
        list->head = new;
        list->tail = new;
        list->cursor = new;
        new->next = NULL;
        new->prev = NULL;
        list->size++;
    } else {
        // The list already conatins one or more elements
        struct Node *curr;
        new->next = NULL;
        curr = list->tail;
        list->tail = new;
        curr->next = new;
        new->prev = curr;
        list->size++;
    }
    fclose(songfile);
}

// Function that adds a song on the next position relative to cursor
void add_after(struct DLinkedList *list, char song[]) {
    if (list == NULL || list->cursor == NULL) {
        return;
    }
    struct Node *new, *next;
    char title[31];
    FILE *songfile = fopen(song, "rb");
    if (!songfile) {
        printf("Error: could not open file\n");
        return;
    }
    // Determine whether the song the cursor points to is the song to be added
    read_title(songfile, title);
    if (strcmp(list->cursor->header->title, title) == 0) {
        fclose(songfile);
        return;
    }
    // Finds and deletes the song if it was already present
    del_song(list, song);
    // Reading all header data
    new = (struct Node*)malloc(sizeof(struct Node));
    new->header = (struct Header*)malloc(sizeof(struct Header));
    read_header(songfile, new->header);
    if (list->cursor->next != NULL) {
        // The item the cursor points to is the only item in the list
        next = list->cursor->next;
        list->cursor->next = new;
        new->next = next;
        new->prev = list->cursor;
        next->prev = new;
        list->size++;
    } else {
        // There are more items than just the one the cursor points to
        list->cursor->next = new;
        new->prev = list->cursor;
        new->next = NULL;
        list->tail = new;
        list->size++;
    }
    fclose(songfile);
}

// Remove the first element in the list
int del_first(struct DLinkedList *list) {
    if (list == NULL || list->head == NULL) {
        return -1;
    }
    if (list->size == 1) {
        // If the first element is the only element in the list
        free(list->head->header);
        free(list->head);
        list->head = NULL;
        list->tail = NULL;
        list->cursor = NULL;
        list->size--;
    } else {
        // If there are more than one elements in the list
        struct Node *curr;
        curr = list->head;
        // Move the cursor if it points to deleted song
        if (list->cursor == curr) {
            list->cursor = curr->next;
        }
        list->head = curr->next;
        list->head->prev = NULL;
        list->size--;
        free(curr->header);
        free(curr);
    }
    return 0;
}

// Remove the last element in the list
int del_last(struct DLinkedList *list) {
    if (list == NULL || list->tail == NULL) {
        return -1;
    }
    if (list->size == 1) {
        // If the last element is the only element in the list
        free(list->tail->header);
        free(list->tail);
        list->head = NULL;
        list->tail = NULL;
        list->cursor = NULL;
        list->size--;
    } else {
        // If there are more than one elemnts in the list
        struct Node *curr;
        curr = list->tail;
        // Move the cursor if it points to deleted song
        if (list->cursor == curr) {
            list->cursor = curr->prev;
        }
        list->tail = curr->prev;
        list->tail->next = NULL;
        list->size--;
        free(curr->header);
        free(curr);
    }
    return 0;
}

// Delete the element the cursor points to
int del_curr(struct DLinkedList *list) {
    if (list == NULL || list->cursor == NULL) {
        return -1;
    }
    if (list->size == 1) {
        // If the element the cursor points to is the only element in the list
        free(list->cursor->header);
        free(list->cursor);
        list->head = NULL;
        list->tail = NULL;
        list->cursor = NULL;
        list->size--;
    } else {
        if (list->cursor->prev == NULL) {
            // If the cursor points to the first element
            del_first(list);
        } else {
            if (list->cursor->next == NULL) {
                // If the cursor points to the last element
                del_last(list);
            } else {
                // If the cursor points to an element that is not the list edge
                struct Node *curr, *next, *prev;
                curr = list->cursor;
                // Move cursor to the right if possible, to the left if not
                if (curr->next != NULL) {
                    list->cursor = curr->next;
                } else {
                    list->cursor = curr->prev;
                }
                next = curr->next;
                prev = curr->prev;
                next->prev = prev;
                prev->next = next;
                list->size--;
                free(curr->header);
                free(curr);
            }
        }
    }
    return 0;
}

// Delete a particular song
int del_song(struct DLinkedList *list, char song[]) {
    if (list == NULL || list->head == NULL) {
        return -1;
    }
    struct Node *prev, *next, *find;
    char title[31];
    // Get the songs title from mp3 file
    FILE *songfile = fopen(song, "rb");
    if (!songfile) {
        printf("Error: could not open file\n");
        return 0;
    }
    read_title(songfile, title);
    fclose(songfile);
    // Find the postion of the song if it exists
    find = search_dup(list, title);
    if (find == NULL) {
        return -1;
    }
    next = find->next;
    prev = find->prev;
    if (prev == NULL) {
        // If song to be deleted is the first in the list
        del_first(list);
    } else {
        if (next == NULL) {
            // If the song to be deleted is the last in the list
            del_last(list);
        } else {
            // If the song is not one of the list bounds
            if (list->cursor == find) {
                // Move cursor to the right if it points to this song
                list->cursor = next;
            }
            prev->next = next;
            next->prev = prev;
            list->size--;
            free(find->header);
            free(find);
        }
    }
    return 0;
}

// Move cursor to the next postion on the right
void move_next(struct DLinkedList *list, FILE *out) {
    if (list->cursor == NULL) {
        fprintf(out, "Error: no track playing\n");
        return;
    }
    if (list->cursor->next != NULL) {
        list->cursor = list->cursor->next;
    }
}

// Move cursor to the next position on the left
void move_prev(struct DLinkedList *list, FILE *out) {
    if (list->cursor == NULL) {
        fprintf(out, "Error: no track playing\n");
        return;
    }
    if (list->cursor->prev != NULL) {
        list->cursor = list->cursor->prev;
    }
}

// Display information about the first song in the playlist
void show_first(struct DLinkedList *list, FILE *out) {
    if (list->head == NULL) {
        fprintf(out, "Error: show empty playlist\n");
        return;
    }
    fprintf(out, "Title: %.30s\n", list->head->header->title);
    fprintf(out, "Artist: %.30s\n", list->head->header->artist);
    fprintf(out, "Album: %.30s\n", list->head->header->album);
    fprintf(out, "Year: %d\n", list->head->header->year);
}

// Display information about the last song in the playlist
void show_last(struct DLinkedList *list, FILE *out) {
    if (list->tail == NULL) {
        fprintf(out, "Error: show empty playlist\n");
        return;
    }
    fprintf(out, "Title: %.30s\n", list->tail->header->title);
    fprintf(out, "Artist: %.30s\n", list->tail->header->artist);
    fprintf(out, "Album: %.30s\n", list->tail->header->album);
    fprintf(out, "Year: %d\n", list->tail->header->year);
}

// Display information about the song the cursor points to
void show_curr(struct DLinkedList *list, FILE *out) {
    if (list->cursor == NULL) {
        fprintf(out, "Error: show empty playlist\n");
        return;
    }
    fprintf(out, "Title: %.30s\n", list->cursor->header->title);
    fprintf(out, "Artist: %.30s\n", list->cursor->header->artist);
    fprintf(out, "Album: %.30s\n", list->cursor->header->album);
    fprintf(out, "Year: %d\n", list->cursor->header->year);
}

// Display the titles of all songs in the playlist
void show_playlist(struct DLinkedList *list, FILE *out) {
    if (list == NULL) {
        return;
    }
    if (list->head == NULL) {
        fprintf(out, "[]\n");
        return;
    }
    struct Node *curr;
    curr = list->head;
    fprintf(out, "[");
    while (curr->next != NULL) {
        fprintf(out, "%.30s; ", curr->header->title);
        curr = curr->next;
    }
    fprintf(out, "%.30s]\n", curr->header->title);
}

// Free all memory allocated to list nodes and node headers
void free_list(struct DLinkedList *list) {
    if (list == NULL) {
        return;
    }
    if (list->head == NULL) {
        free(list);
        return;
    }
    struct Node *curr, *node;
    curr = list->head;
    while (curr != list->tail) {
        node = curr;
        curr = curr->next;
        free(node->header);
        free(node);
    }
    free(curr->header);
    free(curr);
    free(list);
}
