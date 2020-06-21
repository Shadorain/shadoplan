// Libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
// Files
#include "config.h"
#include "options.h"
#include "shadomenu.h"

#define MAX_CATEGORIES 50
#define MAX_TODOS 1000

struct DateTime {
    int month, day, hour, minute;
    int ind;
};

struct Priority {
    int pri;
    int ind;
};

struct Rules {
    char VERSION[3];
    int priority,lines;
    char pri[4];
    char title[96];
    char desc[512];
    char cat[36];
    char due[16];
    char cdue[16];
    char date[16];
    char cdate[16];

    // Commands
    char *col1;
    char *col2;
    char *col3;
    char *col4;
    char *col5;
    char *col6;

    // Dynamic Alloc
    char *cats[MAX_CATEGORIES];
    char *c1[MAX_TODOS];
    char *c2[MAX_TODOS];
    char *c3[MAX_TODOS];
    char *c4[MAX_TODOS];
    char *c5[MAX_TODOS];
    char *c6[MAX_TODOS];
};

// Function Declaration
void add (struct Rules r);
void catClean();
int compareDT(const void *a, const void *b);
int compareP(const void *a, const void *b);
void list (struct Rules r, char method[]);
void listDate(struct Rules r);
void listPriority(struct Rules r);
void listTree(struct Rules r);
int validateTime(int hour, int min, struct Rules r);
int validateDate(int month, int day, struct Rules r);

// t -a/--add option, adds new todo to list
void add (struct Rules r) {
    FILE *td, *ct;
    char buf[36];
    int catadd = 1;

    // Opens and writes data to file
    td = fopen(tdpath, "a");
    fprintf(td, "\"%s\",\"%s\",\"%d\",\"%s\",\"%s\",\"%s\"\n",r.title,r.desc,r.priority,r.cat,r.due,r.date); 
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

// Sorting function for dates and time
int compareDT(const void *a, const void *b) {
    const struct DateTime *d1 = (const struct DateTime *)a;
    const struct DateTime *d2 = (const struct DateTime *)b;
    if (d1->month < d2->month) return -1;
    else if (d1->month > d2->month) return 1;
    else if (d1->day < d2->day) return -1;
    else if (d1->day > d2->day) return 1;
    else if (d1->hour < d2->hour) return -1;
    else if (d1->hour > d2->hour) return 1;
    else if (d1->minute < d2->minute) return -1;
    else if (d1->minute > d2->minute) return 1;
    else return 0;
}

// Sort funct for priority
int compareP(const void *a, const void *b) {
    const struct Priority *p1 = (const struct Priority *)a;
    const struct Priority *p2 = (const struct Priority *)b;
    if (p1->pri < p2->pri) return -1;
    else if (p1->pri > p2->pri) return 1;
    else return 0;
}

// t -l/--list option, displays list in specified way
void list(struct Rules r, char method[]) {
    FILE *c1, *c2, *c3, *c4, *c5, *c6, *ct, *td;
    char buf[36];
    char buffer[652];

    r.col1 = "cat /home/shadow/.todos/todo | awk -F'\",\"' '{print $1}' | cut -c2-";
    r.col2 = "cat /home/shadow/.todos/todo | awk -F'\",\"' '{print $2}'";
    r.col3 = "cat /home/shadow/.todos/todo | awk -F'\",\"' '{print $3}'";
    r.col4 = "cat /home/shadow/.todos/todo | awk -F'\",\"' '{print $4}'";
    r.col5 = "cat /home/shadow/.todos/todo | awk -F'\",\"' '{print $5}'";
    r.col6 = "cat /home/shadow/.todos/todo | awk -F'\",\"' '{print $6}' | rev | cut -c2- | rev";

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
        r.c5[i] = malloc(strlen(r.due) + 1);
        if (r.c5[i])
            strcpy(r.c5[i], r.due);
        i++;
    }
    fclose(c5);

    // Grabs each date
    c6 = popen(r.col6, "r");
    memset(r.c6, 0, MAX_TODOS * sizeof(char *));
    i=0;
    while (fgets(r.due, sizeof(r.due), c6) != 0) {
        r.c6[i] = malloc(strlen(r.due) + 1);
        if (r.c6[i])
            strcpy(r.c6[i], r.due);
        i++;
    }
    fclose(c6);

    if (!strcmp(method,"tree"))
        listTree(r);
    else if (!strcmp(method,"interactive")) {
        
    } else if (!strcmp(method,"date"))
        listDate(r);
    else if (!strcmp(method, "priority"))
        listPriority(r);

    // Plain
    if (!strcmp(method,"plain"))
        printf("Title, Description, Priority, Category, Due-Time, Due-Date\n");
    td = fopen(tdpath, "r");
    while (fgets(buffer, sizeof(buffer), td) != 0) {
        if (!strcmp(method,"plain"))
            printf("%s",buffer);
    }
    fclose(td);
}

