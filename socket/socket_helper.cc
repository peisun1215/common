extern "C" {
#include "csapp.h"
}

#include <string>

using namespace std;

typedef int int32;
typedef unsigned uint32;

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
  static int32 Accept(int32 socketfd) {
    sockaddr in_addr;
    socklen_t clientlen = sizeof(sockaddr);
    int32 connfd = accept(socketfd, &in_addr, &clientlen);
    return connfd;
  }
};

#define MAXLINE 1024*10

void StartClient(char** argv) {
  int clientfd;
  char *host, *port, buf[MAXLINE];
  rio_t rio;
  host = argv[1];
  port = argv[2];
  clientfd = Open_clientfd(host, stoi(port));
  Rio_readinitb(&rio, clientfd);
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
  }
}

void StartServer(char** argv) {
  int listenfd, connfd;
  socklen_t clientlen;
  sockaddr_storage clientaddr; /* Enough room for any addr */
  char client_hostname[MAXLINE], client_port[MAXLINE];
  listenfd = Open_listenfd(stoi(argv[1]));
  while (1) {
    clientlen = sizeof(sockaddr_storage); /* Important! */
    connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
    getnameinfo((SA *) &clientaddr, clientlen,
                client_hostname, MAXLINE, client_port, MAXLINE, 0);
    printf("Connected to (%s, %s)\n", client_hostname, client_port);
    echo(connfd);
    Close(connfd);
  }
  exit(0);
}

int main(int argc, char** argv) {
  if (argc == 3) StartClient(argv);
  else StartServer(argv);
}
