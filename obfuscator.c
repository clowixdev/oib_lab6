#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define BUFF 100000
#define MAX_PLENGHT 100
#define MAX_NAME_LEN 10000
#define MAX_NAMES_AMT 100
#define REDEF_NAME_LEN 10

struct config
{
    int delete_whitespaces;
    int delete_comments;
    int change_names;
    int add_trash;

    char trash_cycle[MAX_PLENGHT];
    char trash_var[MAX_PLENGHT];
    char trash_func[MAX_PLENGHT];
} cfg;

void clear(char* arr, int len) {
    for (int i = 0; i < len; i++) {
        arr[i] = '\0';
    }
}

/*copies first n symbols of arr2 -> arr1*/
void copy(char* arr1, char* arr2, int n) {
    for (int i = 0; i < n; i++) {
        arr1[i] = arr2[i];
    }
}

void reverse(char* str, int str_len) {
    char* new_str;
    new_str = (char*)malloc(sizeof(char) * str_len);
    for (int i = 0; i < str_len; i++) {
        new_str[i] = str[str_len - 1 - i];
    }

    copy(str, new_str, str_len);
    free(new_str);
}

void display(char* arr) {
    for (int i = 0; arr[i] != 0; i++) {
        printf("%c", arr[i]);
    }
    printf("\n");
}

int get_lenght(char* arr) {
    int len = 0;
    while (arr[len] != '\0') {
        len++;
    }

    return len;
}

int no_letters(char* name, int name_len) {
    char alphabet[27] = "abcdefghijklmnopqrstuvwxyz\0";

    for (int i = 0; i < name_len; i++) {
        for (int j = 0; j < 27; j++) {
            if (name[i] == alphabet[j]) {
                return false;
            }
        }
    }
    return true;
}

char* gen_name(int name_len) {
    char* name = (char*)malloc(sizeof(char) * name_len + 1);
    if (name == NULL) {
        printf("error occured while allocating memory!\n");
    }
    clear(name, name_len + 1);

    char alphabet[27] = "abcdefghijklmnopqrstuvwxyz\0";

    int i;
    for (i = 0; i < name_len; i++) {
        name[i] = alphabet[rand() % 26];
    }
    name[i] = '\0';
    return name;
}

int format_name(char* name, int name_len) {
    char* f_name = (char*)malloc(sizeof(char) * name_len + 1);
    if (f_name == NULL) {
        printf("error occured while allocating memory!\n");
    }
    clear(f_name, name_len + 1);

    int j = 0;
    for (int i = 0; i < name_len; i++) {
        if (name[i] == '*') {
            continue;
        }
        else if (name[i] == '[') {
            break;
        }
        f_name[j] = name[i];
        j++;
    }
    f_name[j] = '\0';

    copy(name, f_name, name_len);
    free(f_name);

    return j;
}

int if_on_name(char* code, int pos, char* name) {
    char alphabet[27] = "abcdefghijklmnopqrstuvwxyz\0";
    int len = 0;
    while (name[len] != '\0') {
        len++;
    }

    for (int i = 0; i < 27; i++) {
        if ((code[pos - 1] == alphabet[i] || code[pos + len] == alphabet[i]) || \
            (code[pos + len] == '_' || code[pos - 1] == '_')) {
            return false;
        }
    }

    for (int i = 0; i < len; i++) {
        if (code[pos++] != name[i]) {
            return false;
        }
    }
    return true;
}

void add_new_name(char* code, int pos, char* new_name) {
    int i = 0;
    while (i < REDEF_NAME_LEN) {
        code[pos++] = new_name[i];
        i++;
    }
}

int is_name_founded(char** matrix, char* name, int names_amt) {
    for (int i = 0; i < names_amt; i++) {
        if (strcmp(matrix[i], name) == 0) {
            return true;
        }
    }
    return false;
}

int read_parameter(FILE* stream, char* destination) {

    char* param_string = (char*)malloc(sizeof(char) * MAX_PLENGHT);
    if (param_string == NULL) {
        printf("error occured while allocating memory!\n");
    }
    clear(param_string, MAX_PLENGHT);
    int i = 0;
    char c;
    while ((c = fgetc(stream)) != '=')
    {
    }
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
    }
    else {
        int i = 0;
        while ((c = fgetc(stream)) != '"') {
            destination[i] = c;
            i++;
        }
        destination[i + 1] = '\0';
    }

    free(param_string);
}

