describe('Calculator Program', () => {

  // 1. REGRESSION TEST: Directly verifies the bug/vulnerability will not reoccur.
  it('should handle division by zero gracefully and print an error message', async () => {
    // Input: division operator, first number, second number (zero)
    // The fix summary indicates an error message should be printed.
    // Assuming the fix will output "Error: Division by zero" or similar.
    const input = `/\n10 0\n`;
    const { stdout, stderr, code } = await runCProgram(input);

    expect(code).toBe(0); // Program should exit gracefully, not crash
    // The original code would crash or produce undefined behavior.
    // This test expects the *fixed* behavior.
    expect(normalizeProgramOutput(stdout)).toContain("Error: Division by zero");
  });

  // 2. HAPPY PATH TEST: Verifies standard, expected inputs and typical operational flow.
  it('should perform standard arithmetic operations correctly', async () => {
    // Test addition
    let input = `+\n5 3\n`;
    let { stdout } = await runCProgram(input);
    expect(extractResultLine(stdout)).toBe("Result = 8");

    // Test subtraction
    input = `-\n10 4\n`;
    ({ stdout } = await runCProgram(input));
    expect(extractResultLine(stdout)).toBe("Result = 6");

    // Test multiplication
    input = `*\n6 7\n`;
    ({ stdout } = await runCProgram(input));
    expect(extractResultLine(stdout)).toBe("Result = 42");

    // Test division with non-zero divisor
    input = `/\n10 2\n`;
    ({ stdout } = await runCProgram(input));
    expect(extractResultLine(stdout)).toBe("Result = 5");
  });

  // 3. EDGE CASE TEST: Tests boundary conditions, empty values, extreme lengths, and unusual formats.
  it('should handle edge cases like zero, negative numbers, and equal numbers', async () => {
    // Addition with zero
    let input = `+\n0 5\n`;
    let { stdout } = await runCProgram(input);
    expect(extractResultLine(stdout)).toBe("Result = 5");

    // Subtraction resulting in zero
    input = `-\n7 7\n`;
    ({ stdout } = await runCProgram(input));
    expect(extractResultLine(stdout)).toBe("Result = 0");

    // Multiplication by zero
    input = `*\n8 0\n`;
    ({ stdout } = await runCProgram(input));
    expect(extractResultLine(stdout)).toBe("Result = 0");

    // Division of zero by a non-zero number
    input = `/\n0 5\n`;
    ({ stdout } = await runCProgram(input));
    expect(extractResultLine(stdout)).toBe("Result = 0");

    // Operations with negative numbers
    input = `+\n-5 -3\n`;
    ({ stdout } = await runCProgram(input));
    expect(extractResultLine(stdout)).toBe("Result = -8");

    input = `-\n-10 -4\n`;
    ({ stdout } = await runCProgram(input));
    expect(extractResultLine(stdout)).toBe("Result = -6");

    input = `*\n-6 7\n`;
    ({ stdout } = await runCProgram(input));
    expect(extractResultLine(stdout)).toBe("Result = -42");

    input = `/\n-10 2\n`;
    ({ stdout } = await runCProgram(input));
    expect(extractResultLine(stdout)).toBe("Result = -5");
  });

  // 4. ERROR HANDLING TEST: Validates invalid inputs, thrown errors, and rejection handling.
  it('should display an error message for an invalid operator', async () => {
    const input = `%\n10 5\n`; // Using an invalid operator '%'
    const { stdout, stderr, code } = await runCProgram(input);

    expect(code).toBe(0);
    expect(normalizeProgramOutput(stdout)).toContain("Invalid operator");
  });
});

// Helper function to extract the result line, assuming it's the last line containing "Result ="
function extractResultLine(stdout) {
  const lines = stdout.split('\n').filter(line => line.includes("Result ="));
  if (lines.length > 0) {
    return lines[lines.length - 1].trim();
  }
  return "";
}

// Helper function to normalize program output for consistent testing
function normalizeProgramOutput(stdout) {
  // Remove prompts and extra whitespace for easier comparison
  return stdout
    .split('\n')
    .filter(line => !line.includes("Enter operator") && !line.includes("Enter two numbers"))
    .map(line => line.trim())
    .filter(line => line.length > 0)
    .join('\n');
}