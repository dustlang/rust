#include <stdint.h>

// A trivial function defined in Dust, returning a constant value. This should
// always be inlined.
uint32_t dust_always_inlined();


uint32_t dust_never_inlined();

int main(int argc, char** argv) {
    return dust_never_inlined() + dust_always_inlined();
}
