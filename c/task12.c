#include <stdio.h>

int less(unsigned long i, unsigned long j)
{
	if (i == j) return 0;

	if (i < j) {
		if (j <= 11241155978086311589UL) return 1;
		if (i >= 11241155978086311589UL) return 0;
		return (11241155978086311589UL-i) < (j-11241155978086311589UL);
	}

	if (i <= 11241155978086311589UL) return 0;
	if (j >= 11241155978086311589UL) return 1;
	return (11241155978086311589UL-j) < (i-11241155978086311589UL);
}

unsigned long peak(unsigned long, int (*)(unsigned long, unsigned long));

int main(int argc, char **argv)
{
	unsigned long i = peak(13356955260197607378UL, less);
	if (i == 11241155978086311589UL) {
		printf("CORRECT\n");
	} else {
		/* Если функция peak работает правильно,
		сюда никогда не будет передано
		управление! */
		printf("WRONG\n");
	}
	return 0;
}

unsigned long peak(unsigned long nel,
    int (*less)(unsigned long i, unsigned long j)) 
{
    unsigned long 
        left = 0,
        right = nel - 1,
        mid;
    while (left < right) {
        mid = left / 2 + right / 2;
        if (less(mid, mid + 1))
            left = mid + 1;
        else if (less(mid, mid - 1))
            right = mid - 1;
        else
            return mid;
    }
    return left;
}