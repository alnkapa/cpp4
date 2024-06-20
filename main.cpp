/*! \mainpage print ip
 *
 * study test execution
 */

#include <array>
#include <cstdint>
#include <iostream>
#include <list>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>
/**
 * \brief  print_ip implementation
 */
namespace impl {
    /**
     * \brief check method for c_str()
     */
    namespace c_str {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
        template <typename T>
        static auto check(int) -> decltype(std::declval<T>().c_str(), std::true_type());
        template <typename>
        static std::false_type check(...);
        template <typename U>
        struct has {
            static constexpr bool value = decltype(check<U>(int{}))::value;
        };
#endif /* DOXYGEN_SHOULD_SKIP_THIS */
    }  // namespace c_str

    /**
     * \brief check for operator <<
     */
    namespace operator_shift {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
        template <typename T>
        static auto check(int) -> decltype(std::declval<std::ostream &>() << std::declval<T>(), std::true_type());
        template <typename>
        static std::false_type check(...);
        template <typename U>
        struct has {
            static constexpr bool value = decltype(check<U>(int{}))::value;
        };
#endif
    }  // namespace operator_shift
    /**
     * \brief check method for begin() and end()
     */
    namespace begin_end {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
        template <typename T>
        static auto check(int) -> decltype(std::declval<T>().begin(), std::declval<T>().end(), std::true_type());

        template <typename>
        static std::false_type check(...);

        template <typename U>
        struct has {
            static constexpr bool value = decltype(check<U>(int{}))::value;
        };
#endif
    }  // namespace begin_end
    /**
     * \brief check for tuple
     */
    namespace tuple {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
        template <typename T1>
        constexpr bool has() {
            return true;
        }
        /**
         *  \brief compare type
         */
        template <typename T1, typename T2, typename... Other>
        constexpr bool has() {
            if (!std::is_same<T1, T2>::value) {
                return false;
            }
            if (sizeof...(Other) != 0) {
                return has<T2, Other...>();
            } else {
                return true;
            }
        };
#endif
    }  // namespace tuple

    /**
     * \brief try tuple or give up
     */
    namespace iteration4 {
#ifndef DOXYGEN_SHOULD_SKIP_THIS
        template <typename T, std::size_t... N>
        constexpr void print_tuple(const T &in, std::index_sequence<N...>) {
            (..., (std::cout << (N == 0 ? "" : ".") << std::get<N>(in)));
        }
#endif
        /**
         * \brief display tuple
         */
        template <typename... Types>
        constexpr void print_ip(const std::tuple<Types...> &in) {
            static_assert(tuple::has<Types...>() == true, "all type in tuple mast be same");
            print_tuple(in, std::make_index_sequence<sizeof...(Types)>());
            std::cout << std::endl;
        }
        /**
         * \brief give up
         */
        template <typename T>
        constexpr void print_ip(const T &in) {
            std::cout << "undefined" << std::endl;
        }
    }  // namespace iteration4
    /**
     * \brief try any container or go to iteration4
     */
    namespace iteration3 {
        /**
         * \brief display any container
         */
        template <typename T, typename = std::enable_if_t<begin_end::has<T>::value, bool>>
        constexpr void print_ip(const T &in, int) {
            bool is_point = false;
            for (auto it = in.begin(); it != in.end(); it++) {
                if (is_point) {
                    std::cout << ".";
                } else {
                    is_point = true;
                }
                std::cout << (*it);
            }
            std::cout << std::endl;
        }
        /**
         * \brief go to iteration4
         */
        template <typename T>
        constexpr void print_ip(const T &in, long) {
            iteration4::print_ip(in);
        }
    }  // namespace iteration3
    /**
     * \brief try string or go to iteration3
     */
    namespace iteration2 {
        /**
         * \brief display string
         */
        template <typename T, typename = std::enable_if_t<c_str::has<T>::value, bool>>
        constexpr void print_ip(const T &in, int) {
            std::cout << in << std::endl;
        }
        /**
         * \brief go to iteration3
         */
        template <typename T>
        constexpr void print_ip(const T &in, long) {
            iteration3::print_ip(in, int{});
        }
    }  // namespace iteration2
    /**
     * \brief try integral number or go to iteration2
     */
    namespace iteration1 {
        /**
         *  \brief display integral number
         */
        template <typename T, typename = std::enable_if_t<std::is_integral_v<T>, bool>>
        constexpr void print_ip(const T &in, int) {
            union {
                T in;
                std::uint8_t arr[sizeof(T)];
            } un;
            un.in = in;
            bool is_point = false;
            for (int i = sizeof(T) - 1; i > -1; i--) {
                if (is_point) {
                    std::cout << ".";
                }
                is_point = true;
                std::cout << int(un.arr[i]);
            }
            std::cout << std::endl;
        }
        /**
         * \brief go to iteration2
         */
        template <typename T>
        constexpr void print_ip(const T &in, long) {
            iteration2::print_ip(in, int{});
        }
    }  // namespace iteration1
}  // namespace impl

/**
 * \brief pint ip address
 * \param[in] ip address in format as describe in task.txt
 */
template <typename T>
constexpr void print_ip(const T &in) {
    impl::iteration1::print_ip(in, int{});
}

int main() {
    print_ip(int8_t{-1});                            // 255
    print_ip(int16_t{0});                            // 0.0
    print_ip(int32_t{2130706433});                   // 127.0.0.1
    print_ip(int64_t{8875824491850138409});          // 123.45.67.89.101.112.131.41
    print_ip(std::string{"Hello, World !"});         // Hello, World!
    print_ip(std::vector<int>{100, 200, 300, 400});  // 100.200.300.400
    print_ip(std::list<short>{400, 300, 200, 100});  // 400.300.200.100
    print_ip(std::make_tuple(123, 456, 789, 0));     // 123.456.789.0
    // print_ip(std::make_tuple(123, 456, 789, "привет")); // ошибка
    return 0;
}