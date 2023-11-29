#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/file.h>
#include <unistd.h>


#define C_RED     "\x1b[31m"
#define C_GREEN   "\x1b[32m"
#define C_YELLOW  "\x1b[33m"
#define C_BLUE    "\x1b[34m"
#define C_MAGENTA "\x1b[35m"
#define C_CYAN    "\x1b[36m"
#define C_RESET   "\x1b[0m"

#define C_ERR(x) C_RED x C_RESET


void print_help(void) {
    printf(
        C_BLUE "Usage:" C_RESET " run-safe [flags] -c <shell command>\n\n"
        C_GREEN "Flags:\n" C_RESET
        "    --help        - display help\n"
        "    -l <path>     - lockfile path\n"
        "    -Lw           - 'lockfile wait' - wait for the other instances to stop and then run\n\n"
    );
}


void throw_argument_error(const char * sample) {
    if (sample != NULL)
        fprintf(stderr, C_ERR("error: wrong argument: " C_BLUE "%s\n"), sample);
    else
        fprintf(stderr, C_ERR("error: wrong argument\n"));

    exit(1);
}


int main(int argc, const char * argv[]) {
    int exit_status = 0;

    FILE * lockfile;
    int lockfile_fd;
    const char * lockfile_path = NULL;
    unsigned int lock_mode = LOCK_EX | LOCK_NB;

    const char * command = NULL;

    const char * cur_arg;
    if (argc < 2) {
        print_help();
        return 1;
    }
    for (int i = 1; i < argc; i++) {
        cur_arg = argv[i];

        if (cur_arg[0] == '-') switch (cur_arg[1]) {
            // --x
            case '-':
                if (strcmp(cur_arg + 2, "help") == 0) {
                    print_help();
                    return 0;
                }
                throw_argument_error(cur_arg);
                break;
            // -Lx
            case 'L':
                if (cur_arg[2] == 'w') lock_mode &= ~LOCK_NB;
                break;
            // -x
            case 'l':
                lockfile_path = argv[++i];
                break;
            case 'c':
                command = argv[++i];
                break;
            default:
                throw_argument_error(cur_arg);
        } else {
            throw_argument_error(cur_arg);
        }
    }
    if (command == NULL) {
        fprintf(stderr, C_ERR("error: no command specified\n"));
        return 1;
    }

    if (lockfile_path != NULL) {
        lockfile = fopen(lockfile_path, "w");
        if (lockfile == NULL) {
            fprintf(stderr, C_ERR("error: can't open the lockfile '%s'\n"), lockfile_path);
            return 1;
        }
        lockfile_fd = fileno(lockfile);
        if (flock(lockfile_fd, lock_mode) != 0) {
            return 0;
        }
        fprintf(lockfile, "%d", getpid());
        fflush(lockfile);
    }

    system(command);

    if (lockfile_path != NULL) {
        if (flock(lockfile_fd, LOCK_UN) != 0) {
            fprintf(stderr, C_ERR("error: can't unlock the lockfile '%s'\n"), lockfile_path);
            exit_status = 1;
        }
        fclose(lockfile);
    }

    return exit_status;
}
