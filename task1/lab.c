#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <glob.h>

#include "md5.h"


int main (int argc, char **argv) {
    if (argc < 2) {
        printf("Use 'seq_lab -h' to display command line options.\n\n");
        return 0;
    }

    char buff[512];
    glob_t globbuf;

    int opt;
    bool pass = false;

    // Обрабатываем опции
    while((opt = getopt(argc, argv, "hp:r:sowa:")) != -1) {
        switch (opt)
        {
        case 'h': // Вывод справки
            printf("Usage: sudo seq_lab [options]\n\n");
            printf("  -h\t\tDisplay command line options.\n  -p <password>\tEnter password.\n  -r <password>\tRemove all data and change password.\n  -s\t\tStart protection.\n  -o\t\tStop protection.\n  -w\t\tPrint files for protection.\n  -a <file>\tAdd files for protection.\n\n");
            return 0;
        case 'p': ;// Ввод пароля для работы с программой
            // Открываем файл temlate.tbl для чтения
            FILE *tm = fopen("./template.tbl", "r");
            if (!tm) {
                fprintf(stderr, "ERROR: problem with template.tbl\nUsage: sudo seq_lab [options]\n\n");
                return 0;
            }

            if (!fscanf(tm, "%s", buff)) { // Читаем первую строчку (хэш пароля)
                fprintf(stderr, "ERROR: problem with data in template.tbl\n\n");
                return 0;
            }
            uint8_t *p;
            char arg_pass[128];
            p = md5String(optarg); // Получаем хэш пароля из опции 
            for(unsigned int i = 0; i < 16; ++i){
		        sprintf(arg_pass + i * 2, "%02x", p[i]);
	        }
            
            pass = !strcmp(arg_pass, buff); // Сравниваем с хэшем из файла
            if (!pass) {
                fprintf(stderr, "ERROR: wrong password.\n\n");
                return 0;
            }

            int tmp = 0;
            while (!feof(tm)) { // Читаем построчно имена и маски файлов
                if (!fscanf(tm, "%s", buff)) {
                    fprintf(stderr, "ERROR: problem with data in template.tbl\n\n");
                    return 0;
                }
                
                // Находим файлы по заданным именам и маскам
                if (tmp) glob(buff, GLOB_APPEND, NULL, &globbuf);
                else {
                    glob(buff, 0, NULL, &globbuf);
                    tmp++;
                }
            }

            fclose(tm);
            break;
            
        case 'r':
            if (pass) {
                system("chattr -i ./template.tbl");

                // Открываем файл temlate.tbl для записи
                FILE *tm = fopen("./template.tbl", "w");
                if (!tm) {
                    fprintf(stderr, "ERROR: problem with template.tbl\nUsage: sudo seq_lab [options]\n\n");
                    return 0;
                }

                uint8_t *p;
                p = md5String(optarg); // Получаем хэш пароля из опции 
                for(unsigned int i = 0; i < 16; ++i){
                    sprintf(arg_pass + i * 2, "%02x", p[i]);
                }

                fprintf(tm, "%s", arg_pass); // Записываем новый пароль
                fclose(tm);

                chmod("./template.tbl", 0);
                system("chattr +i ./template.tbl");

                goto END;

            }
            break;
        
        case 's': // Защита
            if (pass) {
                for (size_t i = 0; i < globbuf.gl_pathc; i++) {
                    if(chmod(globbuf.gl_pathv[i], strtol("0000", 0, 8)) < 0) { // Меняем права доступа
                        fprintf(stderr, "ERROR: problem with protection.\n\n");
                        goto END;
                    }

                    char *fpath; 
                    fpath = calloc(globbuf.gl_pathc + 30, sizeof(char));
                    sprintf(fpath, "chattr +i %s", globbuf.gl_pathv[i]);
                    system(fpath);
                    free(fpath);
                }
            }
            break;
        
        case 'o': // Остановка защиты
            if (pass) {
                for (size_t i = 0; i < globbuf.gl_pathc; i++) {
                    char *fpath; 
                    fpath = calloc(globbuf.gl_pathc + 30, sizeof(char));
                    sprintf(fpath, "chattr -i %s", globbuf.gl_pathv[i]);
                    system(fpath);
                    free(fpath);

                    if(chmod(globbuf.gl_pathv[i], strtol("0664", 0, 8)) < 0) { // Меняем права доступа
                        fprintf(stderr, "ERROR: problem with protection.\n\n");
                        goto END;
                    }
                }
            }
            break;
        
        case 'w': // Вывод имён всех файлов для защиты
            if (pass) {
                for (size_t i = 0; i < globbuf.gl_pathc; i++)
                    printf("%s\n", globbuf.gl_pathv[i]);
            }
            break;
            
        case 'a': // Добавление имён файлов и масок в template.tbl
            if (pass) {
                system("chattr -i ./template.tbl");
                FILE *tm = fopen("./template.tbl", "a");
                if (!tm) {
                    fprintf(stderr, "ERROR: problem with template.tbl\nUsage: sudo seq_lab [options]\n\n");
                    goto END;
                }

                if (optarg) {
                    fprintf(tm, "\n%s", optarg);
                    printf("File added\n\n");
                }
                else {
                    printf("Usage: sudo seq_lab -p <password> -a <file path or mask>\n\n");
                }
                fclose(tm);
                system("chattr +i ./template.tbl");

                goto END;
            }
            break;
        }
        
    }

    END:
    globfree(&globbuf);
    return 0;
}

