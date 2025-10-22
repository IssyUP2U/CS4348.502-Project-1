#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

#define STR_SIZE 256
#define MAX_HISTORY 100

// Helper to validate only letters
bool is_letters_only(const char *s) {
    for (int i = 0; s[i]; i++) {
        if (!isalpha((unsigned char)s[i])) return false;
    }
    return true;
}

// Helper to convert to uppercase
void to_uppercase(char *s) {
    for (int i = 0; s[i]; i++) {
        s[i] = toupper((unsigned char)s[i]);
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }

    int log_in[2], enc_in[2], enc_out[2];
    if (pipe(log_in) == -1 || pipe(enc_in) == -1 || pipe(enc_out) == -1) {
        perror("pipe");
        exit(1);
    }

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

    // Parent process
    close(log_in[0]);
    close(enc_in[0]);
    close(enc_out[1]);

    FILE *log_fp = fdopen(log_in[1], "w");
    FILE *enc_in_fp = fdopen(enc_in[1], "w");
    FILE *enc_out_fp = fdopen(enc_out[0], "r");

    fprintf(log_fp, "START Driver started.\n");
    fflush(log_fp);

    char cmd[STR_SIZE], arg[STR_SIZE], response[STR_SIZE];
    char history[MAX_HISTORY][STR_SIZE];
    int hist_count = 0;
    bool running = true;

    //option selection loop
    while (running) {
        printf("\nCommands: password | encrypt | decrypt | history | quit\n> ");
        if (fgets(cmd, sizeof(cmd), stdin) == NULL) break;
        cmd[strcspn(cmd, "\n")] = '\0';

        //lowercase command
        for (int i = 0; cmd[i]; i++) cmd[i] = tolower((unsigned char)cmd[i]);

        if (strcmp(cmd, "quit") == 0) {
            fprintf(enc_in_fp, "QUIT\n");
            fflush(enc_in_fp);
            fprintf(log_fp, "QUIT Exiting program.\n");
            fflush(log_fp);
            running = false;
        }
        else if (strcmp(cmd, "password") == 0) {
            printf("Enter new password string: ");
            fgets(arg, sizeof(arg), stdin);
            arg[strcspn(arg, "\n")] = '\0';

            if (!is_letters_only(arg)) {
                printf("ERROR: Password must contain only letters.\n");
                continue;
            }

            to_uppercase(arg);
            fprintf(enc_in_fp, "PASS %s\n", arg);
            fflush(enc_in_fp);
            fprintf(log_fp, "PASS Password set.\n");
            fflush(log_fp);

            
            if (fgets(response, sizeof(response), enc_out_fp)) {
                printf("%s\n", response);
            }
        }
        else if (strcmp(cmd, "encrypt") == 0 || strcmp(cmd, "decrypt") == 0) {
            bool use_history = false;
            printf("Use history? (y/n): ");
            char choice;
            scanf(" %c", &choice);
            while (getchar() != '\n'); // clear buffer
            if (tolower(choice) == 'y' && hist_count > 0) {
                use_history = true;
            }

            if (use_history) {
                printf("\nHistory:\n");
                for (int i = 0; i < hist_count; i++) {
                    printf("%d. %s\n", i + 1, history[i]);
                }
                printf("Select number (or 0 to enter new): ");
                int sel = 0;
                scanf("%d", &sel);
                while (getchar() != '\n');

                if (sel > 0 && sel <= hist_count) {
                    strcpy(arg, history[sel - 1]);
                } else {
                    printf("Enter new string: ");
                    fgets(arg, sizeof(arg), stdin);
                    arg[strcspn(arg, "\n")] = '\0';
                }
            } else {
                printf("Enter string: ");
                fgets(arg, sizeof(arg), stdin);
                arg[strcspn(arg, "\n")] = '\0';
            }

            if (!is_letters_only(arg)) {
                printf("ERROR: Input must contain only letters.\n");
                continue;
            }

            to_uppercase(arg);
            fprintf(enc_in_fp, "%s %s\n", strcmp(cmd, "encrypt") == 0 ? "ENCRYPT" : "DECRYPT", arg);
            fflush(enc_in_fp);

            if (fgets(response, sizeof(response), enc_out_fp)) {
                response[strcspn(response, "\n")] = '\0';
                printf("%s\n", response);
                fprintf(log_fp, "%s %s\n", strcmp(cmd, "encrypt") == 0 ? "ENCRYPT" : "DECRYPT", response);
                fflush(log_fp);

                // Store both input and result in history
                if (hist_count < MAX_HISTORY) {
                    strcpy(history[hist_count++], arg);
                    if (strncmp(response, "RESULT ", 7) == 0) {
                        strcpy(history[hist_count++], response + 7);
                    }
                }
            }
        }
        else if (strcmp(cmd, "history") == 0) {
            printf("\nHistory:\n");
            if (hist_count == 0) {
                printf("(empty)\n");
            } else {
                for (int i = 0; i < hist_count; i++) {
                    printf("%d. %s\n", i + 1, history[i]);
                }
            }
        }
        else {
            printf("Unknown command.\n");
        }
    }

    fprintf(log_fp, "STOP Driver exited.\n");
    fprintf(log_fp, "QUIT\n");
    fflush(log_fp);

    fclose(log_fp);
    fclose(enc_in_fp);
    fclose(enc_out_fp);

    waitpid(logger_pid, NULL, 0);
    waitpid(encrypt_pid, NULL, 0);

    return 0;
}
