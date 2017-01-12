#include "Halide.h"
#include <stdio.h>
#ifndef _MSC_VER
#include <unistd.h>
#endif

using namespace Halide;

void testCompileToOutput(Func j) {
    std::string fn_object = "compile_to_multitarget";
#ifdef _MSC_VER
    std::string expected_lib = fn_object + ".lib";
#else
    std::string expected_lib = fn_object + ".a";
#endif
    std::string expected_h = fn_object + ".h";

    Internal::file_unlink_or_die(fn_object);
    Internal::file_unlink_or_die(expected_lib);
    Internal::file_unlink_or_die(expected_h);

    std::vector<Target> targets = {
        Target("host-profile-debug"),
        Target("host-profile"),
    };
    j.compile_to_multitarget_static_library(fn_object, j.infer_arguments(), targets);

    Internal::file_exists_or_die(fn_object);
    Internal::file_exists_or_die(expected_lib);
    Internal::file_exists_or_die(expected_h);
}

int main(int argc, char **argv) {
    Param<float> factor("factor");
    Func f, g, h, j;
    Var x, y;
    f(x, y) = x + y;
    g(x, y) = cast<float>(f(x, y) + f(x+1, y));
    h(x, y) = f(x, y) + g(x, y);
    j(x, y) = h(x, y) * 2 * factor;

    f.compute_root();
    g.compute_root();
    h.compute_root();

    testCompileToOutput(j);

    printf("Success!\n");
    return 0;
}
