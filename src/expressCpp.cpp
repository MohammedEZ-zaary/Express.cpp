#include "../include/expressCpp.hpp"
#include "../include/core/contentTypeMap.hpp"
#include "../include/core/helpFunctions.hpp"
#include "../include/core/platformInterfaces/platformFactory.hpp"
// #include "../../include/httpServer/headerParsing/form-data.hpp"
#include "../include/core/fileSystemManagment.hpp"
#include "../include/core/request.hpp"
#include "../include/core/response.hpp"
#include "../include/core/route.hpp"
#include <filesystem>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>

using std::string;
namespace fs = std::filesystem;
// craet content type header of file base on file end extantion
ContentTypeMapper contentType; // create instance

ExpressCpp::ExpressCpp() {
  // create pool threads
  platform = createPlatform();
  pool = std::make_unique<PoolManagment>();
};
void ExpressCpp::route(
    const std::string &routeName,
    const std::function<void(const Request &, Response &res)> &lambdaExcuter) {
  try {

    platform->setRoute(Route(
        routeName, [this, lambdaExcuter](const Request &req, Response &res) {
          lambdaExcuter(req, res);
        }));
  } catch (std::runtime_error &e) {
    std::cerr << "Error can not add Route to the routes vector using setRoute "
                 "method ."
              << e.what() << std::endl;
  }
};

void ExpressCpp::run(int port) {

  // set the port
  platform->setPort(port);
  if (platform->createSocket() && platform->bindSocket() &&
      platform->listenForConnections()) {
    // start acceptting client requests
    platform->acceptConnections();
  }
  // Add Ene Error Handler Here
}

void ExpressCpp::serveStaticFile(const std::string &staticFolderName) {
  fs::path directoryPath = staticFolderName;

  if (fs::exists(directoryPath) && fs::is_directory(directoryPath)) {
    for (const auto &entry : fs::directory_iterator(directoryPath)) {
      if (fs::is_directory(entry.path())) {
        serveStaticFile(entry.path().string());
      } else {
        std::string clonePath = entry.path().string();

        size_t index = clonePath.find("/templates");
        if (index != std::string::npos) {
          std::string extractRoutName =
              clonePath.substr(index + std::string("/templates").length());

          std::string extractPathOfStaticFile =
              fs::absolute(entry.path()).string();
          route(extractRoutName, [this, extractPathOfStaticFile](
                                     const Request &req, Response &res) {
            res.sendFile(extractPathOfStaticFile);
          });
        }
      }
    }
  } else {
    if (getStaticRootFolderPath() != "") {
      std::cerr << "\033[1;31m[!] Please check the path of the static folders. "
                   "the folder "
                << directoryPath << " Not Found" << "\033[0m\n";
    }
  }
}
void ExpressCpp::setStaticRouteFolder(const std::string &folderPath) {
  // insert new static folder path
  staticRootFolderPath = folderPath;
  // add all files and folders to new Route
  ExpressCpp::serveStaticFile(getStaticRootFolderPath());
}
std::string ExpressCpp::getStaticRootFolderPath() {
  return staticRootFolderPath;
}
std::string ExpressCpp::readFileContent(const std::string &filePath) {
  //  read files like javascript , html , css ...
  return FileManager::readFileContent(filePath);
}

// vector<HandsomeServer::MultipartFormData::clientFinelFile>
// HandsomeServer::MultipartFormData::saveMultiPartFile(
//     requestHeader request, std::string path, int memoryAlloc,
//     std::function<void(MultipartFormData::fileProgress)> per) {
//   int multiPartSockets =
//       httpserver.getRoute(request,
//       request.uri).multipartFormDataClientSocket;

//   vector<HandsomeServer::MultipartFormData::clientFinelFile> results =
//       Multipart_FormData::handleMultipartRequest(multiPartSockets, request,
//                                                  path, memoryAlloc, per);

//   return results;
// };
