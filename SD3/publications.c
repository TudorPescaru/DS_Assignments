/* Copyright 2020 Pescaru Tudor-Mihai */
#include "publications.h"

#define HTSIZE 35797

/* Generically defined Node struct */

typedef struct node {
    struct node *next, *prev;
    void *data;
} Node;

/* Generically defined LinkedList struct */

typedef struct linked_list {
    Node *head, *tail;
    int size;
} LinkedList;

/* Generically defined HashTable struct */

typedef struct hash_table {
    LinkedList **buckets;
    int size;
} HashTable;

/**
 * Struct defined to store all relevant information for papers
 * (used across all tasks)
 */

typedef struct paper_data {
    char *title;
    char *venue;
    int year;
    char **author_names;
    int64_t *author_ids;
    char **institutions;
    int num_authors;
    char **fields;
    int num_fields;
    int64_t id;
    int64_t *references;
    int num_refs;
} PaperData;

/**
 * Struct defined to store the number of citations for a paper
 * (used in tasks 1, 2, 3, 5, 8)
 */

typedef struct cit_data {
    int citations;
    int64_t id;
} CitData;

/**
 * Struct defined to store just the venue of a paper
 * (used in task 2)
 */

typedef struct ven_data {
    char *venue;
    int64_t id;
} VenData;

/**
 * Struct defined to store just the field of a paper
 * (used in task 5, 7)
 */

typedef struct field_data {
    char *field;
    int64_t id;
} FieldData;

/**
 * Struct defined to store the id that influences another paper
 * (used in task 3)
 */

typedef struct influence_data {
    int64_t id;
    int64_t infl_id;
} InflData;

/**
 * Struct defined to store the ids of all the papers an author worked on
 * (used in task 8)
 */

typedef struct author_data {
    int64_t id;
    LinkedList *papers;
} AuthorData;

/**
 * Struct defined to store all the collaborators of an author
 * (used in task 4)
 */

typedef struct collab_data {
    int64_t id;
    LinkedList *collabs;
} CollabData;

/**
 * Main struct that stores HashTables used in solving all tasks
 * the years array is used in task 6
 */

struct publications_data {
    /* HashTable that stores paper data based on paper id */
    HashTable *main;
    /* Hashtable that stores citations data based on paper id */
    HashTable *cit;
    /* Hashtable that stores venue data based on venue name*/
    HashTable *ven;
    /* Hashtable that stores field data based on field name*/
    HashTable *field;
    /* Hashtable that stores influence data based on paper id */
    HashTable *infl;
    /* Hashtable that stores author paper data based on author id */
    HashTable *auth;
    /* Hashtable that stores collaborators data based on author id */
    HashTable *col;
    /* Array that stores number of papers based on paper year */
    int *years;
};

/* Hashing function for strings (taken from lab 4) */

static unsigned int hash(void *s) {
    unsigned char *puchar_s = (unsigned char*)s;
    unsigned int hash = 5381;
    int c;

    while ((c = *puchar_s++)) {
        hash = ((hash << 5u) + hash) + c;
    }

    return hash;
}

/* List initialization function */

static void init_list(LinkedList *list) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

/* Generic node adding function */

static void add_node(LinkedList *list, void *new_data) {
    Node *new;
    if (list == NULL) {
        return;
    }
    new = (Node*)malloc(sizeof(Node));
    DIE(new == NULL, "malloc - new node");
    new->data = new_data;
    new->next = NULL;
    new->prev = NULL;
    if (list->size == 0) {
        list->head = new;
        list->tail = new;
    } else {
        list->tail->next = new;
        new->prev = list->tail;
        list->tail = new;
    }
    list->size++;
}

/* Function that initializes and adds a CitData struct for a paper, to a list */

static void add_cit(PublData *data, int64_t id) {
    CitData *citdata;
    int index;
    if (data == NULL) {
        return;
    }
    index = id % data->cit->size;
    citdata = (CitData*)malloc(sizeof(CitData));
    DIE(citdata == NULL, "malloc - citdata");
    citdata->id = id;
    citdata->citations = 0;
    add_node(data->cit->buckets[index], citdata);
}

/* Function that initializes and adds a VenData struct for a paper, to a list */

static void add_ven(PublData *data, int64_t id, char *venue) {
    VenData *vendata;
    int index;
    if (data == NULL) {
        return;
    }
    index = hash(venue) % data->ven->size;
    vendata = (VenData*)malloc(sizeof(VenData));
    DIE(vendata == NULL, "malloc - vendata");
    vendata->id = id;
    vendata->venue = (char*)malloc((strlen(venue) + 1) * sizeof(char));
    DIE(vendata->venue == NULL, "malloc - vendata->venue");
    snprintf(vendata->venue, strlen(venue) + 1, "%s", venue);
    add_node(data->ven->buckets[index], vendata);
}

/**
 * Function that initializes and adds a FieldData struct 
 * for a paper, to a list
 */

static void add_field(PublData *data, int64_t id, char *field) {
    FieldData *fielddata;
    int index;
    if (data == NULL) {
        return;
    }
    index = hash(field) % data->field->size;
    fielddata = (FieldData*)malloc(sizeof(FieldData));
    DIE(fielddata == NULL, "malloc - fielddata");
    fielddata->id = id;
    fielddata->field = (char*)malloc((strlen(field) + 1) * sizeof(char));
    DIE(fielddata->field == NULL, "malloc - fielddata->field");
    snprintf(fielddata->field, strlen(field) + 1, "%s", field);
    add_node(data->field->buckets[index], fielddata);
}

/**
 * Function that initializes and adds an InflData struct 
 * for a paper, to a list
 */

