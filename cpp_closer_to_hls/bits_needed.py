def calculate_fixed_point_bits(value, int_error_margin_percent, frac_error_margin_percent):
    if value == 0:
        return (1, 0, 0, 0)  # Handle zero separately to avoid division by zero errors.

    int_part, frac_part = divmod(abs(value), 1)
    target_int_error = abs(int_part * int_error_margin_percent / 100)
    target_frac_error = frac_error_margin_percent / 100

    int_bits = int(int_part).bit_length() + 1  # Plus one for the sign bit
    min_int_bits = int_bits if int_part == 0 or abs(int_part - (2 ** (int_bits - 1))) <= target_int_error else int_bits + 1

    optimal_frac_part = 0
    frac_bits = 0
    while frac_bits < 30:
        frac_part *= 2
        current_bit = int(frac_part)
        frac_part -= current_bit
        optimal_frac_part += current_bit * (2 ** -frac_bits)
        if abs(frac_part) <= target_frac_error:
            break
        frac_bits += 1

    fixed_point_value = int_part + optimal_frac_part if value >= 0 else -(abs(int_part) + optimal_frac_part)
    margin_error = abs((fixed_point_value - value) / value) * 100  # Error as percentage

    return min_int_bits, frac_bits, fixed_point_value, margin_error

def process_file(filename, x, int_error_margin_percent, frac_error_margin_percent):
    with open(filename, 'r') as file:
        lines = [line.strip() for line in file if line.strip()]

    # Initialize max values for each position in the group
    max_values = [{"max_int_bits": 0, "max_frac_bits": 0, "max_value": 0, "max_fixed_point": 0, "max_error": 0} for _ in range(x)]

    for i in range(0, len(lines), x):
        for pos in range(x):
            if i + pos < len(lines):
                numbers = map(float, lines[i + pos].split())
                for number in numbers:
                    int_bits, frac_bits, fixed_point_value, margin_error = calculate_fixed_point_bits(
                        number, int_error_margin_percent, frac_error_margin_percent)
                    if int_bits > max_values[pos]["max_int_bits"]:
                        max_values[pos]["max_int_bits"] = int_bits
                        max_values[pos]["max_value"] = number
                        max_values[pos]["max_fixed_point"] = fixed_point_value
                        max_values[pos]["max_error"] = margin_error
                    if frac_bits > max_values[pos]["max_frac_bits"]:
                        max_values[pos]["max_frac_bits"] = frac_bits
                        max_values[pos]["max_value"] = number
                        max_values[pos]["max_fixed_point"] = fixed_point_value
                        max_values[pos]["max_error"] = margin_error

    return max_values

# Example usage
filename = 'test_stimulus_and_results/matscat_stim_8_8/ray_out_first_bounce.txt'
x = 2  # Number of lines per group
int_error_margin_percent = 5  # Integer margin of error as a percentage
frac_error_margin_percent = 10  # Fractional margin of error as a percentage
results = process_file(filename, x, int_error_margin_percent, frac_error_margin_percent)
for idx, result in enumerate(results):
    print(f"Position {idx + 1}:")
    print(f"  Max int bits: {result['max_int_bits']}, Value: {result['max_value']}, Fixed point: {result['max_fixed_point']}, Error: {result['max_error']:.2f}%")
    print(f"  Max frac bits: {result['max_frac_bits']}, Value: {result['max_value']}, Fixed point: {result['max_fixed_point']}, Error: {result['max_error']:.2f}%")
