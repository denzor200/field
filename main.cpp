// This file is a "Hello, world!" in C++ language by GCC for wandbox.
#include <iostream>
#include <string_view>
#include <cstdlib>
#include <cstring>
#include <boost/metaparse/string.hpp>
#include <boost/pfr/core.hpp>

namespace detail {

template<typename S>
struct named_name_t
{
    using string_type = S;
};

template<typename S>
constexpr auto named_name = named_name_t<S>{};

struct name_t
{
    template<char ... c>
    constexpr const auto& operator= (boost::metaparse::string<c...> s) const noexcept
    {
        using S = decltype(s);
        return named_name<S>;
    }
};

template<typename T, const auto& A>
class field_t
{
    T value;
public:

    // TODO: more user-friendly public interface for field_t??
    // maybe i need to inherit field_t by T..
    constexpr T& data() noexcept { return value; }
    constexpr const T& data() const noexcept { return value; }
    
    static constexpr std::string_view name() noexcept {
        using S = typename std::decay_t<decltype(A)>::string_type;
        const auto& value = boost::mpl::c_str<S>::value;
        return {value, sizeof(value) / sizeof(char) - 1};
    }
};


} // end of namespace detail

constexpr auto name = detail::name_t{};

template<typename T, const auto& A>
using field = detail::field_t<T, A>;





#ifdef _STR
#  error _STR already defined
#endif
#define _STR(S) BOOST_METAPARSE_STRING(S){}

struct gps_position
{
    field<int, (name=_STR("degrees"))> degrees;
    field<int, (name=_STR("minutes"))> minutes;
    field<float, (name=_STR("seconds"))> seconds;
};


int main()
{
    auto pos = gps_position{};
    pos.degrees.data() = 100;
    pos.minutes.data() = 200;
    pos.seconds.data() = 400;

    // TODO: handle collisions in names
    boost::pfr::for_each_field(pos, [](auto&& member) {
        using M = std::decay_t<decltype(member)>;
        std::cout << M::name() << " = " << member.data() << std::endl;
    });

    std::cout << "Hello, Wandbox!" << std::endl;
}

// GCC reference:
//   https://gcc.gnu.org/

// C++ language references:
//   https://cppreference.com/
//   https://isocpp.org/
//   http://www.open-std.org/jtc1/sc22/wg21/

// Boost libraries references:
//   https://www.boost.org/doc/
