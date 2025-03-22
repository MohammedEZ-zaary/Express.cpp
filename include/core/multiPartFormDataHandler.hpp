#ifndef FORM_DATA_HEADER
#define FORM_DATA_HEADER
#include "request.hpp"
#include <functional>
#include <map>
#include <memory.h>
#include <string>
#include <vector>

// windows
#ifdef _WIN32
#include <winsock2.h>
#endif

// linux
#ifdef __linux__
#include <sys/socket.h>
#endif

using std::string;
using std::vector;
struct fileMetaData {
  string fieldName;
  string fileName;
};

struct FileInfo {
  string fileName;
  string fullBoundary;
  string boundaryId;
  string boundaryEnd;
  string filePath;
  std::map<string, string> text;
  unsigned long long fileSize = 0;
  bool isFileComplete = false;
  bool status = false;
};

struct File {
  string fileName;
  string filePath;
  unsigned long long fileSize = 0;
  bool status = false;
};

struct fileProgress {
  string fileName;
  bool status = false;
  int TotalFilesProgress = 0;
};

class FormData {
public:
  FormData(const Request &request, SOCKET clientSocket, int memoryAllocation,
           string uploadFolderPath);

private:
  void startReceive();
  bool checkMethod();
  bool checkMemoryAllocation();
  bool checkUploadedFolderPath();
  int calculateTotaleReceivedData();
  void extractBoundary(std::string metaData);
  void extractFileMetaData();

  SOCKET CLIENT_SOCKET;
  int MEMORY_ALLOCATION;
  const Request REQUEST;
  const string PATH_UPLOAD_FILES;
  string boundary;
  string boundaryEnd;

  std::vector<char> dataContent;
  std::vector<int> trackReceivedDataSize;
};

// namespace Multipart_FormData {
//
// ;
// struct FileInfo {
//   string fileName;
//   string fullBoundary;
//   string boundaryId;
//   string boundaryEnd;
//   string filePath;
//   std::map<string, string> text;
//   unsigned long long fileSize = 0;
//   bool isFileComplete = false;
//   bool status = false;
// };
//
// struct clientFinelFile {
//   string fileName;
//   string filePath;
//   unsigned long long fileSize = 0;
//   bool status = false;
// };
//
// struct fileProgress {
//   string fileName;
//   bool status = false;
//   int TotalFilesProgress = 0;
// };

// string trim(const string &str);
//
// std::string generateRandomString(size_t length);
//
// string extractFullBoundary(string contentType);
//
// std::vector<clientFinelFile>
// handleMultipartRequest(SOCKET clientSocket, const Request &,
//                        string uploadFolderPath, int memoryAlloc,
//                        std::function<void(fileProgress)> progress);
//
// bool isContentTypeFormData(const string &contentType);
//
// string extractBoundaryNumber(string boundary);
//
// string extractFileName(const string &buffer);
//
// void setFileBoundary(FileInfo &store, const Request &request);
//
// void removeMetaDataFromBuffer(string &bufferForProccess);
//
// void handleMultipleFiles(const Request &request, string uploadFolderPath,
//                          FileInfo &fileInfo,
//                          std::vector<clientFinelFile> &files, vector<char>
//                          body, const unsigned long long &currentFileSize);
// // extract the fiald Name from Header Meta data
// string extractFialdName(std::string &buffer);
//
// bool isThereContentTypeInBuffer(const string &buffer);
// } // namespace Multipart_FormData
#endif