void import_config() {
    FILE* config_file = fopen("obf_config.cfg", "r");

    cfg.delete_whitespaces = read_parameter(config_file, (char*)NULL);
    cfg.delete_comments = read_parameter(config_file, (char*)NULL);
    cfg.change_names = read_parameter(config_file, (char*)NULL);
    cfg.add_trash = read_parameter(config_file, (char*)NULL);

    read_parameter(config_file, cfg.trash_cycle);
    read_parameter(config_file, cfg.trash_var);
    read_parameter(config_file, cfg.trash_func);

    printf("config parameters:\ncfg.delete_whitespaces=%d\ncfg.delete_comments=%d\ncfg.change_names=%d\ncfg.add_trash=%d\ncfg.trash_cycle=%s\ncfg.trash_var=%s\ncfg.trash_func=%s\n\n", \
        cfg.delete_whitespaces, cfg.delete_comments, cfg.change_names, cfg.add_trash, cfg.trash_cycle, cfg.trash_var, cfg.trash_func);
}

void delete_comments(char* code, int code_len) {
    char* obf_code = (char*)malloc(sizeof(char) * code_len + 1);
    if (obf_code == NULL) {
        printf("error occured while allocating memory!\n");
    }
    clear(obf_code, code_len + 1);

    int i, j = 0;
    bool cm = false;
    for (i = 0; i < code_len; i++) {
        if (code[i] == '/' && code[i + 1] == '*') {
            cm = true;
        }
        else if (code[i - 2] == '*' && code[i - 1] == '/') {
            cm = false;
        }

        if (cm == true) {
            continue;
        }

        obf_code[j] = code[i];
        j++;
    }
    obf_code[j] = '\0';

    copy(code, obf_code, code_len);
    free(obf_code);
}

void change_vars(char* code, int code_len) {
    char* obf_code = (char*)malloc(sizeof(char) * code_len);
    if (obf_code == NULL) {
        printf("error occured while allocating memory!\n");
    }
    clear(obf_code, code_len);
    
    int i = 0;

    char** names_matrix = (char**)malloc(MAX_NAMES_AMT * MAX_NAME_LEN * sizeof(char*));
    for (i = 0; i < MAX_NAMES_AMT; i++) {
        char* found_name = (char*)malloc(sizeof(char) * MAX_NAME_LEN);
        clear(found_name, MAX_NAME_LEN);
        names_matrix[i] = found_name;
    }

    i = 0;
    int j = 0, k = 0, l = 0;
    int name_char_i = 0;
    int i_name = 0;
    char name[MAX_NAME_LEN] = { 0 };
    for (i = 0; code[i] != '\0'; i++) {
        if (code[i] == ' ' && code[i + 1] == '=' && code[i + 2] == ' ') {
            for (k = i - 1; (code[k] != ' ' && code[k] != '(' && code[k] != '\t' && code[k] != '\n' && \
                code[k] != ';' && code[k] != '{' && code[k] != '}' && \
                code[k] != '.' && code[k] != ','); k--) {
                    
                name[name_char_i] = code[k];
                name_char_i++;
            }
            if (no_letters(name, name_char_i) == true) {
                clear(name, name_char_i);
                name_char_i = 0;
                continue;
            }
            reverse(name, name_char_i);
            name_char_i = format_name(name, name_char_i);
            if (is_name_founded(names_matrix, name, i_name) == false) {
                copy(names_matrix[i_name++], name, name_char_i + 1);
            }
            clear(name, name_char_i);
            name_char_i = 0;
        }
    }

    printf("\n");
    for (i = 0; i < i_name; i++) {
        k = 0;
        char* new_name = gen_name(REDEF_NAME_LEN);
        printf("generated %s with len = %d instead of %s\n", new_name, REDEF_NAME_LEN, names_matrix[i]);
        for (j = 0; code[j] != '\0'; j++) {
            if (if_on_name(code, j, names_matrix[i]) == true) {
                add_new_name(obf_code, k, new_name);
                k += REDEF_NAME_LEN;
                j += get_lenght(names_matrix[i]) - 1;
                continue;
            }
            obf_code[k] = code[j];
            k++;
        }
        free(new_name);
        copy(code, obf_code, code_len);
        if (i + 1 < i_name) {
            clear(obf_code, code_len);
        }
    }

    copy(code, obf_code, code_len);

    printf("\n");
    for (i = 0; i < i_name; i++) {
        free(names_matrix[i]);
    }
    free(names_matrix);
    free(obf_code);
}