// t -l date : sorts by date and time
void listDate(struct Rules r) {
    int month=0,day=0,hour=0,min=0,tmon=0,tday=0,tmmr=0,tdy=0;
    int ndateInd[r.lines];
    struct DateTime dates[r.lines];
    time_t now; time(&now);
    struct tm *local = localtime(&now);
    tmon = local->tm_mon+1; tday = local->tm_mday;

    for (int i=0; i < r.lines; i++) {
        r.c1[i][strcspn(r.c1[i], "\n")] = 0;
        r.c2[i][strcspn(r.c2[i], "\n")] = 0;
        r.c3[i][strcspn(r.c3[i], "\n")] = 0;
        r.c4[i][strcspn(r.c4[i], "\n")] = 0;
        r.c5[i][strcspn(r.c5[i], "\n")] = 0;
        r.c6[i][strcspn(r.c6[i], "\n")] = 0;
        sscanf(r.c6[i], "%d/%d",&month,&day);
        sscanf(r.c5[i], "%d:%d",&hour,&min);
        if(month==0 && day==0) // find empty dated todos (handled below)
            ndateInd[i]=i+1;
        dates[i].month=month;
        dates[i].day=day;
        dates[i].hour=hour;
        dates[i].minute=min;
        dates[i].ind=i;
        if (dates[i].month==tmon && dates[i].day==tday) tdy=1;
        if (dates[i].month==tmon && dates[i].day==tday+1) tmmr=1;
    }

    qsort(dates, r.lines, sizeof(dates[0]), compareDT);
    // Sorted Todos!!
    if (tdy==1) {
        printf("\n |     Today     |>-\n");
        printf(" | Date  | Time  | P | Title               | Description                                        | Category       |\n");
        printf(" |-------=-------=---=---------------------=----------------------------------------------------=----------------|\n");
        for (int i=0;i<r.lines;i++)
            if (dates[i].month!=0 || dates[i].day!=0)
                if (dates[i].month==tmon && dates[i].day==tday)
                    printf(" | %-5s | %-5s | %-1s | %-19.18s | %-50.50s | %-14.14s |\n",r.c6[dates[i].ind], r.c5[dates[i].ind], r.c3[dates[i].ind],r.c1[dates[i].ind],r.c2[dates[i].ind],r.c4[dates[i].ind]); }
    if (tmmr==1) {
        printf("\n |   Tomorrow    |>-\n");
        printf(" |-------=-------=---=---------------------=----------------------------------------------------=----------------|\n");
        for (int i=0;i<r.lines;i++)
            if (dates[i].month!=0 || dates[i].day!=0)
                if (dates[i].month==tmon && dates[i].day==tday+1)
                    printf(" | %-5s | %-5s | %-1s | %-19.18s | %-50.50s | %-14.14s |\n",r.c6[dates[i].ind], r.c5[dates[i].ind], r.c3[dates[i].ind],r.c1[dates[i].ind],r.c2[dates[i].ind],r.c4[dates[i].ind]); }
    printf("\n |  Coming up... |>-\n");
    printf(" |-------=-------=---=---------------------=----------------------------------------------------=----------------|\n");
    for (int i=0;i<r.lines;i++)
        if (dates[i].month!=0 || dates[i].day!=0)
            printf(" | %-5s | %-5s | %-1s | %-19.18s | %-50.50s | %-14.14s |\n",r.c6[dates[i].ind], r.c5[dates[i].ind], r.c3[dates[i].ind],r.c1[dates[i].ind],r.c2[dates[i].ind],r.c4[dates[i].ind]);

    // Non Dated TODOs
    printf("\n\n | Non-dated |>-\n");
    printf(" | Time  | P | Title               | Description                                        | Category       |\n");
    printf(" |-------=---=---------------------=----------------------------------------------------=----------------|\n");
    for (int i=0;i<r.lines;i++) {
        ndateInd[i]=ndateInd[i]-1;
        if (ndateInd[i] != -1)
            printf(" | %-5s | %-1s | %-19.18s | %-50.50s | %-14.14s |\n",r.c5[i],r.c3[i],r.c1[i],r.c2[i],r.c4[i]); }
}

