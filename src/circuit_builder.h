#ifndef _CIRCUIT_BUILDER_H_
#define _CIRCUIT_BUILDER_H_

enum COMPONENT_TYPE {
    CT_NONE,
    CT_CLOCK,
    CT_PIN,
    CT_AND,
    CT_OR,
    CT_ADDER
};

const char* COMPONENT_TYPE_STRING(COMPONENT_TYPE type);

class CircuitBuilder {
public:
    static CircuitBuilder& get() {
        static CircuitBuilder instance;
        return instance;
    }

    void set_build_component(COMPONENT_TYPE type = CT_NONE) { m_build_type = type; }
    COMPONENT_TYPE get_build_type() const { return m_build_type; }

private:
    COMPONENT_TYPE          m_build_type;

    CircuitBuilder() : m_build_type(CT_NONE) {};
};

#endif // _CIRCUIT_BUILDER_H_