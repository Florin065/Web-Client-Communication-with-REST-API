#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include <stdbool.h>
#include <iostream>
#include "helpers.hpp"
#include "requests.hpp"
#include "nlohmann/json.hpp"

#define HOST "34.254.242.81"
#define PORT 8080

#define POST_REGISTER   "/api/v1/tema/auth/register"
#define POST_LOGIN      "/api/v1/tema/auth/login"
#define GET_ACCESS      "/api/v1/tema/library/access"
#define BOOK            "/api/v1/tema/library/books"
#define GET_LOGOUT      "/api/v1/tema/auth/logout"
#define CONTENT_TYPE    "application/json"

#define SIZE            100

char *message;
char *response;
int sockfd;

char *input;
char *token;
char *cookies;

// Exit condition
bool leave = false;

void post_register() {
    char *username = (char *) calloc(SIZE, sizeof(char));
    char *password = (char *) calloc(SIZE, sizeof(char));

    std::cout << "username : ";
    fgets(username, SIZE, stdin);
    strtok(username, "\n");

    std::cout << "password : ";
    fgets(password, SIZE, stdin);
    strtok(password, "\n");

    nlohmann::json j {
        {"username", username},
        {"password", password}
    };

    message = compute_post_request(HOST, POST_REGISTER, CONTENT_TYPE, j, 0, NULL, 0);

    sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);

    close_connection(sockfd);
}

void post_login() {
    char *username = (char *) calloc(SIZE, sizeof(char));
    char *password = (char *) calloc(SIZE, sizeof(char));

    std::cout << "username : ";
    fgets(username, SIZE, stdin);
    strtok(username, "\n");

    std::cout << "password : ";
    fgets(password, SIZE, stdin);
    strtok(password, "\n");

    nlohmann::json j {
        {"username", username},
        {"password", password}
    };

    message = compute_post_request(HOST, POST_LOGIN, CONTENT_TYPE, j, 0, NULL, 0);

    sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);

    close_connection(sockfd);

    char *str = strstr(response, "Set-Cookie: ");

    if (str) {
        strtok(str, " ;");
        char *dummy = strtok(NULL, " ;");

        cookies = (char *) calloc(SIZE, sizeof(char));

        strcpy(cookies, dummy);
    }
}

void get_access() {

}

void get_books() {

}

void get_bookid() {

}

void post_book() {

}

void delete_bookid() {

}

void get_logout() {

}

void default_case() {
    printf("Bad command \"%s\"\n\n", input);
    printf("Available commands:\n");
    printf("register\nlogin\nenter_library\nget_books\nget_book\nadd_book\ndelete_book\nlogout\nexit\n");
    printf("\nPlease enter one of these\n");
}

int main(int argc, char const *argv[])
{
    message  = (char *) calloc(SIZE, sizeof(char));
    response = (char *) calloc(SIZE, sizeof(char));

    while(!leave) {
        input = (char*) calloc(SIZE, sizeof(char));
        fgets(input, SIZE, stdin);
        strtok(input, "\n");

        switch (input[0]) {
            case 'r':
                if (!strncmp(input, "register", 8)) {
                    post_register();
                } else {
                    default_case();
                }

                break;

            case 'l':
                if (!strncmp(input, "login", 5)) {
                    post_login();
                } else if (!strncmp(input, "logout", 6)) {
                    get_logout();
                } else {
                    default_case();
                }

                break;

            case 'e':
                if (!strncmp(input, "enter_library", 13)) {
                    get_access();
                } else if (!strncmp(input, "exit", 4)) {
                    leave = true;
                } else {
                    default_case();
                }

                break;

            case 'g':
                if (!strncmp(input, "get_books", 9)) {
                    get_books();
                } else if (!strncmp(input, "get_book", 8)) {
                    get_bookid();
                } else {
                    default_case();
                }

                break;

            case 'a':
                if (!strncmp(input, "add_book", 8)) {
                    post_book();
                } else {
                    default_case();
                }

                break;

            case 'd':
                if (!strncmp(input, "delete_book", 11)) {
                    delete_bookid();
                } else {
                    default_case();
                }
                break;

            default:
                default_case();

                break;
        }

        free(input);
    }

    free(message);
    free(response);

    return 0;
}
