#include <stdio.h>
#include <string.h>

#define KEY '5'

void encrypt_psw(char *psw, int len) {
    for (int i = 0; i != len; i++) {
        psw[i] = psw[i] ^ KEY;
    }
}

int psw_check(char *psw) {
    FILE *stream = fopen("psw.txt", "r");
    char r_psw[100];
    fgets(r_psw, 100, stream);
    fclose(stream);

    encrypt_psw(r_psw, strlen(r_psw));

    if (strcmp(r_psw, psw) == 0) {
        return 0;
    } else {
        return -1;
    }
}

int main(void) {

    char password[100] = { 0 };

    printf("Input password >>> ");
    fflush(stdin);
    scanf("%s", &password);

    if (psw_check(password) != 0) {
        printf("Authentication error!");
    } else {
        printf("Authentication passed!");
    }

    scanf("%d", &password);

    return 0;
}