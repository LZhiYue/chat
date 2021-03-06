#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

bool welcome() {
  system("reset");
  printf("+----------------------------------+\n");
  printf("|       WELCOME TO SOCKETCHAT      |\n");
  printf("+----------------------------------+\n");
  while (1) {
    printf("For signin, enter 'Y'; for exit, enter 'N': ");
    char input[1024];
    scanf("%s", input);
    if ((input[0] == 'Y' || input[0] == 'y') && strlen(input) == 1)
      return 1;
    else if ((input[0] == 'N' || input[0] == 'n') && strlen(input) == 1)
      return 0;
    else
      printf("[ERROR] Invalid input!\n");
  }
}

// 封装的 POST 方法，第一个参数为发往服务器的信息，第二个参数为接受服务器的信息
void post(char *sendMessage, char *receiveMessage) {
  int bufferSize = 1024;
  int port = 2014;
  int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in serverAddress;
  memset(&serverAddress, 0, sizeof(serverAddress));
  serverAddress.sin_family = AF_INET;  // IPv4
  serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");  // 服务器
  serverAddress.sin_port = htons(port);  // 端口
  connect(clientSocket,
          (struct sockaddr*)&serverAddress,
          sizeof(serverAddress));
  // 向服务器发送数据
  send(clientSocket, sendMessage, strlen(sendMessage), 0);
  // 读回服务器的数据
  read(clientSocket, receiveMessage, bufferSize);
  // 关闭客户端
  close(clientSocket);
}

void signout() {
  int bufferSize = 1024;
  char sendMessage[] = "SignoutRequest";
  char receiveMessage[bufferSize];
  post(sendMessage, receiveMessage);
  if (strcmp(receiveMessage, "Bye") == 0) {
    printf("Sign out successfully!\n");
    exit(0);
  } else {
    printf("[ERROR] Sign out failed! Please retry.\n");
  }
}

void groupchat() {
  system("clear");
  int bufferSize = 1024;
  char temp[] = "GroupchatRequest";
  char sendMessage[bufferSize];
  char receiveMessage[bufferSize];
  post(temp, receiveMessage);
  printf("%s\n", receiveMessage);
  printf("for checking message, enter [CHECK]\n");
  printf("for exit ,enter [EXIT]\n");
  char c = getchar();
  while (1) {
    fgets(sendMessage, bufferSize, stdin);
    sendMessage[strlen(sendMessage) - 1] = '\0';
    post(sendMessage, receiveMessage);
    printf("%s\n", receiveMessage);
    if (strcmp(receiveMessage, "EXIT") == 0) break;
  }
  system("clear");
}

void operate() {
  while (1) {
    printf("For group chat enter 'G'\n");
    printf("For exit, enter 'Q': ");
    char input[1024];
    scanf("%s", input);
    if ((input[0] == 'Q' || input[0] == 'q') && strlen(input) == 1)
      signout();
    else if ((input[0] == 'G' || input[0] == 'g') && strlen(input) == 1)
      groupchat();
    else
      printf("[ERROR] Invalid input!\n");
  }
}

void signin() {
  int bufferSize = 1024;
  char sendMessage[] = "SigninRequest";
  char receiveMessage[bufferSize];
  post(sendMessage, receiveMessage);
  printf("\nSign in successfully! The online clients are following:\n");
  printf("+-----------+----------------------+\n");
  printf("| Index     | Address              |\n");
  printf("+-----------+----------------------+\n");
  // 解析数据
  const char *split = "|";
  char *p;
  p = strtok(receiveMessage, split);
  int count = 0;
  while (p != NULL) {
    printf("| %d         | %s", ++count, p);
    for (int i = 0; i < 21 - strlen(p); i++) printf(" ");
    printf("|\n");
    p = strtok(NULL, split);
  }
  printf("+-----------+----------------------+\n");
  operate();
}

int main() {
  if (!welcome())
    return 0;
  // 登录
  printf("- Creating a client service...\n");
  printf("- Receiving message from server...\n");
  printf("- Exit automatically if failed.\n");
  signin();
  return 0;
}
