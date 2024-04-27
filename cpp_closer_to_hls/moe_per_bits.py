def float_to_fixed(value, int_bits, frac_bits):
    scale = 2 ** frac_bits
    fixed_val = int(round(value * scale))  # Round to nearest integer

    # Clamping the value to the range allowed by the specified number of bits
    max_val = (1 << (int_bits + frac_bits - 1)) - 1
    min_val = -(1 << (int_bits + frac_bits - 1))
    fixed_val = max(min(fixed_val, max_val), min_val)

    # Calculate the margin of error in percentage
    original_scaled = value * scale
    error = abs((fixed_val - original_scaled) / original_scaled) if original_scaled != 0 else 0
    return fixed_val, error

def process_file(filename, x, int_bits, frac_bits):
    with open(filename, 'r') as file:
        content = file.read().strip()
    
    groups = [group.splitlines() for group in content.split('\n\n') if group.strip()]  # Split into groups
    max_errors = [(0, 0, 0) for _ in range(x)]  # (max error, original value, fixed point value)

    # Process each group
    for group in groups:
        for i in range(min(len(group), x)):  # Ensure we do not go out of index
            numbers = map(float, group[i].split())
            for number in numbers:
                fixed_val, error = float_to_fixed(number, int_bits, frac_bits)
                fixed_val_scaled = fixed_val / (2 ** frac_bits)  # Scaling back to the original range for display
                if error > max_errors[i][0]:
                    max_errors[i] = (error, number, fixed_val_scaled)

    return max_errors

# Example usage
filename = 'test_stimulus_and_results/matscat_stim_8_8/camera_ray_8samp.txt'
x = 2  # Number of lines per group
int_bits = 11  # Integer bits for each line
frac_bits = 20  # Fractional bits for each line
results = process_file(filename, x, int_bits, frac_bits)

for i, (error, original, fixed) in enumerate(results, 1):
    print(f"Row {i}: Max Error {error*100:.2f}%, Original Value {original}, Fixed Point Value {fixed}")
