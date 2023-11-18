#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

#define BUF_SIZE 8192

int main(int argc, char *argv[]) {
    int source_fd, dest_fd;
    ssize_t bytes_read, bytes_written;
    char buffer[BUF_SIZE];

    if (argc != 3) {
        fprintf(stderr, "Usage: %s source_file destination_file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    source_fd = open(argv[1], O_RDONLY);
    if (source_fd == -1) {
        perror("Failed to open source_file");
        exit(EXIT_FAILURE);
    }

    dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (dest_fd == -1) {
        perror("Failed to open/create destination_file");
        exit(EXIT_FAILURE);
    }

    while ((bytes_read = read(source_fd, buffer, BUF_SIZE)) > 0) {
        bytes_written = write(dest_fd, buffer, bytes_read);
        if (bytes_written != bytes_read) {
            perror("Error writing to destination_file");
            exit(EXIT_FAILURE);
        }
    }
    if (bytes_read == -1) {
        perror("Error reading from source_file");
        exit(EXIT_FAILURE);
    }

    if (close(source_fd) == -1) {
        perror("Error closing source_file");
        exit(EXIT_FAILURE);
    }

    if (close(dest_fd) == -1) {
        perror("Error closing destination_file");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
