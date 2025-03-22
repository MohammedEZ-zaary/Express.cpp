#include "../../include/core/httpParsing.hpp"

void ParsingHttpHeader::operator<<(const string &buff) { buffer << buff; }
void ParsingHttpHeader::parse() {
  std::string line;
  string key = "";
  string value = "";
  size_t first;
  size_t second;
  // set Body data if any
  parsedHeader.body = extractBody(buffer.str());
  // std::cout << "body content 1 :" << buffer.str() << std::endl;
  // std::cout << "body content :" << parsedHeader.body << std::endl;
  while (std::getline(buffer, line)) {
    first = line.find(":");
    second = first + 1;
    if (first != string::npos) {
      key = line.substr(0, first);
      value = line.substr(second);
      // set header as key and value
      headerAsKeyValue.insert(
          std::make_pair(nullSpaces::trim(key), nullSpaces::trim(value)));
    } else {
      // extract data from http header and set it to its struct parsedHeader
      // params at same function
      if (parsedHeader.method == "") {
        parsedHeader.method = extractMethod(line);
      }
      if (parsedHeader.url == "") {
        parsedHeader.url = extractUrl(line);
      }
    }
  };
  setHeaderValuesToParsedStruct();
  setParams(parsedHeader.url);
};

string ParsingHttpHeader::extractBody(const string &buffer) {
  // find the beginning of the body binry
  size_t newLine = buffer.find("\r\n\r\n");
  if (newLine != string::npos) {
    return buffer.substr(newLine + 4);
  }
  return "";
};
ParsedHeader ParsingHttpHeader::getParsedHeader() { return parsedHeader; }

string ParsingHttpHeader::extractMethod(const string &line) {
  if (line.find("GET") != string::npos || line.find("POST") != string::npos ||
      line.find("DELETE") != string::npos ||
      line.find("HEAD") != string::npos || line.find("PATCH") != string::npos ||
      line.find("OPTIONS") != string::npos ||
      line.find("PUT") != string::npos) {
    return nullSpaces::trim(line.substr(0, line.find("/")));
  }
  return "";
}

string ParsingHttpHeader::extractUrl(const string &line) {
  /*
      input : GET /simple/url/code HTTP/1.1
      output : /simple/url/code
  */
  if (line.find("HTTP") == string::npos) {
    return "";
  }
  size_t target = line.find("/");
  return line.substr(target, line.find(' ', target) - target);
};
void ParsingHttpHeader::setParams(const string &url) {

  if (url.find("=") == string::npos) {
    return;
  }
  size_t target = url.find("?") + 1;
  string result;

  // From this : /core/id?name=John&age=3348374837443794897 HTTP/1.1
  if (target != string::npos) {
    result = url.substr(target);
    result = result.substr(0, result.find(' '));
    // Result  is this :  name=John&age=3348374837443794897&id=333
  }
  if (result.find('&') == string::npos && result.find("=") != string::npos) {
    makeKeyValueSet(result);
    return;
  };

  int i = 0;
  int prv = 0;
  while (i < result.length())
    if (result[i] == '&') {
      if (prv == 0) {

        makeKeyValueSet(result.substr(prv, i));
        prv = i;
      } else
        makeKeyValueSet(result.substr(prv + 1, i - prv - 1));
      prv = i;
    }
  i += 1;

  makeKeyValueSet(result.substr(prv + 1));
};
void ParsingHttpHeader::makeKeyValueSet(const string &pare) {
  /*
      input : name=33
      output : parsedHeader.push(name , 33) (key , value)
  */
  if (pare == "") {
    return;
  }
  size_t separatorPos = pare.find('=');
  parsedHeader.params.insert(std::make_pair(
      pare.substr(0, separatorPos),
      encodeDecodeUrl::decodeUrl(pare.substr(separatorPos + 1))));
};

void ParsingHttpHeader::setHeaderValuesToParsedStruct() {
  try {
    if (headerAsKeyValue.count("Host")) {
      parsedHeader.host = headerAsKeyValue.at("Host");
    }
    if (headerAsKeyValue.count("User-Agent")) {
      parsedHeader.userAgent = headerAsKeyValue.at("User-Agent");
    }
    if (headerAsKeyValue.count("Connection")) {
      parsedHeader.connection = headerAsKeyValue.at("Connection");
    }
    if (headerAsKeyValue.count("Content-Type")) {
      parsedHeader.contentType = headerAsKeyValue.at("Content-Type");
    }
    if (headerAsKeyValue.count("Content-Length")) {
      parsedHeader.contentLength = convertingTypes::convertStringToULongInt(
          headerAsKeyValue.at("Content-Length"));
    } else {
      parsedHeader.contentLength = 0;
    }
    if (headerAsKeyValue.count("Referer")) {
      parsedHeader.referer = headerAsKeyValue.at("Referer");
    };
    if (headerAsKeyValue.count("sec-ch-ua-platform")) {
      parsedHeader.platform = headerAsKeyValue.at("sec-ch-ua-platform");
    }
    if (headerAsKeyValue.count("Accept-Language")) {
      parsedHeader.acceptLanguage = headerAsKeyValue.at("Accept-Language");
    }
    if (headerAsKeyValue.count("Accept")) {
      parsedHeader.accept = headerAsKeyValue.at("Accept");
    }

  } catch (const std::out_of_range &e) {
    std::cerr << e.what() << std::endl;
  }
};
