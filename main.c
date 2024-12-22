#include <stdio.h>
#include <wchar.h>
#include <locale.h>

#define STR_LEN_MAX 0xffff


char HangulInitial[19][2] = {
    {"g"},
    {"kk"},
    {"n"},
    {"d"},
    {"tt"},
    {"r"},
    {"m"},
    {"b"},
    {"pp"},
    {"s"},
    {"ss"},
    {""},
    {"j"},
    {"jj"},
    {"ch"},
    {"K"},
    {"T"},
    {"P"},
    {"h"}
};
char HangulMedial[21][3] = {
    {"a"},
    {"ae"},
    {"ya"},
    {"yae"},
    {"eo"},
    {"e"},
    {"yeo"},
    {"ye"},
    {"o"},
    {"wa"},
    {"wae"},
    {"oe"},
    {"yo"},
    {"u"},
    {"wo"},
    {"we"},
    {"wi"},
    {"yu"},
    {"eu"},
    {"ui"},
    {"i"}
};
char HangulFinal[28][2] = {
    {""},
    {"k"},
    {"kk"},
    {"ks"},
    {"n"},
    {"nj"},
    {"nh"},
    {"t"},
    {"l"},
    {"lk"},
    {"lm"},
    {"lp"},
    {"ls"},
    {"lT"},
    {"lP"},
    {"lh"},
    {"m"},
    {"p"},
    {"ps"},
    {"s"},
    {"ss"},
    {"ng"},
    {"j"},
    {"ch"},
    {"K"},
    {"T"},
    {"P"},
    {"h"}
};

int checkConsonant(char ch) {
    char* consonants = "bcdghjklmnprst";
    for (int i = 0; consonants[i] != 0; i++) {
        if (ch == consonants[i]) return 1;
    }
    return 0;
}

int checkVowel(char ch) {
    char* vowels = "aeiouwy";
    for (int i = 0; vowels[i] != 0; i++) {
        if (ch == vowels[i]) return 1;
    }
    return 0;
}

void pullArray(char* array, int index, int arrsize) {
    for (int i = index; i < arrsize - 1; i++) {
        if (array[i] == 0 && array[i + 1] == 0) return;
        else array[i] = array[i + 1];
    }
    array[arrsize] = 0;
}

int main(void) {
    setlocale(LC_ALL, "");
    wchar_t Text[STR_LEN_MAX] = {0, };
{
    FILE* file;
    char fname[256];

    printf("File name: ");
    scanf("%s", (char*)fname);
    file = fopen(fname, "r");
    if (!file) {
        printf("No file name \'%s\'.\n", fname);
        return 0;
    }
    
    for (int i = 0; !feof(file); i++) Text[i] = fgetwc(file);
    fclose(file);
}

    char result[STR_LEN_MAX] = {0, };
{
    int index = 0;
    int first, initial, medial, final;

    for (int i = 0; i < STR_LEN_MAX && Text[i] != 0; i++) {
        first = 0xac00;
        initial = 0;
        medial = 0;
        final = 0;

        if (Text[i] < 0xac00 || 0xd7a3 < Text[i]) {
            result[index++] = Text[i];
            continue;
        }

        while (!(first <= Text[i] && Text[i] <= first + 587)) {
            initial++;
            first += 588;
        }
        while (!(first <= Text[i] && Text[i] <= first + 27)) {
            medial++;
            first += 28;
        }
        while (!(first == Text[i])) {
            final++;
            first++;
        }

        for (int j = 0; j < 2; j++) {
            if (HangulInitial[initial][j] == 0) continue;
            else result[index++] = HangulInitial[initial][j];
        }
        for (int j = 0; j < 3; j++) {
            if (HangulMedial[medial][j] == 0) continue;
            else result[index++] = HangulMedial[medial][j];
        }
        for (int j = 0; j < 2; j++) {
            if (HangulFinal[final][j] == 0) continue;
            else result[index++] = HangulFinal[final][j];
        }
    }
}

    for (int i = 0; i < STR_LEN_MAX && result[i] != 0; i++) {
        if (i > 0) {
            if (checkVowel(result[i + 1]) && result[i - 1] != result[i]) {
                if (result[i] == 'k') result[i] = 'g';
                else if (result[i] == 't') result[i] = 'd';
                else if (result[i] == 'p') result[i] = 'b';
                else if (result[i] == 'l') result[i] = 'r';
            }
            if (checkVowel(result[i - 1]) && (checkConsonant(result[i + 1]) || !checkVowel(result[i + 1]))) {
                if (checkConsonant(result[i + 2]) || !checkVowel(result[i + 2])) {
                    if (result[i] == 'k' || result[i] == 'p' || result[i] == 's') {
                        if (result[i + 1] == 's') pullArray((char*)result, i + 1, STR_LEN_MAX);
                    }
                    else if (result[i] == 'l') {
                        if (result[i + 1] == 'k' || result[i + 1] == 'm' || result[i + 1] == 'P') pullArray((char*)result, i, STR_LEN_MAX);
                        else if (result[i + 1] == 'p' || result[i + 1] == 's' || result[i + 1] == 't') pullArray((char*)result, i + 1, STR_LEN_MAX);
                        else if (result[i + 1] == 'h') {
                            pullArray((char*)result, i + 1, STR_LEN_MAX);
                            if (result[i + 1] == 'g') result[i + 1] = 'K';
                            else if (result[i + 1] == 'd') result[i + 1] = 'T';
                            else if (result[i + 1] == 'b') result[i + 1] = 'P';
                        }
                    }
                    else if (result[i] == 'c') {
                        pullArray((char*)result, i + 1, STR_LEN_MAX);
                        result[i] == 't';
                    }
                }
                if (result[i] == 's' || result[i] == 'j') result[i] = 't';
            }
            if (result[i] == 'l' && result[i + 1] == 'r') result[i + 1] = 'l';
        }

        if (result[i] == 'K') result[i] = 'k';
        else if (result[i] == 'T') result[i] = 't';
        else if (result[i] == 'P') result[i] = 'p';
    }

    printf("%s\n", result);

    return 0;
}