#include <iostream>
#include <vector>
#include <algorithm>
#include "lib/zip_iterator.h"

int main() {
    using utils::zip_iterator;

    auto intList = std::vector<int>{1, 2, 3};
    auto charList = std::vector<char>{'a', 'b', 'c'};

    auto zip_itr = utils::make_zip_iterator(intList.cbegin(), charList.cbegin(), intList.cbegin());

    auto zip_end = utils::make_zip_iterator(intList.cend(), charList.cend(), intList.cend());

    std::for_each(zip_itr, zip_end,
            [] (std::tuple<const int&, const char&, const int&> tuple)
            {
                std::cout << std::get<0>(tuple) << std::get<1>(tuple) << std::get<2>(tuple) << std::endl;
            });

//    auto zip_result = *zip_itr;

    std::cout << "Stuff" << std::endl;

}