#include <iostream>
#include <cmath>
#include <cstdint>

template<int num_f_bits>
class FixedPoint {
public:
    // Constructor from a double
    explicit FixedPoint(double value) {
        // Scaling the input value by the number of fractional bits and rounding to the nearest integer
        fixedValue = static_cast<int64_t>(std::round(value * (1LL << fractionalBits)));
    }

    // Default constructor
    FixedPoint() : fixedValue(0) {}

    // Addition operator
    FixedPoint operator+(const FixedPoint& other) const {
        return FixedPoint::fromRaw(fixedValue + other.fixedValue);
    }

    // Subtraction operator
    FixedPoint operator-(const FixedPoint& other) const {
        return FixedPoint::fromRaw(fixedValue - other.fixedValue);
    }

    // Division operator, using double for calculation
    FixedPoint operator/(const FixedPoint& other) const {
        if (other.fixedValue == 0) {
            std::cerr << "Division by zero error" << std::endl;
            return FixedPoint(0); // Handle division by zero if necessary
        }
        double result = this->toDouble() / other.toDouble(); // Convert both to double, divide
        return FixedPoint(result); // Convert back to fixed point
    }

    // Multiplication operator, using double for calculation
    FixedPoint operator*(const FixedPoint& other) const {
        double result = this->toDouble() * other.toDouble(); // Convert both to double, multiply
        return FixedPoint(result); // Convert back to fixed point
    }

    // Conversion to double
    double toDouble() const {
        return static_cast<double>(fixedValue) / (1LL << fractionalBits);
    }

    // Static factory method to create a FixedPoint from a raw fixed-point value (for internal use)
    static FixedPoint fromRaw(int64_t raw) {
        FixedPoint fp;
        fp.fixedValue = raw;
        return fp;
    }

private:
    int64_t fixedValue;  // The raw fixed-point value. Includes one sign bit implicitly.
    static const int fractionalBits = num_f_bits; // Number of fractional bits
};