void delete_whitespaces(char* code, int code_len) {
    char* obf_code = (char*)malloc(sizeof(char) * code_len + 1);
    if (obf_code == NULL) {
        printf("error occured while allocating memory!\n");
    }
    clear(obf_code, code_len + 1);

    /*delete double [\t, \n, '  ']*/
    int i, j = 0;
    bool directive = false;

    for (i = 0; i < code_len; i++) {

        if (code[i] == '#') {
            directive = true;
        }
        else if (code[i - 1] == '\n') {
            directive = false;
        }

        if (code[i] == '\\' && code[i + 1] == '\n') {
            obf_code[j] = ' ';
            j++;
            continue;
        }

        if (((code[i - 1] == ' ' && code[i] == ' ') || \
            (code[i + 1] == ' ' && code[i] == ' ')) && directive == false) {
            continue;
        }

        if ((code[i] == '\n' || code[i] == '\t') && directive == false) {
            continue;
        }

        if ((code[i] == '\n' && code[i + 1] == ' ') || \
            (code[i] == '\n' && code[i - 1] == ' ') && directive == false) {
            continue;
        }

        if ((((code[i - 1] == '{' || code[i - 1] == '}') || \
            (code[i + 1] == '{' || code[i + 1] == '}')) && (code[i] == '\n')) && directive == false) {
            continue;
        }

        obf_code[j] = code[i];
        j++;
    }
    obf_code[j] = '\0';

    copy(code, obf_code, code_len);
    free(obf_code);
}

/*function that add some trash in code. 
modes:
f - add funcs, c - add cycles, v - add vars*/
int add_trash(char* code, int code_len, int amount, char mode) {
    char* obf_code = (char*)malloc(sizeof(char) * code_len);
    if (obf_code == NULL) {
        printf("error occured while allocating memory!\n");
    }
    clear(obf_code, code_len);

    int brackets_counter = 0, times_placed = 1;
    int* placed_i = (int*)calloc(amount + 1, sizeof(int));

    int i, j = 0;
    for (i = 0; code[i] != '\0'; i++) {
        obf_code[j] = code[i];
        j++;

        if (code[i] == '{') {
            brackets_counter++;
        }
        else if (code[i] == '}') {
            brackets_counter--;
        }

        if (mode == 'f') {
            if (brackets_counter == 0 && \
                i >= (placed_i[times_placed - 1] + code_len / 7) && \
                times_placed <= amount) {

                placed_i[times_placed] = i;
                times_placed++;
                for (int k = 0; k < strlen(cfg.trash_func); k++) {
                    obf_code[j] = cfg.trash_func[k];
                    j++;
                }
            }
        }
        else if (mode == 'v' || mode == 'c') {
            if (brackets_counter == 1 && (obf_code[j - 1] == '{' || obf_code[j - 1] == ';') && \
                i >= (placed_i[times_placed - 1] + code_len / 13) && \
                times_placed <= amount) {

                placed_i[times_placed] = i;
                times_placed++;
                if (mode == 'v') {
                    for (int k = 0; k < strlen(cfg.trash_var); k++) {
                        obf_code[j] = cfg.trash_var[k];
                        j++;
                    }
                }
                else if (mode == 'c') {
                    for (int k = 0; k < strlen(cfg.trash_cycle); k++) {
                        obf_code[j] = cfg.trash_cycle[k];
                        j++;
                    }
                }
            }
        }
        else {
            printf("incorrect option in code\n");
            return 0;
        }
    }

    copy(code, obf_code, code_len);
    free(obf_code);

    return times_placed - 1;
}

int main(int argc, char* argv[]) {

    import_config();

    FILE* input_stream = fopen(argv[1], "r");

    if (argv[1] == NULL) {
        printf("error occured while reading launch arguments\n");
        return 0;
    }

    char* code = (char*)malloc(sizeof(char) * BUFF);
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

    /* int actual_size = code_len + (strlen(cfg.trash_cycle) + \
         strlen(cfg.trash_var) + strlen(cfg.trash_func)) + ((REDEF_NAME_LEN + 1) * MAX_NAMES_AMT); */

    int actual_size = BUFF;

    char* f_code = (char*)malloc(sizeof(char) * actual_size + 1);
    if (f_code == NULL) {
        printf("error occured while allocating memory!\n");
    }
    clear(f_code, actual_size + 1);

    copy(f_code, code, actual_size + 1);
    int operations_done = 0;
    if (cfg.delete_comments == 1) {
        delete_comments(f_code, actual_size);
        printf("deleted all comments\n");
        operations_done++;
    }
    if (cfg.add_trash == 1) {
        add_trash(f_code, actual_size, 1, 'f');
        add_trash(f_code, actual_size, 1, 'v');
        add_trash(f_code, actual_size, 1, 'c');
        printf("added all trash\n");
        operations_done++;
    }
    if (cfg.delete_whitespaces == 1) {
        delete_whitespaces(f_code, actual_size);
        printf("deleted all whitespaces\n");
        operations_done++;
    }
    if (cfg.change_names == 1) {
        change_vars(f_code, actual_size);
        printf("changed all names\n");
        operations_done++;
    }
    f_code[actual_size - 1] = '\0';

    FILE* output_stream = fopen("obfuscated_code.txt", "w");

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