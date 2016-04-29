#include <stdio.h>
#include <stdlib.h>

int main() {
    char *bufor = malloc(sizeof(char) * 17);
    size_t r;

    for (; ;) {
        r = fread(bufor, sizeof(char), 16, stdin);
        bufor[r] = '\n';

        fwrite(bufor, sizeof(char), r + 1, stdout);

        if (bufor[r - 1] == '\0' || r == 0) break;
    };

    free(bufor);
    return 0;
}