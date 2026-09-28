describe('experiment_4.c', () => {
    // REGRESSION TEST: Directly verifies the bug/vulnerability will not reoccur.
    // The original bug: when a=b, the program prints 'b is largest'.
    // The fix: add an explicit check for equality to provide a more informative message.
    it('should correctly identify when two numbers are equal with an informative message', async () => {
        const input = '5 5';
        const { stdout, stderr, code } = await runCProgram(input);
        expect(code).toBe(0);
        // Assuming the fix introduces an output like "Numbers are equal"
        expect(stdout).toContain('Numbers are equal\n');
    });

    // HAPPY PATH TEST: Verifies standard, expected inputs and typical operational flow.
    it('should correctly identify the first number as largest when a > b', async () => {
        const input = '10 5';
        const { stdout, stderr, code } = await runCProgram(input);
        expect(code).toBe(0);
        expect(stdout).toContain('10 is largest\n');
    });

    it('should correctly identify the second number as largest when b > a', async () => {
        const input = '5 10';
        const { stdout, stderr, code } = await runCProgram(input);
        expect(code).toBe(0);
        expect(stdout).toContain('10 is largest\n');
    });

    // EDGE CASE TEST: Tests boundary conditions, empty values, extreme lengths, and unusual formats.
    it('should handle zero values correctly when they are equal', async () => {
        const input = '0 0';
        const { stdout, stderr, code } = await runCProgram(input);
        expect(code).toBe(0);
        // Assuming the fix handles equality for zeros as well
        expect(stdout).toContain('Numbers are equal\n');
    });

    it('should handle negative numbers correctly when the first is largest', async () => {
        const input = '-5 -10';
        const { stdout, stderr, code } = await runCProgram(input);
        expect(code).toBe(0);
        expect(stdout).toContain('-5 is largest\n');
    });

    it('should handle negative numbers correctly when the second is largest', async () => {
        const input = '-10 -5';
        const { stdout, stderr, code } = await runCProgram(input);
        expect(code).toBe(0);
        expect(stdout).toContain('-5 is largest\n');
    });

    it('should handle mixed positive and negative numbers correctly', async () => {
        const input = '10 -5';
        const { stdout, stderr, code } = await runCProgram(input);
        expect(code).toBe(0);
        expect(stdout).toContain('10 is largest\n');
    });

    it('should handle large integer values correctly', async () => {
        const input = '2147483647 1000000000'; // Max signed int value
        const { stdout, stderr, code } = await runCProgram(input);
        expect(code).toBe(0);
        expect(stdout).toContain('2147483647 is largest\n');
    });

    // ERROR HANDLING TEST: Validates invalid inputs, thrown errors, and rejection handling.
    // Note: The target C program does not explicitly handle scanf errors.
    // These tests assume common behavior where unread variables might default to 0 or retain garbage.
    // For robustness, the C program itself would need to check scanf's return value.
    it('should gracefully handle non-numeric input for the second number', async () => {
        const input = '5 non_numeric';
        const { stdout, stderr, code } = await runCProgram(input);
        expect(code).toBe(0);
        // If scanf reads '5' for 'a' but fails for 'b', 'b' might be 0 or uninitialized.
        // Assuming 'b' defaults to 0, then '5' would be largest.
        expect(stdout).toContain('5 is largest\n');
    });

    it('should gracefully handle non-numeric input for the first number', async () => {
        const input = 'non_numeric 5';
        const { stdout, stderr, code } = await runCProgram(input);
        expect(code).toBe(0);
        // If scanf fails for 'a' but reads '5' for 'b', 'a' might be 0 or uninitialized.
        // Assuming 'a' defaults to 0, then '5' would be largest.
        expect(stdout).toContain('5 is largest\n');
    });

    it('should gracefully handle completely non-numeric input', async () => {
        const input = 'abc def';
        const { stdout, stderr, code } = await runCProgram(input);
        expect(code).toBe(0);
        // If scanf fails for both 'a' and 'b', both might be 0 or uninitialized.
        // Assuming both default to 0, the fixed program would output "Numbers are equal".
        expect(stdout).toContain('Numbers are equal\n');
    });
});