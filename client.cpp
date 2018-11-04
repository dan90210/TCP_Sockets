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
  
  int maxValue;
  printf("Please enter the max value: ");
  cin >> maxValue;
  
  // Create array and default everything to true
  bool bitsetValues[maxValue+1];
  memset(bitsetValues, true, sizeof(bitsetValues));
  
	
  // Size of the array
  write(sockfd, &maxValue, sizeof(maxValue));
  

  int i = 2;
  while (bitsetValues[i] != true && i < maxValue/2) {
	 i++;
  }
  if (i >= maxValue/2) {
	
  } else {
	for (int k=i*2; k<=maxValue; k += i) {
		bitsetValues[k] = false;
	}
  }
  cout << "Sending: ";
  for (int p = 2; p<=maxValue; p++) {
	if (bitsetValues[p]) {
		cout << p << " ";
	}
  }
  cout << "\n";
  
  write(sockfd, &i, sizeof(i));
  write(sockfd, bitsetValues, sizeof(bitsetValues));
  
   while(true) {
	  read(sockfd, &i, sizeof(i));
	  read(sockfd, bitsetValues, sizeof(bitsetValues));
	  cout << "Received: ";
	  for (int p = 2; p<=maxValue; p++) {
		if (bitsetValues[p]) {
			cout << p << " ";
		}
	  }
	  cout << "\n";
	  i++;
	  while (bitsetValues[i] != true && i < maxValue/2) {
		 i++;
	  }
	  if (i >= maxValue/2) {
		  break;
	  } else {  
		for (int k=i*2; k<=maxValue; k += i) {
			bitsetValues[k] = false;
		}
	  }
	  
	  cout << "Sending: ";
	  for (int p = 2; p<=maxValue; p++) {
		if (bitsetValues[p]) {
			cout << p << " ";
		}
	  }
	  cout << "\n";
	  
	  write(sockfd, &i, sizeof(i));
	  write(sockfd, bitsetValues, sizeof(bitsetValues));
  }
  
  close(sockfd);
  return 0;
}
