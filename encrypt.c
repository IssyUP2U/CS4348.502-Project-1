#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define STR_SIZE 11

int main() {
    char key[STR_SIZE], pass[STR_SIZE];

    printf("Enter a key (10 letters long): ");
    fgets(key, 10, stdin);
    while (getchar() != '\n'); 
    key[STR_SIZE-1] = '\0';
    
    printf("\nEnter your password (10 letters max): ");
    scanf("%10s", pass);
    pass[STR_SIZE-1] = '\0';
    while (getchar() != '\n'); 

    printf("Key: %s\nPassword: %s\n", key, pass);

    char encrypted_pass[STR_SIZE];
    char decrypted_pass[STR_SIZE];
    for (int i = 0; i < strlen(pass); i++){
        encrypted_pass[i] = pass[i] + key[i];
    }
    encrypted_pass[STR_SIZE-1] = '\0';
    for (int i = 0; i < strlen(pass); i++){
        decrypted_pass[i] = encrypted_pass[i] - key[i];
    }
    decrypted_pass[STR_SIZE-1] = '\0';

    printf("\nEncryption: %s\nDecryption: %s\n", encrypted_pass, decrypted_pass);
}
