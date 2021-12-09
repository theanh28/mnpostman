#ifndef CONNECT_MOD_H
#define CONNECT_MOD_H

int connect_to_host(char *, int);
SSL_CTX* init_CTX();
void SSL_show_certs(SSL *);

#endif
