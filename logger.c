// Type "QUIT" to quit
// format: YYYY-MM-DD HH:MM [ACTION] MESSAGE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void write_log(FILE *logfile, const char *line) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char timestr[20];
    strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M", t);

    char action[64], message[512];
    if (sscanf(line, "%s %[^\n]", action, message) < 2) {
        strcpy(message, "");
    }

    fprintf(logfile, "%s [%s] %s\n", timestr, action, message);
    fflush(logfile);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <logfile>\n", argv[0]);
        return 1;
    }

    FILE *logfile = fopen(argv[1], "a");
    if (!logfile) {
        perror("fopen");
        return 1;
    }

    char line[512];
    while (fgets(line, sizeof(line), stdin)) {
        line[strcspn(line, "\n")] = '\0';
        if (strcmp(line, "QUIT") == 0)
            break;
        write_log(logfile, line);
    }

    fclose(logfile);
    return 0;
}
