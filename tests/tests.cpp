#include <catch2/catch_test_macros.hpp>
#include <mylib/mylib.h>
#include <numeric>
#include <vector>

TEST_CASE("mylib_sum", "[sum]") {
    const int n = 2048;
    std::vector<int> src(n);
    std::iota(src.begin(), src.end(), 1);
    REQUIRE(mylib_sum(n, src.data()) == n * (n + 1) / 2);
}
