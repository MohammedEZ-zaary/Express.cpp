#include "../include/core/platformInterfaces/windowsPlatform.hpp"
#include "../include/core/httpParsing.hpp"
#include <iostream>


WindowsPlatform::WindowsPlatform(): serverSocket(INVALID_SOCKET)  {
    
} ;

void WindowsPlatform::setPort(int port) {
    this->port =  port ;
} ;


bool WindowsPlatform::createSocket(){
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return false;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return false;
    }
    return true ;
};

bool WindowsPlatform::bindSocket(){
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Bind failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return false;
    }else {
        return true ;
    }
};

bool WindowsPlatform::listenForConnections(){
    if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Listen failed: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return false;
    }

    std::cout << "Server started on port " << port << std::endl;
    return true ;
};

void WindowsPlatform::acceptConnections(){
       // Set the server socket to non-blocking mode
       u_long mode = 1; // 1 for non-blocking, 0 for blocking
       
       ioctlsocket(serverSocket, FIONBIO, &mode);
       while (true) {
           fd_set readSet;
           FD_ZERO(&readSet);
           FD_SET(serverSocket, &readSet);

           // Use select() to wait for incoming connections
           int selectResult = select(0, &readSet, nullptr, nullptr, nullptr);
           if (selectResult == SOCKET_ERROR) {
               std::cerr << "Select failed: " << WSAGetLastError() << std::endl;
               break;
           }

           if (FD_ISSET(serverSocket, &readSet)) {
               sockaddr_in clientAddr;
               int clientAddrSize = sizeof(clientAddr);

               // Accept the connection
               SOCKET clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
               if (clientSocket == INVALID_SOCKET) {
                   std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
                   continue;
               }

               // Set the client socket to non-blocking mode
               ioctlsocket(clientSocket, FIONBIO, &mode);

               // Process the client request asynchronously

               
              pool.addIo([this, clientSocket]() {
                processClientRequest(clientSocket);
               });
           }
       }

       closesocket(serverSocket);
       WSACleanup();
};

void WindowsPlatform::processClientRequest(SOCKET clientSocket)
{
    char buffer[4096];
    int bytesReceived;
    ParsingHttpHeader parseHeader ;

    while (true) {
        bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived <= 0) {
            if (WSAGetLastError() == WSAEWOULDBLOCK) {
                // No data available, wait for the next event
                Sleep(100);
                continue;
            }
            std::cerr << "Connection closed or error: " << WSAGetLastError() << std::endl;
            break;
        }
        buffer[bytesReceived] = '\0';
        std::string buffAsString(buffer);
        parseHeader << buffAsString ; 
        parseHeader.startParsing();
        
        // Null-terminate the received data and print it
        // std::cout  << buffAsString << std::endl;

        // Construct an HTTP "Hello, World!" response
        const char* httpResponse =
            "HTTP/1.1 200 OK\r\n"              // Status line
            "Content-Type: text/plain\r\n"      // Header
            "Content-Length: 13\r\n"            // Header
            "Connection: close\r\n"              // Header
            "\r\n"                              // Empty line to separate headers from body
            "Hello, World!";                    // Response body

        // Send the HTTP response to the client
        int bytesSent = send(clientSocket, httpResponse, strlen(httpResponse), 0);
        if (bytesSent == SOCKET_ERROR) {
            std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
            break;
        }
        // std::cout << "Sent HTTP response to client" << std::endl;

        // Close the client socket after sending the response
        break;
    }
    closesocket(clientSocket);
}
