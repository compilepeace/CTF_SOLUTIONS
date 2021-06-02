/* 
 * Author: Abhinav Thakur
 * Email : compilepeace@gmail.com
 *
 * Description: Program to feed input to input2 challenge of pwnable.kr, before compiling the
 * 		source code, edit the TARGET_PATH macro to the absolute pathname of target
 * 		binary.
 *
 */


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define TARGET_PATH 		"./input"
#define CREATE_FILE_PATH	"\x0a"
#define PORT_NUMBER 		"37660"
#define PIPE_READ 		0
#define PIPE_WRITE		1

extern char **environ;			// to use environment variables

int main (int argc, char *argv[], char *envp[]) {

	pid_t pid;
	int pipefd[2];
	char *t_argv[101] = { };

	/* STAGE 1 (argv) */
	for (int i = 0; i < 101; ++i) 
		t_argv[i] = "A";

	t_argv[0] = TARGET_PATH; 
	t_argv['A'] = "\x00";
	t_argv['B'] = "\x20\x0a\x0d";
	t_argv['C'] = PORT_NUMBER;			
	t_argv[100] = NULL;		


	/* STAGE 3 (environment) */
	if (setenv ("\xde\xad\xbe\xef", "\xca\xfe\xba\xbe", 1) == -1 ) {
		perror ("while setting environment variable \"\\xde\\xad\\xbe\\xef\"=\"\\xca\\xfe\\xba\\xbe\"");
		exit (1);
	}

	if (pipe(pipefd) == -1) {
		perror ("while creating pipe for I/O with target");
		exit (1);
	}

	pid = fork ();
	if (pid == -1) {
		perror ("failed to create a child process");
		exit (1);
	}

	if (pid == 0) { 
		// child process : launch the target process
		
		// close the write end of child's pipe
		close ( pipefd[PIPE_WRITE] );
		
		// duplicate child pipe's read end to fd 0/2 
		if (	dup2 ( pipefd[PIPE_READ], 0) == -1 ||
			dup2 ( pipefd[PIPE_READ], 2) == -1) {
			perror ("error while duplicating child's read end of pipe to fd 0/2");
			exit (1);
		}

		// launch the target
		if (execve (TARGET_PATH, t_argv, environ) == -1) {
			perror ("execve failed");
			exit(1);
		}
	}

	if (pid > 0) {
		// Parent process
		fprintf (stderr, "PID of target process: %d\n", pid);
		

		/* STAGE 2 (STDIO) */
		// close the read end of parent pipe (to allow only outgoing data from parent)
		close ( pipefd[PIPE_READ] );
		write (pipefd[PIPE_WRITE], "\x00\x0a\x00\xff", 4);
		write (pipefd[PIPE_WRITE], "\x00\x0a\x02\xff", 4);


		/* STAGE 4 (FILE) */
		int fd = open (CREATE_FILE_PATH, O_CREAT|O_EXCL|O_WRONLY, S_IRUSR|S_IRGRP|S_IROTH);
		if (fd == -1) {
			perror ("while creating file \\x0a");
		}
		write (fd, "\x00\x00\x00\x00", 4);
		close (fd);


		/* STAGE 5 (NETWORK) */
		struct sockaddr_in host_details;
		int sockfd;

		sockfd = socket (AF_INET, SOCK_STREAM, 0);
		if (sockfd == -1) {
			perror ("while creating socket");
			exit (1);
		}

		host_details.sin_family 	= AF_INET;
		host_details.sin_port  		= htons (atoi(PORT_NUMBER));
		host_details.sin_addr.s_addr	= inet_addr("127.0.0.1");			// local host address

		sleep(5);
		if (connect (sockfd, (struct sockaddr *)&host_details, sizeof(struct sockaddr)) == -1) {
			perror ("while connecting to host");
			exit (1);
		}	
		
		if (send (sockfd, "\xde\xad\xbe\xef", 4, 0) != 4) {
			perror ("while sending final bytes to the target");
			exit (1);
		}
		
		fprintf (stderr, "closing socket, bye!\n");
		close(sockfd);
	}
}
