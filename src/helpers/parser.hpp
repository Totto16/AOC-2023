#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wparentheses"
#endif

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-local-typedefs"
#pragma clang diagnostic ignored "-Wparentheses"
#endif

#include <lexy/action/parse.hpp>     // lexy::parse
#include <lexy/action/trace.hpp>     // lexy::trace
#include <lexy/encoding.hpp>         // lexy::utf8_encoding
#include <lexy/input/file.hpp>       //
#include <lexy_ext/report_error.hpp> // lexy_ext::report_error

#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#ifdef __clang__
#pragma clang diagnostic pop
#endif

// TODO