void listPriority(struct Rules r) {
    int priority=0,low=0,med=0,high=0;
    int npriInd[r.lines];
    struct Priority pris[r.lines];
    for (int i=0;i<r.lines;i++) {
        r.c1[i][strcspn(r.c1[i], "\n")] = 0;
        r.c2[i][strcspn(r.c2[i], "\n")] = 0;
        r.c3[i][strcspn(r.c3[i], "\n")] = 0;
        r.c4[i][strcspn(r.c4[i], "\n")] = 0;
        r.c5[i][strcspn(r.c5[i], "\n")] = 0;
        r.c6[i][strcspn(r.c6[i], "\n")] = 0;
        sscanf(r.c3[i], "%d",&priority);
        if (priority==0) npriInd[i]=i+1;
        pris[i].pri=priority;
        pris[i].ind=i;
        if (pris[i].pri==1 || pris[i].pri==2 || pris[i].pri==3) high=1;
        if (pris[i].pri==4 || pris[i].pri==5 || pris[i].pri==6) med=1;
        if (pris[i].pri==7 || pris[i].pri==8 || pris[i].pri==9) low=1;
    }

    qsort(pris, r.lines, sizeof(pris[0]), compareP);
    // Sorted Todos!!
    printf("\n | P | Title               | Description                                        | Category       | Date  | Time  |");
    if (high==1) { //High priority
        printf("\n |  High Priority  |>-\n");
        printf(" |---=---------------------=----------------------------------------------------=----------------=-------=-------|\n");
        for (int i=0;i<r.lines;i++)
            if (pris[i].pri!=0)
                if (pris[i].pri==1 || pris[i].pri==2 || pris[i].pri==3)
                    printf(" | %1s | %-19.18s | %-50.50s | %-14.14s | %-5s | %-5s |\n",r.c3[pris[i].ind], r.c1[pris[i].ind], r.c2[pris[i].ind],r.c4[pris[i].ind],r.c5[pris[i].ind],r.c6[pris[i].ind]); }
    if (med==1) { //Medium priority
        printf("\n | Medium Priority |>-\n");
        printf(" |---=---------------------=----------------------------------------------------=----------------=-------=-------|\n");
        for (int i=0;i<r.lines;i++)
            if (pris[i].pri!=0)
                if (pris[i].pri==4 || pris[i].pri==5 || pris[i].pri==6)
                    printf(" | %1s | %-19.18s | %-50.50s | %-14.14s | %-5s | %-5s |\n",r.c3[pris[i].ind], r.c1[pris[i].ind], r.c2[pris[i].ind],r.c4[pris[i].ind],r.c5[pris[i].ind],r.c6[pris[i].ind]); }
    if (low==1) { // low priority
        printf("\n |  Low Priority   |>-\n");
        printf(" |---=---------------------=----------------------------------------------------=----------------=-------=-------|\n");
        for (int i=0;i<r.lines;i++)
            if (pris[i].pri!=0)
                if (pris[i].pri==7 || pris[i].pri==8 || pris[i].pri==9)
                    printf(" | %1s | %-19.18s | %-50.50s | %-14.14s | %-5s | %-5s |\n",r.c3[pris[i].ind], r.c1[pris[i].ind], r.c2[pris[i].ind],r.c4[pris[i].ind],r.c5[pris[i].ind],r.c6[pris[i].ind]); }

    // Unprioritized TODOs
    printf("\n\n |  Unprioritized  |>-\n");
    printf(" | Title               | Description                                        | Category       | Time  | Date  |\n");
    printf(" |---------------------=----------------------------------------------------=----------------=-------=-------|\n");
    for (int i=0;i<r.lines;i++) {
        npriInd[i]=npriInd[i]-1;
        if (npriInd[i] != -1)
            printf(" | %-19.18s | %-50.50s | %-14.14s | %-5s | %-5s |\n",r.c1[i],r.c2[i],r.c4[i],r.c5[i],r.c6[i]); }
}

