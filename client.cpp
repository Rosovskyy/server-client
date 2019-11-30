#include <iostream>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]){

    struct sockaddr_in server;
    char buf[1024];
    std::string arguments;
    if (argc > 2) {
        for (int i = 2; i < argc-1; i++) {
            arguments += argv[i];
            if (argc != 2) {
                arguments += " ";
            }
        }
        arguments += argv[argc-1];
    }
    strcpy(buf, arguments.c_str());

    int sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0) {
        exit(EXIT_FAILURE);
    }

    memset(&server, 0, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(2250);

    int inet = inet_aton(argv[1], &server.sin_addr);
    if (inet <= 0) {
        exit(EXIT_FAILURE);
    }

    int res = connect(sd, (struct sockaddr *) &server, sizeof(server));
    if (res == -1) {
        exit(EXIT_FAILURE);
    }

    if (write(sd, buf, strlen(buf)) != strlen(buf)) {
        exit(EXIT_FAILURE);
    }

    char res_buffer[1024];
    recv(sd, res_buffer, sizeof(res_buffer), 0);
    printf("Server: %s\n", res_buffer);

    return 0;
}
