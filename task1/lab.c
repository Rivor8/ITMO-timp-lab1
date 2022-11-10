#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <glob.h>

#include "md5.h"


int main (int argc, char **argv) {
    if (argc < 2) {
        printf("Use 'seq_lab -h to display command line options.\n\n");
        return 0;
    }

    FILE *tm = fopen("./template.tbl", "r");
    if (!tm) {
        fprintf(stderr, "ERROR: problem with template.tbl\nUsage: sudo seq_lab [options]\n\n");
        return 0;
    }

    // struct stat tm_stat;
    // int dis_tmplt = open("./template.tbl", O_RDONLY);
    // fstat(dis_tmplt, &tm_stat);
    // int tm_size = tm_stat.st_size;
    // close(dis_tmplt);

    // char *buff = calloc(sizeof(char), tm_size);

    char buff[512];
    glob_t globbuf;

    int opt;
    bool pass = false;

    while((opt = getopt(argc, argv, "hp:r:sow")) != -1) {
        switch (opt)
        {
        case 'h':
            printf("Usage: sudo seq_lab [options]\n\n");
            printf("  -h\t\tDisplay command line options.\n  -p <password>\tEnter password.\n  -r <password>\tRemove all data and change password.\n  -s\t\tStart protection.\n  -o\t\tStop protection.\n  -w\t\tPrint files for protection.\n\n");
            goto END;
        case 'p':
            if (!fscanf(tm, "%s", buff)) {
                fprintf(stderr, "ERROR: problem with data in template.tbl\n\n");
                goto END;
            }
            uint8_t *p;
            char arg_pass[128];
            p = md5String(optarg);
            for(unsigned int i = 0; i < 16; ++i){
		        sprintf(arg_pass + i * 2, "%02x", p[i]);
	        }
            
            pass = !strcmp(arg_pass, buff);

            if (!pass) {
                fprintf(stderr, "ERROR: wrong password.\n\n");
                goto END;
            }
            int tmp = 0;
            while (!feof(tm)) {
                if (!fscanf(tm, "%s", buff)) {
                    fprintf(stderr, "ERROR: problem with data in template.tbl\n\n");
                    goto END;
                }
                
                if (tmp) glob(buff, GLOB_APPEND, NULL, &globbuf);
                else {
                    glob(buff, 0, NULL, &globbuf);
                    tmp++;
                }
            }
            break;
            
        case 'r':
            if (pass) {
                // do something
            }
            break;
        
        case 's':
            if (pass) {
                // do something
            }
            break;
        
        case 'o':
            if (pass) {
                // do something
            }
            break;
        
        case 'w':
            if (pass) {
                for (size_t i = 0; i < globbuf.gl_pathc; i++)
                    printf("%s\n", globbuf.gl_pathv[i]);
            }
            break;
        }
    }

    END:
    fclose(tm);
    return 0;
}

