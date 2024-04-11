#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define BUFF 1000
#define MAX_PLENGHT 100

struct config
{
    int trash_cycles;
    char trash_cycle[MAX_PLENGHT];

    int trash_vars;
    char trash_var[MAX_PLENGHT];

    int trash_funcs;
    char trash_func[MAX_PLENGHT];
} cfg;

void clear(char *arr, int len) {
    for (int i = 0; i != len; i++) {
        arr[i] = '\0';
    }
}

//copies first n symbols of arr2 -> arr1
void copy(char *arr1, char *arr2, int n) {
    for (int i = 0; i < n; i++) {
        arr1[i] = arr2[i];
    }
}

void display(char *arr) {
    for (int i = 0; arr[i] != 0; i++) {
        printf("%c", arr[i]);
    }
    printf("\n");
}

int read_parameter(FILE *stream, char *destination) {

    char *param_string = (char *)malloc(sizeof(char) * MAX_PLENGHT);
    if (param_string == NULL) {
        printf("error occured while allocating memory!\n");
    }
    clear(param_string, MAX_PLENGHT);
    int i = 0;
    char c;
    while ((c = fgetc(stream)) != '=') 
        { }
    c = fgetc(stream);

    if (destination == NULL) {
        int param_value = 0;

        while (c != '\0' && c != '\n' && c != EOF) {
            param_value = param_value * 10 + (c - '0');

            i++;
            c = fgetc(stream);
        }
        c = fgetc(stream);

        return param_value;
    } else {
        int i = 0;
        while ((c = fgetc(stream)) != '"') {
            destination[i] = c;
            i++;
        }
        destination[i+1] = '\0';
    }

    free(param_string);
}

void import_config() {
    FILE *config_file = fopen("obf_config.cfg", "r");

    cfg.trash_cycles = read_parameter(config_file, (char *)NULL);
    cfg.trash_vars = read_parameter(config_file, (char *)NULL);
    cfg.trash_funcs = read_parameter(config_file, (char *)NULL);

    read_parameter(config_file, cfg.trash_cycle);
    read_parameter(config_file, cfg.trash_var);
    read_parameter(config_file, cfg.trash_func);

    printf("config parameters:\ncfg.trash_cycles=%d\ncfg.trash_vars=%d\ncfg.trash_funcs=%d\ncfg.trash_cycle=%s\ncfg.trash_var=%s\ncfg.trash_func=%s\n\n",\
            cfg.trash_cycles, cfg.trash_vars, cfg.trash_funcs, cfg.trash_cycle, cfg.trash_var, cfg.trash_func);
}

void format_string(char *code, char *f_code, int len) {
    //delete [\t, \n, '  ']
    int i, j = 0;
    bool cm = false;

    for (i = 0; i < len; i++) {

        if (code[i] == '/' && code[i+1] == '*') {
            cm = true;
        } else if (code[i-2] == '*' && code[i-1] == '/') {
            cm = false;
        }

        if ((code[i] == '\t' || code[i] == '\n' || (code[i-1] == ' ' && code[i] == ' ')) || cm == true) {
            continue;
        }
        f_code[j] = code[i];
        j++;
    }
    f_code[j] = '\0';
}

//function that add some trash in code. 
//modes:
//f - add funcs, c - add cycles, v - add vars
int add_trash(char* code, int code_len, int amount, char mode) {
    char *obf_code = (char *)malloc(sizeof(char) * code_len);
    if (obf_code == NULL) {
        printf("error occured while allocating memory!\n");
    }
    clear(obf_code, code_len);

    int brackets_counter = 0, times_placed = 1;
    int *placed_i = (int *)calloc(amount, sizeof(int));

    int i, j = 0;
    for (i = 0; code[i] != '\0'; i++) {
        obf_code[j] = code[i];
        j++;

        if (code[i] == '{') {
            brackets_counter++;
        } else if (code[i] == '}') {
            brackets_counter--;
        }

        // printf("data: %d %d %d %d %d %d\n", i, placed_i[times_placed - 1] + code_len/8, code_len/8, brackets_counter, times_placed, amount);

        if (mode == 'f') {
            if (brackets_counter == 0 && \
                i >= (placed_i[times_placed - 1] + code_len / 6) && \
                times_placed <= amount) {

                placed_i[times_placed] = i;
                times_placed++;
                for (int k = 0; k < strlen(cfg.trash_func); k++) {
                    obf_code[j] = cfg.trash_func[k];
                    j++;
                }
            }
        } else if (mode == 'v' || mode == 'c') {
            if (brackets_counter == 1 && (obf_code[j-1] == '{' || obf_code[j-1] == ';') && \
                i >= (placed_i[times_placed - 1] + code_len / 12) && \
                times_placed <= amount) {

                placed_i[times_placed] = i;
                times_placed++;
                if (mode == 'v') {
                    for (int k = 0; k < strlen(cfg.trash_var); k++) {
                        obf_code[j] = cfg.trash_var[k];
                        j++;
                    }
                } else if (mode == 'c') {
                    for (int k = 0; k < strlen(cfg.trash_cycle); k++) {
                        obf_code[j] = cfg.trash_cycle[k];
                        j++;
                    }
                }
            }
        } else {
            printf("incorrect option in code\n");
            return 0;
        }
    }

    copy(code, obf_code, code_len);
    free(obf_code);

    return times_placed - 1;
}

int main(int argc, char *argv[]) {

    import_config();

    FILE *input_stream = fopen(argv[1], "r");

    if (argv[1] == NULL) {
        printf("error occured while reading launch arguments\n");
        return 0;
    }

    char *code = (char *)malloc(sizeof(char) * BUFF);
    if (code == NULL) {
        printf("error occured while allocating memory!\n");
    }
    clear(code, BUFF);

    int code_len = 0;
    char read_char;
    while ((read_char = fgetc(input_stream)) != EOF) {
        code[code_len++] = read_char;
    }
    printf("%d character were read\n", code_len);

    int actual_size = code_len + (strlen(cfg.trash_cycle) * cfg.trash_cycles + \
    strlen(cfg.trash_var) * cfg.trash_vars + \
    strlen(cfg.trash_func) * cfg.trash_funcs);

    char *f_code = (char *)malloc(sizeof(char) * actual_size);
    if (f_code == NULL) {
        printf("error occured while allocating memory!\n");
    }
    clear(f_code, actual_size);

    format_string(code, f_code, actual_size);
    printf("%d trash funcs added\n", add_trash(f_code, actual_size, cfg.trash_funcs, 'f'));
    printf("%d trash vars added\n", add_trash(f_code, actual_size, cfg.trash_vars, 'v'));
    printf("%d trash cycles added\n", add_trash(f_code, actual_size, cfg.trash_cycles, 'c'));

    FILE *output_stream = fopen("obfuscated_code.txt", "w");

    fwrite(f_code, sizeof(char), strlen(f_code), output_stream);
    printf("%d character were written\n", strlen(f_code));

    printf("\nresult: \n");
    puts(f_code);
    printf("\n");

    int numclosed = _fcloseall();
    printf("%d files closed by _fcloseall\n", numclosed);

    free(f_code);
    free(code);

    return 0;
}