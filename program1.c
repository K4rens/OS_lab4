#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "contract.h"

int main(void) {
    printf("Program1 (compile-time linked with impl_a). Enter commands:\n");     // инструкции пользователю
    printf("1 k               -> compute pi with k terms\n");
    printf("2 n a1 a2 ... an -> sort n integers using Sort\n");
    printf("q                 -> quit\n");

    char line[1024];
    while (1) {
        if (!fgets(line, sizeof(line), stdin)) break;
        char *p = line;
        while (*p && (*p == ' ' || *p == '\t')) ++p;
        if (*p == '\n' || *p == '\0') continue;
        if (*p == 'q') break;

        // парсим команду
        int cmd = 0;
        if (sscanf(p, "%d", &cmd) != 1) {
            printf("Invalid command\n");
            continue;
        }

        if (cmd == 1) {
            int k;
            if (sscanf(p, "%*d %d", &k) != 1) {
                printf("Usage: 1 k\n");
                continue;
            }
            float res = Pi(k);
            printf("Result: Pi(~%d terms) = %.10f\n", k, (double)res);
        } else if (cmd == 2) {
            int n;
            char *s = p;
            if (sscanf(s, "%*d %d", &n) != 1) {
                printf("Usage: 2 n a1 a2 ... an\n");
                continue;
            }
            // выделим массив
            int *arr = (int*)malloc(sizeof(int) * n);
            if (!arr) { printf("Memory error\n"); continue; }
            // прочитаем числа по очереди
            char *cur = s;
            // пропускаем первые два токена
            for (int skip = 0; skip < 2; ++skip) {
                while (*cur && *cur != ' ' && *cur != '\t') ++cur;
                while (*cur && (*cur == ' ' || *cur == '\t')) ++cur;
            }
            int read = 0;
            for (int i = 0; i < n; ++i) {
                int val;
                int consumed = 0;
                if (sscanf(cur, "%d%n", &val, &consumed) != 1) { read = i; break; }
                arr[i] = val;
                read++;
                cur += consumed;
            }
            if (read != n) {
                printf("Not enough integers provided. read=%d expected=%d\n", read, n);
                free(arr);
                continue;
            }
            int *out = Sort(arr, n);
            printf("Result: sorted array:");
            for (int i = 0; i < n; ++i) printf(" %d", out[i]);
            printf("\n");
            free(arr);
        } else {
            printf("Unknown command\n");
        }
    }

    printf("Exiting program1\n");
    return 0;
}
