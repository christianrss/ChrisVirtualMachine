#include <iostream>

#include "src/Logger.h"
#include "src/vm/ChrisVM.h"
/**
 * Chris VM main executable.
 */
int main(int argc, char const *argv[]) {

    ChrisVM vm;

    auto result = vm.exec(R"(
        (> 5 10)
    )");

    log(result);

    std::cout << "All done!\n";

    return 0;
}