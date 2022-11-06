#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdbool.h>

#include "md5.h"


int main (int argc, char **argv) {
    if (argc < 2) {
        printf("Use 'seq_lab -h to display command line options.\n\n");
        return 0;
    }

    FILE *template = fopen("./template.tbl", "r");
    if (!template) {
        fprintf(stderr, "ERROR: problem with template.tbl\nUsage: sudo seq_lab [options]\n\n");
        return 0;
    }

    int opt;
    bool pass = false;
    char buff[255];
    while((opt = getopt(argc, argv, "hp:n:sow")) != -1) {
        switch (opt)
        {
        case 'h':
            printf("Usage: seq_lab [options]\n\n");
            printf("  -h\t\tDisplay command line options.\n  -p <password>\tEnter password.\n  -n <password>\tChange password.\n  -s\t\tStart protection.\n  -o\t\tStop protection.\n  -w\t\tPrint files for protection.\n\n");
            goto END;
        case 'p':
            // Remake
            if (!fscanf(template, "%s", buff)) {
                fprintf(stderr, "ERROR: problem with scan template.tbl\n\n");
                goto END;
            }
            printf("%s\n", buff);
            break;
        }
    }


    END:
    fclose(template);
    return 0;
}

