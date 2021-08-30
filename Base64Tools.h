#ifndef BASE_64_TOOLS_H
#define BASE_64_TOOLS_H

#include "Constants.h"

#include <string>
#include <vector>
#include <utility>


static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

std::string base64_decode(std::string const& encoded_string);

//void printBits(std::string const& string);

void printBits(std::vector<bool> const& vec);

std::vector<bool> getBits(std::string const& string);

std::vector<std::vector<SENSOR>> plotCode_to_sensors(std::string const& base64_string);

#endif //BASE_64_TOOLS_H