static void add_infl(PublData *data, int64_t id, int64_t infl_id) {
    InflData *infldata;
    int index;
    if (data == NULL) {
        return;
    }
    index = id % data->infl->size;
    infldata = (InflData*)malloc(sizeof(InflData));
    DIE(infldata == NULL, "malloc - infldata");
    infldata->id = id;
    infldata->infl_id = infl_id;
    add_node(data->infl->buckets[index], infldata);
}

/**
 * Function that initializes and adds an AuthorData struct 
 * for a paper, to a list
 */

static void add_author(PublData *data, int64_t a_id, int64_t p_id) {
    AuthorData *authordata;
    LinkedList *list;
    Node *curr;
    int index, found = 0;
    int64_t *id;
    if (data == NULL) {
        return;
    }
    id = (int64_t*)malloc(sizeof(int64_t));
    DIE(id == NULL, "malloc - id");
    *id = p_id;
    index = a_id % data->auth->size;
    list = data->auth->buckets[index];
    /* We first check to see if we need to add information or just update it */
    if (list->size != 0) {
        curr = list->head;
        while (curr != NULL) {
            authordata = (AuthorData*)curr->data;
            if (authordata->id == a_id) {
                found = 1;
                break;
            }
            curr = curr->next;
        }
    }
    if (found) {
        add_node(authordata->papers, id);
    } else {
        authordata = (AuthorData*)malloc(sizeof(AuthorData));
        DIE(authordata == NULL, "malloc - authordata");
        authordata->id = a_id;
        authordata->papers = (LinkedList*)malloc(sizeof(LinkedList));
        DIE(authordata->papers == NULL, "malloc - authordata->papers");
        init_list(authordata->papers);
        add_node(authordata->papers, id);
        add_node(data->auth->buckets[index], authordata);
    }
}

/**
 * Function that initializes and adds a CollabData struct 
 * for a paper, to a list
 */

static void add_collaborator(PublData *data, int64_t a_id, int64_t c_id) {
    CollabData *collabdata;
    LinkedList *list;
    Node *curr;
    int index, found = 0;
    int64_t *id;
    if (data == NULL) {
        return;
    }
    id = (int64_t*)malloc(sizeof(int64_t));
    DIE(id == NULL, "malloc - id");
    *id = c_id;
    index = a_id % data->col->size;
    list = data->col->buckets[index];
    /* We first check to see if we need to add information or just update it */
    if (list->size != 0) {
        curr = list->head;
        while (curr != NULL) {
            collabdata = (CollabData*)curr->data;
            if (collabdata->id == a_id) {
                found = 1;
                break;
            }
            curr = curr->next;
        }
    }
    if (found) {
        add_node(collabdata->collabs, id);
    } else {
        collabdata = (CollabData*)malloc(sizeof(CollabData));
        DIE(collabdata == NULL, "malloc - collabdata");
        collabdata->id = a_id;
        collabdata->collabs = (LinkedList*)malloc(sizeof(LinkedList));
        DIE(collabdata->collabs == NULL, "malloc - collabdata->collabs");
        init_list(collabdata->collabs);
        add_node(collabdata->collabs, id);
        add_node(data->col->buckets[index], collabdata);
    }
}

/**
 * Function that removes a node from the start of a list
 * (used in emulating a queue)
 */

static void remove_front(LinkedList *list) {
    Node *curr;
    if (list == NULL) {
        return;
    }
    curr = list->head;
    list->head = list->head->next;
    list->size--;
    free(curr);
}

/**
 * Function that removes a node from the end of a list
 * (used in emulating a stack)
 */

static void remove_back(LinkedList *list) {
    Node *curr;
    if (list == NULL) {
        return;
    }
    curr = list->tail;
    list->tail = list->tail->prev;
    list->size--;
    free(curr);
}

/* Generic function that determines if an element is in a list */

static int in_list(LinkedList* list, void *data) {
    Node *curr;
    if (list == NULL || list->size == 0) {
        return 0;
    }
    curr = list->head;
    while (curr != NULL) {
        if (curr->data == data) {
            return 1;
        }
        curr = curr->next;
    }
    return 0;
}

/* Particularized function that determines if an id is in a list */

static int in_list_id(LinkedList *list, int64_t id) {
    Node *curr;
    int64_t find;
    if (list == NULL || list->size == 0) {
        return 0;
    }
    curr = list->head;
    while (curr != NULL) {
        find = *((int64_t*)curr->data);
        if (find == id) {
            return 1;
        }
        curr = curr->next;
    }
    return 0;
}

/**
 * Function that finds the element used to store the number
 * of citations for a paper if it has been cited
 */

static CitData* has_citations(PublData *data, int64_t id) {
    int index;
    Node *curr;
    LinkedList *list;
    CitData *cit;
    if (data == NULL) {
        return NULL;
    }
    index = id % data->cit->size;
    list = data->cit->buckets[index];
    if (list->size != 0) {
        curr = list->head;
        while (curr != NULL) {
            cit = (CitData*)curr->data;
            if (cit->id == id) {
                return cit;
            }
            curr = curr->next;
        }
    }
    return NULL;
}

/* Function that finds the element used to store paper data for a paper */

static PaperData* find_paper(PublData *data, int64_t id) {
    int index;
    Node *curr;
    LinkedList *list;
    PaperData *paper;
    if (data == NULL) {
        return NULL;
    }
    index = id % data->main->size;
    list = data->main->buckets[index];
    if (list->size != 0) {
        curr = list->head;
        while (curr != NULL) {
            paper = (PaperData*)curr->data;
            if (paper->id == id) {
                return paper;
            }
            curr = curr->next;
        }
    }
    return NULL;
}

/** 
 * Function that finds the element used to 
 * store the collaborators for an author
 */

