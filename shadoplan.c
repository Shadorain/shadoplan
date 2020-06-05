#include <stdio.h>
#include <string.h>
#include <config.h>

typedef struct Entry Entry;
struct Entry {
    char name[100];
    char descstr[512];
    
};

/* void add */

void addDesc(Entry *e) {}

int main () {
    // in other file
    testFunc();

    return 0;
}
