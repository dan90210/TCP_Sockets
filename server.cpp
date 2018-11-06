// create a socket, get file descriptor
// bind to an address
// listen on a bort and wait for a connectino to be established
// accept the connection from Client
// send/receive
// shutdown to end read/write
// close to release data

#define _BSD_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <bitset>
#include <math.h>

#define PORT 9554
#define MAXVALUE 30

using namespace std;

void error(const char *msg) {
	perror(msg);
	exit(1);
}

int main (int argc, char *argv[]) {

	/*
	============================
	SET UP CONNECTION
	============================
	*/

	int sockfd, newsockfd;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;

	// create a socket
	// socket(int domain, int type, int protocol)
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) error("ERROR opening socket");

	// clear address structure
	bzero((char *) &serv_addr, sizeof(serv_addr));

	// setup the host_addr structure for use in bind call
	// server byte order
	serv_addr.sin_family = AF_INET;

	// automatically be filled with current hosts IP address
	serv_addr.sin_addr.s_addr = INADDR_ANY;

	// convert short integer value for port must be converted into network byte order
	serv_addr.sin_port = htons(PORT);

	// bind(ind fd, struct socaddr *local_addr, socklen_t addr_length)
	// bind() passes file descriptor, the address of the structure,
	// and the length of the address structure
	// This bind() call will bind the socket to the current IP address on port: PORT
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		error("ERROR on binding");
	}

	// This listen() call tells the socket to listen to the incoming connections
	// The listen() function places all incoming connection into a backlog queue
	// until accept() call accepts the connection
	// max backlog queue is 5
	listen(sockfd, 5);

	// the accept() call actually accepts an incoming connections
	clilen = sizeof(cli_addr);

	/* This accept() function will write the connecting client's address info
	into the address structure and the size of that structer is clilen.
	The accept() returns a new socket file descriptor for the accepted connection.
	So, the origial socket file descriptor can continue to be used
	for accepting new connections while the new socket file descriptor is used for
	communcicating with the connected client.
	*/
	newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

	if (newsockfd < 0) {
		error("ERROR on accept");
	}

	// printf("server: got connection from %s port %d\n",
	//          inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

	/*
	===============================
	DO STUFF ONCE CONNECTED
	===============================
	*/
	
	int i;
	
	// Create bitset and set all to 1
	//bitset<MAXVALUE> bitsetValues;
	//bitsetValues.set();
	
	// Data structure for what the other side has
	bitset<MAXVALUE> otherSideBitset;
	
	
	
	read(newsockfd, &i, sizeof(i));
	read(newsockfd, &otherSideBitset, sizeof(otherSideBitset));
	
	// bitsetValues = (bitsetValues &= otherSideBitset);
	
	cout << "Received: ";
	for (int p = 2; p <= MAXVALUE; p++) {
		if (otherSideBitset[p]) {
			cout << p << " ";
		}
	}
	cout << "\n";
	
	// Find the next prime
	while (otherSideBitset[i] != 1 && i <= sqrt(MAXVALUE)) {
		i++;
	}
	
	// We have reached our highest value to check
	if (i > sqrt(MAXVALUE)) {
		// Send -1 to signal we have run out
	}
	
	cout << "Prime: " << i << "\n";
	
	// Set multiples of i to false
	for (int k=i*2; k<=MAXVALUE; k += i) {
		otherSideBitset[k] = false;
	}
	
	// Set i to false
	otherSideBitset[i] = false;
	
	cout << "Sent: ";
	for (int p = 2; p <= MAXVALUE; p++) {
		if (otherSideBitset[p]) {
			cout << p << " ";
		}
	}
	cout << "\n\n";
	
	write(newsockfd, &i, sizeof(i));
	write(newsockfd, &otherSideBitset, sizeof(otherSideBitset));
	
	close(newsockfd);
	close(sockfd);
	return 0;
	

	// Highest value we are checking to
	//int maxValue;

	//// Read in max value from client
	//read(newsockfd, &maxValue, sizeof(int));

	//bool *bitsetValues = new bool[maxValue + 1];
	//for (int i = 0; i < sizeof(bitsetValues)/sizeof(bitsetValues[0]); i = i+1) {
	//	bitsetValues[i] = true;
	//}
	//int i;
	//while(true) {
	//	read(newsockfd, &i, sizeof(i));
	//	read(newsockfd, bitsetValues, sizeof(bitsetValues));
	//	cout << "Received: ";
	//	for (int p = 2; p<=maxValue; p++) {
	//		if (bitsetValues[p]) {
	//			cout << p << " ";
	//		}
	//	}
	//	cout << "\n";
	//	i++;
	//	while (bitsetValues[i] != true && i < maxValue/2) {
	//		i++;
	//	}
	//	if (i >= maxValue/2) {
	//		break;
	//	} else {
	//		cout << "Prime: " << i << "\n";
	//		for (int k=i*2; k<=maxValue; k += i) {
	//			bitsetValues[k] = false;
	//		}
	//	}
	  
	//	cout << "Sending: ";
	//	for (int p = 2; p<=maxValue; p++) {
	//		if (bitsetValues[p]) {
	//			cout << p << " ";
	//		}
	//	}
	//	cout << "\n\n";

	//	write(newsockfd, &i, sizeof(i));
	//	write(newsockfd, bitsetValues, sizeof(bitsetValues));
	//}
	//close(newsockfd);
	//close(sockfd);
	//return 0;
}
