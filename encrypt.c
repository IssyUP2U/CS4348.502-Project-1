#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define STR_SIZE 256

int main() {
    char key[STR_SIZE], pass[STR_SIZE];
    char encrypted_pass[STR_SIZE];
    char decrypted_pass[STR_SIZE];
    int key_index = 0;
    int key_len = strlen(key);

    printf("Select an option:\nPASS - Sets the current passkey to use when encrypting or decrypting.\nENCRYPT - Using a Vigenère cypher with the current passkey, encrypt the argument and output the result. If no passkey is set output an error.\nDECRYPT - Using a Vigenère cypher with the current passkey, decrypt the argument and output the result. If no passkey is set output an error.\nQUIT - Exit the program.\n\n");

    printf("Enter a key (10 letters long): ");
    fgets(key, 10, stdin);
    while (getchar() != '\n'); 
    key[STR_SIZE-1] = '\0';
    
    printf("\nEnter your password (10 letters max): ");
    scanf("%10s", pass);
    pass[STR_SIZE-1] = '\0';
    while (getchar() != '\n'); 

    printf("Key: %s\nPassword: %s\n", key, pass);


    printf("\nEncryption: %s\nDecryption: %s\n", encrypted_pass, decrypted_pass);

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
                char base = isupper(cipher[i]) ? 'A' : 'a';
                char key_char = toupper(key[key_index % key_len]) - 'A';
                decrypted[i] = ((toupper(cipher[i]) - 'A' - key_char + 26) % 26) + base;
                key_index++;
            } else {
                decrypted[i] = cipher[i];
            }
        }
        decrypted[strlen(cipher)] = '\0';
    }
}
