#ifndef HELP_FUNCTIONS
#define HELP_FUNCTIONS
#include <iostream>
#include <map>
#include <string>

namespace convertingTypes {
unsigned long long convertStringToULongInt(std::string &stringNumber);

};

namespace stringUtils {
std::string generateRandomString(size_t length);

};
namespace nullSpaces {
std::string trim(const std::string &str);

};
namespace encodeDecodeUrl {
static std::map<std::string, char> urlDecodingMap = {
    {"%20", ' '}, {"%3A", ':'},  {"%2F", '/'}, {"%3F", '?'}, {"%23", '#'},
    {"%5B", '['}, {"%5D", ']'},  {"%40", '@'}, {"%21", '!'}, {"%24", '$'},
    {"%26", '&'}, {"%27", '\''}, {"%28", '('}, {"%29", ')'}, {"%2A", '*'},
    {"%2B", '+'}, {"%2C", ','},  {"%3B", ';'}, {"%3D", '='}, {"%3C", '<'},
    {"%3E", '>'}, {"%22", '"'},  {"%25", '%'}, {"%7B", '{'}, {"%7D", '}'},
    {"%7C", '|'}, {"%5C", '\\'}, {"%5E", '^'}, {"%60", '`'}};
std::string decodeUrl(std::string &url);
}; // namespace encodeDecodeUrl

#endif
