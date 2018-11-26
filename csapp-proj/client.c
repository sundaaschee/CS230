#include <stdio.h>
#include "csapp.h"

int fib(int n){
	if(n <= 1)
		return n;
	else
		return fib(n-1) + fib(n-2);
}

int main(int argc, char const *argv[])
{

	int clientfd = Open_clientfd ("elnux1.cs.umass.edu", 7856); //should use this to connect to the server
	if(clientfd > 0){
		printf("Connected to server.\n");
	}

	FILE* file = fdopen(clientfd, "a+");
	printf("Sending Spire ID 30479922\n");
	fprintf(file, "30479922\n");

	char *line = NULL;
	size_t count;

	getline(&line, &count, file);
	printf("Received problem: %s", line);
	
	int arg;
	count = sscanf(line, "FIB %d", &arg);
	int solution = fib(arg);

	free(line);
	line = NULL;

	printf("Sending solution: %i\n", solution);
	fprintf(file,"%i", solution);
	getline(&line, &count, file);
	printf("%s", line);

	free(line);
	line = NULL;

	printf("Sending bonus: x86\n");
	fprintf(file, "x86");
	getline(&line, &count, file);
	printf("%s", line);

	close(clientfd);
	fclose(file);
	printf("Disconnected from server.\n");
	
	return 0;
}