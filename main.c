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
urgency_lvl: %d (0 - 10)\n\
author: %s\n\
date:   %s\n\
time:   %s\n\
\t\tTITLE:   %s\n\
\t\tCONTENT: %s\n\n\n\
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
        return 0;
}

int get_auth(char *auth)
{
        char *logname_env = "LOGNAME";
        strcpy(auth, getenv(logname_env));
        if (!auth) {
                fprintf(stderr, "%s %s\n", "Error: No matching value found in the environment: ", logname_env);
                strcpy(auth, " ");
                return -1;
        }
        return 0;
}

int get_date(char *date)
{
        FILE *f_date = popen("date \"+%Y-%m-%d\"", "r");
        if (!f_date) {
                fprintf(stderr, "%s\n", "Error: can't determine the date");
                return -1;
        }
        fscanf(f_date, "%s", date);
        pclose(f_date);
        return 0;
}

int get_time(char *time)
{
        FILE *f_time = popen("date +%r", "r");
        if (!f_time) {
                fprintf(stderr, "%s\n", "Error: can't determine the time");
                return -1;
        }
        fscanf(f_time, "%s", time);
        pclose(f_time);
        return 0;
}


int main(int argc, char **argv)
{
        char *usage_str = "usage: n0_0t3z <file_name> <title> [<urgency_lvl>] [<editor_name>]";
        if (argc < 3) {
                fprintf(stderr, "%s\n", usage_str);
                exit(1);
        }

        char *file_name = *++argv;
        char full_path[1024];
        char *path = fill_path(full_path);

        // check the directory
        struct stat st = {0};
        if (stat(path, &st) == -1) {
                mkdir(path, 0700);
        }

        DIR *dir = opendir(path);
        if (!dir) {
                fprintf(stderr, "%s %s\n", "Error: can't open the directory: ", path);
                exit(1);
        }

        if (is_file_name_collides(dir, file_name)) {
                fprintf(stderr, "%s%s%s\n", "Error: can't use '", file_name, "' file name, file already exists");
                exit(1);
        }

        char *title = (char *) malloc(255*sizeof(char));
        if (!title) {
                fprintf(stderr, "%s\n", "Error: can't malloc for title:(");
                exit(1);
        }
        strcpy(title, *++argv);

        int urgency_lvl = 0; // default urgency_lvl
        if (*++argv) {
                urgency_lvl = atoi(*++argv);
        }

        // create file
        sprintf(full_path, "%s/%s", path, file_name);
        FILE *file = fopen(full_path, "w");
        if (!file) {
                fprintf(stderr, "%s%s\n", "Error: can't open a file: ", path);
                perror("fopen");
                exit(1);
        }

        char *auth = (char *) malloc(128*sizeof(char));
        if (!auth) {
                fprintf(stderr, "%s\n", "Error: can't malloc for auth:(");
                exit(1);
        }
        get_auth(auth);

        char *date = (char *) malloc(128*sizeof(char));
        if (!date) {
                fprintf(stderr, "%s\n", "Error: can't malloc for date:(");
                exit(1);
        }
        get_date(date);

        char *time = (char *) malloc(255*sizeof(char));
        if (!time) {
                fprintf(stderr, "%s\n", "Error: can't malloc for time:(");
                exit(1);
        }
        get_time(time);

        // constructing final_template
        char final_template[1024<<8];
        snprintf(final_template, sizeof(final_template), template, urgency_lvl, auth, date, time, title, "");
        fprintf(stdout, "%s", final_template);

        // write into it (with tamplate)
        fprintf(file, "%s", final_template);

        // open a default text editor
        // let write a note
        // ...

        closedir(dir); // TODO: check when i really need to close it...
        fprintf(stdout, "%s\n", path);
        free(auth);
        free(date);
        free(title);
        free(time);
        fprintf(stdout, "%s\n", "FIN");
        return 0;
}
