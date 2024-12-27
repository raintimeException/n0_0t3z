/*
 * simple note-taking program named 'n0_0t3z'
 * (you can create an alias in your '$HOME/.bashrc' for easier access:))
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

const char *template = "\n\
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

struct stat st = {0};
struct dirent *ent = {0};

int main(int argc, char **argv)
{
        //printf("%s", template);
        (void)argc;
        (void)argv;
        /*
        if (argc < 2) {
                printf("usage: n0_0t3z <file_name>\n");
                exit(1);
        }
        */
        int i;
        char *file = "change_in_the_code"; //*++argv;
        char full_path[1024];
        int count_in_dir_assumed = 1024<<8;
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

        // check files inside
        char *file_names[count_in_dir_assumed];
        for (i = 0; (ent = readdir(dir)) != NULL; ++i) {
                file_names[i] = ent->d_name;
        }
        file_names[++i] = NULL;
        // TODO: implement more sophisticated search
        for (i = 0; file_names[i] != NULL; ++i) {
                if (strcmp(file_names[i], file) == 0) {
                        printf("Error: can't use %s file name, file already exists\n", file);
                        exit(1);
                }
                //printf("%s\n", file_names[i]);
        }

        // create file
        // write into it (with tamplate)
        //      for tamplate use other programs (cal, date...)
        // open a default text editor
        // let write a note
        // ...
        closedir(dir);
        printf("%s\n", path);
        return 0;
}
