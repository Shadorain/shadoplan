#include <stdio.h>
#include <string.h>
#include <config.h>

typedef struct Entry Entry;
struct Entry {
    char name[100];
    char descstr[512];
    
};

/* void add */

void usage() {
    fputs("usage: dmenu [-bfiv] [-l lines] [-p prompt] [-fn font] [-m monitor]\n"
            "             [-nb color] [-nf color] [-sb color] [-sf color] [-w windowid]\n", stderr);
}
void addDesc(Entry *e) {}

int main () {
    // in other file
    testFunc();

    return 0;
}
