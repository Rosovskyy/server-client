#include <iostream>
#include <unistd.h>
#include <netinet/in.h>
#include <sstream>
#include <time.h>

int main() {
    struct sockaddr_in server;
    char buf[1024];
    int sd = socket(AF_INET, SOCK_STREAM, 0);
    if (sd < 0) {
        exit(EXIT_FAILURE);
    }

    memset(&server, 0, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(2250);

    int bnd_code = bind(sd,  (struct sockaddr *) &server, sizeof(server));
    if (bnd_code < 0) {
        exit(EXIT_FAILURE);
    }
    int lst_code = listen(sd, 1);
    if (lst_code < 0) {
        exit(EXIT_FAILURE);
    }

    for (;;) {
        int psd = accept(sd, 0, 0);
        if (psd < 0) {
            continue;
        }

        int cc = recv(psd, buf, 1024, 0);
        if (cc == 0) {
            exit(EXIT_SUCCESS);
        }

        std::string sendString;
        std::stringstream ss;
        if (buf[0] == 't' || buf[0] == 'd') {
            time_t now = time(0);
            struct tm tstruct;
            char time_buf[80];
            tstruct = *localtime(&now);
            if (buf[0] == 't') {
                strftime(time_buf, sizeof(time_buf), "%X", &tstruct);
                sendString = time_buf;
            } else {
                strftime(time_buf, sizeof(time_buf), "%Y-%m-%d", &tstruct);
                sendString = time_buf;
            }
        } else if (buf[0] == 'h') {
            sendString = "Hello!";
        } else if (buf[0] == 'm') {
            std::string input = buf;
            int word_count(0);
            std::stringstream ss(input);
            std::string word;
            while (ss >> word) ++word_count;
            sendString = std::to_string(--word_count);
        }

        send(psd, sendString.c_str(), strlen(sendString.c_str()), 0);

        if (close(psd) == -1) {
            exit(EXIT_FAILURE);
        }

        close(psd);
    }
}
