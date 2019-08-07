#include <burst/integer/intpow.hpp>

#include <doctest/doctest.h>

#include <cstdint>
#include <stdexcept>
#include <type_traits>

TEST_SUITE("intpow")
{
    TEST_CASE("any_power_of_zero_is_zero")
    {
        CHECK(burst::intpow(0, 1) == 0);
        CHECK(burst::intpow(0, 2) == 0);
        CHECK(burst::intpow(0, 10) == 0);
        CHECK(burst::intpow(0, 100500) == 0);
    }

    TEST_CASE("zero_power_of_zero_is_one")
    {
        CHECK(burst::intpow(0, 0) == 1);
    }

    TEST_CASE("any_power_of_one_is_one")
    {
        CHECK(burst::intpow(1, 0) == 1);
        CHECK(burst::intpow(1, 1) == 1);
        CHECK(burst::intpow(1, 2) == 1);
        CHECK(burst::intpow(1, 10) == 1);
        CHECK(burst::intpow(1, 100500) == 1);
    }

    TEST_CASE("power_must_be_non_negative")
    {
        CHECK_THROWS_AS(burst::intpow(2, -1), std::domain_error);
    }

    TEST_CASE("base_may_be_negative")
    {
        CHECK(burst::intpow(-1, 2) == 1);
        CHECK(burst::intpow(-3, 3) == -27);
    }

    TEST_CASE("any_base_to_the_power_of_zero_is_one")
    {
        CHECK(burst::intpow(2, 0) == 1);
        CHECK(burst::intpow(-2, 0) == 1);
        CHECK(burst::intpow(20, 0) == 1);
        CHECK(burst::intpow(-100500, 0) == 1);
    }

    TEST_CASE("return_type_deferred_as_base_type")
    {
        CHECK(std::is_same<decltype(burst::intpow(1, 10)), decltype(1)>::value);
        CHECK(std::is_same<decltype(burst::intpow(1l, 10)), decltype(1l)>::value);
        CHECK(std::is_same<decltype(burst::intpow(1ll, 10)), decltype(1ll)>::value);
        CHECK(std::is_same<decltype(burst::intpow(1u, 10)), decltype(1u)>::value);
        CHECK(std::is_same<decltype(burst::intpow(1ul, 10)), decltype(1ul)>::value);
        CHECK(std::is_same<decltype(burst::intpow(1ull, 10)), decltype(1ull)>::value);
    }

    TEST_CASE("power_of_product_is_product_of_powers")
    {
        // (ab) ^ n = (a ^ n)(b ^ n)
        const auto a = 10;
        const auto b = 3;
        const auto n = 4;
        CHECK(burst::intpow(a * b, n) == burst::intpow(a, n) * burst::intpow(b, n));
    }

    TEST_CASE("product_of_powers_with_same_base_equals_that_base_to_the_power_of_sum_of_those_powers")
    {
        // (a ^ n)(a ^ m) = a ^ (n + m)
        const auto a = 4;
        const auto n = 3;
        const auto m = 8;

        // Clang7 сошёл с ума, и не даёт записать это в одну строку. Пришлось выносить один из
        // множителей в отдельную переменную.
        const auto y = burst::intpow(a, m);
        CHECK(burst::intpow(a, n) * y == burst::intpow(a, n + m));
    }

    TEST_CASE("some_base_in_power_of_n_to_the_power_of_m_equals_that_base_to_the_power_of_product_of_n_and_m")
    {
        // (a ^ n) ^ m = a ^ (nm)
        const auto a = 7;
        const auto n = 10;
        const auto m = 2;
        CHECK(burst::intpow(burst::intpow(a, n), m) == burst::intpow(a, n * m));
    }

    TEST_CASE("intpow_is_a_constexpr_function")
    {
        constexpr auto power = burst::intpow(3, 4);
        CHECK(power == 81);
    }

    TEST_CASE("return_value_type_is_type_of_first_argument")
    {
        using first_argument_type = std::uint32_t;
        using second_argument_type = std::int64_t;
        static_assert(not std::is_same<first_argument_type, second_argument_type>::value, "");

        using result_type = decltype(burst::intpow(first_argument_type{2}, second_argument_type{5}));

        CHECK(std::is_same<result_type, first_argument_type>::value);
    }
}
