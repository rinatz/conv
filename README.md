# conv

conv is a utility function to convert integers, character strings and other objects for C++.
It is inspired by std.conv module for D programming language.

## Install

conv is a header only library and you just copy include/conv to your project directory.

## Examples

conv::to&lt;T&gt;() converts an integer, floating point, boolean value
and character strings to the type of T.

    conv::to<char>(10);               // == static_cast<char>(10)
    conv::to<std::string>(10);        // "10"
    conv::to<std::string>(3.14);      // "3.14"
    conv::to<std::string>(true);      // "true"
    conv::to<int>("00001234");        // 1234
    conv::to<int>("0xFF");            // 255
    conv::to<std::wstring>("Hello");  // L"Hello"

STL containers can be converted as well.

    std::pair<int, int> p(10, 20);
    std::string pstring = conv::to<std::string>(p);  // "(10, 20)"

    std::vector<int> v;
    v.push_back(0);
    v.push_back(1);
    v.push_back(2);
    std::string vstring = conv::to<std::string>(v);  // "[0, 1, 2]"

    std::map<std::string, int> m;
    m["a"] = 0;
    m["b"] = 1;
    m["c"] = 2;
    std::string mstring = conv::to<std::string>(m);  // "{a: 0, b: 1, c: 2}"

but it now only supports std::pair, std::vector and std::map.

When converting strings to STL containers, you can use the function of parse().

    std::vector<int> v = conv::parse<std::vector<int> >("[0, 1, 2]");

Left and right brackets and comma can be changed to other strings.

    std::vector<int> v = conv::parse<std::vector<int> >("<0|1|2>", conv::lbracket("<").rbracket(">").comma("|"));

## Note

Note that the type of char is treated not as a character string but as an integer.
i.e., conv::to&lt;char&gt;("1") is not 49, which is the ascii code for '1', but 1, which is an integer.

## For user defined types

When you want to convert user defined type of T to string, you should define operator&lt;&lt;(std::ostream&, T).
And conv::to&lt;std::string&gt;(T) will be enabled.
(conv::to&lt;std::string&gt;() for STL containers are like so.)

## Version

0.3.2

## Compilers

conv has tested on Visual C++ 2013 and maybe it works for other compilers as well.
