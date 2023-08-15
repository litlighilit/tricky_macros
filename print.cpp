static_assert(__cplusplus>=202002, "requires at least std c++20"); 
#include<array>
#include<format>
#include<iostream>
template <size_t _Cnt>
consteval auto make_format_string_placeholders() -> std::array<char, _Cnt * 2>
{
    return []<size_t... _Idx>(std::integer_sequence<size_t, _Idx...> &&) {
        return std::array{ (_Idx % 2 == 0 ? '{' : '}')... };
    }(std::make_index_sequence<_Cnt * 2>{});
}

template <size_t _Cnt>
struct format_string_placeholders {
    static constexpr auto arr = make_format_string_placeholders<_Cnt>();
    static constexpr auto str/*: const std::string_view */= std::string_view{ std::data(arr), _Cnt * 2 };
};

void print(const auto &...args)
{
    std::cout << std::format(
        format_string_placeholders<sizeof...(args)>::str,
        args...
    ) << std::endl;
}
/*
int main(){
    print(1,'d',"str");
}
*/