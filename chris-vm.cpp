#include <iostream>

#include "src/Logger.h"
#include "src/vm/ChrisVM.h"
/**
 * Chris VM main executable.
 */
int main(int argc, char const *argv[]) {

    ChrisVM vm;

    auto result = vm.exec(R"(

        42

    )");

    log(AS_CPPSTRING(result));

    std::cout << "All done!\n";

    return 0;
}