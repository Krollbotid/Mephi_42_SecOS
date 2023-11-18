#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <mode> <file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    char *mode_str = argv[1];
    char *filename = argv[2];
    
    mode_t mode = (mode_t) strtol(mode_str, NULL, 8);
    
    if (chmod(filename, mode) == -1) {
        perror("chmod");
        exit(EXIT_FAILURE);
    }
    
    printf("File permissions changed successfully.\n");
    return EXIT_SUCCESS;
}