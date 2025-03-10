#ifndef HTTP_PARSING 
#define HTTP_PARSING
#include <sstream>
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include "helpFunctions.hpp"

using std::string ;

struct ParsedHeader {
    string method ;
    string host ;
    string url ;
    string connection ;
    string platform ;
    string acceptLanguage ;
    string userAgent ;
    string accept ; 
    string contentType ;
    string referer ;
    string body ;
    std::map<string , string> params ;
    unsigned long long  contentLength ;

};
class ParsingHttpHeader {

    public:
        
        void operator<<(std::string& buff){
            this->buffer << buff ;
        }

        void startParsing () {
            std::string line ;
            string key = "";
            string value = "" ;
            size_t first ;
            size_t second ;
            while(std::getline(buffer , line)){
                first = line.find(":") ;
                second =  first + 1 ;
                if(first != string::npos){
                    key = line.substr(0 ,first);
                    value =  line.substr(second);
                    // set header as key and value
                    headerAsKeyValue.insert(std::make_pair(nullSpaces::trim(key) , nullSpaces::trim(value))) ;
                }else {
                    // extract data from http header and set it to its struct parsedHeader params at same function 
                    extractMethodSet(line);
                    extractUrlSet(line);
                }
            } ;
            setHeaderValuesToParsedStruct();
            setParams(parsedHeader.url);


        }
        void extractMethodSet(string& line) {
            if(line.find("GET") != string::npos || 
            line.find("POST") != string::npos ||
            line.find("DELETE") != string::npos || 
            line.find("HEAD") != string::npos || 
            line.find("PATCH") != string::npos || 
            line.find("OPTIONS") != string::npos || 
            line.find("PUT") != string::npos ){
                parsedHeader.method =  line.substr(0 , line.find("/")) ;
            }
        }

        void extractUrlSet(string & line ){
            /*
                input : GET /simple/url/code HTTP/1.1
                output : /simple/url/code
            */
            if(line.find("HTTP") == string::npos){
                return ; 
            }
            size_t target = line.find("/") ;
            parsedHeader.url = line.substr(target , line.find(' ', target) - target) ;

            
        }            
        void setParams(string& url ){
            
            if(url.find("=") == string::npos) {
                return ; 
            }
            size_t target = url.find("?") + 1 ;
            string result ;
          
            // From this : /core/id?name=John&age=3348374837443794897 HTTP/1.1 
            if(target != string::npos ) {
                result =  url.substr(target);
                result =  result.substr(0 , result.find(' ')) ;
            // Result  is this :  name=John&age=3348374837443794897&id=333
            }

            
            if(result.find('&') == string::npos && result.find("=") != string::npos){
                makeKeyValueSet(result) ;
                return  ;
            }
            int i = 0 ;
            int prv = 0 ;

            while(i < result.length()) {

                if(result[i] == '&'){
                    if(prv == 0){
                        makeKeyValueSet(result.substr(prv , i ));
                        prv = i  ;
                    }else {

                        makeKeyValueSet(result.substr(prv + 1 ,  i - prv  - 1));
                        prv = i  ;
                    }

                }
                i+=1 ;
            }

            makeKeyValueSet(result.substr(prv + 1));
        }
        void makeKeyValueSet(string & pare) {
            /*
                input : name=33
                output : parsedHeader.push(name , 33) (key , value)
            */
            if(pare == ""){
                return  ;
            }
            size_t separatorPos = pare.find('=') ;

            parsedHeader.params.insert(
                std::make_pair(pare.substr(0,separatorPos) , 
                encodeDecodeUrl::decodeUrl(pare.substr(separatorPos + 1)))) ;

        }
        void xWwwFormUrlEncoded() { // application/x-www-form-urlencoded
            
        }

        void jsonQuery () { // application/json

        }
        void multipartFormData () { // multipart/form-data

        } 

        void otherForms() {
            // application/javascript
            //application/yaml
            //text/html
            //application/graphql
            // application/octet-stream
        }
        

    void printStruct(){
            std::cout << "parsed header structe : " << std::endl ;
            std::cout << parsedHeader.method << std::endl;
            std::cout << parsedHeader.host << std::endl;
            std::cout << parsedHeader.referer << std::endl;
            std::cout << parsedHeader.connection << std::endl;
            std::cout << parsedHeader.platform << std::endl;
            std::cout << parsedHeader.acceptLanguage << std::endl;
            std::cout << parsedHeader.userAgent << std::endl;
            std::cout << parsedHeader.accept << std::endl; 
            std::cout << parsedHeader.contentType << std::endl;
            std::cout << parsedHeader.contentLength << std::endl ;
    }
    ParsedHeader getParsedHeader() {
        return parsedHeader ;
    }
    private:
        std::stringstream buffer ;
        std::map<string , string> headerAsKeyValue  ;
        std::map<string , string> params ;
        std::map<string , string> query ;
        ParsedHeader parsedHeader ;

    void setHeaderValuesToParsedStruct(){
            try{
                if(headerAsKeyValue.count("Host")){

                parsedHeader.host =   headerAsKeyValue.at("Host") ;
                }
                if(headerAsKeyValue.count("User-Agent")){
                 parsedHeader.userAgent =  headerAsKeyValue.at("User-Agent");
                }
                if(headerAsKeyValue.count("Connection")){
                    parsedHeader.connection =  headerAsKeyValue.at("Connection");
                }
                 if(headerAsKeyValue.count("Content-Type")){
                    parsedHeader.contentType =  headerAsKeyValue.at("Content-Type");
                }
                if(headerAsKeyValue.count("Content-Length")){
                       parsedHeader.contentLength =  convertingTypes::convertStringToULongInt(
                        headerAsKeyValue.at("Content-Length")) ;
                }else {
                       parsedHeader.contentLength =  0 ;
                }
                if(headerAsKeyValue.count("Referer")){
                       parsedHeader.referer =  headerAsKeyValue.at("Referer") ;
                }
                if(headerAsKeyValue.count("sec-ch-ua-platform")){
                       parsedHeader.platform =  headerAsKeyValue.at("sec-ch-ua-platform") ;
                }
                if(headerAsKeyValue.count("Accept-Language")){
                       parsedHeader.acceptLanguage =  headerAsKeyValue.at("Accept-Language") ;
                }
                if(headerAsKeyValue.count("Accept")){
                       parsedHeader.accept =  headerAsKeyValue.at("Accept") ;
                }
            
            }catch(const std::out_of_range& e){
                 std::cerr << e.what() << std::endl ;
            }
            
        }
};

#endif