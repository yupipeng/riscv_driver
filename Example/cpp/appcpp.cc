#include "appcpp.h"
#include <cstdio>

class CppClass {

public:
    CppClass() {
        printf("CppClass constructor\r\n");
    }
    ~CppClass() {
        printf("CppClass destructor\r\n");
    }
};
// #define FMT_HEADER_ONLY
#include "fmt/core.h"
#include <string>
extern "C"{ void * __dso_handle = 0 ;}
void cpp_main() {

    CppClass* cppClass = new CppClass(); // c++ and memory allocation
    fmt::print("Hello World From {}! built at {}\r\n", "Nuclei RISC-V Processor", __TIME__);

    // while (true);
}