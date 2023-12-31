#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define SERVER_ADDR "127.0.0.1"

int main(int argc, char const *argv[])
{

  /*  Create socket file descriptor.
   *
   *  socket(domain, type, protocol)
   *
   *  domain: specifies communication domain (AF_LOCAL for processes on same host,
   *            AF_INET for processes on different hosts connected with IPV4)
   *  type: tcp (SOCK_STREAM) or udp (SOCK_DGRAM)
   *  protocol: IANA protocol number (see: https://www.iana.org/assignments/protocol-numbers/protocol-numbers.xhtml)
   *              specifies which protocol should be used on the socket.
   */
  int client_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (client_fd < 0)
  {
    perror("socket creation failed");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in serv_addr = {
      .sin_family = AF_INET,   /* Type of address (IPv4) */
      .sin_port = htons(PORT), /* Port Number. */
  };
  int addrlen = sizeof(serv_addr);
  struct sockaddr *addr_ptr = (struct sockaddr *)&serv_addr;

  /* Convert IPv4/IPv6 addresses from string to binary form */
  if (inet_pton(AF_INET, SERVER_ADDR, &serv_addr.sin_addr) <= 0)
  {
    printf("Invalid or unsupported address\n");
    exit(EXIT_FAILURE);
  }

  /* Attempts to connect the client socket to the specified address. */
  if (connect(client_fd, addr_ptr, addrlen) < 0)
  {
    printf("Connection failed\n");
    exit(EXIT_FAILURE);
  }

  /* Send data to server. */
  char message[100];
  printf("Enter your message: ");
  fgets(message, sizeof(message), stdin);

  send(client_fd, message, strlen(message), 0);

  /* Read and print data from server. */
  char buffer[1024] = {0};
  read(client_fd, buffer, 1024);
  printf("%s\n", buffer);

  /* Close connection between client and server. */
  close(client_fd);

  return 0;
}