#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

#ifndef n0_0t3z_H
#define n0_0t3z_H

#define LOG_INFO        0
#define DEFAULT_SIZE    1024

char *n_default_dir_name  = "notes";
char *n_default_home_name = "HOME";

static char template_sample[DEFAULT_SIZE] = "\
urgency_lvl: %d (0 - 10)\n\
author: %s\n\
date:   %s\n\
time:   %s\n\
\t\tTITLE:   %s\n\
\t\tCONTENT: %s\n\n\n\
";

typedef struct t_full_path {
    char full_path[DEFAULT_SIZE];
    char *home;
    char *dir;
    char *file_name;
} t_full_path;

typedef struct t_n {
    char *templ;
    t_full_path *t_full_path;
    char *auth;
    char *date;
    char *_time;
} t_n;

void n_set_templ(t_n *n);
void n_fill_full_path(t_full_path *t_full_path, char *file_name);
int n_is_file_name_collides(t_full_path *t_full_path);
int n_set_auth(t_n *n);
int n_set_date(t_n *n);
int n_set_time(t_n *n);

#ifdef n0_0t3z_IMPLEMENTATION

void n_set_templ(t_n *n)
{
    n->templ = template_sample;
}

void n_fill_full_path(t_full_path *t_full_path, char *file_name)
{

    char *HOME = getenv(n_default_home_name);
    if (!HOME) {
        fprintf(stderr, "%s %s", "Error: No matching value found in the environment: ", n_default_home_name);
        exit(1);
    }
    t_full_path->home = HOME;
    t_full_path->dir = n_default_dir_name;
    sprintf(t_full_path->full_path, "%s/%s/%s", HOME, n_default_dir_name, file_name);
}


int n_is_file_name_collides(t_full_path *t_full_path)
{
        int i;
        int count_in_dir_assumed = DEFAULT_SIZE<<8;
        char **file_names = (char **) malloc(count_in_dir_assumed*sizeof(int));
        struct dirent *ent = {0};

        DIR *_dir = opendir(t_full_path->dir);
        for (i = 0; (ent = readdir(_dir)) != NULL; ++i) {
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
                if (strcmp(file_names[i], t_full_path->file_name) == 0) {
                        return 1;
                }
        }
        free(file_names);       // possibly leaks a little if something fails :)
        return 0;
}

int n_set_auth(t_n *n)
{
        char *logname = "LOGNAME";
        strcpy(n->auth, getenv(logname));
        if (!n->auth) {
                fprintf(stderr, "%s %s\n", "Error: No matching value found in the environment: ", logname);
                strcpy(n->auth, " ");
                return -1;
        }
        return 0;
}

int n_set_date(t_n *n)
{
        FILE *f_date = popen("date \"+%Y-%m-%d\"", "r");
        if (!f_date) {
                fprintf(stderr, "%s\n", "Error: can't determine the date");
                return -1;
        }
        fscanf(f_date, "%s", n->date);
        pclose(f_date);
        return 0;
}

int n_set_time(t_n *n)
{
        FILE *f_time = popen("date +%r", "r");
        if (!f_time) {
                fprintf(stderr, "%s\n", "Error: can't determine the time");
                return -1;
        }
        fscanf(f_time, "%s", n->_time);
        pclose(f_time);
        return 0;
}

#endif // n0_0t3z_IMPLEMENTATION
#endif //n0_0t3z_H
