// Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Files
#include "config.h"
#include "options.h"
#include "shadomenu.h"

#define MAX_CATEGORIES 50
#define MAX_TODOS 1000

struct Rules {
    char VERSION[3];
    int priority;
    char pri[4];
    char title[96];
    char desc[512];
    char cat[36];
    char due[8];

    // Commands
    char *col1;
    char *col2;
    char *col3;
    char *col4;
    char *col5;

    // Dynamic Alloc
    char *cats[MAX_CATEGORIES];
    char *c1[MAX_TODOS];
    char *c2[MAX_TODOS];
    char *c3[MAX_TODOS];
    char *c4[MAX_TODOS];
    char *c5[MAX_TODOS];
};

// Function Declaration
void add (struct Rules r);
void catClean();
void list (struct Rules r, char method[]);
void listTree(struct Rules r);

// t -a/--add option, adds new todo to list
void add (struct Rules r) {
    FILE *td, *ct;
    char buf[36];
    int catadd = 1;

    // Opens and writes data to file
    td = fopen(tdpath, "a");
    fprintf(td, "\"%s\",\"%s\",\"%d\",\"%s\",\"%s\"\n",r.title,r.desc,r.priority,r.cat,r.due); 
    // Close Opened File
    fclose(td);

    // Check categories file if added category exists, if not then add
    ct = fopen(categories, "a+");
    while (fgets(buf, sizeof(buf), ct) != 0) {
        size_t len = strlen(buf);
        if (len > 0 && buf[len-1] == '\n')
            buf[--len] = '\0';
        if (strcmp(buf, r.cat))
            catadd=1;
        else {
            catadd=0;
            break;
        }
    }
    if(catadd==1)
        fprintf(ct, "%s\n",r.cat);
    fclose(ct);
}

// c -c/--clean option, cleans categories file
void catClean() {
    FILE *ct, *fp;
    char buf[36];
    char *awkcmd = "cat /home/shadow/.todos/todo | awk -F'\",\"' '{print $4}' | awk '!visited[$0]++'"; //Declared awk command here

    // Clean Confirmation
    printf("--<| This will rebuild all current categories and remove extras not in use. |>-- \nContinue [Y/n] ❱ ");
    char c = getchar();
    if (c=='y'||c=='Y') {
        fp = popen(awkcmd,"r");
        ct = fopen(categories, "w");
        // Reads all lines from awkcmd output and prints them to ct file
        while (fgets(buf, sizeof(buf), fp) != 0) {
            fprintf(ct, "%s",buf);
        }
        // Clean up
        fclose(fp);
        fclose(ct);
        printf("<--| Successfully cleaned categories file |-->\n");
    }
}

// t -l/--list option, displays list in specified way
void list(struct Rules r, char method[]) {
    FILE *td;
    char buf[652];
    r.col1 = "cat /home/shadow/.todos/todo | awk -F'\",\"' '{print $1}' | cut -c2-";
    r.col2 = "cat /home/shadow/.todos/todo | awk -F'\",\"' '{print $2}'";
    r.col3 = "cat /home/shadow/.todos/todo | awk -F'\",\"' '{print $3}'";
    r.col4 = "cat /home/shadow/.todos/todo | awk -F'\",\"' '{print $4}'";
    r.col5 = "cat /home/shadow/.todos/todo | awk -F'\",\"' '{print $3}' | rev | cut -c2- | rev";

    if (!strcmp(method,"tree"))
        listTree(r);

    if (!strcmp(method,"plain"))
        printf("Title, Description, Priority, Category, Due-Date\n");

    // Opens and reads data
    td = fopen(tdpath, "r");
    while (fgets(buf, sizeof(buf), td) != 0) {
        /* fgets(buf, 652, td); */
        if (!strcmp(method,"plain")) {
            printf("%s",buf);
        } else if (!strcmp(method,"interactive")) {
            
        } else if (!strcmp(method,"date")) {
            
        } else if (!strcmp(method, "priority")) {

        }
    }
    // Close Opened File
    fclose(td);
}

