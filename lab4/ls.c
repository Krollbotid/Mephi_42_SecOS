#include <stdio.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    struct linux_dirent {
        unsigned long d_ino;
        off_t d_off;
        unsigned short d_reclen;
        char d_name[];
    };
    const int BUF_SIZE = 1024;
    char buf[BUF_SIZE];

    char *path = ".";
    if (argc == 2) {
        path = argv[1];
    }

    if (argc > 2) {
        fprintf(stderr, "Usage: %s [directory]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int fd = open(path, O_RDONLY | O_DIRECTORY);
    if (fd == -1) {
        perror("Failed to open directory");
        exit(EXIT_FAILURE);
    }

    while(true) {
        int nread = syscall(SYS_getdents, fd, buf, 1024);
        if (nread == -1) {
            perror("Error reading directory");
            exit(EXIT_FAILURE);
        }
        if (nread == 0) {
            break;
        }

        for (int bpos = 0; bpos < nread;) { 
            struct linux_dirent *d = (struct linux_dirent *) (buf + bpos);
            printf("%s\n", d->d_name);
            bpos += d->d_reclen;
        }
    }

    if (close(fd) == -1) {
        perror("Failed to close directory");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
