#ifndef MQTT3_ONLINE_NOTICE
#define MQTT3_ONLINE_NOTICE

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <errno.h>

#define DT_PORT  6789   //服务器端口
#define DT_ADDR  "127.0.0.1"  //服务器ip

int get_online_string(char *id, int online, int keepalive, char data[], int sock);
int send_online(char* cliend_id, int online, int keepalive, int sock);
int get_dt_sockfd();
int send_dt_msg(unsigned char* data, long data_len);
int send_msg(char* id, unsigned char* data, long data_len);

#endif
