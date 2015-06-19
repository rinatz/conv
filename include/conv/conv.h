/**
 * @file
 *
 * Utility functions to convert among integers, character strings
 * and other objects for C++.
 * It is inspired std.conv module of D programming language.
 *
 * conv::to<T>() converts an integer, floating point, boolean value
 * and character strings to the type of T:
 *
 *      conv::to<char>(10);               // == static_cast<char>(10)
 *      conv::to<std::string>(10);        // "10"
 *      conv::to<std::string>(3.14);      // "3.14"
 *      conv::to<std::string>(true);      // "true"
 *      conv::to<int>("00001234");        // 1234
 *      conv::to<int>("0xFF");            // 255
 *      conv::to<std::wstring>("Hello");  // L"Hello"
 *
 * Note that the type of char is treated not as a character string
 * but as an integer.
 * i.e., conv::to<char>("1") is not 49, which is the ascii code of '1',
 * but 1, which is an integer.
 *
 * STL containers can be converted as well:
 *
 *      std::pair<int, int> p(10, 20);
 *      std::string pstring = conv::to<std::string>(p);  // "(10, 20)"
 *
 *      std::vector<int> v;
 *      v.push_back(0);
 *      v.push_back(1);
 *      v.push_back(2);
 *      std::string vstring = conv::to<std::string>(v);  // "[0, 1, 2]"
 *
 *      std::map<std::string, int> m;
 *      m["a"] = 0;
 *      m["b"] = 1;
 *      m["c"] = 2;
 *      std::string mstring = conv::to<std::string>(m);  // "{a: 0, b: 1, c: 2}"
 *
 * but it now only supports std::pair, std::vector and std::map.
 *
 * When you want to convert user defined type of T to string,
 * you should define operator<<(std::ostream&, T).
 * And conv::to<std::string>(T) will be enabled.
 * (conv::to<std::string>() for STL containers are like so.)
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Kenichiro IDA
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#ifndef CONV_CONV_H_
#define CONV_CONV_H_

#include <cassert>
#include <cstdlib>

#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace conv {

const std::string& version() {
    static std::string ver("0.3.0");
    return ver;
}

//-----------------------------------------------------------------------------

namespace internal {

template <typename CharT>
const std::basic_string<CharT>& space();

template <>
inline const std::string& space<char>() {
    static const std::string s(" \t\v\r\n");
    return s;
}

template <>
inline const std::wstring& space<wchar_t>() {
    static const std::wstring s(L" \t\v\r\n");
    return s;
}

//-----------------------------------------------------------------------------

template <typename CharT>
const std::basic_string<CharT>& hex();

template <>
inline const std::string& hex<char>() {
    static const std::string s("0x");
    return s;
}

template <>
inline const std::wstring& hex<wchar_t>() {
    static const std::wstring s(L"0x");
    return s;
}

}  // namespace internal

//-----------------------------------------------------------------------------

template <typename T>
class to {
   public:
    template <typename U>
    explicit to(const U& value) {
        value_ = static_cast<T>(value);
    }

    explicit to(const char* str) { from_string<char>(str); }
    explicit to(const std::string& str) { from_string<char>(str); }

    explicit to(const wchar_t* str) { from_string<wchar_t>(str); }
    explicit to(const std::wstring& str) { from_string<wchar_t>(str); }

    operator T() const { return value_; }

   private:
    template <typename CharT>
    void from_string(const std::basic_string<CharT>& str) {
        typedef std::basic_string<CharT> string_t;
        typedef std::basic_istringstream<CharT> isstream_t;

        static const string_t& space = internal::space<CharT>();
        static const string_t& hex = internal::hex<CharT>();

        size_t first = str.find_first_not_of(space);
        assert(first != string_t::npos);

        size_t last = str.find_last_not_of(space);
        assert(last != string_t::npos);

        string_t trimmed = str.substr(first, last - first + 1);
        assert(trimmed.find_first_of(space) == string_t::npos);

        isstream_t in(trimmed);

        if (in.str().size() >= 2 && in.str().substr(0, 2) == hex) {
            in >> std::hex >> value_;
        } else {
            in >> value_;
        }
    }

    T value_;
};

//-----------------------------------------------------------------------------

template <>
class to<char> {
   public:
    template <typename U>
    explicit to(const U& value) {
        value_ = static_cast<char>(value);
    }

    explicit to(const char* str) {
        value_ = static_cast<char>(to<int>(str));
    }

    explicit to(const std::string& str) {
        value_ = static_cast<char>(to<int>(str));
    }

    explicit to(const wchar_t* wstr) {
        value_ = static_cast<char>(to<int>(wstr));
    }

    explicit to(const std::wstring& wstr) {
        value_ = static_cast<char>(to<int>(wstr));
    }

    operator char() const { return value_; }

   private:
    char value_;
};

//-----------------------------------------------------------------------------

template <>
class to<signed char> {
   public:
    template <typename U>
    explicit to(const U& value) {
        value_ = static_cast<char>(value);
    }

    explicit to(const char* str) {
        value_ = static_cast<signed char>(to<int>(str));
    }

    explicit to(const std::string& str) {
        value_ = static_cast<signed char>(to<int>(str));
    }

    explicit to(const wchar_t* wstr) {
        value_ = static_cast<signed char>(to<int>(wstr));
    }

    explicit to(const std::wstring& wstr) {
        value_ = static_cast<signed char>(to<int>(wstr));
    }

    operator signed char() const { return value_; }

   private:
    signed char value_;
};

//-----------------------------------------------------------------------------

template <>
class to<unsigned char> {
   public:
    template <typename U>
    explicit to(const U& value) {
        value_ = static_cast<char>(value);
    }

    explicit to(const char* str) {
        value_ = static_cast<unsigned char>(to<int>(str));
    }

    explicit to(const std::string& str) {
        value_ = static_cast<unsigned char>(to<int>(str));
    }

    explicit to(const wchar_t* str) {
        value_ = static_cast<unsigned char>(to<int>(str));
    }

    explicit to(const std::wstring& wstr) {
        value_ = static_cast<unsigned char>(to<int>(wstr));
    }

    operator unsigned char() const { return value_; }

   private:
    unsigned char value_;
};

//-----------------------------------------------------------------------------

template <>
class to<bool> {
   public:
    template <typename T>
    explicit to(const T& value) {
        value_ = static_cast<T>(value);
    }

    explicit to(const char* str) { value_ = !std::string(str).empty(); }
    explicit to(const std::string& str) { value_ = !str.empty(); }

    explicit to(const wchar_t* wstr) { value_ = !std::wstring(wstr).empty(); }
    explicit to(const std::wstring& wstr) { value_ = !wstr.empty(); }

    operator bool() const { return value_; }

   private:
    bool value_;
};

//-----------------------------------------------------------------------------

template <>
class to<std::string> : public std::string {
   public:
    template <typename U>
    explicit to(const U& value) {
        std::ostringstream out;
        out << value;
        std::string::operator=(out.str());
    }

    explicit to(char value) { from_int8(value); }
    explicit to(signed char value) { from_int8(value); }
    explicit to(unsigned char value) { from_int8(value); }

    explicit to(bool value) {
        value ? std::string::operator=("true")
              : std::string::operator=("false");
    }

    explicit to(const char* str) : std::string(str) {}
    explicit to(const std::string& str) : std::string(str) {}

    explicit to(const wchar_t* wstr) { from_wstring(wstr); }
    explicit to(const std::wstring& wstr) { from_wstring(wstr); }

   private:
    void from_int8(int value) {
        std::ostringstream out;
        out << value;
        std::string::operator=(out.str());
    }

    void from_wstring(const std::wstring& wstr) {
        std::vector<char> mbs(wstr.size() * MB_CUR_MAX + 1);
        std::wcstombs(mbs.data(), wstr.data(), mbs.size());
        std::string::operator=(mbs.data());
    }
};

//-----------------------------------------------------------------------------

template <>
class to<std::wstring> : public std::wstring {
   public:
    template <typename U>
    explicit to(const U& value) {
        std::wostringstream out;
        out << value;
        std::wstring::operator=(out.str());
    }

    explicit to(char value) { from_int8(value); }
    explicit to(signed char value) { from_int8(value); }
    explicit to(unsigned char value) { from_int8(value); }

    explicit to(bool value) {
        value ? std::wstring::operator=(L"true")
              : std::wstring::operator=(L"false");
    }

    explicit to(const char* str) { from_string(str); }
    explicit to(const std::string& str) { from_string(str); }

    explicit to(const wchar_t* wstr) : std::wstring(wstr) {}
    explicit to(const std::wstring& wstr) : std::wstring(wstr) {}

   private:
    void from_int8(int value) {
        std::wostringstream out;
        out << value;
        std::wstring::operator=(out.str());
    }

    void from_string(const std::string& str) {
        std::vector<wchar_t> wcs(str.size() + 1);
        std::mbstowcs(wcs.data(), str.data(), wcs.size());
        std::wstring::operator=(wcs.data());
    }
};

//-----------------------------------------------------------------------------

template <typename T1, typename T2>
class to<std::pair<T1, T2> > : public std::pair<T1, T2> {
   public:
    template <typename U1, typename U2>
    explicit to(const std::pair<U1, U2>& p) {
        std::pair<T1, T2>::first = to<T1>(p.first);
        std::pair<T1, T2>::second = to<T2>(p.second);
    }
};

//-----------------------------------------------------------------------------

template <typename T>
class to<std::vector<T> > : public std::vector<T> {
   public:
    template <typename U>
    explicit to(const std::vector<U>& v)
        : std::vector<T>(v.size()) {
        for (size_t i = 0; i < v.size(); ++i) {
            std::vector<T>::operator[](i) = to<T>(v[i]);
        }
    }
};

//-----------------------------------------------------------------------------

template <typename K1, typename V1>
class to<std::map<K1, V1> > : public std::map<K1, V1> {
   public:
    template <typename K2, typename V2>
    explicit to(const std::map<K2, V2>& m) {
        typedef typename std::map<K2, V2>::const_iterator iterator;

        for (iterator iter = m.begin(); iter != m.end(); ++iter) {
            std::map<K1, V1>::operator[](to<K1>(iter->first)) =
                to<V1>(iter->second);
        }
    }
};

//-----------------------------------------------------------------------------

class parse_options {
   public:
       parse_options() : lbracket_("["), rbracket_("]"), comma_(",") {}

       parse_options& lbracket(const std::string& s) {
           lbracket_ = s;
           return *this;
       }

       const std::string& lbracket() const { return lbracket_; }

       parse_options& rbracket(const std::string& s) {
           rbracket_ = s;
           return *this;
       }

       const std::string& rbracket() const { return rbracket_; }

       parse_options& comma(const std::string& s) {
           comma_ = s;
           return *this;
       }

       const std::string& comma() const { return comma_; }

   private:
       std::string lbracket_;
       std::string rbracket_;
       std::string comma_;
};

inline parse_options lbracket(const std::string& s) {
    return parse_options().lbracket(s);
}

inline parse_options rbracket(const std::string& s) {
    return parse_options().rbracket(s);
}

inline parse_options comma(const std::string& s) {
    return parse_options().comma(s);
}

template <typename VecT>
inline VecT parse(const std::string& str, const parse_options& opt = parse_options()) {
    typedef char char_t;
    typedef std::basic_string<char_t> string_t;
    typedef typename VecT::value_type value_t;

    static const string_t& space = internal::space<char_t>();

    VecT vec;

    size_t first = str.find_first_not_of(space);
    assert(first != string_t::npos);

    if (!opt.lbracket().empty()) {
        assert(str[first] == opt.lbracket()[0]);
        first = str.find_first_not_of(space, first + 1);
    }

    size_t last = str.find_last_not_of(space);
    assert(last != string_t::npos);

    if (!opt.rbracket().empty()) {
        assert(str[last] == opt.rbracket()[0]);
        last = str.find_last_not_of(space, last - 1);
    }

    assert(first < last);

    std::string fields = str.substr(first, last - first + 1);
    first = 0;
    size_t pos = 0;

    while ((pos = fields.find_first_of(opt.comma(), first)) != string_t::npos) {
        assert(pos > first);

        value_t v = to<value_t>(fields.substr(first, pos - first));
        vec.push_back(v);
        first = pos + 1;
    }

    value_t v = to<value_t>(fields.substr(first, pos - first));
    vec.push_back(v);

    return vec;
}

//-----------------------------------------------------------------------------

template <typename T>
inline std::ostream& operator<<(std::ostream& out, const to<T>& value) {
    out << static_cast<T>(value);
    return out;
}

template <typename T>
inline std::wostream& operator<<(std::wostream& out, const to<T>& value) {
    out << static_cast<T>(value);
    return out;
}

//-----------------------------------------------------------------------------

template <typename T1, typename T2>
inline std::ostream& operator<<(std::ostream& out, const std::pair<T1, T2>& p) {
    out << "(" << p.first << ", " << p.second << ")";
    return out;
}

template <typename T1, typename T2>
inline std::wostream& operator<<(std::wostream& out,
                                 const std::pair<T1, T2>& p) {
    out << L"(" << p.first << L", " << p.second << L")";
    return out;
}

//-----------------------------------------------------------------------------

template <typename T>
inline std::ostream& operator<<(std::ostream& out, const std::vector<T>& v) {
    if (v.empty()) {
        return out;
    }

    typedef typename std::vector<T>::const_iterator iterator;

    out << "[";

    iterator iter = v.begin();
    out << *iter;
    ++iter;

    for (/* NOP */; iter != v.end(); ++iter) {
        out << ", " << *iter;
    }

    out << "]";

    return out;
}