static CollabData* find_collabs(PublData *data, int64_t id) {
    int index;
    Node *curr;
    LinkedList *list;
    CollabData *collabdata;
    if (data == NULL) {
        return NULL;
    }
    index = id % data->col->size;
    list = data->col->buckets[index];
    if (list->size != 0) {
        curr = list->head;
        while (curr != NULL) {
            collabdata = (CollabData*)curr->data;
            if (collabdata->id == id) {
                return collabdata;
            }
            curr = curr->next;
        }
    }
    return NULL;
}

/** 
 * Function that frees memory allocated for a list
 * used on lists which do not need node data to be freed
 */

static void free_list(LinkedList *list) {
    Node *curr, *prev;
    if (list == NULL) {
        return;
    }
    if (list->size != 0) {
        prev = list->head;
        curr = list->head->next;
        while (curr != NULL) {
            free(prev);
            prev = curr;
            curr = curr->next;
        }
        free(prev);
    }
    free(list);
}

/** 
 * Function that frees memory allocated for a list
 * also frees node data
 */

static void free_list_data(LinkedList *list) {
    Node *curr, *prev;
    if (list == NULL) {
        return;
    }
    if (list->size != 0) {
        prev = list->head;
        curr = list->head->next;
        while (curr != NULL) {
            free(prev->data);
            free(prev);
            prev = curr;
            curr = curr->next;
        }
        free(prev->data);
        free(prev);
    }
    free(list);
}

/* Function used in allocating all memory for main data structure */

PublData* init_publ_data(void) {
    int i;
    PublData *data;
    /* Allocate memory for the whole data structure */
    data = (PublData*)malloc(sizeof(PublData));
    DIE(data == NULL, "malloc - data");
    /* Allocate memory for Hashtable of PaperData */
    data->main = (HashTable*)malloc(sizeof(HashTable));
    DIE(data->main == NULL, "malloc - data->cita");
    data->main->buckets = (LinkedList**)malloc(HTSIZE * sizeof(LinkedList*));
    DIE(data->main->buckets == NULL, "malloc - data->buckets");
    data->main->size = HTSIZE;
    /* Allocate memory for Hashtable of CitData */
    data->cit = (HashTable*)malloc(sizeof(HashTable));
    DIE(data->cit == NULL, "malloc - data->cit");
    data->cit->buckets = (LinkedList**)malloc(HTSIZE * sizeof(LinkedList*));
    DIE(data->cit->buckets == NULL, "malloc - data->cit->buckets");
    data->cit->size = HTSIZE;
    /* Allocate memory for Hashtable of VenData */
    data->ven = (HashTable*)malloc(sizeof(HashTable));
    DIE(data->ven == NULL, "malloc - data->ven");
    data->ven->buckets = (LinkedList**)malloc(HTSIZE * sizeof(LinkedList*));
    DIE(data->ven->buckets == NULL, "malloc - data->ven->buckets");
    data->ven->size = HTSIZE;
    /* Allocate memory for Hashtable of FieldData */
    data->field = (HashTable*)malloc(sizeof(HashTable));
    DIE(data->field == NULL, "malloc - data->field");
    data->field->buckets = (LinkedList**)malloc(HTSIZE * sizeof(LinkedList*));
    DIE(data->field->buckets == NULL, "malloc - data->field->buckets");
    data->field->size = HTSIZE;
    /* Allocate memory for Hashtable of InflData */
    data->infl = (HashTable*)malloc(sizeof(HashTable));
    DIE(data->infl == NULL, "malloc - data->infl");
    data->infl->buckets = (LinkedList**)malloc(HTSIZE * sizeof(LinkedList*));
    DIE(data->infl->buckets == NULL, "malloc - data->infl->buckets");
    data->infl->size = HTSIZE;
    /* Allocate memory for Hashtable of AuthorData */
    data->auth = (HashTable*)malloc(sizeof(HashTable));
    DIE(data->auth == NULL, "malloc - data->auth");
    data->auth->buckets = (LinkedList**)malloc(HTSIZE * sizeof(LinkedList*));
    DIE(data->auth->buckets == NULL, "malloc data->auth->buckets");
    data->auth->size = HTSIZE;
    /* Allocate memory for Hashtable of CollabData */
    data->col = (HashTable*)malloc(sizeof(HashTable));
    DIE(data->col == NULL, "malloc - data->col");
    data->col->buckets = (LinkedList**)malloc(HTSIZE * sizeof(LinkedList));
    DIE(data->col->buckets == NULL, "malloc - data->col->buckets");
    data->col->size = HTSIZE;
    /* Allocate memory for array of paper years */
    data->years = (int*)calloc(2021, sizeof(int));
    DIE(data->years == NULL, "calloc - data->years");
    /* Allocate memory for all Hashtable buckets */
    for (i = 0; i < HTSIZE; i++) {
        data->main->buckets[i] = (LinkedList*)malloc(sizeof(LinkedList));
        DIE(data->main->buckets[i] == NULL, "malloc - data->buckets[i]");
        init_list(data->main->buckets[i]);
        data->cit->buckets[i] = (LinkedList*)malloc(sizeof(LinkedList));
        DIE(data->cit->buckets[i] == NULL, "malloc - data->cit->buckets[i]");
        init_list(data->cit->buckets[i]);
        data->ven->buckets[i] = (LinkedList*)malloc(sizeof(LinkedList));
        DIE(data->ven->buckets[i] == NULL, "malloc - data->ven->buckets[i]");
        init_list(data->ven->buckets[i]);
        data->field->buckets[i] = (LinkedList*)malloc(sizeof(LinkedList));
        DIE(data->field->buckets[i] == NULL,
            "malloc - data->field->buckets[i]");
        init_list(data->field->buckets[i]);
        data->infl->buckets[i] = (LinkedList*)malloc(sizeof(LinkedList));
        DIE(data->infl->buckets[i] == NULL, "malloc - data->infl->buckets[i]");
        init_list(data->infl->buckets[i]);
        data->auth->buckets[i] = (LinkedList*)malloc(sizeof(LinkedList));
        DIE(data->auth->buckets[i] == NULL, "malloc - data->auth->buckets[i]");
        init_list(data->auth->buckets[i]);
        data->col->buckets[i] = (LinkedList*)malloc(sizeof(LinkedList));
        DIE(data->col->buckets[i] == NULL, "malloc - data->col->buckets[i]");
        init_list(data->col->buckets[i]);
    }
    return data;
}

