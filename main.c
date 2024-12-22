#include <stdio.h>
#include <wchar.h>
#include <locale.h>

#define START {
#define END }
#define STR_LEN_MAX 0xffff


char HangulInitial[19][2] = {
    "g\0",
    "kk",
    "n\0",
    "d\0",
    "tt",
    "r\0",
    "m\0",
    "b\0",
    "pp",
    "s\0",
    "ss",
    "\0\0",
    "j\0",
    "jj",
    "ch",
    "k\0",
    "t\0",
    "p\0",
    "h\0"
};
char HangulMedial[21][3] = {
    "a\0\0",
    "ae\0",
    "ya\0",
    "yae",
    "eo\0",
    "e\0\0",
    "yeo",
    "ye\0",
    "o\0\0",
    "wa\0",
    "wae",
    "oe\0",
    "yo\0",
    "u\0\0",
    "wo\0",
    "we\0",
    "wi\0",
    "yu\0",
    "eu\0",
    "ui\0",
    "i\0\0"
};
char HangulFinal[28][2] = {
    "\0\0",
    "g\0",
    "kk",
    "ks",
    "n\0",
    "nj",
    "nh",
    "t\0",
    "l\0",
    "lg",
    "lm",
    "lb",
    "ls",
    "lt",
    "lp",
    "lh",
    "m\0",
    "b\0",
    "ps",
    "s\0",
    "ss",
    "ng",
    "j\0",
    "ch",
    "k\0",
    "t\0",
    "p\0",
    "h\0"
};

int main(void) {
    setlocale(LC_ALL, "");

    wchar_t Text[STR_LEN_MAX] = {0, };
START
    FILE* file;
    char fname[256];

    wprintf(L"File name: ");
    scanf("%s", (char*)fname);
    file = fopen(fname, "r");
    if (!file) {
        wprintf(L"No file name \'%s\'.", fname);
        return 0;
    }
    
    for (int i = 0; !feof(file); i++) Text[i] = fgetwc(file);
    fclose(file);
END

wprintf(L"%S", Text);

START
    char result[STR_LEN_MAX] = {0, };

    for (int i = 0; i < 1; i++) {
        int initial, medial, final = 0;

        while (!(0xac00 + initial * 588 <= (int)Text[i] && (int)Text[i] <= 0xac00 + (initial + 1) * 588)) initial++;

        wprintf(L"\n\n%s", HangulInitial[initial]);

        
    }
END


    return 0;
}