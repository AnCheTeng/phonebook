#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "phonebook_opt.h"

/* FILL YOUR OWN IMPLEMENTATION HERE! */
entry *findName(char lastName[], entry *pHead)
{
    while (pHead != NULL) {
        if (strcasecmp(lastName, pHead->lastName) == 0)
            return pHead;
        pHead = pHead->pNext;
    }
    return NULL;
}

entry *append(char lastName[], entry *e)
{
    /* allocate memory for the new entry and put lastName */
    e->pNext = (entry *) malloc(sizeof(entry));
    e = e->pNext;
    strcpy(e->lastName, lastName);
    e->pNext = NULL;
    return e;
}


/* HASH Optimization */
hashTable *createHashTable(int tableSize)
{
    hashTable *ht = NULL;

    /* allocate memory for the HashTable */
    if((tableSize < 1)
            || ((ht = (hashTable* )malloc(sizeof(hashTable))) == NULL)
            || ((ht->list = (entry** )malloc(sizeof(entry*) * tableSize)) == NULL)) {
        return NULL;
    } else {
        /* initialization */
        for(int i = 0; i < tableSize; i++)
            ht->list[i] = NULL;

        ht->size = tableSize;
    }

    return ht;
}

entry* findNameHash(char *key, hashTable *ht)
{
    hashIndex index = BDKRhash(key, ht);

    for(entry* foundObj = ht->list[index]; foundObj!=NULL; foundObj = foundObj->pNext) {
        if(strcasecmp(key, foundObj->lastName)==0) {
            return foundObj;
        }
    }

    return NULL;
}

entry* appendHash(char *key, hashTable *ht)
{
    hashIndex index = BDKRhash(key, ht);
    entry* newEntry;

    if((newEntry = (entry* )malloc(sizeof(entry))) == NULL) {
        return NULL;
    }

    strcpy(newEntry->lastName, key);
    newEntry->pNext = ht->list[index];
    ht->list[index] = newEntry;

    return newEntry;
}

hashIndex BDKRhash(char *key, hashTable *ht)
{
    unsigned int seed = 13131; // 31 131 1313 13131 131313 etc..
    hashIndex hashValue = 0;

    while (*key) {
        hashValue = hashValue * seed + (*key++);
    }

    return hashValue % ht->size;
}

hashIndex APhash(char *key, hashTable *ht)
{
    hashIndex hashValue = 0;

    for (int i=0; *key; i++) {
        if ((i & 1) == 0) {
            hashValue ^= ((hashValue << 7) ^ (*key++) ^ (hashValue >> 3));
        } else {
            hashValue ^= (~((hashValue << 11) ^ (*key++) ^ (hashValue >> 5)));
        }
    }

    return hashValue % ht->size;
}