void listTree(struct Rules r) {
    FILE *c1, *c2, *c3, *c4, *c5, *ct;
    int count=0;
    char buf[36];

    // Get each category to branch from
    ct = fopen(categories, "r");
    memset(r.cats, 0, MAX_CATEGORIES * sizeof(char *));
    int i=0;
    while (fgets(buf, sizeof(buf), ct) != 0) {
        r.cats[i] = malloc(strlen(buf) + 1);
        if (r.cats[i])
            strcpy(r.cats[i], buf);
        i++;
    }
    fclose(ct);

    // Grabs each title
    c1 = popen(r.col1, "r");
    memset(r.c1, 0, MAX_TODOS * sizeof(char *));
    i=0;
    while (fgets(r.title, sizeof(r.title), c1) != 0) {
        r.c1[i] = malloc(strlen(r.title) + 1);
        if (r.c1[i])
            strcpy(r.c1[i], r.title);
        i++;
    }
    fclose(c1);

    // Grabs each description
    c2 = popen(r.col2, "r");
    memset(r.c2, 0, MAX_TODOS * sizeof(char *));
    i=0;
    while (fgets(r.desc, sizeof(r.desc), c2) != 0) {
        r.c2[i] = malloc(strlen(r.desc) + 1);
        if (r.c2[i])
            strcpy(r.c2[i], r.desc);
        i++;
    }
    fclose(c2);

    // Grabs each priority
    c3 = popen(r.col3, "r");
    memset(r.c3, 0, MAX_TODOS * sizeof(char *));
    i=0;
    while (fgets(r.pri, sizeof(r.pri), c3) != 0) {
        r.c3[i] = malloc(strlen(r.pri) + 1);
        if (r.c3[i])
            strcpy(r.c3[i], r.pri);
        i++;
    }
    fclose(c3);

    // Grabs each category
    c4 = popen(r.col4, "r");
    memset(r.c4, 0, MAX_TODOS * sizeof(char *));
    i=0;
    while (fgets(r.cat, sizeof(r.cat), c4) != 0) {
        r.c4[i] = malloc(strlen(r.cat) + 1);
        if (r.c4[i])
            strcpy(r.c4[i], r.cat);
        i++;
    }
    fclose(c4);

    // Grabs each time
    c5 = popen(r.col5, "r");
    memset(r.c5, 0, MAX_TODOS * sizeof(char *));
    i=0;
    while (fgets(r.due, sizeof(r.due), c5) != 0) {
        printf("%s", r.due);
        r.c5[i] = malloc(strlen(r.due) + 1);
        if (r.c5[i])
            strcpy(r.c5[i], r.due);
        i++;
    }
    fclose(c5);
    
    // Test
    printf("%s",r.c1[2]);
    printf("%s",r.c2[2]);
    printf("%s",r.c3[2]);
    printf("%s",r.c4[2]);
    printf("%s",r.c5[2]);
}

int main(int argc, char *argv[]) {
    struct Rules r;
    int TUI = 1;
    strncpy(r.VERSION, "1.0", 3);
    // Checks if options given, if not then runs Tui
    if (argc==2 && (!strcmp(argv[1], "-v") || !strcmp(argv[1], "--version"))) { // Prints Version
        printf("shadoplan-%s\n", r.VERSION);
        exit(0);
    } else if (argc==2 && (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))) { // General Help Menu
        printf("Run 'man shadoplan' for full documentation.\n");
        usage();
    } else if (argc > 2 && (!strcmp(argv[1],"t") || !strcmp(argv[1],"c") || !strcmp(argv[1],"x"))) {
        if (!strcmp(argv[2], "-h") || !strcmp(argv[2], "--help")) { // Depth Help Menu
            char arg=*argv[1];
            helpText(arg);
        } else if (!strcmp(argv[1],"t") && (!strcmp(argv[2], "-a") || !strcmp(argv[2], "--add"))) { // t:Add Option
            if (argc < 5) //If Title or Description empty
                usage();
            else if (argc < 6) //If priority empty
                r.priority=0;
            else if (argc < 7) { //If category empty
                r.priority=atoi(argv[5]);
                strcpy(r.cat,"");
            } else if (argc < 8) { //If Due Date empty
                r.priority=atoi(argv[5]);
                strcpy(r.cat, argv[6]);
                strcpy(r.due,"");
            } else if (argc > 8)
                usage();
            else {
                r.priority=atoi(argv[5]);
                strcpy(r.cat, argv[6]);
                strcpy(r.due, argv[7]);
            }
            if(!(r.priority >= 0 && r.priority < 10)) {
                printf("Priority must be a digit (0-9)\n");
                usage();
            }
            strcpy(r.title, argv[3]);
            strcpy(r.desc, argv[4]);
            add(r);
            exit(0);
        } else if (!strcmp(argv[1],"t") && (!strcmp(argv[2], "-l") || !strcmp(argv[2], "--list"))) { // t: List Option
            if (argc < 4)
                list(r, "tree");
            else if (argc==4 && !strcmp(argv[3], "tree"))
                list(r, "tree");
            else if (argc==4 && !strcmp(argv[3], "plain"))
                list(r, "plain");
            else if (argc==4 && !strcmp(argv[3], "interactive"))
                list(r, "interactive");
            else if (argc==4 && !strcmp(argv[3], "date"))
                list(r, "date");
            else if (argc==4 && !strcmp(argv[3], "priority"))
                list(r, "priority");
            else
                helpText('t');
            exit(0);
        } else if (!strcmp(argv[1],"c") && (!strcmp(argv[2], "-c") || !strcmp(argv[2], "--clean"))) { // c: Clean Option: 
            catClean();
            exit(0);
        } else
            usage();
    } else
        usage();


    if (TUI==1) {
        int STATE = shadoplanCurses(); //Start NCurses
        if (STATE==1) {
            printf("Exit Code 1, NCurses Menu has crashed. Please restart program.");
        }
    }
    return 0;
}
