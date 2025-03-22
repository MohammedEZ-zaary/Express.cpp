#include "../../include/core/multiPartFormDataHandler.hpp"
#include "../../include/core/fileSystemManagment.hpp"
// #include "../../include/core/helpFunctions.hpp"
#include <numeric>

FormData::FormData(const Request &request, SOCKET clientSocket,
                   int memoryAllocation, string uploadFolderPath)
    : REQUEST(request), CLIENT_SOCKET(clientSocket),
      MEMORY_ALLOCATION(memoryAllocation), PATH_UPLOAD_FILES(uploadFolderPath),
      dataContent(), trackReceivedDataSize() {

  // start receive data and save it
  startReceive();
};

void FormData::startReceive() {
  // check proprties beffor start
  char buffer[8000] = {0};
  int bytesReceived = 0;
  int bSize = 0;

  if (checkMethod() != true || checkMemoryAllocation() != true ||
      checkUploadedFolderPath() != true) {
    std::cerr << "[FAILED]: Receive data field" << std::endl;
    return;
  }

  if (REQUEST.headerProprties.body.size() > 1) {
    dataContent.insert(dataContent.end(), REQUEST.headerProprties.body.begin(),
                       REQUEST.headerProprties.body.end());
  }

  while (calculateTotaleReceivedData() <
         REQUEST.headerProprties.contentLength) {

    bytesReceived = recv(CLIENT_SOCKET, buffer, sizeof(buffer), 0);
    if (bytesReceived > 1) {
      dataContent.insert(dataContent.end(), buffer, buffer + bytesReceived);
      string b(dataContent.begin(), dataContent.end());
      extractBoundary(b);
      bSize = dataContent.size();
      trackReceivedDataSize.push_back(bSize);
      dataContent.clear();
      continue;
    }
    string b(dataContent.begin(), dataContent.end());
    extractBoundary(b);

    break;
  }
}

bool FormData::checkMethod() {
  if (REQUEST.headerProprties.method != "POST") {

    std::cerr << "[ERROR]: You can not send binry using ["
              << REQUEST.headerProprties.method << "] "
              << "Method Please use POST Method ." << std::endl;
    return false;
  }
  return true;
};
bool FormData::checkUploadedFolderPath() {
  if (!FileManager::isPathFolderExists(PATH_UPLOAD_FILES)) {
    std::cerr << "[ERROR]: Please check upload Folder path " << std::endl;
    return false;
  }
  return true;
};

bool FormData::checkMemoryAllocation() {
  if (MEMORY_ALLOCATION < 1) {
    std::cerr << "[WORNING]: Memory allocation must be bigger then 1MB "
              << std::endl;
    MEMORY_ALLOCATION = 1;
    return true;
  }
  return true;
}

int FormData::calculateTotaleReceivedData() {
  // add comment
  return std::accumulate(trackReceivedDataSize.begin(),
                         trackReceivedDataSize.end(), 0);
};

void FormData::extractBoundary(string metaData) {
  std::cout << "======================boundrary " << std::endl;
  std::cout << metaData << std::endl;
}
