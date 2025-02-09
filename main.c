/*
 * simple note-taking program named 'n0_0t3z'
 * (you can create an alias in your '$HOME/.bashrc' for easier access:))
 *
 * todo:
 *  - usage: n0_0t3z file_name (just this and nothing more...)
 *  - flag to check all current notes and sort them by urgency_lvl (and give a title of each)...
 */

#define n0_0t3z_IMPLEMENTATION
#include "n0_0t3z.h"

int main(int argc, char **argv)
{
    char *usage_str = "usage: n0_0t3z <file_name> <title> [<urgency_lvl>] [<editor_name>]";
    if (argc < 3) {
        fprintf(stderr, "%s\n", usage_str);
        exit(1);
    }

    t_n *n = (t_n *) malloc(sizeof(t_n));
    if (!n) {
        fprintf(stderr, "%s\n", "Error: cannot malloc, good bye");
        exit(-1);
    }

    t_full_path *t_full_path = malloc(sizeof(t_full_path));
    if (!t_full_path) {
        fprintf(stderr, "ERROR: cannot malloc in n_fill_full_path\n");
        exit(-1);
    }
    n_fill_full_path(t_full_path, *++argv);

    // check the directory
    struct stat st = {0};
    char default_dir[DEFAULT_SIZE];
    sprintf(default_dir, "%s/%s", t_full_path->home, (char *) t_full_path->dir);
    if (stat(default_dir, &st) == -1) {
            mkdir(default_dir, 0700);
    }
    printf("default_dir: %s\n", default_dir);
    printf("%s\n", t_full_path->full_path);
    return 0;

/*

    if (!title) {
            exit(1);
    }
    strcpy(title, *++argv);

    int urgency_lvl = 0; // default urgency_lvl
    if (*++argv) {
            urgency_lvl = atoi(*argv);
    }

    // create file
    FILE *file = fopen(strcat(, "w");
    if (!file) {
            fprintf(stderr, "%s%s\n", "Error: can't open a file: ", path);
            perror("fopen");
            exit(1);
    }

    if (!auth) {
            fprintf(stderr, "%s\n", "Error: can't malloc for auth:(");
            exit(1);
    }
    get_auth(auth);

    if (!date) {
            exit(1);
    }
    get_date(date);

    if (!_time) {
            fprintf(stderr, "%s\n", "Error: can't malloc for time:(");
            exit(1);
    }
    get_time(_time);

    // constructing final_template
    char final_template[DEFAULT_SIZE<<8];
    snprintf(final_template, sizeof(final_template), n->templ, urgency_lvl, auth, date, _time, title, "");

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
    */
}
