#include <stdio.h>
#include <stdlib.h>

void step(long n, short* buff0, short* buff1, short* buff2, long* i) {
    *buff0 = 0;
    if(*i < n)
        scanf("%hd", buff0);
    
    if (*i == 1) { // 1st cahracter
        if (*buff1){
            *buff1 = 0;
            *buff0 = 1;
        } else {
            *buff1 = 1;
        }
    }
    if (*buff2 && *buff1) {
        *buff1 = 0;
        *buff2 = 0;
        *buff0 = 1;
    }
    // Shift
    if (*i > 1)
        printf("%hd ", *buff2);
    *buff2 = *buff1;
    *buff1 = *buff0;

    (*i)++;
}

int main() {
    char buffer[32768];
    setvbuf(stdout, buffer, _IOFBF, sizeof(buffer));

	long n;
    scanf("%ld", &n);

    short buff0 = 0,
        buff1 = 0,
        buff2 = 0;

    // Input number
    for (long i = 0; i < n + 2;) {
        switch((n + 1 - i) % 16) {
            case 15: step(n, &buff0, &buff1, &buff2, &i);
            case 14: step(n, &buff0, &buff1, &buff2, &i);
            case 13: step(n, &buff0, &buff1, &buff2, &i);
            case 12: step(n, &buff0, &buff1, &buff2, &i);
            case 11: step(n, &buff0, &buff1, &buff2, &i);
            case 10: step(n, &buff0, &buff1, &buff2, &i);
            case 9: step(n, &buff0, &buff1, &buff2, &i);
            case 8: step(n, &buff0, &buff1, &buff2, &i);
            case 7: step(n, &buff0, &buff1, &buff2, &i);
            case 6: step(n, &buff0, &buff1, &buff2, &i);
            case 5: step(n, &buff0, &buff1, &buff2, &i);
            case 4: step(n, &buff0, &buff1, &buff2, &i);
            case 3: step(n, &buff0, &buff1, &buff2, &i);
            case 2: step(n, &buff0, &buff1, &buff2, &i);
            case 1: step(n, &buff0, &buff1, &buff2, &i);
            case 0: step(n, &buff0, &buff1, &buff2, &i);
        }
    }

    if (buff2)
        printf("%hd", buff2);
    fflush( stdout );

	return 0;
}