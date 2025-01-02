/*
 * simple note-taking program named 'n0_0t3z'
 * (you can create an alias in your '$HOME/.bashrc' for easier access:))
 *
 * todo:
 *      - flag to check all current notes and sort them by urgency_lvl (and give a title of each)...
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#define LOG_INFO        0
#define DEFAULT_SIZE    1024

static char template[DEFAULT_SIZE] = "\
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
        char *home_name = "HOME";
        char *HOME = getenv(home_name);

        if (!HOME) {
                fprintf(stderr, "%s %s", "Error: No matching value found in the environment: ", home_name);
                exit(1);
        }
        sprintf(full_path, "%s/%s", HOME, dir_name);
        return full_path;
}


int is_file_name_collides(DIR *dir, char *file_name)
{
        int i;
        int count_in_dir_assumed = DEFAULT_SIZE<<8;
        char **file_names = (char **) malloc(count_in_dir_assumed*sizeof(int));
        struct dirent *ent = {0};

        for (i = 0; (ent = readdir(dir)) != NULL; ++i) {
                file_names[i] = ent->d_name;
        }
        file_names[++i] = NULL;
#if LOG_INFO == 1
        fprintf(stdout, "%s", "files in the directory: \n");
#endif
        // TODO: implement more sophisticated search
        for (i = 0; file_names[i] != NULL; ++i) {
#if LOG_INFO == 1
                fprintf(stdout, "==>\t%s\n", file_names[i]);
#endif
                if (strcmp(file_names[i], file_name) == 0) {
                        return 1;
                }
        }
        free(file_names);       // possibly leaks a little if something fails :)
        return 0;
}

int get_auth(char *auth)
{
        char *logname = "LOGNAME";
        strcpy(auth, getenv(logname));
        if (!auth) {
                fprintf(stderr, "%s %s\n", "Error: No matching value found in the environment: ", logname);
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

int get_time(char *_time)
{
        FILE *f_time = popen("date +%r", "r");
        if (!f_time) {
                fprintf(stderr, "%s\n", "Error: can't determine the time");
                return -1;
        }
        fscanf(f_time, "%s", _time);
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
        char full_path[DEFAULT_SIZE];
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

        char *title = (char *) malloc(DEFAULT_SIZE*sizeof(char));
        if (!title) {
                fprintf(stderr, "%s\n", "Error: can't malloc for title:(");
                exit(1);
        }
        strcpy(title, *++argv);

        int urgency_lvl = 0; // default urgency_lvl
        if (*++argv) {
                urgency_lvl = atoi(*argv);
        }

        // create file
        sprintf(full_path, "%s/%s", path, file_name);
        FILE *file = fopen(full_path, "w");
        if (!file) {
                fprintf(stderr, "%s%s\n", "Error: can't open a file: ", path);
                perror("fopen");
                exit(1);
        }

        char *auth = (char *) malloc(DEFAULT_SIZE*sizeof(char));
        if (!auth) {
                fprintf(stderr, "%s\n", "Error: can't malloc for auth:(");
                exit(1);
        }
        get_auth(auth);

        char *date = (char *) malloc(DEFAULT_SIZE*sizeof(char));
        if (!date) {
                fprintf(stderr, "%s\n", "Error: can't malloc for date:(");
                exit(1);
        }
        get_date(date);

        char *_time = (char *) malloc(DEFAULT_SIZE*sizeof(char));
        if (!_time) {
                fprintf(stderr, "%s\n", "Error: can't malloc for time:(");
                exit(1);
        }
        get_time(_time);

        // constructing final_template
        char final_template[DEFAULT_SIZE<<8];
        snprintf(final_template, sizeof(final_template), template, urgency_lvl, auth, date, _time, title, "");

#if LOG_INFO == 1
        fprintf(stdout, "%s", final_template);
#endif

        // write into it (with tamplate)
        fprintf(file, "%s", final_template);
        fflush(file);
        fclose(file);
        closedir(dir);

        char *editor_name = *++argv;
        if (editor_name == NULL) {
                fprintf(stderr, "%s\n", "Warn: The name of the text editor has not been provided");
                editor_name = getenv("EDITOR");
        }

        // using default editor to take a note
        if (!editor_name) {
                fprintf(stderr, "%s\n", "Error: Can't get the default editor");
                fprintf(stderr, "%s\n", "Possible solution: check the env variable, execute: $> printenv | grep EDITOR");
                exit(1);
        }

        char *full_editor_comm = (char *) malloc(sizeof(full_path)*2);
        sprintf(full_editor_comm, "%s %s", editor_name, full_path);

        if (system(full_editor_comm) != 0) {
                fprintf(stderr, "%s\n", "Error: Can't run editor");
                perror("system");
                exit(1);
        }

#if LOG_INFO == 1
        fprintf(stdout, "%s\n", path);
#endif

        free(auth);
        free(date);
        free(title);
        free(_time);
        free(full_editor_comm);

#if LOG_INFO == 1
        fprintf(stdout, "%s\n", "FIN");
#endif
        return 0;
}
