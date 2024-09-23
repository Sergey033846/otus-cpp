#include <algorithm>
#include <iostream>
#include <string>
#include <vector>


const int FILTER_2_1ST_OCT = 1;
const int FILTER_3_1ST_OCT = 46;        
const int FILTER_3_2ND_OCT = 70;        
const int FILTER_4_ANY_OCT = 46;


/**
    @brief Функция, разбивающая строку в соответствии с заданным разделителем
  
    Разделяет строку в соответствии с заданным разделителем. Возвращает вектор строк.
    Взята из приложенного к ДЗ шаблона без изменений.
  
    @param[in] str исходная строка
    @param[in] d символ-разделитель
    @return вектор строк или пустой вектор
 */
auto split(const std::string &str, char d)
{
    std::vector<std::string> r;

    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));        

        start = stop + 1;
        stop = str.find_first_of(d, start);
    }

    r.push_back(str.substr(start));

    return r;
}


/**
    @brief Функция, печати списка IP-адресов в соответствии с условием
  
    Выводит в стандартный поток вывода список IP-адресов в соответствии с заданным условием фильтрации.
    Взята из приложенного к ДЗ шаблона с добавлением условия отбора.
  
    @param[in] ip_pool исходный двумерный вектор IP-адресов
    @param[in] filter_func фильтр-функция, применяемая к каждому элементу списка    
 */
void print_ip_pool(const std::vector<std::vector<int>> &ip_pool, bool(*filter_func)(const std::vector<int>&)) {    
    for(auto ip = ip_pool.cbegin(); ip != ip_pool.cend(); ++ip)
    {        
        if(filter_func(*ip))
        {
            for(auto ip_part = ip->cbegin(); ip_part != ip->cend(); ++ip_part)
            {
                if(ip_part != ip->cbegin())
                {
                    std::cout << ".";
                }
                std::cout << *ip_part;
            }
            std::cout << std::endl;
        }
    } 
}


int main(int, const char *[])
{    
    try
    {   
        std::vector<std::vector<int>> ip_pool;

        for(std::string line; std::getline(std::cin, line);)
        {
            /// получаем IP-адрес в виде вектора строк
            auto ip_octets_str = split(split(line, '\t').at(0), '.');   

            /// преобразуем каждый октет IP-адреса в число
            std::vector<int> ip_octets;  
            for(const auto &ip_octet : ip_octets_str)
            {
                ip_octets.push_back(std::stoi(ip_octet));
            }            

            ip_pool.push_back(ip_octets); 
        }

        // обратная лексикографическая сортировка        
        std::sort(ip_pool.rbegin(), ip_pool.rend());
        
        // вывод без фильтра
        print_ip_pool(ip_pool, [](const std::vector<int> &){ return true; });        

        // вывод с фильтром значения первого октета
        print_ip_pool(ip_pool, [](const std::vector<int> &ip){ return ip.at(0) == FILTER_2_1ST_OCT; });        
        
        // вывод с фильтром значений первого и второго октета
        print_ip_pool(ip_pool, [](const std::vector<int> &ip){ return ip.at(0) == FILTER_3_1ST_OCT && ip.at(1) == FILTER_3_2ND_OCT; });        
        
        // вывод с фильтром значений по любому из октетов
        print_ip_pool(ip_pool, 
            [](const std::vector<int> &ip){ return 
                ip.at(0) == FILTER_4_ANY_OCT || ip.at(1) == FILTER_4_ANY_OCT || 
                ip.at(2) == FILTER_4_ANY_OCT || ip.at(3) == FILTER_4_ANY_OCT; });                        
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    
    return 0;
}
