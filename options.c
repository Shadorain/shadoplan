#include <stdio.h>

// File to store all passable options

void usage() {
    fputs("usage: dmenu [-bfiv] [-l lines] [-p prompt] [-fn font] [-m monitor]\n"
            "             [-nb color] [-nf color] [-sb color] [-sf color] [-w windowid]\n", stderr);
}
