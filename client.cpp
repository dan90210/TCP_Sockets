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
#include <bitset>
#include <math.h>

#define PORT 9554
#define MAXVALUE 30

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

	// Keeps track of which prime we are at
	int i = 2;
	// Bitset of all of our primes
	bitset<MAXVALUE> bitsetValues;
	// Stores the cumulative list of primes
	int primeList[MAXVALUE/2] = {0};
	// Keeps track of where the next prime will be placed in primeList
	int primeListIterator = 0;
	
	// Set all to 1
	bitsetValues.set();
	
	
	
	// while (true) {
	for (int k=i*2; k <= MAXVALUE; k += i) {
		bitsetValues[k] = false;
	}
	
	// Store the prime and print out the prime list
	primeList[primeListIterator] = i;
	primeListIterator++;
	bitsetValues[i] = 0;
	cout << "Primes: ";
	int k = 0;
	while(primeList[k] != 0) {
		cout << primeList[k] << " ";
		k++;
	}
	cout << "\n";
	
	cout << "Sent: ";
	for (int p = 2; p <= MAXVALUE; p++) {
		if (bitsetValues[p]) {
			cout << p << " ";
		}
	}
	cout << "\n\n";
	
	// Write values to server
	write(sockfd, &i, sizeof(i));
	write(sockfd, &bitsetValues, sizeof(bitsetValues));
	
	

	
	
	// Read new values from server
	read(sockfd, &i, sizeof(i));
	read(sockfd, &bitsetValues, sizeof(bitsetValues));
	
	cout << "Received: ";
	for (int p = 2; p <= MAXVALUE; p++) {
		if (bitsetValues[p]) {
			cout << p << " ";
		}
	}
	cout << "\n";
	
	primeList[primeListIterator] = i;
	primeListIterator++;
	bitsetValues[i] = 0;
	
	cout << "Primes: ";
	k = 0;
	while(primeList[k] != 0) {
		cout << primeList[k] << " ";
		k++;
	}
	cout << "\n";
	
	// Find the next prime
	while (bitsetValues[i] != 1 && i <= sqrt(MAXVALUE)) {
		i++;
	}
	
	// We have reached our highest value to check
	if (i > sqrt(MAXVALUE)) {
		// Send -1 to signal we have run out
	}
	
	
	
	close(sockfd);
	return 0;
	
	
	
	
	
	
	
	
	
	
	
	//// Create array and default everything to true
	//bool *bitsetValues = new bool[maxValue + 1];
	//for (int i = 0; i < sizeof(bitsetValues)/sizeof(bitsetValues[0]); i = i+1) {
	 // bitsetValues[i] = true;
	//}
  //
	//// Size of the array
	//write(sockfd, &maxValue, sizeof(maxValue));
	//int i = 2;
	//cout << "Prime: " << i << "\n";
	//for (int k=i*2; k<=maxValue; k += i) {
	//	bitsetValues[k] = false;
	//}
	//
	//cout << "Sent: ";
	//for (int p = 2; p<=maxValue; p++) {
	//	if (bitsetValues[p]) {
	//		cout << p << " ";
	//	}
	//}
	//cout << "\n\n";
	//
	//write(sockfd, &i, sizeof(i));
	//write(sockfd, bitsetValues, sizeof(bitsetValues));
//
	//while(true) {
	//	read(sockfd, &i, sizeof(i));
	//	read(sockfd, bitsetValues, sizeof(bitsetValues));
	//	cout << "Received: ";
	//	//int counter = 0;
	//	for (int p = 2; p<=maxValue; p++) {
	//		if (bitsetValues[p]) {
	//			cout << p << " ";
	//		}
	//	}
	//	cout << "\n";
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
	 // 
	//	cout << "Sent: ";
	//	for (int p = 2; p<=maxValue; p++) {
	//		if (bitsetValues[p]) {
	//			cout << p << " ";
	//		}
	//	}
	//	cout << "\n\n";
	//
	//	write(sockfd, &i, sizeof(i));
	//	write(sockfd, bitsetValues, sizeof(bitsetValues));
	//}
  
	//close(sockfd);
	//return 0;
}
