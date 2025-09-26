#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main() {
    int fd1, fd2;
    fd_set read_fds;
    struct timeval timeout;

    // Open two example files (you can use sockets or pipes as well)
    fd1 = open("file1.txt", O_RDONLY);
    fd2 = open("file2.txt", O_RDONLY);

    if (fd1 == -1 || fd2 == -1) {
        perror("Error opening files");
        exit(EXIT_FAILURE);
    }

    // Clear the file descriptor set
    FD_ZERO(&read_fds);

    // Add file descriptors to the set
    FD_SET(fd1, &read_fds);
    FD_SET(fd2, &read_fds);

    // Set the timeout (5 seconds)
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    // Use select to monitor file descriptors
    int max_fd = (fd1 > fd2) ? fd1 : fd2;
    int ready_fds = select(max_fd + 1, &read_fds, NULL, NULL, &timeout);
    int bytes_read =0;
    if (ready_fds == -1) {
        perror("select");
        exit(EXIT_FAILURE);
    } else if (ready_fds == 0) {
        printf("No data available within the timeout period.\n");
    } else {
        printf("Data is available on the following file descriptors:\n");

        if (FD_ISSET(fd1, &read_fds)) {
            printf("File Descriptor 1 (file1.txt)\n");
            // Read data from fd1
            char* c = (char*) malloc(2048);
            bytes_read = read(fd1, c, sizeof(c));
            printf("Read %d bytes : %s\n", bytes_read, c);
        }

        if (FD_ISSET(fd2, &read_fds)) {
            printf("File Descriptor 2 (file2.txt)\n");
            // Read data from fd2
        }
    }

    // Close the file descriptors
    close(fd1);
    close(fd2);

    return 0;
}
