#include <stdio.h>	//printf
#include <string.h>	//strlen
#include <sys/socket.h>	//socket
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>
#include <stdlib.h>

#define SIZE 1024

void send_file(int sock)
{
  FILE *fp;
  char data[SIZE] = {0};
  char *filename = "send1.txt";
  char message[1024];
  fp = fopen(filename, "r");
  if (fp == NULL)
  {
    perror("[-]Error in reading file.");
    exit(1);
  }

  while (fgets(data, SIZE, fp) != NULL)
  {
    if (send(sock, data, strlen(data), 0) == -1)
    {
      perror("[-]Error in sending file.");
      exit(1);
    }
    bzero(data, SIZE);
  }
  fclose(fp);
  return;
}

void write_file(int sock)
{
  FILE *fp;
  char *filename = "clientrecv1.txt";
  char buffer[SIZE];
  char message[1024];
  fp = fopen("clientrecv1.txt", "w");
  printf("shich file would you like: ");
  scanf("%s", message);
  send(sock, message, strlen(message), 0);
  while (1)
  {
    int n = recv(sock, buffer, SIZE, 0);
    puts(buffer);
    //there was error handling here but yeahh its gone now
    fprintf(fp, "%s", buffer);
    bzero(buffer, SIZE);
    fclose(fp);
    return;
  }
}

int main(int argc, char *argv[])
{
  int sock;
  struct sockaddr_in server;
  char message[1024], server_reply[2000];

  //Create socket
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0)
  {
    perror("[-]Error in socket");
    exit(1);
  }
  FILE *fp;
  char *filename = "send1.txt";
  if (sock == -1)
  {
    printf("Could not create socket");
  }
  puts("Socket created");

  server.sin_addr.s_addr = inet_addr("127.0.0.1");
  server.sin_family = AF_INET;
  server.sin_port = htons(8080);

  //Connect to remote server
  if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
  {
    perror("connect failed. Error");
    return 1;
  }

  puts("Connected\n");

  //keep communicating with server
  int choice;
  do
  {

    printf("\n\tMENU DEMONSTRATION");
    printf("\n\t------------------------------");
    printf("\n\n\t 1. OPTION 1: Upload file ");
    printf("\n\t 2. OPTION 2: Download file");
    printf("\n\t 3. OPTION 3: Upload entered text");
    printf("\n\t 4. EXIT");
    printf("\n\n Enter Your Choice: ");
    scanf("%d", &choice);
    switch (choice)
    {
    case 1: //upload file
      printf("\n\nYOU SELECTED OPTION 1\n");
      send_file(sock);
      break;
    case 2: // Download file: this only works if "send_file(sd);" is uncommented in file_server
      printf("\n\nYOU SELECTED OPTION 2\n");
      write_file(sock);
      break;
    case 3:
      printf("\n\nYOU SELECTED OPTION 3\n");
      printf("Enter message : ");
      scanf("%s", message);
      if (send(sock, message, strlen(message), 0) < 0)
      {
        puts("Send failed");
      }
      break;
    case 4:
      printf("\n\nYOU SELECTED OPTION 4\n");
      break;
    default:
      printf("\n\nINVALID SELECTION...Please try again\n");
      scanf("%*s");
    }

  } while (choice != 4);

  close(sock);
  return 0;
}
