#include <stdio.h>
#include <string.h>

#define MAX 100

void strip_newline(char *s) {
    s[strcspn(s, "\n")] = 0;
}


unsigned int fake_hash(const char *str) {
    unsigned int h = 0;
    for (int i = 0; str[i]; i++) {
        h = h * 31 + str[i];
    }
    return h;
}

int dictionary_attack(const char *target_user, unsigned int target_hash, FILE *dict) {
    char word[MAX];

    rewind(dict);

    while (fgets(word, sizeof(word), dict)) {
        strip_newline(word);

        if (fake_hash(word) == target_hash) {
            printf("%s: %s (dictionary)\n", target_user, word);
            return 1;
        }
    }
    return 0;
}

void brute_force(const char *target_user, unsigned int target_hash) {
    char pin[5];

    for (int i = 0; i <= 9999; i++) {
        sprintf(pin, "%04d", i);

        if (fake_hash(pin) == target_hash) {
            printf("%s: %s (brute force)\n", target_user, pin);
            return;
        }
    }
}

int main(int argc, char *argv[]) {

    FILE *shadow = fopen("shadow.txt", "r");
    FILE *dict = fopen("dictionary.txt", "r");

    if (!shadow || !dict) {
        printf("Error opening files\n");
        return 1;
    }

    char line[200];

    while (fgets(line, sizeof(line), shadow)) {

        strip_newline(line);

        char user[100];
        char hash_str[100];

        sscanf(line, "%[^:]:%s", user, hash_str);

        unsigned int hash = (unsigned int)atoi(hash_str);

        if (!dictionary_attack(user, hash, dict)) {
            brute_force(user, hash);
        }
    }

    fclose(shadow);
    fclose(dict);

    return 0;
}
