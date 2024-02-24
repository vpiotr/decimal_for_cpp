//
// Created by piotr on 2/24/24.
//

#include <string>
#include <sstream>

std::string uint_to_string(unsigned int arg) {
    std::ostringstream out;
    out << arg;
    return (out.str());
}

