#include <stdio.h>
#include "send_msg.h"

int dt_sockfd = -1;
struct sockaddr_in servaddr;
struct sockaddr_in hk_servaddr;

int get_online_string(char *id, int online, int keepalive, char data[], int sock) {
    snprintf(data, 70, "%s%s%s%d%s%d%s%d%s", "k100$", id, "&k101$", online, "&k102$", keepalive, "&k103$", sock, "\r\n\r\n");
    return 1;
}

int get_dt_sockfd() {
    if ((dt_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        close(dt_sockfd);
        dt_sockfd = -1;
        memset(&servaddr, 0, sizeof (servaddr));
        return -3;
    }
    if (servaddr.sin_port == 0) {
        servaddr.sin_family = AF_INET;
        servaddr.sin_port = htons(DT_PORT);
        if (inet_pton(AF_INET, DT_ADDR, &servaddr.sin_addr) <= 0) {
            close(dt_sockfd);
            dt_sockfd = -1;
            memset(&servaddr, 0, sizeof (servaddr));
            return -4;
        }
    }
    if (connect(dt_sockfd, (struct sockaddr*) &servaddr, sizeof (servaddr)) < 0) {
        close(dt_sockfd);
        dt_sockfd = -1;
        memset(&servaddr, 0, sizeof (servaddr));
        return -5;
    } 
    return dt_sockfd;
}

int send_online(char* id, int online, int keepalive, int sock) {
    if (!id) {
        return 0;
    }
    char data[70] = {0};
    if ((id[0] == 'd') && (id[1] == 't')) {
        get_online_string(id, online, keepalive, data, sock);
        return send_dt_msg((unsigned char*) data, strlen(data));
    }
    return 0;
}

int send_dt_msg(unsigned char* data, long data_len) {
    if (dt_sockfd < 0) {
        dt_sockfd = get_dt_sockfd();
        if (dt_sockfd < 0) {
            dt_sockfd = get_dt_sockfd();
            if (dt_sockfd < 0) {
                return 0;
            }
        }
    }
    int n = send(dt_sockfd, data, data_len, 0);
    if (n < 0) {
        close(dt_sockfd);
        dt_sockfd = get_dt_sockfd();
        n = send(dt_sockfd, data, data_len, 0);
        if (n < 0) {
            close(dt_sockfd);
            dt_sockfd = get_dt_sockfd();
            n = send(dt_sockfd, data, data_len, 0);
            if (n < 0) {
                close(dt_sockfd);
                dt_sockfd = -1;
                n = -6;
            }
        }
    }
    return n;
}

int send_msg(char* id, unsigned char* data, long data_len) {
    if (!id) {
        return 0;
    }
    if ((id[0] == 'd') && (id[1] == 't')) {
        return send_dt_msg(data, data_len);
    }
    return 0;
}
