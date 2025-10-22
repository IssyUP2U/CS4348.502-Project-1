#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define STR_SIZE 256

void encrypt(const char *reg_pass, const char *key, char *encrypted);
void decrypt(const char *cipher, const char *key, char *decrypted);

//Encryption Function
void encrypt(const char *reg_pass, const char *key, char *encrypted) {
    int key_len = strlen(key);
    int key_index = 0;

    for (int i = 0; reg_pass[i] != '\0'; i++) {
        if (isalpha(reg_pass[i])) {
            char base = isupper(reg_pass[i]) ? 'A' : 'a';
            char key_char = toupper(key[key_index % key_len]) - 'A';
            encrypted[i] = ((toupper(reg_pass[i]) - 'A' + key_char) % 26) + base;
            key_index++;
        } else {
            encrypted[i] = reg_pass[i];
        }
    }
    encrypted[strlen(reg_pass)] = '\0';
}

//Decryption Function
void decrypt(const char *cipher, const char *key, char *decrypted) {
    int key_len = strlen(key);
    int key_index = 0;

    for (int i = 0; cipher[i] != '\0'; i++) {
        if (isalpha(cipher[i])) {
            char key_char = toupper(key[key_index % key_len]) - 'A';

            if (isupper(cipher[i])) {
                decrypted[i] = ((cipher[i] - 'A' - key_char + 26) % 26) + 'A';
            } else {
                decrypted[i] = ((cipher[i] - 'a' - key_char + 26) % 26) + 'a';
            }
            key_index++;
        } else {
            decrypted[i] = cipher[i];
        }
    }
    decrypted[strlen(cipher)] = '\0';
}

int main() {
    char key[STR_SIZE] = "";
    char option[STR_SIZE];
    char output[STR_SIZE];
    //printf("Select an option:\nPASS - Sets the current password.\nKEY - Sets the encryption key\nENCRYPT - Using a Vigenère cypher with the current passkey, encrypt the argument and output the result. If no passkey is set output an error.\nDECRYPT - Using a Vigenère cypher with the current passkey, decrypt the argument and output the result. If no passkey is set output an error.\nQUIT - Exit the program.\n");
    while (1) {
        printf("\nChoose what you want to do: ");  
        if (fgets(option, sizeof(option), stdin) == NULL)
            break;
        for (int i = 0; text[i]; i++) {
            if (!isalpha(text[i])) {
                printf("ERROR - Input must contain only letters\n");
                fflush(stdout);
                continue;
            }
        }

        option[strcspn(option, "\n")] = '\0';

        if (strncmp(option, "PASS ", 5) == 0) {
            strcpy(key, option + 5);
            printf("RESULT Password set\n");
            fflush(stdout);
        } 
        else if (strncmp(option, "ENCRYPT ", 8) == 0) {
            if (strlen(key) == 0) {
                printf("ERROR Password not set\n");
                fflush(stdout);
                continue;
            }
            encrypt(option + 8, key, output);
            printf("RESULT %s\n", output);
            fflush(stdout);
        }
        else if (strncmp(option, "DECRYPT ", 8) == 0) {
            if (strlen(key) == 0) {
                printf("ERROR Password not set\n");
                fflush(stdout);
                continue;
            }
            decrypt(option + 8, key, output);
            printf("RESULT %s\n", output);
            fflush(stdout);
        }
        else if (strcmp(option, "QUIT") == 0) {
            break;
        }
        else {
            printf("ERROR Unknown command\n");
            fflush(stdout);
        }
    }
    return 0;
}