/* Function that frees memory allocated for PaperData structs */

static void destroy_paper_data(struct paper_data *paper) {
    int i;
    if (paper == NULL) {
        return;
    }
    free(paper->title);
    free(paper->venue);
    for (i = 0; i < paper->num_authors; i++) {
        free(paper->author_names[i]);
        free(paper->institutions[i]);
    }
    free(paper->author_names);
    free(paper->author_ids);
    free(paper->institutions);
    for (i = 0; i < paper->num_fields; i++) {
        free(paper->fields[i]);
    }
    free(paper->fields);
    free(paper->references);
    free(paper);
}

/* Function that frees all alocated memory */

void destroy_publ_data(PublData* data) {
    LinkedList *bucket;
    Node *curr, *prev;
    VenData *vendata;
    FieldData *fielddata;
    AuthorData *authordata;
    CollabData *collabdata;
    int i;
    if (data == NULL) {
        return;
    }
    /* Free data for PaperData HashTable */
    for (i = 0; i < data->main->size; i++) {
        bucket = data->main->buckets[i];
        if (bucket->size != 0) {
            prev = bucket->head;
            curr = bucket->head->next;
            while (curr != NULL) {
                destroy_paper_data((PaperData*)prev->data);
                free(prev);
                prev = curr;
                curr = curr->next;
            }
            destroy_paper_data((PaperData*)prev->data);
            free(prev);
        }
        free(bucket);
    }
    free(data->main->buckets);
    /* Free data for CitData HashTable */
    for (i = 0; i < data->cit->size; i++) {
        bucket = data->cit->buckets[i];
        if (bucket->size != 0) {
            prev = bucket->head;
            curr = bucket->head->next;
            while (curr != NULL) {
                free(prev->data);
                free(prev);
                prev = curr;
                curr = curr->next;
            }
            free(prev->data);
            free(prev);
        }
        free(bucket);
    }
    free(data->cit->buckets);
    /* Free data for VenData HashTable */
    for (i = 0; i < data->ven->size; i++) {
        bucket = data->ven->buckets[i];
        if (bucket->size != 0) {
            prev = bucket->head;
            curr = bucket->head->next;
            while (curr != NULL) {
                vendata = (VenData*)prev->data;
                free(vendata->venue);
                free(prev->data);
                free(prev);
                prev = curr;
                curr = curr->next;
            }
            vendata = (VenData*)prev->data;
            free(vendata->venue);
            free(prev->data);
            free(prev);
        }
        free(bucket);
    }
    free(data->ven->buckets);
    /* Free data for FieldData HashTable */
    for (i = 0; i < data->field->size; i++) {
        bucket = data->field->buckets[i];
        if (bucket->size != 0) {
            prev = bucket->head;
            curr = bucket->head->next;
            while (curr != NULL) {
                fielddata = (FieldData*)prev->data;
                free(fielddata->field);
                free(prev->data);
                free(prev);
                prev = curr;
                curr = curr->next;
            }
            fielddata = (FieldData*)prev->data;
            free(fielddata->field);
            free(prev->data);
            free(prev);
        }
        free(bucket);
    }
    free(data->field->buckets);
    /* Free data for InflData HashTable */
    for (i = 0; i < data->infl->size; i++) {
        bucket = data->infl->buckets[i];
        if (bucket->size != 0) {
            prev = bucket->head;
            curr = bucket->head->next;
            while (curr != NULL) {
                free(prev->data);
                free(prev);
                prev = curr;
                curr = curr->next;
            }
            free(prev->data);
            free(prev);
        }
        free(bucket);
    }
    free(data->infl->buckets);
    /* Free data for AuthorData HashTable */
    for (i = 0; i < data->auth->size; i++) {
        bucket = data->auth->buckets[i];
        if (bucket->size != 0) {
            prev = bucket->head;
            curr = bucket->head->next;
            while (curr != NULL) {
                authordata = (AuthorData*)prev->data;
                free_list_data(authordata->papers);
                free(prev->data);
                free(prev);
                prev = curr;
                curr = curr->next;
            }
            authordata = (AuthorData*)prev->data;
            free_list_data(authordata->papers);
            free(prev->data);
            free(prev);
        }
        free(bucket);
    }
    free(data->auth->buckets);
    /* Free data for CollabData HashTable */
    for (i = 0; i < data->col->size; i++) {
        bucket = data->col->buckets[i];
        if (bucket->size != 0) {
            prev = bucket->head;
            curr = bucket->head->next;
            while (curr != NULL) {
                collabdata = (CollabData*)prev->data;
                free_list_data(collabdata->collabs);
                free(prev->data);
                free(prev);
                prev = curr;
                curr = curr->next;
            }
            collabdata = (CollabData*)prev->data;
            free_list_data(collabdata->collabs);
            free(prev->data);
            free(prev);
        }
        free(bucket);
    }
    free(data->col->buckets);
    /* Free data for all HashTables */
    free(data->ven);
    free(data->cit);
    free(data->main);
    free(data->field);
    free(data->infl);
    free(data->auth);
    free(data->col);
    free(data->years);
    free(data);
}

