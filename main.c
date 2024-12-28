/*
 * simple note-taking program named 'n0_0t3z'
 * (you can create an alias in your '$HOME/.bashrc' for easier access:))
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define DEBUG 0         // 1-true, 0-false

const char *template = "\
author: \n\
time: \n\
calend: \n\
        ************\n\
        ************\n\
        ************\n\
        ************\n\
  TITLE: \n\
  CONTENT: \n\
        ... \n\
";

char *dir_name = "notes";

char *fill_path(char *full_path)
{
        char *HOME = getenv("HOME");
        if (!HOME) {
                printf("Error: No matching value found in the environment.\n");
                exit(1);
        }
        sprintf(full_path, "%s/%s", HOME, dir_name);
        return full_path;
}

struct dirent *ent = {0};
int is_file_name_collides(DIR *dir, char *file_name)
{
        int i;
        int count_in_dir_assumed = 1024<<8;
        char *file_names[count_in_dir_assumed];

        for (i = 0; (ent = readdir(dir)) != NULL; ++i) {
                file_names[i] = ent->d_name;
        }
        file_names[++i] = NULL;
#if DEBUG == 1
        printf("files in the directory: \n");
#endif
        // TODO: implement more sophisticated search
        for (i = 0; file_names[i] != NULL; ++i) {
#if DEBUG == 1
                printf("==>\t%s\n", file_names[i]);
#endif
                if (strcmp(file_names[i], file_name) == 0) {
                        return 1;
                }
        }
        return -1;
}

struct stat st = {0};

int main(int argc, char **argv)
{
#if DEBUG == 1
        printf("==> TEMPLATE <==\n");
        printf("%s", template);
        printf("==> TEMPLATE <==\n");
#endif
        (void)argc;
        (void)argv;
/* @production: uncoment this
        if (argc < 2) {
                printf("usage: n0_0t3z <file_name>\n");
                exit(1);
        }
*/
        char *file_name = "change_in_the_code"; //*++argv;
        char full_path[1024];
        char *path = fill_path(full_path);

        // check the directory
        if (stat(path, &st) == -1) {
                mkdir(path, 0700);
        }

        DIR *dir = opendir(path);
        if (!dir) {
                printf("Error: can't open the directory: %s\n", path);
                exit(1);
        }
/* @production: uncoment this
        if (is_file_name_collides(dir, file_name)) {
                printf("Error: can't use '%s' file name, file already exists\n", file_name);
                exit(1);
        }
*/

        // create file
        sprintf(full_path, "%s/%s", path, file_name);
        FILE *file = fopen(full_path, "w");
        if (!file) {
                printf("Error: can't open a file: %s\n", path);
                perror("fopen");
                exit(1);
        }
        // for tamplate use other programs (cal, date...)

        // write into it (with tamplate)
        fprintf(file, "%s", template);

        // open a default text editor
        // let write a note
        // ...

        closedir(dir); // TODO: check when i really need to close it...
        printf("%s\n", path);
        return 0;
}
