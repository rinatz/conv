#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <map>
#include <string>
#include <utility>
#include <vector>

#include <conv/conv.h>

TEST_CASE("string -> int", "[string]") {
    REQUIRE(conv::to<int>("1234") == 1234);
    REQUIRE(conv::to<int>("  1234") == 1234);
    REQUIRE(conv::to<int>("1234  ") == 1234);
    REQUIRE(conv::to<int>("  1234  ") == 1234);
    REQUIRE(conv::to<int>("001234") == 1234);
    REQUIRE(conv::to<int>("0000") == 0);
}

TEST_CASE("hex string -> int", "[string]") {
    REQUIRE(conv::to<int>("0xFF") == 255);
    REQUIRE(conv::to<int>("0x000000FF") == 255);
}

TEST_CASE("string -> char", "[char]") {
    REQUIRE(conv::to<char>("1") == 1);
    REQUIRE(conv::to<signed char>("1") == 1);
    REQUIRE(conv::to<unsigned char>("1") == 1);

    REQUIRE(conv::to<signed char>("0xFF") == -1);
    REQUIRE(conv::to<unsigned char>("0xFF") == 255);
}

TEST_CASE("string -> bool", "[bool]") {
    REQUIRE(conv::to<bool>("") == false);
    REQUIRE(conv::to<bool>("  ") == true);
    REQUIRE(conv::to<bool>("true") == true);
    REQUIRE(conv::to<bool>("false") == true);
}

TEST_CASE("pair -> string", "[pair]") {
    std::pair<int, int> p(10, 20);
    REQUIRE(conv::to<std::string>(p) == "(10, 20)");
}

TEST_CASE("vector -> string", "[vector]") {
    std::vector<int> v;
    v.push_back(0);
    v.push_back(1);
    v.push_back(2);

    REQUIRE(conv::to<std::string>(v) == "[0, 1, 2]");
}

TEST_CASE("map -> string", "map") {
    std::map<std::string, int> m;
    m["a"] = 0;
    m["b"] = 1;
    m["c"] = 2;

    REQUIRE(conv::to<std::string>(m) == "{a: 0, b: 1, c: 2}");
}

TEST_CASE("parse string", "[string]") {
    std::vector<int> v(3);
    v[0] = 0;
    v[1] = 1;
    v[2] = 2;

    REQUIRE(conv::parse<std::vector<int> >("[0,1,2]") == v);
    REQUIRE(conv::parse<std::vector<int> >("[0, 1, 2]") == v);
    REQUIRE(conv::parse<std::vector<int> >("  [0,1,2]  ") == v);
    REQUIRE(conv::parse<std::vector<int> >("[0 1 2]", conv::comma(" ")) == v);
    REQUIRE(conv::parse<std::vector<int> >("  0,1,2]", conv::lbracket("")) == v);
    REQUIRE(conv::parse<std::vector<int> >("[0,1,2  ", conv::rbracket("")) == v);
    REQUIRE(conv::parse<std::vector<int> >("[0[1[2]", conv::comma("[")) == v);
    REQUIRE(conv::parse<std::vector<int> >("[0]1]2]", conv::comma("]")) == v);
}