void add_paper(PublData* data, const char* title, const char* venue,
    const int year, const char** author_names, const int64_t* author_ids,
    const char** institutions, const int num_authors, const char** fields,
    const int num_fields, const int64_t id, const int64_t* references,
    const int num_refs) {
    PaperData *new_paper;
    CitData *citdata;
    int index, i, j;
    if (data == NULL) {
        return;
    }
    index = id % data->main->size;
    /* Allocate memory for PaperData struct */
    new_paper = (PaperData*)malloc(sizeof(PaperData));
    DIE(new_paper == NULL, "malloc - new_paper");
    /* Add paper id */
    new_paper->id = id;
    /* Add paper title */
    new_paper->title = (char*)malloc((strlen(title) + 1) * sizeof(char));
    DIE(new_paper->title == NULL, "malloc - new_paper->title");
    snprintf(new_paper->title, strlen(title) + 1, "%s", title);
    /* Add paper venue */
    new_paper->venue = (char*)malloc((strlen(venue) + 1) * sizeof(char));
    DIE(new_paper->venue == NULL, "malloc - new_paper->venue");
    snprintf(new_paper->venue, strlen(venue) + 1, "%s", venue);
    /* Add paper year */
    new_paper->year = year;
    /* Increase array of years for given year */
    data->years[new_paper->year]++;
    /* Add paper author names, ids and institutions */
    new_paper->author_names = (char**)malloc(num_authors * sizeof(char*));
    DIE(new_paper->author_names == NULL, "malloc - author_names");
    new_paper->author_ids = (int64_t*)malloc(num_authors * sizeof(int64_t));
    DIE(new_paper->author_ids == NULL, "malloc - author_ids");
    new_paper->institutions = (char**)malloc(num_authors * sizeof(char*));
    DIE(new_paper->institutions == NULL, "malloc - institutions");
    for (i = 0; i < num_authors; i++) {
        new_paper->author_names[i] = (char*)malloc(
            (strlen(author_names[i]) + 1) * sizeof(char));
        DIE(new_paper->author_names[i] == NULL, "malloc - author_names[i]");
        snprintf(new_paper->author_names[i], strlen(author_names[i]) + 1,
            "%s", author_names[i]);
        new_paper->author_ids[i] = author_ids[i];
        /* Add this paper for each of it's authors */
        add_author(data, new_paper->author_ids[i], new_paper->id);
        new_paper->institutions[i] = (char*)malloc((strlen(institutions[i])+ 1)
            * sizeof(char));
        DIE(new_paper->institutions[i] == NULL, "malloc - institutions[i]");
        snprintf(new_paper->institutions[i], strlen(institutions[i]) + 1,
            "%s", institutions[i]);
    }
    /* Add collaborators for this paper */
    for (i = 0; i < num_authors; i++) {
        for (j = 0; j < num_authors; j++) {
            if (i != j) {
                add_collaborator(data, new_paper->author_ids[i],
                new_paper->author_ids[j]);
            }
        }
    }
    new_paper->num_authors = num_authors;
    /* Add paper fields */
    new_paper->fields = (char**)malloc(num_fields * sizeof(char*));
    DIE(new_paper->fields == NULL, "malloc - fields");
    for (i = 0; i < num_fields; i++) {
        new_paper->fields[i] = (char*)malloc((strlen(fields[i]) + 1)
            * sizeof(char));
        DIE(new_paper->fields[i] == NULL, "malloc - fields[i]");
        snprintf(new_paper->fields[i], strlen(fields[i]) + 1, "%s", fields[i]);
        /* Add this paper for each of it's fields */
        add_field(data, new_paper->id, new_paper->fields[i]);
    }
    new_paper->num_fields = num_fields;
    /* Add paper references */
    new_paper->references = (int64_t*)malloc(num_refs * sizeof(int64_t));
    DIE(new_paper->references == NULL, "malloc - references");
    for (i = 0; i < num_refs; i++) {
        new_paper->references[i] = references[i];
        /* Increase number of citations for all referenced papers */
        citdata = has_citations(data, new_paper->references[i]);
        if (citdata == NULL) {
            add_cit(data, new_paper->references[i]);
            citdata = has_citations(data, new_paper->references[i]);
            citdata->citations++;
        } else {
            citdata->citations++;
        }
        add_infl(data, new_paper->references[i], new_paper->id);
    }
    new_paper->num_refs = num_refs;
    add_node(data->main->buckets[index], new_paper);
    /* Initialise citation counter for this paper */
    add_cit(data, new_paper->id);
    /* Addthis paper for it's venue */
    add_ven(data, new_paper->id, new_paper->venue);
}

char* get_oldest_influence(PublData* data, const int64_t id_paper) {
    PaperData *paper, *result, *curr, *next;
    LinkedList *q, *visited;
    CitData *cit1, *cit2;
    int i, year = 9999, changes = 0;
    if (data == NULL) {
        return "None";
    }
    /* Determine if the paper for the given id has been added */
    paper = find_paper(data, id_paper);
    if (paper == NULL) {
        return "None";
    }
    /* Initialise BFS lists */
    result = NULL;
    q = (LinkedList*)malloc(sizeof(LinkedList));
    DIE(q == NULL, "malloc - q");
    visited = (LinkedList*)malloc(sizeof(LinkedList));
    DIE(visited == NULL, "malloc - visited");
    init_list(q);
    init_list(visited);
    /* Mark inital paper as visited to prevent loops */
    add_node(visited, paper);
    /* Add all the referenced papers to the queue */
    for (i = 0; i < paper->num_refs; i++) {
        next = find_paper(data, paper->references[i]);
        if (next != NULL && in_list(visited, next) == 0) {
            add_node(q, next);
        }
    }
    /* BFS algorithm */
    while (q->size != 0) {
        curr = (PaperData*)q->head->data;
        add_node(visited, curr);
        remove_front(q);
        for (i = 0; i < curr->num_refs; i++) {
            next = find_paper(data, curr->references[i]);
            if (next != NULL && in_list(visited, next) == 0) {
                add_node(q, next);
            }
        }
        /* Update the oldest influence result if needed */
        if (curr->year < year) {
            result = curr;
            year = curr->year;
            changes = 1;
        } else if (curr->year == year) {
            cit1 = has_citations(data, curr->id);
            cit2 = has_citations(data, result->id);
            if (cit1->citations > cit2->citations) {
                result = curr;
                changes = 1;
            } else if (cit1->citations == cit2->citations) {
                if (curr->id < result->id) {
                    result = curr;
                    changes = 1;
                }
            }
        }
    }
    free_list(q);
    free_list(visited);
    /* Determine if an oldest influence has been found */
    if (changes == 0) {
        return "None";
    }
    return result->title;
}

