#ifndef UTILS_H_
#define UTILS_H_

#include <iostream> /* for std::cerr */
#include <memory>   /* for std::unique_ptr */

#include <arpa/inet.h> /* for socket(),  htons(), inet_pton()*/

int createTCPIPv4Socket(void);

std::unique_ptr<sockaddr_in> createIPv4Address(const char *ip_address,
                                               uint16_t port);

#endif /* UTILS_H_ */