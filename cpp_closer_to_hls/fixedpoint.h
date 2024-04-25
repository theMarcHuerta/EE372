#include <iostream>
#include <cmath>
#include <cstdint>

template<int IntegerBits, int FractionalBits>
class FixedPoint {
public:
    // Constructor from a double
    explicit FixedPoint(double value) {
        // Scaling the input value by the number of fractional bits and rounding to the nearest integer
        fixedValue = static_cast<int32_t>(round(value * (1 << FractionalBits)));
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

        // Division operator
    FixedPoint operator/(const FixedPoint& other) const {
        if (other.fixedValue == 0) {
            std::cerr << "Division by zero error" << std::endl;
            return FixedPoint(0); // Handle division by zero if necessary
        }
        int64_t temp = static_cast<int64_t>(fixedValue) << FractionalBits;  // Scale dividend to preserve fractional part
        temp /= other.fixedValue;  // Perform the integer division
        return FixedPoint::fromRaw(static_cast<int32_t>(temp));
    }
    
    // Multiplication operator
    FixedPoint operator*(const FixedPoint& other) const {
        int64_t temp = static_cast<int64_t>(fixedValue) * static_cast<int64_t>(other.fixedValue);
        temp >>= FractionalBits; // Normalize back to fixed point format
        return FixedPoint::fromRaw(static_cast<int32_t>(temp));
    }

    // Conversion to double
    double toDouble() const {
        return static_cast<double>(fixedValue) / (1 << FractionalBits);
    }

    // Static factory method to create a FixedPoint from a raw fixed-point value (for internal use)
    static FixedPoint fromRaw(int32_t raw) {
        FixedPoint fp;
        fp.fixedValue = raw;
        return fp;
    }

private:
    int32_t fixedValue;  // The raw fixed-point value. Includes one sign bit implicitly.
};