float get_venue_impact_factor(PublData* data, const char* venue) {
    int index;
    float articles = 0, refs = 0;
    char *name;
    LinkedList *list;
    Node *curr;
    CitData *cit;
    VenData *ven;
    PaperData *paper;
    if (data == NULL) {
        return 0.f;
    }
    name = (char*)malloc((strlen(venue) + 1) * sizeof(char));
    DIE(name == NULL, "malloc - name");
    snprintf(name, strlen(venue) + 1, "%s", venue);
    /* Use Hashtable of venues to find all the papers for given venue */
    index = hash(name) % data->ven->size;
    list = data->ven->buckets[index];
    if (list == NULL) {
        return 0.f;
    }
    curr = list->head;
    while (curr != NULL) {
        ven = (VenData*)curr->data;
        if (strcmp(ven->venue, name) == 0) {
            paper = find_paper(data, ven->id);
            if (paper != NULL) {
                /* Get count the number of citations for each paper */
                cit = has_citations(data, ven->id);
                refs += cit->citations;
                /* Count found articles */
                articles += 1;
            }
        }
        curr = curr->next;
    }
    free(name);
    return refs / articles;
}

int get_number_of_influenced_papers(PublData* data, const int64_t id_paper,
    const int distance) {
    CitData *cit;
    PaperData *paper, *currp;
    InflData *infldata;
    LinkedList *s, *visited, *list;
    Node *currn;
    int count = 0, index, found;
    if (data == NULL) {
        return 0;
    }
    /* Determine if the paper for the given id exists */
    paper = find_paper(data, id_paper);
    if (paper == NULL) {
        return 0;
    }
    /* If distance is 1 we can return the number of citations */
    if (distance == 1) {
        cit = has_citations(data, id_paper);
        if (cit == NULL) {
            return 0;
        } else {
            return cit->citations;
        }
    }
    /* Initialization for DFS lists */
    s = (LinkedList*)malloc(sizeof(LinkedList));
    DIE(s == NULL, "malloc - s");
    visited = (LinkedList*)malloc(sizeof(LinkedList));
    DIE(visited == NULL, "malloc - visited");
    init_list(s);
    init_list(visited);
    /* Add starting paper to stack and mark as visited to prevent loops */
    add_node(visited, paper);
    add_node(s, paper);
    /* DFS algorithm */
    while (s->size != 0) {
        currp = (PaperData*)s->tail->data;
        found = 0;
        /* Find the first unvisited paper that influenced the current one */
        index = currp->id % data->infl->size;
        list = data->infl->buckets[index];
        if (list->size != 0) {
            currn = list->head;
            while (currn != NULL) {
                infldata = (InflData*)currn->data;
                if (infldata->id == currp->id) {
                    paper = find_paper(data, infldata->infl_id);
                    if (paper != NULL) {
                        if (!in_list(visited, paper)) {
                            found = 1;
                            break;
                        }
                    }
                }
                currn = currn->next;
            }
        }
        /* Add to stack, mark as visited and count if paper was found */
        if (found == 1 && s->size - 1 < distance) {
            add_node(visited, paper);
            add_node(s, paper);
            count++;
        } else {
            remove_back(s);
        }
    }
    free_list(s);
    free_list(visited);
    return count;
}

int get_erdos_distance(PublData* data, const int64_t id1, const int64_t id2) {
    LinkedList *q, *visited, *collabs;
    Node *curr;
    CollabData *collabdata;
    int *dist, lsize = 1, found = 0, p = 0, d;
    int64_t id, *c_id, start_id = id1;
    /* Initialise BFS lists */
    q = (LinkedList*)malloc(sizeof(LinkedList));
    DIE(q == NULL, "malloc - q");
    init_list(q);
    visited = (LinkedList*)malloc(sizeof(LinkedList));
    DIE(visited == NULL, "malloc - visited");
    init_list(visited);
    /* Initialize array of distances */
    dist = (int*)calloc(lsize, sizeof(int));
    DIE(dist == NULL, "calloc - dist");
    /* Mark starting element as visited to prevent loops */
    add_node(q, &start_id);
    add_node(visited, &start_id);
    /* BFS algorithm */
    while (q->size != 0) {
        id = *((int64_t*)q->head->data);
        /* Get list of collaborators for current author */
        collabdata = find_collabs(data, id);
        if (collabdata == NULL) {
            break;
        }
        collabs = collabdata->collabs;
        if (collabs->size == 0) {
            break;
        }
        remove_front(q);
        curr = collabs->head;
        /* Add all non-visited collaborators to queue */
        while (curr != NULL) {
            c_id = (int64_t*)curr->data;
            if (!in_list_id(visited, *c_id)) {
                add_node(visited, c_id);
                /* Increment distance for current author */
                lsize++;
                dist = (int*)realloc(dist, lsize * sizeof(int));
                dist[lsize - 1] = dist[p] + 1;
                add_node(q, c_id);
                if (*c_id == id2) {
                    found = 1;
                    break;
                }
            }
            curr = curr->next;
        }
        if (found == 1) {
            break;
        }
        p++;
    }
    d = dist[lsize - 1];
    free_list(q);
    free_list(visited);
    free(dist);
    if (found == 0) {
        return -1;
    }
    return d;
}

