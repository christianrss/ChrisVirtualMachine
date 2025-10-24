#include "src/vm/ChrisVM.h"
/**
 * Chris VM main executable.
 */
int main(int argc, char const *argv[]) {

    ChrisVM vm;

    vm.exec(R"(

        42

    )");

    return 0;
}