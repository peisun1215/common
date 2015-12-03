#include "csapp.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

int main(int argc, char **argv) {
  struct addrinfo *p, *listp, hints;

  char buf[100];
  int rc, flags;
  /* Get a list of addrinfo records */
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET; /* IPv4 only */
  hints.ai_socktype = SOCK_STREAM; /* Connections only */
  if ((rc = getaddrinfo(argv[1], NULL, &hints, &listp)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(rc));
    exit(1);
  }
  /* Walk the list and display each IP address */
  flags = NI_NUMERICHOST; /* Display address instead of name */
  for (p = listp; p; p = p->ai_next) {
    getnameinfo(p->ai_addr, p->ai_addrlen,
		buf, 100, NULL, 0, flags);
    printf("%s\n", buf);
  }
  /* Clean up */
  freeaddrinfo(listp);
  exit(0);
}
