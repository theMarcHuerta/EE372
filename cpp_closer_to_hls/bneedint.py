def calculate_max_integer_bits(numbers):
    max_int_bits = 0
    for number in numbers:
        int_part = int(abs(number))
        if int_part == 0:
            int_bits = 1  # At least 1 bit for zero
        else:
            int_bits = int_part.bit_length() + 1  # Add 1 for the sign bit
        max_int_bits = max(max_int_bits, int_bits)
    return max_int_bits

def process_file(filename, x):
    with open(filename, 'r') as file:
        lines = file.read().strip().split('\n')
    
    results = [0] * x  # Initialize results list for integer bits of each position

    for i in range(0, len(lines), x):
        group = lines[i:i+x]  # Get the current group of x lines
        for idx, line in enumerate(group):
            if line.strip():  # Check if the line is not empty
                numbers = [float(num) for num in line.split()]
                max_int_bits = calculate_max_integer_bits(numbers)
                results[idx] = max(results[idx], max_int_bits)  # Update max integer bits for this line position

    return results

# Example usage
filename = 'test_stimulus_and_results/10bit_rec_ray_1samp/camera_ray_1b_1s_240p.txt'
x = 3  # Number of lines per group
results = process_file(filename, x)
for idx, bits in enumerate(results):
    print(f"Position {idx + 1}: Max integer bits needed: {bits}")
