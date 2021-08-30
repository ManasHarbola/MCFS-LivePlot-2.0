#include "Base64Tools.h"

//#include <iostream> //remove iostream later

static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

std::string base64_decode(std::string const& encoded_string) {
  int in_len = encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  std::string ret;

  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = base64_chars.find(char_array_4[i]);

      char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

      for (i = 0; (i < 3); i++)
        ret += char_array_3[i];
      i = 0;
    }
  }

  if (i) {
    for (j = i; j <4; j++)
      char_array_4[j] = 0;

    for (j = 0; j <4; j++)
      char_array_4[j] = base64_chars.find(char_array_4[j]);

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

    for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
  }

  return ret;
}

/*
void printBits(std::string const& string) {
    for (char c : string) {
        for (int i = 7; i >= 0; i--) {
            std::cout << (c & (1 << i) ? "1" : "0");
        }
        std::cout << " ";
    }
    std::cout << std::endl;
}
*/
/*
void printBits(std::vector<bool> const& vec) {
    int count = 0;
    for (bool c : vec) {
        std::cout << (c ? "1" : "0");
        count++;
        if (count % 8 == 0) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
}
*/

std::vector<bool> getBits(std::string const& string) {
    std::vector<bool> bitFlags(8 * string.size(), false);
    int bit_idx = bitFlags.size() - 1;
    for (char c : string) {
        for (int j = 7; j >= 0; j--) {
            bitFlags[bit_idx] = ((bool)(c & (1 << j)));
            bit_idx--;
        }
    }
    return bitFlags;
}

std::vector<std::vector<SENSOR>> plotCode_to_sensors(std::string const& base64_string) {
    std::string decoded_b64 = base64_decode(base64_string);
    std::vector<bool> bitFlags = getBits(decoded_b64);

    //first -> AVSEN sensors, second -> PROPSEN sensors
    std::vector<std::vector<SENSOR>> avsen_propsen_pair;
    avsen_propsen_pair.push_back(std::vector<SENSOR>());
    avsen_propsen_pair.push_back(std::vector<SENSOR>());

    //currently, 22 is the first PROPSEN bitflag position
    int lastAvsenBitFlag = 22;

    for (int i = 0; i < bitFlags.size(); i++) {
        if (bitFlags[i]) {
            if (i <= lastAvsenBitFlag) {
                avsen_propsen_pair[0].push_back(bitflagToAVSEN.at(i));
            } else {
                avsen_propsen_pair[1].push_back(bitflagToPROPSEN.at(i));
            }
        }
    }

    return avsen_propsen_pair;
}

/*
int main() {
    std::string test = "AIAEAADwAgc=";
    std::string decoded_test = base64_decode(test);
    std::vector<bool> bitflags = getBits(decoded_test);
    printBits(bitflags);

    return 0;
}
*/