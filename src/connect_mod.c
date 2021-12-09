#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#include "connect_mod.h"

int connect_to_host(char * hostname, int port) {
  // AF_INET: create socket that uses IPv4; SOCK_STREAM: type of sequenced, reliable
  // 2-way connection-based byte streams ~ TCP (as opposed to datagrams ~ UDP)
  int socket_fd;
  struct sockaddr_in server_addr;
  struct hostent * hp;
  
  if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    return -1; // remember to broadcast error, saved in errno (extern int errno)
  }
  
  // geting address (contained in struct hostent) from DNS
  if ((hp = gethostbyname(hostname)) == NULL) {
    return -2; // can't find hostname from DNS
  }
  // per documentation, must zero out bytes in the 'server' socket address before filling values
  bzero((char *)&server_addr, sizeof(server_addr));
  // filling necessary values of the 'server' socket address
  server_addr.sin_family = AF_INET; // using IPv4
  server_addr.sin_port = htons(port); // assign port value, remember to change int (host order byte)
                                      // to network order byte. htons = host-to-network-short (short int)
  bcopy((char *)hp->h_addr_list[0], (char *)&server_addr.sin_addr.s_addr, hp->h_length); // IP address, got from DNS
  
  if (connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
    return -1; // error, as above
  }
  return socket_fd;
}

// copied from https://aticleworld.com/ssl-server-client-using-openssl-in-c/
SSL_CTX * init_CTX() {
  SSL_METHOD * method;
  SSL_CTX * ctx;
  OpenSSL_add_all_algorithms(); // Load cryptos, et.al.
  SSL_load_error_strings(); // Bring in and register error messages
  method = TLS_client_method(); // create new client-method instance
  if ((ctx = SSL_CTX_new(method)) == NULL) {; // create new context
    return NULL;
  }
  return ctx;
}

void SSL_show_certs(SSL * ssl) {
  X509 * cert;
  char * line;

  printf("=========\n");
  cert = SSL_get_peer_certificate(ssl);
  if (cert != NULL) {
    printf("Server certificate:\n");
    line = X509_NAME_oneline(X509_get_subject_name(cert), 0, 0);
    printf("Subject: %s\n", line);
    free(line);
    line = X509_NAME_oneline(X509_get_issuer_name(cert), 0, 0);
    printf("Issuer: %s\n", line);
    free(line);
    X509_free(cert);
  } else {
    printf("Info: No client certificates configured.\n");
  }
  printf("=========\n");
}