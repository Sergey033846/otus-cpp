#include <iostream>
#include <vector>
#include <list>
#include <tuple>
#include <type_traits>
#include <string>
#include <cstring>

/** \mainpage Main Page Text
Some main page Text!!!
*/

/**
    @brief Partial specialization функции print_ip для целочисленного типа.
    
    Разбивает заданное число на байты и выводит их от старшего к младшему.
    Использует в качестве разделителя символ "."
    Представляет собой одну функцию.
      
    @param[in] value исходное число    
 */
template<typename T>
typename std::enable_if<std::is_integral<T>::value>::type print_ip(const T& value) {    
    uint8_t bytes[sizeof(T)];
    std::memcpy(bytes, &value, sizeof(T));    
    bool is_first = true;
    for (int i = sizeof(T) - 1; i >= 0; --i) {        
        std::cout << (is_first ? "" : ".") << static_cast<int>(bytes[i]);
        if (is_first) is_first = false;
    }
    std::cout << std::endl;
}


/**
    @brief Partial specialization функции print_ip для строки.
    
    Выводит строку на экран "как есть".
          
    @param[in] value исходная строка    
 */
template<typename T>
typename std::enable_if<std::is_same<T, std::string>::value>::type print_ip(const T& value) {
    std::cout << value << std::endl;
}


/**
    @brief Partial specialization функции print_ip для контейнера std::vector или std::list.

    Выводит элементы контейнера на экран, используя в качестве разделителя символ "."
    Представляет собой одну функцию.
      
    @param[in] container исходный контейнер std::vector или std::list    
 */
template<typename T> 
typename std::enable_if<
                std::is_same<T, std::vector<typename T::value_type>>::value || 
                std::is_same<T, std::list<typename T::value_type>>::value>::type print_ip(const T& container) {
    bool is_first = true;
    for (const auto& item : container) {
        std::cout << (is_first ? "" : ".") << item;
        if (is_first) is_first = false;
    }
    std::cout << std:: endl;
}


/**
    @brief Partial specialization функции print_ip для кортежа std::tuple.

    Выводит элементы кортежа на экран, используя в качестве разделителя символ "."
    Элементы выводятся "как есть".
    Все типы элементов кортежа должны быть одинаковы между собой.
    Представляет собой одну функцию.
      
    @param[in] tuple исходный кортеж std::tuple    
 */
template<typename... Types>
typename std::enable_if<(std::is_same<Types, std::tuple_element_t<0, std::tuple<Types...>>>::value && ...), void>::type
print_ip(const std::tuple<Types...>& tuple) {
    std::apply(
        [](const auto&... items){        
        bool is_first = true;        
        ((std::cout << (is_first ? "" : ".") << items, is_first = false), ...);
        }, 
        tuple);
    std::cout << std::endl;
}


int main() {    
    print_ip( int8_t{-1} );                            // 255
    print_ip( int16_t{0} );                            // 0.0
    print_ip( int32_t{2130706433} );                   // 127.0.0.1
    print_ip( int64_t{8875824491850138409} );          // 123.45.67.89.101.112.131.41
    print_ip( std::string{"Hello, World!"} );          // Hello, World!
    print_ip( std::vector<int>{100, 200, 300, 400} );  // 100.200.300.400
    print_ip( std::list<short>{400, 300, 200, 100} );  // 400.300.200.100
    print_ip( std::make_tuple(123, 456, 789, 0) );     // 123.456.789.0
    //print_ip( std::make_tuple(123, 4.56, 789, 0) );  // Ошибка компиляции
    
    return 0;
}