char** get_most_cited_papers_by_field(PublData* data, const char* field,
    int* num_papers) {
    PaperData *paper, *p1, *p2;
    FieldData *fielddata;
    CitData *cit1, *cit2;
    LinkedList *list, *papers;
    Node *curr, *lcurr = NULL;
    char **sorted, *name;
    int index, swapped, i;
    void *aux;
    if (data == NULL) {
        return NULL;
    }
    papers = (LinkedList*)malloc(sizeof(LinkedList));
    DIE(papers == NULL, "malloc - papers");
    init_list(papers);
    name = (char*)malloc((strlen(field) + 1) * sizeof(char));
    DIE(name == NULL, "malloc - name");
    snprintf(name, strlen(field) + 1, "%s", field);
    /* Use field Hashtable to find all papers in given field */
    index = hash(name) % data->field->size;
    list = data->field->buckets[index];
    if (list->size != 0) {
        curr = list->head;
        while (curr != NULL) {
            fielddata = (FieldData*)curr->data;
            if (strcmp(fielddata->field, field) == 0) {
                paper = find_paper(data, fielddata->id);
                if (paper != NULL) {
                    add_node(papers, paper);
                }
            }
            curr = curr->next;
        }
    }
    /* Update number of papers in field if needed and alloc array of titles*/
    if (papers->size < *num_papers) {
        *num_papers = papers->size;
    }
    sorted = (char**)malloc((*num_papers) * sizeof(char*));
    DIE(sorted == NULL, "malloc - sorted");
    /* Sort list of papers in field based on give criteria using bubble sort */
    do {
        swapped = 0;
        curr = papers->head;
        while (curr->next != lcurr) {
            p1 = (PaperData*)curr->data;
            p2 = (PaperData*)curr->next->data;
            cit1 = has_citations(data, p1->id);
            cit2 = has_citations(data, p2->id);
            /* Check criteria for sort */
            if (cit1->citations < cit2->citations) {
                aux = curr->data;
                curr->data = curr->next->data;
                curr->next->data = aux;
                swapped = 1;
            } else if (cit1->citations == cit2->citations) {
                if (p1->year < p2->year) {
                    aux = curr->data;
                    curr->data = curr->next->data;
                    curr->next->data = aux;
                    swapped = 1;
                } else if (p1->year == p2->year) {
                    if (p1->id > p2->id) {
                        aux = curr->data;
                        curr->data = curr->next->data;
                        curr->next->data = aux;
                        swapped = 1;
                    }
                }
            }
            curr = curr->next;
        }
        lcurr = curr;
    } while (swapped);
    curr = papers->head;
    /* Add titles to array for sorted papers */
    for (i = 0; i < *num_papers; i++) {
        paper = (PaperData*)curr->data;
        sorted[i] = (char*)malloc((strlen(paper->title) + 1) * sizeof(char));
        DIE(sorted[i] == NULL, "malloc - sorted[i]");
        snprintf(sorted[i], strlen(paper->title) + 1, "%s", paper->title);
        curr = curr->next;
    }
    free(name);
    free_list(papers);
    return sorted;
}

int get_number_of_papers_between_dates(PublData* data, const int early_date,
    const int late_date) {
    int count = 0, i;
    if (data == NULL) {
        return 0;
    }
    /* Use year vector and add al values between give years */
    for (i = early_date; i <= late_date; i++) {
        count += data->years[i];
    }
    return count;
}

int get_number_of_authors_with_field(PublData* data, const char* institution,
    const char* field) {
    LinkedList *list, *authors;
    Node *curr, *find;
    PaperData *paper;
    FieldData *fielddata;
    int count = 0, i, found = 0, index;
    char *name;
    if (data == NULL) {
        return 0;
    }
    /* Create list used to mark counted authors */
    authors = (LinkedList*)malloc(sizeof(LinkedList));
    DIE(authors == NULL, "malloc - authors");
    init_list(authors);
    name = (char*)malloc((strlen(field) + 1) * sizeof(char));
    DIE(name == NULL, "malloc - name");
    snprintf(name, strlen(field) + 1, "%s", field);
    /* Use field Hashtable to find all papers in given field */
    index = hash(name) % data->field->size;
    free(name);
    list = data->field->buckets[index];
    if (list == NULL) {
        return 0;
    }
    curr = list->head;
    while (curr != NULL) {
        fielddata = (FieldData*)curr->data;
        if (strcmp(fielddata->field, field) == 0) {
            paper = find_paper(data, fielddata->id);
            if (paper != NULL) {
                for (i = 0; i < paper->num_authors; i++) {
                    /* Check if institution is correct for each author */
                    if (strcmp(paper->institutions[i], institution) == 0) {
                        found = 0;
                        find = authors->head;
                        /* Check if author has been counted */
                        while (find != NULL) {
                            name = (char*)find->data;
                            if (strcmp(paper->author_names[i], name) == 0) {
                                found = 1;
                                break;
                            }
                            find = find->next;
                        }
                        /* Count if not previously counted and mark */
                        if (!found) {
                            count++;
                            add_node(authors, paper->author_names[i]);
                        }
                    }
                }
            }
        }
        curr = curr->next;
    }
    free_list(authors);
    return count;
}

