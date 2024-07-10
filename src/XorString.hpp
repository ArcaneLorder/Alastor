#include <array>
#include <string>
#include <utility>

namespace Alastor
{
    constexpr std::size_t compSeed {
        (__TIME__[7] - '0') * 1U
            + (__TIME__[6] - '0') * 10U
            + (__TIME__[4] - '0') * 60U
            + (__TIME__[3] - '0') * 600U
            + (__TIME__[1] - '0') * 3600U
            + (__TIME__[0] - '0') * 36000U };

    constexpr std::size_t linCongrGen(std::size_t rou)
    {
        return 1013904223U + 1664525U * ((rou > 0U)
            ? linCongrGen(rou - 1U)
            : compSeed & 0xFFFFFFFFU);
    }

    constexpr std::size_t randNumber(std::size_t mn, std::size_t mx)
    {
        return mn + (linCongrGen(10U) % (mx - mn + 1U));
    }

    template <typename, typename>
    class XorStringT;

    template <typename Char, std::size_t ... Is>
    class XorStringT<Char, std::index_sequence<Is...>>
    {
    private:
        Char Value[sizeof...(Is) + 1U];

        static constexpr Char XORKEY = Char(randNumber(0, 0xFFFF));

    public:
        constexpr XorStringT(Char const* const String)
            : Value{ Char(String[Is] ^ (XORKEY + Is))... }
        { }

        constexpr std::array<Char, sizeof...(Is) + 1U> decrypt() const
        {
            return { { Char(Value[Is] ^ (XORKEY + Is)) ..., Char(0) } };
        }

        Char const* get() const
        {
            return Value;
        }
    };

    template <typename T, std::size_t N>
    constexpr auto _xorstr(T(&s)[N])
    {
        return XorStringT < T,
            decltype(std::make_index_sequence<N - 1U>{}) > (s);
    }

}
#define xorstr(str) Alastor::_xorstr(str).decrypt()
