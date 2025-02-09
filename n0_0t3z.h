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

typedef struct t_path {
    char full_path[DEFAULT_SIZE];
    char *home;
    char *dir;
    char *file_name;
} t_path;

typedef struct t_n {
    char *templ;
    int urgency_lvl;
    char auth[DEFAULT_SIZE];
    char date[DEFAULT_SIZE];
    char time[DEFAULT_SIZE];
    char title[DEFAULT_SIZE];
    char *editor;
} t_n;

void n_set_templ(t_n *n);
void n_check_dir(t_path *t_p);
void n_fill_full_path(t_path *t_p, char *file_name);
int n_is_file_name_collides(t_path *t_p);
int n_set_auth(t_n *n);
int n_set_date(t_n *n);
int n_set_time(t_n *n);
int n_set_editor(t_path *t_p, char *editor_name);


#ifdef n0_0t3z_IMPLEMENTATION

void n_set_templ(t_n *n)
{
    n->templ = template_sample;
}

void n_check_dir(t_path *t_p)
{
    struct stat st = {0};
    char default_dir[DEFAULT_SIZE];
    sprintf(default_dir, "%s/%s", t_p->home, (char *) t_p->dir);
    if (stat(default_dir, &st) == -1) {
        mkdir(default_dir, 0700);
    }
}

void n_fill_full_path(t_path *t_p, char *file_name)
{

    char *HOME = getenv(n_default_home_name);
    if (!HOME) {
        fprintf(stderr, "%s%s", "Error: No matching value found in the environment: ", n_default_home_name);
        exit(1);
    }
    t_p->home = HOME;
    t_p->dir = n_default_dir_name;
    t_p->file_name = file_name;
    if (n_is_file_name_collides(t_p)) {
        fprintf(stderr, "%s%s\n", "Error: file already exists: ", t_p->file_name);
        exit(1);
    }
    sprintf(t_p->full_path, "%s/%s/%s", HOME, n_default_dir_name, file_name);
}


int n_is_file_name_collides(t_path *t_p)
{
    int i;
    int count_in_dir_assumed = DEFAULT_SIZE<<8;
    char **file_names = (char **) malloc(count_in_dir_assumed*sizeof(int));
    if (!file_names) {
        fprintf(stderr, "ERROR: cannot malloc %s %d\n", __FILE__, __LINE__);
        exit(-1);
    }
    struct dirent *ent = {0};

    char path_to_dir[DEFAULT_SIZE] = {0};
    strcat(path_to_dir, t_p->home);
    strcat(path_to_dir, "/");
    strcat(path_to_dir, t_p->dir);

    DIR *_dir = opendir(path_to_dir);
    {   // fill file_names
        for (i = 0; (ent = readdir(_dir)) != NULL; ++i) {
            file_names[i] = ent->d_name;
        }
        file_names[++i] = NULL;
    }
    for (i = 0; file_names[i] != NULL; ++i) {
        if (strcmp(file_names[i], t_p->file_name) == 0) {
            free(file_names);
            return 1;
        }
    }
    free(file_names);
    return 0;
}

int n_set_auth(t_n *n)
{
    char *logname = "LOGNAME";
    if (!getenv(logname)) {
        fprintf(stderr, "%s %s\n", "Error: No matching value found in the environment: ", logname);
        strcpy(n->auth, "");
        return -1;
    }
    strcpy(n->auth, getenv(logname));
    return 0;
}

int n_set_date(t_n *n)
{
    FILE *f_date = popen("date \"+%Y-%m-%d\"", "r");
    if (!f_date) {
        fprintf(stderr, "%s\n", "Error: can't determine the date");
        strcpy(n->auth, "");
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
    fscanf(f_time, "%s", n->time);
    pclose(f_time);
    return 0;
}

int n_set_editor(t_path *t_p, char *editor_name)
{
    char *full_editor_command = (char *) malloc(DEFAULT_SIZE + sizeof(t_p->full_path));
    if (!full_editor_command) {
        fprintf(stderr, "ERROR: cannot malloc %s %d\n", __FILE__, __LINE__);
        exit(-1);
    }
    sprintf(full_editor_command, "%s %s", editor_name, t_p->full_path);

    if (system(full_editor_command) != 0) {
        fprintf(stderr, "%s\n", "Error: Can't run editor");
        perror("system");
        goto n_exit;
    }
n_exit:
    free(full_editor_command);
    exit(1);
}

#endif // n0_0t3z_IMPLEMENTATION
#endif //n0_0t3z_H
