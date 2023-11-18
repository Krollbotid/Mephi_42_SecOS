#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <errno.h>

void cat_file(const char* filename) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("open");
        return;
    }

    char buf[4096];
    int num_bytes_read;

    while ((num_bytes_read = read(fd, buf, sizeof(buf))) > 0) {
        if (write(STDOUT_FILENO, buf, num_bytes_read) == -1) {
            perror("write");
            close(fd);
            return;
        }
    }

    if (num_bytes_read == -1) {
        perror("read");
    }

    close(fd);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <file_name> [file_name2 ...]\n", argv[0]);
        return 1;
    }

    int i;
    for (i = 1; i < argc; i++) {
        struct stat st;
        if (stat(argv[i], &st) == 0) {
            if (S_ISREG(st.st_mode)) {
                cat_file(argv[i]);
            } else {
                fprintf(stderr, "Error: '%s' is not a regular file\n", argv[i]);
            }
        } else {
            perror("stat");
        }
    }

    return 0;
}