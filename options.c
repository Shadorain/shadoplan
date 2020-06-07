// Libraries
#include <stdio.h>

// Files
#include "options.h"

void helpText(char arg[]) {
    
}

void usage() {
    fputs("usage: dmenu [-bfiv] [-l lines] [-p prompt] [-fn font] [-m monitor]\n"
            "             [-nb color] [-nf color] [-sb color] [-sf color] [-w windowid]\n", stderr);
}
