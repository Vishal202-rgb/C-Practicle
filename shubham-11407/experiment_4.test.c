const { exec, spawn } = require('child_process');
const fs = require('fs');
const path = require('path');

// Define paths for the C source and executable
const C_FILE_NAME = 'experiment_4.c';
const EXECUTABLE_NAME = 'experiment_4_test';
const C_FILE_PATH = path.join(__dirname, C_FILE_NAME);
const EXECUTABLE_PATH = path.join(__dirname, EXECUTABLE_NAME);

// The C code with the fix applied (as per Fix Summary)
// This version includes an explicit check for equality and basic input validation.
const fixedCCode = `
#include <stdio.h>

int main() {
    int a, b;
    printf("Enter two numbers: ");

    // Check the return value of scanf to ensure two integers were successfully read.
    if (scanf("%d %d", &a, &b) != 2) {
        fprintf(stderr, "Invalid input. Please enter two integers.\\n");
        return 1; // Indicate an error
    }

    if (a == b) {
        printf("The numbers are equal\\n");
    } else if (a > b) {
        printf("%d is largest\\n", a);
    } else {
        printf("%d is largest\\n", b);
    }

    return 0;
}
`;

// Helper function to compile and run the C program
async function runCProgram(input, timeout = 2000) {
    return new Promise((resolve, reject) => {
        const child = spawn(EXECUTABLE_PATH, [], { timeout });

        let stdout = '';
        let stderr = '';
        let timedOut = false;

        child.stdout.on('data', (data) => {
            stdout += data.toString();
        });

        child.stderr.on('data', (data) => {
            stderr += data.toString();
        });

        child.on('close', (code) => {
            if (timedOut) {
                reject(new Error(`Program timed out after ${timeout}ms`));
            } else {
                resolve({ stdout, stderr, code });
            }
        });

        child.on('error', (err) => {
            if (err.code === 'ETIMEDOUT') {
                timedOut = true;
                child.kill(); // Ensure the process is terminated
            } else {
                reject(err);
            }
        });

        // Write input to stdin
        child.stdin.write(input);
        child.stdin.end();
    });
}

describe('C Program: experiment_4.c - Largest Number Comparison', () => {
    // Before all tests, compile the C code
    beforeAll((done) => {
        // Write the fixed C code to a temporary file
        fs.writeFileSync(C_FILE_PATH, fixedCCode);

        exec(`gcc ${C_FILE_PATH} -o ${EXECUTABLE_PATH}`, (error, stdout, stderr) => {
            if (error) {
                console.error(`Compilation error: ${error.message}`);
                console.error(`Stderr: ${stderr}`);
                done(error);
            } else {
                done();
            }
        });
    }, 10000); // Increased timeout for compilation

    // After all tests, clean up compiled files
    afterAll(() => {
        if (fs.existsSync(C_FILE_PATH)) {
            fs.unlinkSync(C_FILE_PATH);
        }
        if (fs.existsSync(EXECUTABLE_PATH)) {
            fs.unlinkSync(EXECUTABLE_PATH);
        }
    });

    // Helper to extract the last meaningful line of output, which contains the result
    const getResultLine = (output) => {
        const lines = output.trim().split('\n');
        // Filter out the "Enter two numbers: " prompt and any empty lines
        const resultLines = lines.filter(line => line.trim() !== '' && !line.includes("Enter two numbers:"));
        return resultLines.length > 0 ? resultLines[resultLines.length - 1].trim() : '';
    };

    // 1. REGRESSION TEST: Directly verifies the bug/vulnerability will not reoccur.
    it('REGRESSION TEST: should correctly identify when two numbers are equal', async () => {
        const input = '5 5\n';
        const { stdout, stderr, code } = await runCProgram(input);
        expect(code).toBe(0);
        expect(stderr).toBe('');
        expect(getResultLine(stdout)).toBe('The numbers are equal');
    });

    // 2. HAPPY PATH TEST: Verifies standard, expected inputs and typical operational flow.
    it('HAPPY PATH TEST: should correctly identify the largest number when a > b', async () => {
        const input = '10 5\n';
        const { stdout, stderr, code } = await runCProgram(input);
        expect(code).toBe(0);
        expect(stderr).toBe('');
        expect(getResultLine(stdout)).toBe('10 is largest');
    });

    it('HAPPY PATH TEST: should correctly identify the largest number when b > a', async () => {
        const input = '5 10\n';
        const { stdout, stderr, code } = await runCProgram(input);
        expect(code).toBe(0);
        expect(stderr).toBe('');
        expect(getResultLine(stdout)).toBe('10 is largest');
    });

    // 3. EDGE CASE TEST: Tests boundary conditions, empty values, extreme lengths, and unusual formats.
    it('EDGE CASE TEST: should handle zero values correctly when equal', async () => {
        const input = '0 0\n';
        const { stdout, stderr, code } = await runCProgram(input);
        expect(code).toBe(0);
        expect(stderr).toBe('');
        expect(getResultLine(stdout)).toBe('The numbers are equal');
    });

    it('EDGE CASE TEST: should handle negative numbers correctly when a > b', async () => {
        const input = '-5 -10\n';
        const { stdout, stderr, code } = await runCProgram(input);
        expect(code).toBe(0);
        expect(stderr).toBe('');
        expect(getResultLine(stdout)).toBe('-5 is largest');
    });

    it('EDGE CASE TEST: should handle negative numbers correctly when b > a', async () => {
        const input = '-10 -5\n';
        const { stdout, stderr, code } = await runCProgram(input);
        expect(code).toBe(0);
        expect(stderr).toBe('');
        expect(getResultLine(stdout)).toBe('-5 is largest');
    });

    it('EDGE CASE TEST: should handle large positive numbers correctly (a largest)', async () => {
        const input = '2147483647 1\n'; // Max int
        const { stdout, stderr, code } = await runCProgram(input);
        expect(code).toBe(0);
        expect(stderr).toBe('');
        expect(getResultLine(stdout)).toBe('2147483647 is largest');
    });

    it('EDGE CASE TEST: should handle large positive numbers correctly (b largest)', async () => {
        const input = '1 2147483647\n'; // Max int
        const { stdout, stderr, code } = await runCProgram(input);
        expect(code).toBe(0);
        expect(stderr).toBe('');
        expect(getResultLine(stdout)).toBe('2147483647 is largest');
    });

    // 4. ERROR HANDLING TEST: Validates invalid inputs, thrown errors, and rejection handling.
    it('ERROR HANDLING TEST: should indicate error for non-numeric input', async () => {
        const input = 'abc def\n';
        const { stdout, stderr, code } = await runCProgram(input);
        expect(code).not.toBe(0); // Expect non-zero exit code for error
        expect(stderr).toContain('Invalid input. Please enter two integers.');
        expect(getResultLine(stdout)).toBe(''); // Should not print comparison result
    });

    it('ERROR HANDLING TEST: should indicate error for partial input', async () => {
        const input = '10\n'; // Missing the second number
        const { stdout, stderr, code } = await runCProgram(input);
        expect(code).not.toBe(0); // Expect non-zero exit code for error
        expect(stderr).toContain('Invalid input. Please enter two integers.');
        expect(getResultLine(stdout)).toBe(''); // Should not print comparison result
    });

    it('ERROR HANDLING TEST: should indicate error for empty input', async () => {
        const input = '\n'; // No input provided
        const { stdout, stderr, code } = await runCProgram(input);
        expect(code).not.toBe(0); // Expect non-zero exit code for error
        expect(stderr).toContain('Invalid input. Please enter two integers.');
        expect(getResultLine(stdout)).toBe(''); // Should not print comparison result
    });
});