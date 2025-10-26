/**
 * Chris value.
 */

#ifndef ChrisValue_h
#define ChrisValue_h

#include <string>

/**
 * Chris value type.
 */
enum class ChrisValueType {
    NUMBER,
    OBJECT,
};

/**
 * Object type.
 */
enum class ObjectType {
    STRING,
    CODE,
};

/**
 * Base object.
 */
struct Object {
    Object(ObjectType type) : type(type) {}
    ObjectType type;
};

/**
 * String object.
 */
struct StringObject : public Object {
    StringObject(const std::string& str)
        : Object(ObjectType::STRING), string(str) {}
    std::string string;
};

/**
 * Chris value (tagged union).
 */
struct ChrisValue {
    ChrisValueType type;
    union {
        double number;
        Object* object;
    };
};



/**
 * Code object.
 */
struct CodeObject: public Object {
    CodeObject(const std::string& name) : Object(ObjectType::CODE), name(name) {}

    /**
     * Name of the unit (usually function name).
     */
    std::string name;

    /**
     * Constant pool.
     */
    std::vector<ChrisValue> constants;

    /**
     * Bytecode.
     */
    std::vector<uint8_t> code;
};

// ------------------------------------------------------------------------
// Constructors:
#define NUMBER(value) ((ChrisValue) { .type = ChrisValueType::NUMBER, .number = value})

#define ALLOC_STRING(value) \
    ((ChrisValue) { .type = ChrisValueType::OBJECT, .object = (Object*)new StringObject(value)})

#define ALLOC_CODE(name) \
    ((ChrisValue) { .type = ChrisValueType::OBJECT, .object = (Object*) new CodeObject(name)})

// ------------------------------------------------------------------------
// Accessors:
#define AS_NUMBER(chrisValue) ((double)(chrisValue).number)
#define AS_OBJECT(chrisValue) ((Object*)(chrisValue).object)

#define AS_STRING(chrisValue) ((StringObject*)(chrisValue).object)
#define AS_CPPSTRING(chrisValue) (AS_STRING(chrisValue)->string)

#define AS_CODE(chrisValue) ((CodeObject*)(chrisValue).object)

// ------------------------------------------------------------------------
// Testers:
#define IS_NUMBER(chrisValue) ((chrisValue).type == ChrisValueType::NUMBER)
#define IS_OBJECT(chrisValue) ((chrisValue).type == ChrisValueType::OBJECT)

#define IS_OBJECT_TYPE(chrisValue, objectType) \
    (IS_OBJECT(chrisValue) && AS_OBJECT(chrisValue)->type == objectType)

#define IS_STRING(chrisValue) IS_OBJECT_TYPE(chrisValue, ObjectType::STRING)
#define IS_CODE(chrisValue) IS_OBJECT_TYPE(chrisValue, ObjectType::CODE)

#endif