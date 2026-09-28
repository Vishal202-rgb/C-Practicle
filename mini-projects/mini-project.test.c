const calculateCompoundInterest = (principal, rate, years) => {
    let balance = principal;

    // The original C code's loop runs from i=1 to years.
    // If years is 0 or negative, the loop does not execute,
    // and the balance remains the principal.
    if (years <= 0) {
        return parseFloat(principal.toFixed(2));
    }

    for (let i = 1; i <= years; i++) {
        const interest = balance * (rate / 100);
        balance = balance + interest;
    }

    // The C program prints with "%.2f", so we round to two decimal places.
    return parseFloat(balance.toFixed(2));
};

describe('Compound Interest Calculator (mini-project.c simulation)', () => {

    // REGRESSION TEST: Verifies the current compound interest logic is correct,
    // addressing the potential misinterpretation of simple vs. compound interest.
    // This test confirms the program correctly implements compound interest.
    it('REGRESSION TEST: should correctly calculate compound interest for a typical scenario', () => {
        const principal = 1000;
        const rate = 10; // 10%
        const years = 2;

        // Expected compound interest calculation:
        // Year 1: Interest = 1000 * 0.10 = 100. Balance = 1100.
        // Year 2: Interest = 1100 * 0.10 = 110. Balance = 1210.
        // Simple interest for comparison would be 1200.
        expect(calculateCompoundInterest(principal, rate, years)).toBe(1210.00);
    });

    // HAPPY PATH TEST: Verifies standard, expected inputs and typical operational flow.
    it('HAPPY PATH TEST: should calculate compound interest correctly for positive principal, rate, and years', () => {
        const principal = 5000;
        const rate = 5; // 5%
        const years = 3;

        // Expected calculation:
        // Year 1: 5000 * 1.05 = 5250.00
        // Year 2: 5250 * 1.05 = 5512.50
        // Year 3: 5512.50 * 1.05 = 5788.125 -> 5788.13 (rounded)
        expect(calculateCompoundInterest(principal, rate, years)).toBe(5788.13);
    });

    // EDGE CASE TEST: Tests boundary conditions, zero values, and extreme rates.
    it('EDGE CASE TEST: should handle boundary conditions like zero years, zero rate, and zero principal', () => {
        // Zero years: Balance should be equal to principal
        expect(calculateCompoundInterest(1500, 10, 0)).toBe(1500.00);

        // Zero rate: Balance should be equal to principal
        expect(calculateCompoundInterest(2000, 0, 5)).toBe(2000.00);

        // Zero principal: Balance should always be zero
        expect(calculateCompoundInterest(0, 10, 3)).toBe(0.00);

        // High rate, few years
        expect(calculateCompoundInterest(100, 100, 1)).toBe(200.00); // 100 + (100 * 100/100) = 200
        expect(calculateCompoundInterest(100, 100, 2)).toBe(400.00); // Year 1: 200. Year 2: 200 + (200 * 100/100) = 400
    });

    // ERROR HANDLING TEST: Validates how the C program's logic (simulated) handles invalid inputs
    // such as negative values, as the original C code does not perform explicit input validation.
    it('ERROR HANDLING TEST: should handle negative principal, rate, or years as per C program logic', () => {
        // Negative years: Loop does not run, balance remains principal
        expect(calculateCompoundInterest(1000, 10, -5)).toBe(1000.00);

        // Negative rate: Balance should decrease over time
        // Year 1: 1000 + (1000 * -0.05) = 950.00
        expect(calculateCompoundInterest(1000, -5, 1)).toBe(950.00);
        // Year 2: 950 + (950 * -0.05) = 902.50
        expect(calculateCompoundInterest(1000, -5, 2)).toBe(902.50);

        // Negative principal: Balance should become more negative over time with positive rate
        // Year 1: -1000 + (-1000 * 0.10) = -1100.00
        expect(calculateCompoundInterest(-1000, 10, 1)).toBe(-1100.00);
        // Year 2: -1100 + (-1100 * 0.10) = -1210.00
        expect(calculateCompoundInterest(-1000, 10, 2)).toBe(-1210.00);

        // Negative principal and negative rate
        // Year 1: -1000 + (-1000 * -0.05) = -1000 + 50 = -950.00
        expect(calculateCompoundInterest(-1000, -5, 1)).toBe(-950.00);
    });
});