#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#define arr_len(a) sizeof((a)) / sizeof((a)[0])
#define MAXLEN 8192

// get options passed to C program
extern char *optarg;
extern int optind;

#include "connect_mod.h"
#include "request_mod.h"

// important global values
int port = -969, show_cert = 0;
int clientfd = -1;
char hostname[MAXLEN];
char request[MAXLEN];
char response[MAXLEN];
int * tls_mode = 0;
SSL_CTX * ctx = 0;
SSL * ssl = 0;
// =======================

int prepare(int argc, char ** argv) {
  tls_mode = (int *)malloc(sizeof(int));
  *tls_mode = 0;
  int opt;
  while((opt = getopt(argc, argv, "p:c")) >= 0) {
    switch (opt) {
      case 'p': port = atoi(optarg); break;
      case 'c': show_cert = 1; break;
    }
  }
  // parse request
  int req_result = create_request(argc, argv, optind, hostname, request, tls_mode);
  if (port == -969) port = (*tls_mode ? 443 : 80);
  if (req_result < 0) return -1;
  return 0;
}

void output_http() {
  send(clientfd, request, strlen(request), 0); // equivalent to write(fd, req, size)
  printf("Sent a request of:\n%s\n************\n", request);

  int n_received = recv(clientfd, response, MAXLEN, 0); // equivalent to read(fd, res, size)
  response[n_received] = '\0'; // response may not contain string terminating character

  printf("Recevied a response:\n%s\n############\n", response);
}
void output_ssl_tls() { // mirror output_http()
  SSL_write(ssl, request, strlen(request)); 
  printf("Sent a request of:\n%s\n************\n", request);

  int n_received = SSL_read(ssl, response, MAXLEN);
  response[n_received] = 0; // terminating character '\0' = 0 in ASCII

  printf("Recevied a response:\n%s\n############\n", response);
}

int main(int argc, char ** argv) {
  if (argc < 2) {
    printf("Wrong input. Please use ./minipostman [-p=port, ...] <request>\n");
    return -1;
  }
  if (prepare(argc, argv) < 0) {
    printf("Something was wrong during set up. Aborting...\n");
    perror("ERROR");
    return -1;
  }

  clientfd = connect_to_host(hostname, port);
  if (clientfd == -2) {
    printf("Cannot find hostname from DNS\n");
    return -1;
  } else if (clientfd == -1) {
    printf("Something was wrong while trying to connect...\n");
    perror("ERROR");
    return -1;
  }
    if (*tls_mode == 1) {
      ctx = init_CTX();
      ssl = SSL_new(ctx); // create new SSL connection state
      SSL_set_fd(ssl, clientfd); // bind socket descriptor to ssl state
      if (SSL_connect(ssl) == -1) {
        ERR_print_errors_fp(stderr);
        return -1;
      }
      if (show_cert) SSL_show_certs(ssl);
    }
  printf("Connect successfully\n");

  if (*tls_mode == 0) {
    output_http();
  } else if (*tls_mode == 1) {
    output_ssl_tls();
  }

  // closing down, de-allocating memory
  free(tls_mode);
  SSL_free(ssl);
  return 0;
}
