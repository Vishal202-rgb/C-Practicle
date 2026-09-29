#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

int main() {
    float r, area;
    printf("Enter radius: ");
    scanf("%f", &r);

    area = M_PI * r * r;
    printf("Area = %.2f\n", area);

    return 0;
}