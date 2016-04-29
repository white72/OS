#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

char returnRandCharAlphaNum(char);

int main() {
    srand(time(NULL));
    char tmp = '\0';

    for (; ;) printf("%c", returnRandCharAlphaNum(tmp));

    return 0;
}
#pragma clang diagnostic pop

char returnRandCharAlphaNum(char tmp){
    tmp = (char) ((rand() % 62) + 48);

    if (tmp > 57) tmp += 7;
    if (tmp > 90) tmp += 6;

    return tmp;
}