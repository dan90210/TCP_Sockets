// create a socket
// bind to an address
// connect to a server
// send/recieve data
// shutdown to end read/write
// close to release data

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>

#define PORT 9555

using namespace std;

void error(const char *msg) {
    perror(msg);
    exit(0);
}

int main(int argc, char*argv[]) {

  /*
  ============================
      SET UP CONNECTION
  ============================
  */

  int sockfd, n;
  struct sockaddr_in serv_addr;
  struct hostent *server;

  if (argc < 2) {
    fprintf(stderr, "usage is %s hostname\n", argv[0]);
    exit(0);
  }

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) error("ERROR opening socket");
  server = gethostbyname(argv[1]);
  if (server == NULL) {
    fprintf(stderr, "ERROR, no such host\n");
    exit(0);
  }
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr,
        (char *)&serv_addr.sin_addr.s_addr,
        server->h_length);
  serv_addr.sin_port = htons(PORT);
  if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    error("ERROR connecting");
  }

  /*
  ===============================
      DO STUFF ONCE CONNECTED
  ===============================
  */
  
  int i;
  printf("Please enter the max value: ");
  cin >> i;
  
  // Create array and fill it with values
  int x[(i-1)];
  
  int j;
  for (j = 0; j<i-1; j++) {
	x[j] = j+2;
  }
  
  int k;
  for (k = 0; k<i-1; k++) {
	cout << x[k] << " ";
  }
  cout << "\n";

  write(sockfd, &i, sizeof(i)); 
  write(sockfd, x, sizeof(x));
  
  close(sockfd);
  return 0;
}
