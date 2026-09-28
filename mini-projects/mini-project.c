/*
 * Description: Compound Interest Calculator
 * This program calculates the final balance using compound interest compounded annually.
 */
#include <stdio.h>

int main() {
    float principal, rate, balance, interest;
    int years, i;

    printf("Enter principal amount: ");
    scanf("%f", &principal);

    printf("Enter rate of interest (in %%): ");
    scanf("%f", &rate);

    printf("Enter number of years: ");
    scanf("%d", &years);

    // Calculate compound interest compounded annually
    balance = principal;

    for (i = 1; i <= years; i++) {
        interest = balance * rate / 100; 
        balance = balance + interest;     
    }

    printf("Final balance after %d years = %.2f\n", years, balance);

    return 0;
}