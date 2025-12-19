#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>


typedef float (*PiFunc)(int);
typedef int*  (*SortFunc)(int*, int); 

static HMODULE currentLib = NULL;
static PiFunc currentPi = NULL;
static SortFunc currentSort = NULL;
static int usingA = 1; //по умолчанию используем impl_a

//загрузить библиотеку по имени, получить указатели на функции
int load_impl(const char* dllName) {
    if (currentLib) {
        FreeLibrary(currentLib);
        currentLib = NULL;
        currentPi = NULL;
        currentSort = NULL;
    }
    currentLib = LoadLibraryA(dllName);
    if (!currentLib) {
        printf("Failed to load %s\n", dllName);
        return 0;
    }
    currentPi = (PiFunc)GetProcAddress(currentLib, "Pi");
    currentSort = (SortFunc)GetProcAddress(currentLib, "Sort");
    if (!currentPi || !currentSort) {
        printf("Failed to get function addresses from %s\n", dllName);
        FreeLibrary(currentLib);
        currentLib = NULL;
        currentPi = NULL;
        currentSort = NULL;
        return 0;
    }
    return 1;
}

int main(void) {
    printf("Program2 (runtime loading). By default loaded: impl_a.dll\n");
    printf("Commands:\n");
    printf("0                 -> switch implementation (impl_a <-> impl_b)\n");
    printf("1 k               -> compute pi with k terms\n");
    printf("2 n a1 a2 ... an  -> sort n integers using Sort\n");
    printf("q                 -> quit\n");

    if (!load_impl("impl_a.dll")) {
        printf("Warning: could not load impl_a.dll at start\n");
    }

    char line[2048];
    while (1) {
        if (!fgets(line, sizeof(line), stdin)) break; //проходим пробелы
        char *p = line;
        while (*p && (*p == ' ' || *p == '\t')) ++p;
        if (*p == '\n' || *p == '\0') continue;
        if (*p == 'q') break;

        //если команда 0 — переключаем реализации
        if (p[0] == '0') {
            usingA = !usingA;
            const char* dll = usingA ? "impl_a.dll" : "impl_b.dll";
            if (load_impl(dll)) {
                printf("Switched to %s\n", dll);
            } else {
                printf("Failed to switch to %s\n", dll);
            }
            continue;
        }

        int cmd = 0;
        if (sscanf(p, "%d", &cmd) != 1) {
            printf("Invalid command\n");
            continue;
        }
        //вычис пи
        if (cmd == 1) {
            int k;
            if (sscanf(p, "%*d %d", &k) != 1) {
                printf("Usage: 1 k\n");
                continue;
            }
            if (!currentPi) { printf("No implementation loaded\n"); continue; }
            float res = currentPi(k);
            printf("Result: Pi(~%d terms) = %.10f\n", k, (double)res);
        } else if (cmd == 2) { //сорт
            int n;
            if (sscanf(p, "%*d %d", &n) != 1) {
                printf("Usage: 2 n a1 a2 ... an\n");
                continue;
            }
            int *arr = (int*)malloc(sizeof(int) * n);
            if (!arr) { printf("Memory error\n"); continue; }
            char *cur = p;
            for (int skip = 0; skip < 2; ++skip) {
                while (*cur && *cur != ' ' && *cur != '\t') ++cur;
                while (*cur && (*cur == ' ' || *cur == '\t')) ++cur;
            }
            int read = 0;  //подсчет
            for (int i = 0; i < n; ++i) {
                int val; int consumed = 0;
                if (sscanf(cur, "%d%n", &val, &consumed) != 1) { read = i; break; }
                arr[i] = val;
                read++; cur += consumed;
            }
            if (read != n) {
                printf("Not enough integers provided. read=%d expected=%d\n", read, n);
                free(arr);
                continue;
            }
            if (!currentSort) { printf("No implementation loaded\n"); free(arr); continue; }
            int *out = currentSort(arr, n);
            printf("Result: sorted array:");
            for (int i = 0; i < n; ++i) printf(" %d", out[i]);
            printf("\n");
            free(arr);
        } else {
            printf("Unknown command\n");
        }
    }

    if (currentLib) FreeLibrary(currentLib);
    printf("Exiting program2\n");
    return 0;
}