void listTree(struct Rules r) {

    // TODO: Check category that is the root. The check each todo category and put each in the correct spot.

}

int validateDate(int month, int day, struct Rules r) {
    int ret=0;

    if(month > 12 || month < 1) ret=1;
    if((month==4 || month==6 || month==9 || month==11) && (day > 30 || day < 1)) ret=1;
    if(month==2 && (day > 29 || day < 1)) ret=1;
    if(day > 31 || day < 1) ret=1;

    if (month==0 || day==0) ret=2;

    return ret;
}

int validateTime(int hour, int min, struct Rules r) {
    int ret=0;

    if(hour > 24 || hour < 1) ret=1;
    if(min  > 60 || min  < 1) ret=1;
    
    if (hour==0 || min==0) ret=2;

    return ret;
}

int main(int argc, char *argv[]) {
    struct Rules r;
    FILE *td;
    int TUI=1, check=0;
    char buf[12]={0};
    td = fopen(tdpath, "r");
    for (int c = getc(td); c != EOF; c = getc(td))
    if (c == '\n')
        r.lines = r.lines + 1;
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
            if (argc < 6) //If priority empty
                r.priority=0;
            if (argc < 7) //If category empty
                strcpy(r.cat,"");
            if (argc < 8) { //If Due Time empty
                int hour=0,min=0;
                printf("--<| Enter TIME or type n |>--\n[HH:MM]: ❱ ");
                scanf("%d:%d",&hour,&min);
                check=validateTime(hour, min, r);
                if (check==1) {
                    printf("Invalid Time\n");
                    exit(0);
                } else if (check==2)
                    strcpy(r.due,"0:0");
                else {
                    snprintf(r.cdue, 12, "%d:%d",hour,min);
                    strcpy(r.due, r.cdue);
                }
            }
            if (argc < 9) { //If Due Date empty
               int month=0,day=0;
               printf("--<| Enter DATE or type n |>--\n[MM/DD]: ❱ ");
               scanf("%d/%d",&month,&day);
               check=validateDate(month, day, r);
               if (check==1) {
                   printf("Invalid Date\n");
                   exit(0);
               } else if (check==2)
                   strcpy(r.date,"0/0");
               else {
                   snprintf(r.cdate, 12, "%d/%d",month,day);
                   strcpy(r.date, r.cdate);
               }
            }
            if (argc > 9)
                usage();
            
            if (argc >= 6 && argc < 10)
                r.priority=atoi(argv[5]);
            if (argc >= 7 && argc < 10)
                strcpy(r.cat, argv[6]);
            if (argc >= 8 && argc < 10) {
                // Time
                int hour=0,min=0;
                strcpy(buf, argv[7]);
                sscanf(buf,"%d:%d",&hour,&min);
                check=validateTime(hour, min, r);
                if (check==1) {
                    printf("Invalid Time\n");
                    exit(0);
                } else if (check==2)
                    strcpy(r.due,"0:0");
                else {
                    snprintf(r.cdue, 12, "%d:%d",hour,min);
                    strcpy(r.due, r.cdue);
                }
            }
            if (argc >= 9 && argc < 10) {
                // Date
                int month=0,day=0;
                strcpy(buf, argv[8]);
                sscanf(buf,"%d/%d",&month,&day);
                check=validateDate(month, day, r);
                if (check==1) {
                    printf("Invalid Date\n");
                    exit(0);
                } else if (check==2)
                    strcpy(r.date,"0/0");
                else {
                    snprintf(r.cdate, 12, "%d/%d",month,day);
                    strcpy(r.date, r.cdate);
                }
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
