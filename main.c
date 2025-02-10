/*
 * simple note-taking program named 'n0_0t3z'
 * (you can create an alias in your '$HOME/.bashrc' for easier access:))
 */

#define n0_0t3z_IMPLEMENTATION
#include "n0_0t3z.h"

int main(int argc, char **argv)
{
    char *usage_str = "usage: n0_0t3z <file_name> <title> [<urgency_lvl>] [<editor_name>]\nusage: n0_0t3z <file_name>";
    //-------------------------------------------------------------
    // n0_0t3z <file_name> <title> [<urgency_lvl>] [<editor_name>]
    //  1 (0)     2 (1)     3 (2)      4 (3)          5 (4)
    //-------------------------------------------------------------
    // n0_0t3z <file_name>
    //  1 (0)     2 (1)
    //-------------------------------------------------------------

    if (argc < 2) {
        fprintf(stderr, "%s\n", usage_str);
        exit(1);
    }

    t_n *n = (t_n *) malloc(sizeof(t_n));
    if (!n) {
        fprintf(stderr, "%s\n", "ERROR: cannot malloc, good bye");
        exit(-1);
    }

    n_set_templ(n);

    t_path *t_p = (t_path*)malloc(sizeof(t_path));
    if (!t_p) {
        fprintf(stderr, "%s\n", "ERROR: cannot malloc, good bye");
        exit(-1);
    }

    n_fill_full_path(t_p, argv[1]);
    n_check_dir(t_p);

    if (argc > 2) {
        strcpy(n->title, argv[2]);
    } else {
        strcpy(n->title, t_p->file_name);
    }
    n->urgency_lvl = 0; // default urgency_lvl
    if (argc > 3) {
        n->urgency_lvl = atoi(argv[3]);
    }

    // create file
    FILE *file = fopen(t_p->full_path, "w");
    if (!file) {
        fprintf(stderr, "%s%s\n", "ERROR: can't open a file: ", t_p->full_path);
        perror("fopen");
        exit(1);
    }

    n_set_auth(n);
    if (strcmp(n->auth, "") == 0) {
        fprintf(stderr, "%s%s\n", "Er: auth: ", n->auth);
        exit(1);
    }

    n_set_date(n);
    if (strcmp(n->date, "") == 0) {
        fprintf(stderr, "%s%s\n", "ERROR: date: ", n->date);
        exit(1);
    }

    n_set_time(n);
    if (strcmp(n->time, "") == 0) {
        fprintf(stderr, "%s%s\n", "ERROR: time: ", n->time);
        exit(1);
    }

    // constructing final_template
    char final_template[DEFAULT_SIZE<<8];
    snprintf(final_template, sizeof(final_template), n->templ, n->urgency_lvl, n->auth, n->date, n->time, n->title, "");

    // write into it (with tamplate)
    fprintf(file, "%s", final_template);
    fflush(file);
    fclose(file);

    char *editor_name;
    if (argc < 5) {
        fprintf(stdout, "%s\n", "WARN: The name of the text editor has not been provided");
        editor_name = getenv("EDITOR");
    } else {
        editor_name = argv[4];
    }
    n_set_editor(t_p, editor_name);

    free(t_p);
    free(n);
    return 0;
}
