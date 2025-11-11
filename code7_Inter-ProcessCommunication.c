#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>

int main(void) {
    int fd1[2], fd2[2], nbytes = 1, a = 0;
    pid_t pid;
    char string[80];
    char readbuffer[80];
    char ch = 'a', ch1 = '\n';
    FILE *fp;

    pipe(fd1); // First pipe
    pipe(fd2); // Second pipe

    if ((pid = fork()) == -1) {
        perror("fork");
        exit(1);
    }

    // ---------- CHILD PROCESS ----------
    if (pid == 0) {
        close(fd1[1]); // Close write end of pipe 1
        read(fd1[0], readbuffer, sizeof(readbuffer)); // Read filename from pipe 1
        printf("\nFilename '%s' is being read by Child Process through Pipe 1...\n", readbuffer);

        fp = fopen(readbuffer, "r");
        if (fp == NULL) {
            perror("Error opening file");
            exit(1);
        }

        close(fd1[0]);
        close(fd2[0]); // Close read end of pipe 2
        printf("\nContents of %s are being sent to Parent Process through Pipe 2...\n", readbuffer);

        while (a != -1) {
            a = fscanf(fp, "%c", &ch);
            if (a != -1)
                write(fd2[1], &ch, sizeof(ch));
        }

        close(fd2[1]);
        fclose(fp);
        exit(0);
    }

    // ---------- PARENT PROCESS ----------
    else {
        close(fd1[0]); // Close read end of pipe 1
        printf("IN PARENT PROCESS\n");
        printf("\nEnter name of file: ");
        scanf("%s", string);

        printf("Filename is being sent by Parent Process to Child Process through Pipe 1...\n");
        write(fd1[1], string, strlen(string) + 1);

        close(fd1[1]);
        close(fd2[1]);

        printf("\nContents of %s are being received by Parent Process through Pipe 2...\n\n", string);
        printf("IN PARENT PROCESS\n");
        printf("\nReceived Message:\n");

        while (nbytes != 0) {
            nbytes = read(fd2[0], &ch1, sizeof(ch1));
            if (nbytes != 0)
                printf("%c", ch1);
        }

        close(fd2[0]);
    }

    return 0;
}





#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#define MAX_BUF 100

int main() {
    char *myfifo1 = "myfifo1", *myfifo2 = "myfifo2", arr[50];
    int i, words, lines;
    int fd, fd1;

    mkfifo(myfifo1, 0666);

    printf("Enter a message:\n");
    fgets(arr, 50, stdin);

    fd = open(myfifo1, O_WRONLY);
    write(fd, arr, strlen(arr) + 1);
    close(fd);

    unlink(myfifo1);

    fd1 = open(myfifo2, O_RDONLY);
    read(fd1, &i, sizeof(i));
    printf("Total characters: %d\n", i);

    read(fd1, &words, sizeof(words));
    printf("Total words: %d\n", words);

    read(fd1, &lines, sizeof(lines));
    printf("Total lines: %d\n", lines);

    close(fd1);
    return 0;
}




#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define MAX_BUF 100

int main() {
    char *myfifo1 = "myfifo1", *myfifo2 = "myfifo2";
    char buf[50];
    FILE *fp;
    int i = 0, words = 0, lines = 0;
    int fd, fd1;

    mkfifo(myfifo2, 0777);

    fd = open(myfifo1, O_RDONLY);
    read(fd, buf, MAX_BUF);
    close(fd);

    printf("\nMessage received is: %s\n", buf);

    while (buf[i] != '\0') {
        if (buf[i] == ' ')
            words++;
        if (buf[i] == '.' || buf[i] == '?' || buf[i] == '!')
            lines++;
        i++;
    }

    printf("\nTotal characters: %d", i);
    printf("\nTotal words: %d", words + 1);
    printf("\nTotal lines: %d\n", lines);

    fp = fopen("abc.txt", "w+");
    fprintf(fp, "Total characters = %d\n", i);
    fprintf(fp, "Total words = %d\n", words + 1);
    fprintf(fp, "Total lines = %d\n", lines);
    fclose(fp);

    unlink(myfifo1);

    fd1 = open(myfifo2, O_WRONLY);
    write(fd1, &i, sizeof(i));
    write(fd1, &words, sizeof(words));
    write(fd1, &lines, sizeof(lines));
    close(fd1);

    return 0;
}
