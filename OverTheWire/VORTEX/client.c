// Author : Abhinav Thakur
// Email  : compilepeace@gmail.com


// Description : This program is meant to connect to hostname: vortex.labs.overthewire.org: 5842 and
//				 and recieve 4 integers and send back the sum to the server. As the server recieves
//				 the correct sum, it sends back the username and password to the level1



// SOME USEFULL STRUCTURES IN UNIX API ARE AS FOLLOWS
/* 

In /usr/include/netdb.h

struct addrinfo
{
  int ai_flags;                 // Input flags.  
  int ai_family;                // Protocol family for socket.  
  int ai_socktype;              // Socket type.  
  int ai_protocol;              // Protocol for socket.  
  socklen_t ai_addrlen;         // Length of socket address. 
  struct sockaddr *ai_addr;     // Socket address for socket.  
  char *ai_canonname;           // Canonical name for service location.  
  struct addrinfo *ai_next;     // Pointer to next in list.  
};



In /usr/include/netinet/in.h

// for IPv4
struct sockaddr_in
{
    __SOCKADDR_COMMON (sin_);
    in_port_t sin_port;             // Port number.  
    struct in_addr sin_addr;        // Internet address.  

    // Pad to size of `struct sockaddr'.  
    unsigned char sin_zero[sizeof (struct sockaddr) -
               __SOCKADDR_COMMON_SIZE -
               sizeof (in_port_t) -
               sizeof (struct in_addr)];
};

// Ditto, for IPv6.  
struct sockaddr_in6
{
    __SOCKADDR_COMMON (sin6_);
    in_port_t sin6_port;            // Transport layer port #
    uint32_t sin6_flowinfo;         // IPv6 flow information 
    struct in6_addr sin6_addr;      // IPv6 address 
    uint32_t sin6_scope_id;         // IPv6 scope-id 
};

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "color.h"


#define FAMILY AF_UNSPEC
#define SOCKTYPE SOCK_STREAM
#define MAXSIZE 13


int main(int argc, char *argv[])
{
	struct addrinfo hints, *server_info = NULL;
	struct addrinfo *parser = NULL;
	int sockfd = 0, status = 0;


	// check for the command line args
	if (argc != 3)
	{
		fprintf(stderr, RED"[-]"RESET" Usage: %s <HOSTNAME> <PORT>\n", argv[0]);
		exit(0x10);	
	}

	// fill in the server config to connect to and get them stored in struct addrinfo *server_info
	memset(&hints, 0, sizeof hints);
	hints.ai_family = FAMILY;
	hints.ai_socktype = SOCKTYPE;

	status = getaddrinfo(argv[1], argv[2], &hints, &server_info);
	if (status != 0x0)
	{
		fprintf(stderr, RED"[-]"RESET" getaddrinfo: ");
		exit(0x11);
	}

	// Parse through the search results (Linked list) and connect to the first we can
	for (parser = server_info; parser != NULL; parser = parser->ai_next)
	{
		// try creating a socket
		sockfd = socket(parser->ai_family, parser->ai_socktype, parser->ai_protocol);
		if (sockfd == -1)
		{
			perror(RED"[-]"RESET" socket: ");
			continue;					// try creating a socket through next node
		}

		// try connecting to server
		status = connect(sockfd, parser->ai_addr, parser->ai_addrlen);
		if (status == -1)
		{
			perror(RED"[-]"RESET" connect: ");
			continue;					// Try through the next node in linked list
		}

		break;
	}


	if (parser == NULL)
	{
		fprintf(stderr, RED"[-]"RESET" client: failed to conect after parser reached NULL (end of linked list)\n");
		exit(0x12);
	}

	void *raw_ip;
	char ip_string[INET6_ADDRSTRLEN];

	
	// If IPv4 is IPv4, convert from network to presentation form
	if (parser->ai_addr->sa_family == AF_INET)
	{
		raw_ip = &( ((struct sockaddr_in *)(parser->ai_addr))->sin_addr );
		inet_ntop(parser->ai_family, raw_ip , ip_string, sizeof ip_string);
		printf(GREEN"\n[+]"RESET" Client: connected to"YELLOW" %s\n", ip_string);
	}

	// If family is IPv6, convert from network to presentation form
	else 
	{
		raw_ip = &( ((struct sockaddr_in6 *)(parser->ai_addr))->sin6_addr );
		inet_ntop(parser->ai_family, raw_ip, ip_string, sizeof ip_string);
		fprintf(stdout, GREEN"\n[+]"RESET"Client: connected to"YELLOW" %s\n", ip_string);
	}
		
	freeaddrinfo(server_info);

	
	unsigned int i, payload = 0;
	unsigned int numbers[4];
	ssize_t bytes_recieved = 0;
	char result[1024];


	fprintf(stdout, YELLOW"\n=-=-=-=-=- NUMBERS RECIEVED (In Network Byte Order) -=-=-=-=-=\n\n");
	
	for(i = 0; i < 4; ++i)
	{
		bytes_recieved = recv(sockfd, &numbers[i], 4, 0x0);
		
		if (bytes_recieved < 4)
		{
			fprintf(stderr, RED"[-]"RESET" Error: recv()\n");
			exit(0x13);
		}

		printf(GREEN"[+]"RESET" Value%d : 0x%08x \n",i, numbers[i]);
		payload += numbers[i];
	}


    printf(GREEN"\n[+]"RESET" Payload is :"RED" 0x%08x \n", payload);
	

	send(sockfd, &payload, sizeof payload, 0);
	
	recv(sockfd, result, sizeof result, 0x0);

	
	fprintf(stdout, GREEN"[+]"RESET" Server Response :"GREEN" %s \n\n", result);


return 0;
}



