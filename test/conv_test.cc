#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <map>
#include <string>
#include <utility>
#include <vector>

#include <conv/conv.h>

using conv::parse;
using conv::to;

using conv::lbracket;
using conv::rbracket;
using conv::comma;

TEST_CASE("string -> int", "[string]") {
    REQUIRE(to<int>("1234") == 1234);
    REQUIRE(to<int>("  1234") == 1234);
    REQUIRE(to<int>("1234  ") == 1234);
    REQUIRE(to<int>("  1234  ") == 1234);
    REQUIRE(to<int>("001234") == 1234);
    REQUIRE(to<int>("0000") == 0);
}

TEST_CASE("hex string -> int", "[string]") {
    REQUIRE(to<int>("0xFF") == 255);
    REQUIRE(to<int>("0x000000FF") == 255);
}

TEST_CASE("string -> char", "[char]") {
    REQUIRE(to<char>("1") == 1);
    REQUIRE(to<signed char>("1") == 1);
    REQUIRE(to<unsigned char>("1") == 1);

    REQUIRE(to<signed char>("0xFF") == -1);
    REQUIRE(to<unsigned char>("0xFF") == 255);
}

TEST_CASE("string -> bool", "[bool]") {
    REQUIRE(to<bool>("") == false);
    REQUIRE(to<bool>("  ") == true);
    REQUIRE(to<bool>("true") == true);
    REQUIRE(to<bool>("false") == true);
}

TEST_CASE("pair -> string", "[pair]") {
    std::pair<int, int> p(10, 20);
    REQUIRE(to<std::string>(p) == "(10, 20)");
}

TEST_CASE("vector -> string", "[vector]") {
    std::vector<int> v;
    v.push_back(0);
    v.push_back(1);
    v.push_back(2);

    REQUIRE(to<std::string>(v) == "[0, 1, 2]");
}

TEST_CASE("map -> string", "map") {
    std::map<std::string, int> m;
    m["a"] = 0;
    m["b"] = 1;
    m["c"] = 2;

    REQUIRE(to<std::string>(m) == "{a: 0, b: 1, c: 2}");
}

TEST_CASE("parse string", "[string]") {
    typedef std::vector<int> vector_t;

    vector_t v(3);
    v[0] = 0;
    v[1] = 1;
    v[2] = 2;

    REQUIRE(parse<vector_t>("[0,1,2]") == v);
    REQUIRE(parse<vector_t>("[0, 1, 2]") == v);
    REQUIRE(parse<vector_t>("  [0,1,2]  ") == v);
    REQUIRE(parse<vector_t>("[0 1 2]", comma(" ")) == v);
    REQUIRE(parse<vector_t>("  0,1,2]", lbracket("")) == v);
    REQUIRE(parse<vector_t>("[0,1,2  ", rbracket("")) == v);
    REQUIRE(parse<vector_t>("[0[1[2]", comma("[")) == v);
    REQUIRE(parse<vector_t>("[0]1]2]", comma("]")) == v);
}
