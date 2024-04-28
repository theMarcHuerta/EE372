def calculate_fixed_point_bits(value, int_error_margin_percent, frac_error_margin_percent):
    if value == 0:
        return (1, 0, 0, 0, 0)  # Handle zero separately to avoid division by zero errors.

    int_part, frac_part = divmod(abs(value), 1)
    target_int_error = abs(int_part * int_error_margin_percent / 100)
    target_frac_error = frac_error_margin_percent / 100

    int_bits = int(int_part).bit_length() + (1 if value < 0 else 0)
    min_int_bits = int_bits
    if int_part != 0 and abs(int_part - (2 ** (min_int_bits - 1))) > target_int_error:
        min_int_bits += 1

    optimal_frac_part = 0
    frac_bits = 0
    while frac_bits < 20:
        frac_part *= 2
        current_bit = int(frac_part)
        frac_part -= current_bit
        optimal_frac_part += current_bit * (2 ** -frac_bits)
        if abs(frac_part) < target_frac_error:
            break
        frac_bits += 1

    fixed_point_value = int_part + optimal_frac_part if value >= 0 else -(abs(int_part) + optimal_frac_part)
    margin_error = abs((fixed_point_value - value) / value) * 100 if value != 0 else 0

    return min_int_bits, frac_bits, fixed_point_value, margin_error, value

def process_file(filename, x, int_error_margin_percent, frac_error_margin_percent):
    with open(filename, 'r') as file:
        lines = [line.strip() for line in file if line.strip()]

    # Initialize results list for each position in the group
    results = [{"max_int_bits": 0, "max_frac_bits": 0, "max_value": 0, "max_fixed_point": 0, "max_error": 0} for _ in range(x)]

    for i in range(0, len(lines), x):
        for pos in range(x):
            if i + pos < len(lines):
                numbers = [float(num) for num in lines[i + pos].split()]
                for number in numbers:
                    int_bits, frac_bits, fixed_point_value, margin_error, original_value = calculate_fixed_point_bits(
                        number, int_error_margin_percent, frac_error_margin_percent)
                    if int_bits > results[pos]["max_int_bits"] or (int_bits == results[pos]["max_int_bits"] and margin_error < results[pos]["max_error"]):
                        results[pos]["max_int_bits"] = int_bits
                        results[pos]["max_value"] = original_value
                        results[pos]["max_fixed_point"] = fixed_point_value
                        results[pos]["max_error"] = margin_error
                    if frac_bits > results[pos]["max_frac_bits"] or (frac_bits == results[pos]["max_frac_bits"] and margin_error < results[pos]["max_error"]):
                        results[pos]["max_frac_bits"] = frac_bits
                        results[pos]["max_value"] = original_value
                        results[pos]["max_fixed_point"] = fixed_point_value
                        results[pos]["max_error"] = margin_error

    return results

# Example usage
filename = 'test_stimulus_and_results/10bit_rec_ray_1samp/quads_hit_routine_vars_and_results.txt'
x = 20  # Number of lines per group
int_error_margin_percent = 5  # Integer margin of error as a percentage
frac_error_margin_percent = 10  # Fractional margin of error as a percentage
results = process_file(filename, x, int_error_margin_percent, frac_error_margin_percent)
for idx, result in enumerate(results):
    print(f"Position {idx + 1}:")
    print(f"  Max int bits: {result['max_int_bits']}, Value: {result['max_value']}, Fixed point: {result['max_fixed_point']}, Error: {result['max_error']:.2f}%")
    print(f"  Max frac bits: {result['max_frac_bits']}, Value: {result['max_value']}, Fixed point: {result['max_fixed_point']}, Error: {result['max_error']:.2f}%")

