#ifndef HELP_FUNCTIONS
#define HELP_FUNCTIONS
#include <string>
#include <iostream>
#include <map>
namespace convertingTypes {
    unsigned long long convertStringToULongInt(std::string & stringNumber) {
        try {
            unsigned long long number = std::stoull(stringNumber);
            return number ;
        } catch (const std::invalid_argument& e) {
            std::cout << "Invalid argument: " << e.what() << std::endl;
        } catch (const std::out_of_range& e) {
            std::cout << "Out of range: " << e.what() << std::endl;
        }
    
    };
};
namespace nullSpaces {
std::string trim(const std::string &str) {
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
};
namespace encodeDecodeUrl {
        std::map<std::string ,char> urlDecodingMap = {
            {"%20", ' '},
            {"%3A", ':'},
            {"%2F", '/'},
            {"%3F", '?'},
            {"%23", '#'},
            {"%5B", '['},
            {"%5D", ']'},
            {"%40", '@'},
            {"%21", '!'},
            {"%24", '$'},
            {"%26", '&'},
            {"%27", '\''},
            {"%28", '('},
            {"%29", ')'},
            {"%2A", '*'},
            {"%2B", '+'},
            {"%2C", ','},
            {"%3B", ';'},
            {"%3D", '='},
            {"%3C", '<'},
            {"%3E", '>'},
            {"%22", '"'},
            {"%25", '%'},
            {"%7B", '{'},
            {"%7D", '}'},
            {"%7C", '|'},
            {"%5C", '\\'},
            {"%5E", '^'},
            {"%60", '`'}
    };
    std::string decodeUrl(std::string & url) {
        std::string result = "";
        if(url != ""){
            int i = 0 ;
            while(i < url.length()){

                if(url[i] == '%' && i + 2 < url.length()){
                    std::string urlDec = url.substr(i ,  3);
                    if(urlDecodingMap.count(urlDec)){
                    result += urlDecodingMap.at(urlDec) ; 
                       i+=3 ; 
                       continue;
                    }
                }
                result += url[i];
                i++;
            }

        }
        return result;
    }
}

#endif