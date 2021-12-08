#include <string.h>
#include <stdlib.h>

#include "request_mod.h"

int create_request(int argc, char ** argv, int offset, char * hostname, char * req) {
  strcpy(req, argv[offset]);
  char * url = argv[offset + 1];

  if (strcasestr(url, "https://") != NULL) {
    url += strlen("https://");
  }
  if (strcasestr(url, "http://") != NULL) {
    url += strlen("http://");
  }

  int hostname_len = strcspn(url, "/");
  strncpy(hostname, url, hostname_len);
  hostname[hostname_len] = '\0';

  url += hostname_len;
  if (*url == '\0') strcpy(url, "/");

  sprintf(req + strlen(req), " %s HTTP/1.1\r\n\
HOST: %s\r\n\
Content-Type: text/plain\r\n\
\r\n", url, hostname);

  return 0;
}