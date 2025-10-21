#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define STR_SIZE 256
//functions
    void setKey(char *key){
        printf("Enter a key: ");
        scanf("%10s", key);
        while (getchar() != '\n'); 
        key[STR_SIZE-1] = '\0';
    }
    void setPass(char *pass){
        printf("\nEnter your password (10 letters max): ");
        scanf("%10s", pass);
        while (getchar() != '\n');
        pass[STR_SIZE-1] = '\0';
    }

    //encryption method. accept pass, key, and encrypted pointers
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

    //decryption method
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
    char key[STR_SIZE], pass[STR_SIZE];
    char encrypted_pass[STR_SIZE];
    char decrypted_pass[STR_SIZE];
    int key_index = 0;
    int key_len = strlen(key);
    char option[STR_SIZE];
    bool quit = false;

    printf("Select an option:\nPASS - Sets the current password.\nKEY - Sets the encryption key\nENCRYPT - Using a Vigenère cypher with the current passkey, encrypt the argument and output the result. If no passkey is set output an error.\nDECRYPT - Using a Vigenère cypher with the current passkey, decrypt the argument and output the result. If no passkey is set output an error.\nQUIT - Exit the program.\n");
    while(quit != true){
        printf("\nChoose what you want to do: ");  
        scanf("%s", option);
        while (getchar() != '\n'); 
        option[STR_SIZE-1] = '\0';  
        for (int i = 0; option[i] != '\0'; i++) {
            option[i] = tolower((unsigned char)option[i]);
        }
        //use chosen method
        if (strcmp(option, "quit") == 0) {
            printf("\n\nEXITING...\n\n");
            quit = true;
        } 
        else if (strcmp(option, "pass") == 0){
            setPass(pass);
        }
        else if (strcmp(option, "key") == 0){
            setKey(key);
        }
        else if (strcmp(option, "encrypt") == 0) {
            encrypt(pass, key, encrypted_pass);
            printf("\nPassword %s encrypted to %s\n\n", pass, encrypted_pass);
        }
        else if (strcmp(option, "decrypt") == 0) {
            decrypt(encrypted_pass, key, decrypted_pass);
            printf("\nEncrypted password %s decrypted to %s\n\n", encrypted_pass, decrypted_pass);
        }
    }

}
