#include <stdio.h>
#include <string.h>

int psw_check(char *psw) {
    FILE *stream = fopen("psw.txt", "r");
    char r_psw[100];
    fgets(r_psw, 100, stream);
    fclose(stream);

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