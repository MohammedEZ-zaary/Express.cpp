#ifndef HTTP_PARSING
#define HTTP_PARSING
#include "helpFunctions.hpp"
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using std::string;

struct ParsedHeader {
  string method;
  string host;
  string url;
  string connection;
  string platform;
  string acceptLanguage;
  string userAgent;
  string accept;
  string contentType;
  string referer;
  string body;
  std::map<string, string> params;
  unsigned long long contentLength;
};

class ParsingHttpHeader {

private:
  std::stringstream buffer;
  std::map<string, string> headerAsKeyValue;
  ParsedHeader parsedHeader;

  void setParams(const string &url);
  void makeKeyValueSet(const string &pare);
  //

  // extracting Methods
  string extractMethod(const string &line);
  /*
      extractUrl() :
          input : GET /simple/url/code HTTP/1.1
          output : /simple/url/code
  */
  string extractUrl(const string &line);

  //

  std::string extractBody(const std::string &buffer);
  void xWwwFormUrlEncoded(); // application/x-www-form-urlencoded
  void jsonQuery();          // application/json
  void multipartFormData();  // multipart/form-data
  void otherForms();
  void setHeaderValuesToParsedStruct();

public:
  void operator<<(const std::string &buff);
  void parse(); // Start Parsing HTTP header and return parsed http struct
  ParsedHeader getParsedHeader();
};

#endif
