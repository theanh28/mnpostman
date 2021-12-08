#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define arr_len(a) sizeof((a)) / sizeof((a)[0])
#define MAXLEN 8192

// get options passed to C program
extern char *optarg;
extern int optind;

#include "connect_mod.h"
#include "request_mod.h"

// important global values
int port = 80;
char hostname[MAXLEN];
char request[MAXLEN];
// =======================

int prepare(int argc, char ** argv) {
  int opt;
  while((opt = getopt(argc, argv, "p:") >= 0)) {
    switch (opt) {
      case 'p': port = atoi(optarg); break;
    }
  }
  // parse request
  int req_result = create_request(argc, argv, optind, hostname, request);
  if (req_result < 0) return -1;
  return 0;
}

int main(int argc, char ** argv) {
  if (argc < 2) {
    printf("Wrong input. Please use ./minipostman [-p=port, ...] <request>\n");
    return -1;
  }
  if (prepare(argc, argv) < 0) {
    printf("Something was wrong during set up. Aborting...\n");
    perror("ERROR: ");
    return -1;
  }
  int clientfd = connect_to_host(hostname, port);
  if (clientfd == -2) {
    printf("Cannot find hostname from DNS\n");
    return -1;
  } else if (clientfd == -1) {
    printf("Something was wrong while trying to connect...\n");
    perror("ERROR: ");
    return -1;
  }
  printf("Connect successfully\n");

  send(clientfd, request, strlen(request), 0); // equivalent to write(fd, req, size)
  printf("Sent a request of:\n%s\n************\n", request);

  char response[MAXLEN];
  int n_received = recv(clientfd, response, MAXLEN, 0); // equivalent to read(fd, res, size)
  response[n_received] = '\0'; // response may not contain string terminating character

  printf("Recevied a response:\n%s\n############\n", response);

  // closing down, de-allocating memory
  return 0;
}