template <typename T>
inline std::wostream& operator<<(std::wostream& out, const std::vector<T>& v) {
    if (v.empty()) {
        return out;
    }

    typedef typename std::vector<T>::const_iterator iterator;

    out << L"[";

    iterator iter = v.begin();
    out << *iter;
    ++iter;

    for (/* NOP */; iter != v.end(); ++iter) {
        out << L", " << *iter;
    }

    out << L"]";

    return out;
}

//-----------------------------------------------------------------------------

template <typename K, typename V>
inline std::ostream& operator<<(std::ostream& out, const std::map<K, V>& m) {
    if (m.empty()) {
        return out;
    }

    typedef typename std::map<K, V>::const_iterator iterator;

    out << "{";

    iterator iter = m.begin();
    out << iter->first << ": " << iter->second;
    ++iter;

    for (/* NOP */; iter != m.end(); ++iter) {
        out << ", " << iter->first << ": " << iter->second;
    }

    out << "}";

    return out;
}

template <typename K, typename V>
inline std::wostream& operator<<(std::wostream& out, const std::map<K, V>& m) {
    if (m.empty()) {
        return out;
    }

    typedef typename std::map<K, V>::const_iterator iterator;

    out << L"{";

    iterator iter = m.begin();
    out << iter->first << L": " << iter->second;
    ++iter;

    for (/* NOP */; iter != m.end(); ++iter) {
        out << L", " << iter->first << L": " << iter->second;
    }

    out << L"}";

    return out;
}

}  // namespace conv

#endif  // CONV_CONV_H_
