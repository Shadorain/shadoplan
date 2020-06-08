// Libraries
#include <stdio.h>

// Files
#include "options.h"

void helpText(char arg[]) {
    if (arg[5] == 't') {
        printf("Usage: sp t [-a/--add] [-d/--del] [-x/--check] [-l/--list] [-s/--set]");
        printf("Options: \n");
        printf("    -a, --add [Title] [Description] [Priority] [Category] [Due Date]\n");
        printf("                                    Adds new TODO from what is passed in.\n");
        printf("                                         Required fields: Title, Description\n");
        printf("                                         Blank fields indicate an unorganized TODO\n");
        printf("    -d, --del [Title]               Fuzzy deletes a TODO by title given.\n");
        printf("              [-f, --force]              Will ask for confirm unless -f, --force passed\n");
        printf("    -x, --check [Title]             Fuzzy finds todo by title and checks it (Goes to completed TODOs)\n");
        printf("              [level]                    Level[1-10]: level of completion for the TODO (Default: 10)\n");
        printf("    -l, --list [format]             Lists TODOs in specified way\n");
        printf("                                         Formats: tree, plain, interactive, date\n");
        printf("    -s, --set [FILE]                Sets the TODO list to be used\n");
        printf("    -h                              Displays this help message\n");
    } else if (arg[5] == 'c') {
        printf("Usage: sp c IN_PROGRESS\n");
    } else if (arg[5]=='x') {
        printf("Usage: sp c IN_PROGRESS\n");
    } else
        usage();
}

void usage() {
    printf("Usage: sp [t/c/x] [-h] [-v]\n");
    printf("    Run -h after [t/c/x] for more specified information.\n");
}