int* get_histogram_of_citations(PublData* data, const int64_t id_author,
    int* num_years) {
    LinkedList *list, *papers = NULL, *hist_papers;
    Node *curr1, *curr2;
    AuthorData *authordata;
    PaperData *paper;
    CitData *cit;
    int index, *hist, minyear = 3000;
    int64_t id;
    /* Use author papers Hashtable to find all papers made by given author */
    index = id_author % data->auth->size;
    list = data->auth->buckets[index];
    hist_papers = (LinkedList*)malloc(sizeof(LinkedList));
    init_list(hist_papers);
    *num_years = 0;
    if (list->size == 0) {
        return NULL;
    }
    curr1 = list->head;
    while (curr1 != NULL) {
        authordata = (AuthorData*)curr1->data;
        if (authordata->id == id_author) {
            papers = authordata->papers;
            if (papers != NULL) {
                curr2 = papers->head;
                while (curr2 != NULL) {
                    id = *((int64_t*)curr2->data);
                    paper = find_paper(data, id);
                    /* Determine oldest published paper */
                    if (paper->year < minyear) {
                        minyear = paper->year;
                    }
                    add_node(hist_papers, paper);
                    curr2 = curr2->next;
                }
            }
            break;
        }
        curr1 = curr1->next;
    }
    if (hist_papers->size == 0) {
        *num_years = 0;
        return NULL;
    } else {
        /* Updated histogram size and allocate array */
        *num_years = 2020 - minyear + 1;
        hist = (int*)calloc(*num_years, sizeof(int));
        curr1 = hist_papers->head;
        while (curr1 != NULL) {
            paper = (PaperData*)curr1->data;
            cit = has_citations(data, paper->id);
            if (cit != NULL) {
                /* Update histogram values for each year */
                hist[2020 - paper->year] += cit->citations;
            }
            curr1 = curr1->next;
        }
        free_list(hist_papers);
        return hist;
    }
}

char** get_reading_order(PublData* data, const int64_t id_paper,
    const int distance, int* num_papers) {
    /* TODO: implement get_reading_order */
    LinkedList *papers, *q, *visited;
    Node *curr, *sort1, *sort2, *sort_start, *sort_end;
    PaperData *paper, *next, *currp, *p1, *p2;
    char **sorted;
    int lsize = 1, i, count = 0, dist = 0, swapped = 0, osize;
    void *aux;
    *num_papers = 0;
    paper = find_paper(data, id_paper);
    if (paper == NULL || paper->num_refs == 0) {
        return NULL;
    }
    osize = paper->num_refs;
    q = (LinkedList*)malloc(sizeof(LinkedList));
    DIE(q == NULL, "malloc - q");
    init_list(q);
    visited = (LinkedList*)malloc(sizeof(LinkedList));
    DIE(visited == NULL, "malloc - visited");
    init_list(visited);
    papers = (LinkedList*)malloc(sizeof(LinkedList));
    DIE(papers == NULL, "malloc - papers");
    init_list(papers);
    add_node(q, paper);
    add_node(visited, paper);
    add_node(papers, paper);
    count = paper->num_refs;
    /* BFS algorithm */
    while (q->size != 0 && dist <= distance) {
        currp = (PaperData*)q->head->data;
        add_node(visited, currp);
        remove_front(q);
        lsize--;
        if (lsize == 0) {
            dist++;
            lsize = count;
            count = 0;
        }
        for (i = 0; i < currp->num_refs; i++) {
            next = find_paper(data, currp->references[i]);
            if (next != NULL && in_list(visited, next) == 0) {
                add_node(q, next);
                if (dist <= distance) {
                    add_node(papers, next);
                }
                count += next->num_refs;
            }
        }
    }
    *num_papers = papers->size - 1;
    if (papers->size > 1) {
        count = osize;
        curr = papers->head->next;
        while (curr != NULL) {
            lsize = count;
            count = 0;
            sort_start = curr;
            for (i = 0; i < lsize && curr != NULL; i++) {
                currp = (PaperData*)curr->data;
                count += currp->num_refs;
                curr = curr->next;
            }
            sort_end = curr;
            sort2 = sort_end;
            do {
                swapped = 0;
                sort1 = sort_start;
                while (sort1->next != sort2) {
                    p1 = (PaperData*)sort1->data;
                    p2 = (PaperData*)sort1->next->data;
                    /* Check criteria for sort */
                    if (p1->year < p2->year) {
                        aux = sort1->data;
                        sort1->data = sort1->next->data;
                        sort1->next->data = aux;
                        swapped = 1;
                    } else if (p1->year == p2->year) {
                        if (p1->id < p2->id) {
                            aux = sort1->data;
                            sort1->data = sort1->next->data;
                            sort1->next->data = aux;
                            swapped = 1;
                        }
                    }
                    sort1 = sort1->next;
                }
                sort2 = sort1;
            } while (swapped);
            if (curr != NULL) {
                curr = curr->next;
            }
        }
        curr = papers->tail;
        sorted = (char**)malloc(*num_papers * sizeof(char*));
        i = 0;
        while (curr != papers->head && i < *num_papers) {
            currp = (PaperData*)curr->data;
            sorted[i] = (char*)malloc((strlen(currp->title) + 1)
             * sizeof(char));
            snprintf(sorted[i], strlen(currp->title) + 1, "%s", currp->title);
            i++;
            curr = curr->prev;
        }
    }
    free_list(q);
    free_list(visited);
    free_list(papers);
    if (*num_papers == 0) {
        return NULL;
    }
    return sorted;
}

char* find_best_coordinator(PublData* data, const int64_t id_author) {
    /* TODO: implement find_best_coordinator */

    return NULL;
}
