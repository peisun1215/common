// A simple Thread per connection server without using threadpool.

extern "C" {
#include "csapp.h"
}

#include <cstdlib>
#include <thread>
#include <string>
#include <iostream>
#include <cassert>

using namespace std;

typedef int int32;
typedef unsigned uint32;

#define MAXLINE 1024*10

void echo(int a);
void SolveAndAnswer(int a);

class TCPSocket {
 public:
  TCPSocket() {}

  enum Protocol {TCP, UDP};

  // Return the socket fd.
  static int32 Dial(const string& host, const string& port) {
    addrinfo hints, *listp;
    // Get a list of potential server addresses.
    memset(&hints, 0, sizeof(addrinfo));
    hints.ai_socktype = SOCK_STREAM; /* Open a connection */
    hints.ai_flags = AI_NUMERICSERV; /* …using numeric port arg. */
    hints.ai_flags |= AI_ADDRCONFIG; /* Recommended for connections */
    getaddrinfo(host.c_str(), port.c_str(), &hints, &listp);

    int32 clientfd = -1;
    addrinfo* p = listp;
    // Walk the list for one that we can successfully connect to.
    for (; p; p = p->ai_next) {
      // Create a socket descriptor.
      if ((clientfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) < 0)
        continue; /* Socket failed, try the next */
      // Connect to the server.
      if (connect(clientfd, p->ai_addr, p->ai_addrlen) != -1)
        break; /* Success */
      close(clientfd); /* Connect failed, try another */
    }

    freeaddrinfo(listp);
    if (!p) /* All connects failed */
      return -1;
    else /* The last connect succeeded */
      return clientfd;
  }

  // GetAddr->Create Socket->Bind->Listen.
  static int32 Listen(const string& port) {
    addrinfo hints, *listp, *p;
    int listenfd, optval = 1;

    // Get a list of potential server addresses.
    memset(&hints, 0, sizeof(addrinfo));
    hints.ai_socktype = SOCK_STREAM; /* Accept connect. */
    hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG; /* …on any IP addr */
    hints.ai_flags |= AI_NUMERICSERV; /* …using port no. */
    getaddrinfo(NULL, port.c_str(), &hints, &listp);

    /// Walk the list for one that we can bind to.
    for (p = listp; p; p = p->ai_next) {
      /* Create a socket descriptor */
      if ((listenfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) < 0)
        continue; /* Socket failed, try the next */

#if 1
      // Eliminates "Address already in use" error from bind.
      Setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
                 (const void *)&optval , sizeof(int));
#endif

      // Bind the descriptor to the address.
      if (bind(listenfd, p->ai_addr, p->ai_addrlen) == 0)
        break; /* Success */

      close(listenfd); /* Bind failed, try the next */
    }

    /* Clean up */
    freeaddrinfo(listp);
    if (!p) /* No address worked */
      return -1;

    // Make it a listening socket ready to accept conn. requests.
    if (listen(listenfd, LISTENQ) < 0) {
      close(listenfd);
      return -1;
    }
    return listenfd;
  }

  // Blocking call.
  static void Accept(int32 socketfd,
                     std::function<void(int32 fd,
                                        string hostname, string port)> f) {
    while (1) {
      sockaddr in_addr;
      socklen_t clientlen = sizeof(sockaddr);
      int32 connfd = accept(socketfd, &in_addr, &clientlen);
      char client_hostname[10240];
      char client_port[1024];
      getnameinfo(&in_addr, clientlen,
                  client_hostname, MAXLINE, client_port, MAXLINE, 0);

      // Use a threadpool.
      std::thread t([=]() {f(connfd, client_hostname, client_port);});
      t.detach();
    }
  }

  static void HandleConnection(int32 connfd, string hostname, string port) {
    std::cout << "Connected to " << hostname << ":" << port << std::endl;
    // echo(connfd);
    SolveAndAnswer(connfd);
    std::cout << "Disconnected from " << hostname << ":" << port << std::endl;
    close(connfd);
  }
};

void StartClient(char** argv) {
  int32 clientfd = TCPSocket::Dial(argv[1], argv[2]);
  if (clientfd < 0) {
    printf("fail!");
    return;
  }

  rio_t rio;
  Rio_readinitb(&rio, clientfd);

  char buf[MAXLINE];
  while (Fgets(buf, MAXLINE, stdin) != NULL) {
    Rio_writen(clientfd, buf, strlen(buf));

    Rio_readlineb(&rio, buf, MAXLINE);
    Fputs(buf, stdout);
  }
  Close(clientfd);
  exit(0);
}

void echo(int connfd) {
  size_t n;
  char buf[MAXLINE];
  rio_t rio;
  Rio_readinitb(&rio, connfd);
  while((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
    printf("server received %d bytes\n", (int)n);
    Rio_writen(connfd, buf, n);
    std::cout << buf;
  }
}

void SolveAndAnswer(int32 connfd) {
  size_t n;
  char buf[MAXLINE];
  rio_t rio;
  Rio_readinitb(&rio, connfd);
  while ((n = Rio_readlineb(&rio, buf, MAXLINE)) != 0) {
    std::cout << "Received: " << buf << endl;

    string s(buf);
    size_t pos = s.find("+");
    assert(pos != string::npos);
    int32 res = stoi(s.substr(0, pos)) + stoi(s.substr(pos + 1));
    strcpy(buf, (to_string(res) + "\n").c_str());
    std::cout << "Result: " << buf << std::endl;
    Rio_writen(connfd, buf, strlen(buf));
  }
}

void StartServer(char** argv) {
  int32 listenfd = TCPSocket::Listen(argv[1]);
  if (listenfd < 0) {
    printf("Failed to start server");
    return;
  }

  TCPSocket::Accept(listenfd, [](int32 connfd, string hostname, string port) {
      TCPSocket::HandleConnection(connfd, hostname, port);
    });

  exit(0);
}

int main(int argc, char** argv) {
  if (argc == 3) StartClient(argv);
  else StartServer(argv);
}
