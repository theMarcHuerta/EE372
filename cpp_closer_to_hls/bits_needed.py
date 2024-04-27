def calculate_fixed_point_bits(value):
    int_part, frac_part = divmod(abs(value), 1)
    int_bits = int(int_part).bit_length() + (1 if value < 0 else 0)  # Convert int_part to int and add 1 bit for sign if negative
    frac_bits = 0
    while frac_part != 0 and frac_bits <= 20:  # Limit to 20 fractional bits for practicality
        frac_part *= 2
        frac_bits += 1
        frac_part -= int(frac_part)
    return int_bits, frac_bits

def process_file(filename, x):
    with open(filename, 'r') as file:
        lines = [line.strip() for line in file if line.strip()]

    max_bits_and_values = [(0, 0)] * x  # Initialize max bits and values for each position

    # Process each position within the groups
    for index in range(0, len(lines), x):
        for pos in range(x):
            if index + pos < len(lines):
                numbers = map(float, lines[index + pos].split())
                line_max_bits = 0
                max_value = 0
                for number in numbers:
                    int_bits, frac_bits = calculate_fixed_point_bits(number)
                    total_bits = int_bits + frac_bits
                    if total_bits > line_max_bits:
                        line_max_bits = total_bits
                        max_value = number
                if line_max_bits > max_bits_and_values[pos][0]:
                    max_bits_and_values[pos] = (line_max_bits, max_value)

    return max_bits_and_values

# Example usage
filename = 'test_stimulus_and_results/matscat_stim_8_8/rays_generated.txt'
x = 2  # Number of lines per group
max_bits_and_values = process_file(filename, x)
for position, (bits, value) in enumerate(max_bits_and_values, 1):
    print(f"Maximum bits required for position {position}: {bits}, Number requiring max bits: {value}")

