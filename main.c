#include <stdio.h>
#include <wchar.h>
#include <locale.h>

#define STR_LEN_MAX 0xffff


// 초성
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
// 중성
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
// 종성
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

// 자음인지 확인
int checkConsonant(char ch) {
    char* consonants = "bcdghjklmnprst";
    for (int i = 0; consonants[i] != 0; i++) {
        if (ch == consonants[i]) return 1;
    }
    return 0;
}

// 모음인지 확인
int checkVowel(char ch) {
    char* vowels = "aeiouwy";
    for (int i = 0; vowels[i] != 0; i++) {
        if (ch == vowels[i]) return 1;
    }
    return 0;
}

// 배열을 앞으로 한 칸 앞당기는 함수
void pullArray(char* array, int index, int arrsize) {
    for (int i = index; i < arrsize - 1; i++) {
        if (array[i] == 0 && array[i + 1] == 0) return;
        else array[i] = array[i + 1];
    }
    array[arrsize] = 0;
}

void pushArray(char* array, int index, int arrsize) {
    for (int i = arrsize; i > index; i--) {
        if (array[i] == 0 && array[i - 1] == 0) continue;
        else array[i] = array[i - 1];
    }
    array[index] = 0;
}

int main(void) {
    setlocale(LC_ALL, "");
    wchar_t Text[STR_LEN_MAX] = {0, };
{
    FILE* file;
    char fname[256];

    // 입력받은 이름으로 파일 열기
    printf("File name: ");
    scanf("%s", (char*)fname);
    file = fopen(fname, "r");

    // 파일 없는 경우 예외 처리
    if (!file) {
        printf("No file name \'%s\'.\n", fname);
        return 0;
    }
    
    for (int i = 0; !feof(file); i++) Text[i] = fgetwc(file);
    fclose(file);
}

    char result[STR_LEN_MAX * 2] = {0, };
{
    int index = 0;
    int first, initial, medial, final;

    for (int i = 0; i < STR_LEN_MAX && Text[i] != 0; i++) {
        first = 0xac00; // 첫 글자 '가'
        initial = 0; // 초성
        medial = 0; // 중성
        final = 0; // 종성

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

        // 로마자로 변환하여 result에 추가
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
            // 유기음화 처리
            if (checkVowel(result[i + 1]) && result[i - 1] != result[i]) {
                if (result[i] == 'k') result[i] = 'g';
                else if (result[i] == 't') result[i] = 'd';
                else if (result[i] == 'p') result[i] = 'b';
                else if (result[i] == 'l') result[i] = 'r';
            }

            // 겹받침 처리
            if (checkVowel(result[i - 1]) && (checkConsonant(result[i + 1]) || !checkVowel(result[i + 1]))) {
                if (checkConsonant(result[i + 2]) || !checkVowel(result[i + 2])) {
                    // 'ㄳ', 'ㅄ', 'ㅆ' 처리
                    if ((result[i] == 'k' || result[i] == 'p' || result[i] == 's') && result[i + 1] == 's') {
                        pullArray((char*)result, i + 1, STR_LEN_MAX);
                    }
                    
                    // 'ㄶ' 처리
                    else if (result[i] == 'n') {
                        if (result[i + 1] == 'h') {
                            pullArray((char*)result, i + 1, STR_LEN_MAX);
                            // 격음화 처리
                            if (result[i + 1] == 'g') result[i + 1] = 'K';
                            else if (result[i + 1] == 'd') result[i + 1] = 'T';
                            else if (result[i + 1] == 'b') result[i + 1] = 'P';
                        }
                    }

                    // 'ㄹ' 계열 겹받침 처리
                    else if (result[i] == 'l') {
                        if (result[i + 1] == 'k' || result[i + 1] == 'm' || result[i + 1] == 'P')
                            pullArray((char*)result, i, STR_LEN_MAX);
                        else if (result[i + 1] == 'p' || result[i + 1] == 's' || result[i + 1] == 't')
                            pullArray((char*)result, i + 1, STR_LEN_MAX);
                        else if (result[i + 1] == 'h') {
                            pullArray((char*)result, i + 1, STR_LEN_MAX);
                            // 격음화 처리
                            if (result[i + 1] == 'g') result[i + 1] = 'K';
                            else if (result[i + 1] == 'd') result[i + 1] = 'T';
                            else if (result[i + 1] == 'b') result[i + 1] = 'P';
                        }
                    }
                    
                    // 'ㅊ' 처리
                    else if (result[i] == 'c') {
                        pullArray((char*)result, i + 1, STR_LEN_MAX);
                        result[i] = 't';
                    }
                }
                
                // 'ㅅ', 'ㅈ', 처리
                if (result[i] == 's' || result[i] == 'j') result[i] = 't';

                // 'ㅎ' 종성의 격음화 처리
                if (result[i] == 'h') {
                    pullArray((char*)result, i, STR_LEN_MAX);

                    if (result[i] == 'g') result[i] = 'K';
                    else if (result[i] == 'd') result[i] = 'T';
                    else if (result[i] == 'b') result[i] = 'P';
                }

                // 'ㄱ' 비음화 처리
                if (result[i] == 'k' && result[i + 1] == 'n') {
                    pushArray((char*)result, i, STR_LEN_MAX);
                    result[i] = 'n';
                    result[i + 1] = 'g';
                }

                // 'ㄴ' 유음화 처리
                if (result[i] == 'n' && result[i + 1] == 'r') result[i] = 'l';

                // 'ㄷ' 비음화 처리
                if (result[i] == 't' && result[i + 1] == 'n') result[i] = 'n';

                // 'ㅂ' 비음화 처리
                if (result[i] == 'p' && (result[i + 1] == 'n' || result[i + 1] == 'r')) result[i] = 'm';
                
                // 'ㄹ' 비음화 처리
                if (result[i] == 'm' && result[i + 1] == 'r') result[i + 1] = 'n';
            }

            // 'ㄹ'이 두 번 연속되는 경우 처리
            if (result[i] == 'l' && result[i + 1] == 'r') result[i + 1] = 'l';
        }

        // 구분을 위해 대문자로 표기했던 거센소리를 소문자로 변환
        if (result[i] == 'K') result[i] = 'k';
        else if (result[i] == 'T') result[i] = 't';
        else if (result[i] == 'P') result[i] = 'p';
    }

    printf("%s\n", result);

    return 0;
}