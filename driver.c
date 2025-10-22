#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define STR_SIZE 256

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }

    int log_in[2], enc_in[2], enc_out[2];
    pipe(log_in);
    pipe(enc_in);
    pipe(enc_out);

    pid_t logger_pid = fork();
    if (logger_pid == 0) {
        dup2(log_in[0], STDIN_FILENO);
        close(log_in[1]);
        execlp("./logger", "logger", argv[1], NULL);
        perror("execlp logger");
        exit(1);
    }

    pid_t encrypt_pid = fork();
    if (encrypt_pid == 0) {
        dup2(enc_in[0], STDIN_FILENO);
        dup2(enc_out[1], STDOUT_FILENO);
        close(enc_in[1]);
        close(enc_out[0]);
        execlp("./encrypt", "encrypt", NULL);
        perror("execlp encrypt");
        exit(1);
    }

    // Close unused pipe ends in parent
    close(log_in[0]);
    close(enc_in[0]);
    close(enc_out[1]);

    FILE *log_fp = fdopen(log_in[1], "w");
    FILE *enc_in_fp = fdopen(enc_in[1], "w");
    FILE *enc_out_fp = fdopen(enc_out[0], "r");

    fprintf(log_fp, "START Driver started.\n");
    fflush(log_fp);

    char cmd[STR_SIZE], arg[STR_SIZE], response[STR_SIZE];

    while (1) {
        printf("Command (password/encrypt/decrypt/history/quit): ");
        fgets(cmd, sizeof(cmd), stdin);
        cmd[strcspn(cmd, "\n")] = '\0';

        if (strcmp(cmd, "quit") == 0) {
            fprintf(enc_in_fp, "QUIT\n");
            fprintf(log_fp, "QUIT Exiting program.\n");
            fflush(enc_in_fp);
            fflush(log_fp);
            break;
        } else if (strcmp(cmd, "password") == 0) {
            printf("Enter passkey: ");
            fgets(arg, sizeof(arg), stdin);
            arg[strcspn(arg, "\n")] = '\0';
            fprintf(enc_in_fp, "PASS %s\n", arg);
            fflush(enc_in_fp);
            fprintf(log_fp, "PASSKEY Set.\n");
        } else if (strcmp(cmd, "encrypt") == 0) {
            printf("Enter string to encrypt: ");
            fgets(arg, sizeof(arg), stdin);
            arg[strcspn(arg, "\n")] = '\0';
            fprintf(enc_in_fp, "ENCRYPT %s\n", arg);
            fflush(enc_in_fp);

            fgets(response, sizeof(response), enc_out_fp);
            response[strcspn(response, "\n")] = '\0';
            printf("%s\n", response);
            fprintf(log_fp, "ENCRYPT %s\n", response);
        } else if (strcmp(cmd, "decrypt") == 0) {
            printf("Enter string to decrypt: ");
            fgets(arg, sizeof(arg), stdin);
            arg[strcspn(arg, "\n")] = '\0';
            fprintf(enc_in_fp, "DECRYPT %s\n", arg);
            fflush(enc_in_fp);

            fgets(response, sizeof(response), enc_out_fp);
            response[strcspn(response, "\n")] = '\0';
            printf("%s\n", response);
            fprintf(log_fp, "DECRYPT %s\n", response);
        }
    }

    // Cleanup
    fprintf(log_fp, "QUIT\n");
    fflush(log_fp);
    fclose(log_fp);
    fclose(enc_in_fp);
    fclose(enc_out_fp);

    waitpid(logger_pid, NULL, 0);
    waitpid(encrypt_pid, NULL, 0);

    return 0;
}
