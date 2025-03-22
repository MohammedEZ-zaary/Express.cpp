#include "../../include/core/helpFunctions.hpp"
#include <random>

std::string stringUtils::generateRandomString(size_t length) {
  const std::string characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                 "abcdefghijklmnopqrstuvwxyz"
                                 "0123456789";

  std::random_device rd;        // Seed generator
  std::mt19937 generator(rd()); // Random number generator
  std::uniform_int_distribution<> distribution(0, characters.size() - 1);

  std::string randomString;
  for (size_t i = 0; i < length; ++i) {
    randomString += characters[distribution(generator)];
  }

  return randomString;
};

unsigned long long
convertingTypes::convertStringToULongInt(std::string &stringNumber) {
  try {
    unsigned long long number = std::stoull(stringNumber);
    return number;
  } catch (const std::invalid_argument &e) {
    std::cout << "Invalid argument: " << e.what() << std::endl;
  } catch (const std::out_of_range &e) {
    std::cout << "Out of range: " << e.what() << std::endl;
  }
  return 0;
};

std::string nullSpaces::trim(const std::string &str) {
  // Find the position of the first non-whitespace character
  size_t start = str.find_first_not_of(" \t\n\r");

  // If the string is all whitespace, return an empty string
  if (start == std::string::npos) {
    return "";
  }

  // Find the position of the last non-whitespace character
  size_t end = str.find_last_not_of(" \t\n\r");

  // Return the trimmed string
  return str.substr(start, end - start + 1);
}
// std::string nullSpaces::trim(const std::string &str) {
//   // Find the position of the first non-whitespace character
//   size_t start = str.find_first_not_of(" \t\n\r");
//
//   // If the string is all whitespace, return an empty string
//   if (start == std::string::npos) {
//     return "";
//   }
//
//   // Find the position of the last non-whitespace character
//   size_t end = str.find_last_not_of(" \t\n\r");
//
//   // Return the trimmed string
//   return str.substr(start, end - start + 1);
// };

std::string encodeDecodeUrl::decodeUrl(std::string &url) {
  std::string result = "";
  if (url != "") {
    int i = 0;
    while (i < url.length()) {
      if (url[i] == '%' && i + 2 < url.length()) {
        std::string urlDec = url.substr(i, 3);
        if (urlDecodingMap.count(urlDec)) {
          result += encodeDecodeUrl::urlDecodingMap.at(urlDec);
          i += 3;
          continue;
        }
      }
      result += url[i];
      i++;
    }
  }
  return result;
}
