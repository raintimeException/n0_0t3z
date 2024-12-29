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

char template[1024] = "\
author: %s\n\
date:   %s\n\
time:   %s\n\
  TITLE:   %s\n\
  CONTENT: %s\n\n\n\
";

char *fill_path(char *full_path)
{
        char *dir_name = "notes";
        char *home_env = "HOME";
        char *HOME = getenv(home_env);

        if (!HOME) {
                fprintf(stderr, "%s %s", "Error: No matching value found in the environment: ", home_env);
                exit(1);
        }
        sprintf(full_path, "%s/%s", HOME, dir_name);
        return full_path;
}

int is_file_name_collides(DIR *dir, char *file_name)
{
        int i;
        struct dirent *ent = {0};
        int count_in_dir_assumed = 1024<<8;
        char *file_names[count_in_dir_assumed];

        for (i = 0; (ent = readdir(dir)) != NULL; ++i) {
                file_names[i] = ent->d_name;
        }
        file_names[++i] = NULL;
#if DEBUG == 1
        fprintf(stdout, "%s", "files in the directory: \n");
#endif
        // TODO: implement more sophisticated search
        for (i = 0; file_names[i] != NULL; ++i) {
#if DEBUG == 1
                fprintf(stdout, "==>\t%s\n", file_names[i]);
#endif
                if (strcmp(file_names[i], file_name) == 0) {
                        return 1;
                }
        }
        return -1;
}

char *get_author(void)
{
        char *logname_env = "LOGNAME";
        char *author = getenv(logname_env);
        if (!author) {
                fprintf(stderr, "%s %s\n", "Error: No matching value found in the environment: ", logname_env);
                strcpy(author, " ");
        }
        fprintf(stdout, "%s\n", author);
        return author;
}

struct stat st = {0};

int main(int argc, char **argv)
{
        (void)argc;
        (void)argv;
/* @production: uncoment this
        if (argc < 2) {
                fprintf(stderr, "%s\n", "usage: n0_0t3z <file_name>");
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
                fprintf(stderr, "%s %s\n", "Error: can't open the directory: ", path);
                exit(1);
        }
/* @production: uncoment this
        if (is_file_name_collides(dir, file_name)) {
                fprintf(stderr, "%s%s%s\n", "Error: can't use '", file_name, "' file name, file already exists");
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
        char *author = get_author();

        char date[2<<6] = {0};
        FILE *f_date = popen("date \"+%Y-%m-%d\"", "r");
        if (!f_date) {
                fprintf(stderr, "%s\n", "Error: can't determine the date");
        }
        fscanf(f_date, "%s", date);
        pclose(f_date);

        // constructing final_template
        char final_template[1024<<8];
        snprintf(final_template, sizeof(final_template), template, author, date, "", "", "");
        fprintf(stdout, "%s", final_template);


        // write into it (with tamplate)
        fprintf(file, "%s", final_template);

        // open a default text editor
        // let write a note
        // ...

        closedir(dir); // TODO: check when i really need to close it...
        printf("%s\n", path);
        return 0;
}
