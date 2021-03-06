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
#define MAXVALUE 11500

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
	bitset<MAXVALUE>* bitsetValues = new bitset<MAXVALUE>();
	// Stores the cumulative list of primes
	int primeList[(int)ceil(sqrt(MAXVALUE))] = {0};
	// Keeps track of where the next prime will be placed in primeList
	int primeListIterator = 0;
	// We look for this code from the other side to know if we are done
	int code = -1;
	
	// Set all to 1
	bitsetValues->set();
	
	// Set all multiples of i to false
	for (int k=i*2; k <= MAXVALUE; k += i) {
		(*bitsetValues)[k] = 0;
	}
	
	// Store the prime and print out the prime list
	primeList[primeListIterator] = i;
	primeListIterator++;
	(*bitsetValues)[i] = 0;
	
	// Print out primes
	cout << "Primes: ";
	int k = 0;
	while(primeList[k] != 0) {
		cout << primeList[k] << " ";
		k++;
	}
	cout << "\n";
	
	// Print out sent
	cout << "Sent: ";
		// We want to print a, b, c, ... x, y, z
		if (bitsetValues->count() > 6) {
			int startCount = 0;
			int safetyCheck = 2;
			while (startCount < 3 && safetyCheck < MAXVALUE) {
				if ((*bitsetValues)[safetyCheck]) {
					cout << safetyCheck << " ";
					startCount++;
				}
				safetyCheck++;
			}
			
			cout << "... ";
			
			int endCount = 3;
			int safetyCheck1 = MAXVALUE;
			int a[3];
			
			while (endCount > 0 && safetyCheck1 > 0) {
				if ((*bitsetValues)[safetyCheck1]) {
					a[endCount-1] = safetyCheck1;
					endCount--;
				}
				safetyCheck1--;
			}
			
			for (int z = 0; z < 3; z++) {
				cout << a[z] << " ";
			}	
		} 
		// We want to print all of what we sent
		else {
			for (int p = 2; p <= MAXVALUE; p++) {
				if ((*bitsetValues)[p]) {
					cout << p << " ";
				}
			}
		}
		cout << "\n\n";
	
	// Write values to server
	write(sockfd, &i, sizeof(i));
	write(sockfd, bitsetValues, sizeof(*bitsetValues));

	// Do this until we are done
	while (true) {
		// Read new values from server
		read(sockfd, &i, sizeof(i));
		// We got to sqrt(MAXVALUE)
		if (i == code) {
			break;
		}
		read(sockfd, bitsetValues, sizeof(*bitsetValues));

		// Print out received
		cout << "Received: ";
		// We want to print a, b, c, ... x, y, z
		if (bitsetValues->count() > 6) {
			int startCount = 0;
			int safetyCheck = 2;
			while (startCount < 3 && safetyCheck < MAXVALUE) {
				if ((*bitsetValues)[safetyCheck]) {
					cout << safetyCheck << " ";
					startCount++;
				}
				safetyCheck++;
			}
			
			cout << "... ";
			
			int endCount = 3;
			int safetyCheck1 = MAXVALUE;
			int a[3];
			
			while (endCount > 0 && safetyCheck1 > 0) {
				if ((*bitsetValues)[safetyCheck1]) {
					a[endCount-1] = safetyCheck1;
					endCount--;
				}
				safetyCheck1--;
			}
			
			for (int z = 0; z < 3; z++) {
				cout << a[z] << " ";
			}	
		} 
		// We want to print all of what we received
		else {
			for (int p = 2; p <= MAXVALUE; p++) {
				if ((*bitsetValues)[p]) {
					cout << p << " ";
				}
			}
		}
		cout << "\n";
		
		// Store the prime and print out the prime list
		primeList[primeListIterator++] = i;
		(*bitsetValues)[i] = 0;
		
		// Find the next prime
		while ((*bitsetValues)[i] != 1 && i <= sqrt(MAXVALUE)) {
			i++;
		}
		
		// We have reached our highest value to check
		if (i > sqrt(MAXVALUE)) {
			// Send -1 to signal we have run out
			int code1 = -1;
			write(sockfd, &code1, sizeof(code1));
			break;
		}
		
		// Set multiples of i to false
		for (int k=i*2; k <= MAXVALUE; k += i) {
			(*bitsetValues)[k] = 0;
		}	
		
		// Store the prime and print out the prime list
		primeList[primeListIterator++] = i;
		(*bitsetValues)[i] = 0;
		
		// Print out all of our primes
		cout << "Primes: ";
		int k = 0;
		while(primeList[k] != 0) {
			cout << primeList[k] << " ";
			k++;
		}
		cout << "\n";
		
		// Print out what we sent
		cout << "Sent: ";
		// We want to print a, b, c, ... x, y, z
		if (bitsetValues->count() > 6) {
			int startCount = 0;
			int safetyCheck = 2;
			while (startCount < 3 && safetyCheck < MAXVALUE) {
				if ((*bitsetValues)[safetyCheck]) {
					cout << safetyCheck << " ";
					startCount++;
				}
				safetyCheck++;
			}
			
			cout << "... ";
			
			int endCount = 3;
			int safetyCheck1 = MAXVALUE;
			int a[3];
			
			while (endCount > 0 && safetyCheck1 > 0) {
				if ((*bitsetValues)[safetyCheck1]) {
					a[endCount-1] = safetyCheck1;
					endCount--;
				}
				safetyCheck1--;
			}
			
			for (int z = 0; z < 3; z++) {
				cout << a[z] << " ";
			}	
		} 
		// We want to print all of what we sent
		else {
			for (int p = 2; p <= MAXVALUE; p++) {
				if ((*bitsetValues)[p]) {
					cout << p << " ";
				}
			}
		}
		cout << "\n\n";
		
		// Write values to server
		write(sockfd, &i, sizeof(i));
		write(sockfd, bitsetValues, sizeof(*bitsetValues));
	}
	
	// Print our final list of primes
	cout << "\nFinal list of Primes: ";
	int p = 0;
	while(primeList[p] != 0) {
		cout << primeList[p] << " ";
		p++;
	}
	// Everything left in bitsetValues is prime
	int m = 2;
	while (m <= MAXVALUE) {
		if ((*bitsetValues)[m]) {
			cout << m << " ";
		}
		m++;
	}
	
	delete bitsetValues;
	
	cout << "\n\n";
	close(sockfd);
	return 0;
}
