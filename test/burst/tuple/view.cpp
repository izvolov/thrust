#include <burst/tuple/view.hpp>
#include <utility/io/tuple.hpp>

#include <doctest/doctest.h>
#include <boost/tuple/tuple.hpp>

#include <string>
#include <tuple>

namespace // anonymous
{

    struct dummy
    {
        dummy () = default;
        dummy (const dummy &) = delete;
        dummy & operator = (const dummy &) = delete;
        dummy (dummy &&) = delete;
        dummy & operator = (dummy &&) = delete;
        ~dummy () = default;
    };
} // namespace anonymous

TEST_SUITE("view")
{
    TEST_CASE("gets_subview_of_a_tuple")
    {
        const auto t = std::make_tuple('a', 'b', 'c');
        CHECK(burst::view<0, 1>(t) == std::make_tuple('a', 'b'));
    }

    TEST_CASE("elements_of_view_of_lvalue_tuple_are_lvalues")
    {
        auto t = std::make_tuple(42, std::string("qwe"), 3.14);

        auto l = burst::view<0, 1>(t);
        std::get<0>(l) = 17;
        std::get<1>(l).append("rty");

        CHECK(std::get<0>(t) == 17);
        CHECK(std::get<1>(t) == "qwerty");
    }

    TEST_CASE("neither_copies_nor_moves")
    {
        burst::view<1>(std::forward_as_tuple(1, dummy{}, 3.14));
    }

    TEST_CASE("is_compatible_with_boost_tuple")
    {
        const auto t = boost::make_tuple(42, 3.14, true);
        const auto l = burst::view<0, 2>(t);
        CHECK(std::get<1>(l) == true);
    }
}
