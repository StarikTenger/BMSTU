#include  <stdio.h>

int array[] = { 1, 2, 30, 45, 50, 51, 55, 60 };
const int k = 2;

int compare(unsigned long i)
{
    if (array[i] == k) return 0;
    if (array[i]  < k) return -1;
    return 1;
}

unsigned long binsearch(unsigned long, int (*)(unsigned long));

int main(int argc, char  **argv)
{
    printf("%lu\n", binsearch(8, compare));
    return 0;
}

unsigned long binsearch(unsigned long nel, int (*compare)(unsigned long i)) {
    unsigned long left = 0,
        right = nel - 1;

    while (left + 1 < right) {
        unsigned long mid = (left + right) / 2;
        //printf("%lu %lu\n", left, right);
        switch (compare(mid)) {
            case 1:
                right = mid;
                break;
            case -1:
                left = mid;
                break;
            default:
                return mid;
        }            
    }
    return nel;
}