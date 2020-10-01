#include <stdio.h>

unsigned char array[] = {
	78,
	91,
	37,
	107,
	111,
	192,
	228,
	199,
	128,
	201,
	67,
	210,
	182,
	10,
	6,
	246,
	24,
	46,
	234,
	91,
	4,
	22,
	177,
	139,
	142,
	29,
	148,
	9,
	3,
	128,
	231,
	131,
	90,
	107,
	168,
	241,
	251,
	59,
	196,
	108,
	59,
	91,
	36,
	135,
	251,
	184,
	179,
	221
};

int compare(void *a, void *b)
{
	return (int)(*(unsigned char*)a) - (int)(*(unsigned char*)b);
}

int maxarray(void*, unsigned long, unsigned long,
	int (*)(void *a, void *b));

int main(int argc, char **argv)
{
	printf("%d\n", maxarray(array, 48, sizeof(unsigned char), compare));
	return 0;
}

int maxarray(void  *base, unsigned long nel, unsigned long width, 
int (*compare)(void *a, void *b)) {
    void *max = base;
    int idx = 0;
    for (unsigned long i = 0; i < nel; i++) 
    {
        void* elem = (void*)((char*)base + width * i);
        if (compare(elem, max) > 0) {
            max = elem;
            idx = i;
        }
    }
    return idx;
}
