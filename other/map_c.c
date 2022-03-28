#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 13

typedef struct node_s {
    int value;
    int key;
    struct node_s* prev;
    struct node_s* next;
} node_t;

typedef struct map_s {
    node_t* node;
    struct map_s* next;
} map_t;

typedef struct lru_s {
    int capacity;
    int cnt;
    node_t* head;
    map_t map[HASH_SIZE];
} lru_t;

// map_t map[HASH_SIZE] = {0};

lru_t* CreateLru(int capacity) {
    lru_t* obj = malloc(sizeof(lru_t));
    obj->capacity = capacity;
    obj->head = NULL;
    obj->cnt = 0;

    memset(obj->map, 0, HASH_SIZE * sizeof(map_t));
    // memset(map, 0, HASH_SIZE * sizeof(map_t));
}

void PutLru(lru_t* obj, int value, int key) {
    if (obj->cnt == 0) {
        node_t* new_node = malloc(sizeof(node_t));
        new_node->key = key;
        new_node->value = value;
        new_node->prev = NULL;
        new_node->next = NULL;
        obj->head = new_node;
        int pos = key % HASH_SIZE;
        // map[pos].node = new_node;
        obj->map[pos].node = new_node;
        obj->cnt += 1;
        return ;
    }

    node_t* new_node = malloc(sizeof(node_t));
    new_node->key = key;
    new_node->value = value;
    new_node->prev = NULL;
    new_node->next = obj->head;
    new_node->next->prev = new_node;
    obj->head = new_node;
    int pos = key % HASH_SIZE;
    // map[pos].node = new_node;
    obj->map[pos].node = new_node;
    obj->cnt += 1;
}

int GetLru(lru_t* obj, int key) {
    int pos = key % HASH_SIZE;

    map_t* find = &(obj->map[pos]);
    // map_t* find = &map[pos];

    while (find != NULL) {
        node_t* node = find->node;
        if (node != NULL && node->key == key) {
            if (obj->head  == node) {
                return node->value;
            }
            if (node->prev != NULL) {
                node->prev->next = node->next;
            }
            if (node->next != NULL) {
                node->next->prev = node->prev;
            }
            node->prev = NULL;
            node->next = obj->head;
            node->next->prev = node;
            obj->head = node;
            return node->value;
        }
        find = find->next;
    }
    return -1;
}

int main(void) {
    lru_t* lru_cache = CreateLru(10);

    PutLru(lru_cache, 1, 1);
    PutLru(lru_cache, 2, 2);
    PutLru(lru_cache, 3, 3);

    int find_value;
    find_value = GetLru(lru_cache, 4);
    printf("key %d find %d \n", 4, find_value);
    find_value = GetLru(lru_cache, 3);
    printf("key %d find %d \n", 3, find_value);
    find_value = GetLru(lru_cache, 2);
    printf("key %d find %d \n", 2, find_value);
    find_value = GetLru(lru_cache, 1);
    printf("key %d find %d \n", 1, find_value);
    find_value = GetLru(lru_cache, 7);
    printf("key %d find %d \n", 7, find_value);
    find_value = GetLru(lru_cache, 3);
    printf("key %d find %d \n", 3, find_value);
    return 0;
}