/**
 * Chris value.
 */

#ifndef ChrisValue_h
#define ChrisValue_h

/**
 * Chris value type.
 */
enum class ChrisValueType {
    NUMBER,
};

/**
 * Chris value (tagged union).
 */
struct ChrisValue {
    ChrisValueType type;
    union {
        double number;
    };
};

// ------------------------------------------------------------------------
// Constructors:

#define NUMBER(value) ((ChrisValue) {ChrisValueType::NUMBER, .number = value})

// ------------------------------------------------------------------------
// Accessors:
#define AS_NUMBER(chrisValue) ((double)(chrisValue).number)

#endif