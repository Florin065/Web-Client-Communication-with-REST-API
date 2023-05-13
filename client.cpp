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

#define HOST            "34.254.242.81"
#define PORT            8080

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
std::string cookies;

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

    sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

    message = compute_post_request(HOST, POST_REGISTER, CONTENT_TYPE, j, 2, NULL, 0);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts("\n");
    puts(response);

    close_connection(sockfd);
}

void post_login() {
    char *username = (char *) calloc(SIZE, sizeof(char));
    char *password = (char *) calloc(SIZE, sizeof(char));

    user:
    std::cout << "\nusername : ";
    fgets(username, SIZE, stdin);

    if (!strncmp(username, "\n", strlen("\n"))) {
        std::cout << "\nUsername doesn't exist\n";
        goto user;
    }

    pass:
    std::cout << "password : ";
    fgets(password, SIZE, stdin);

    if (!strncmp(password, "\n", strlen("\n"))) {
        std::cout << "\nPassword doesn't match\n";
        goto pass;
    }

    strtok(username, "\n");
    strtok(password, "\n");

    nlohmann::json j {
        {"username", username},
        {"password", password}
    };

    sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

    message = compute_post_request(HOST, POST_LOGIN, CONTENT_TYPE, j, 2, NULL, 0);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts("\n");
    puts(response);

    char *str = strstr(response, "Set-Cookie: ");

    if (str) {
        strtok(str, " ;");
        char *dummy = strtok(NULL, " ;");

        cookies = dummy;
    }

    // printf("Cookie : %s\n", cookies.c_str());

    close_connection(sockfd);
}

void get_access() {
    sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

    message = compute_get_request(HOST, GET_ACCESS, NULL, &cookies, 1);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts("\n");
    puts(response);

    close_connection(sockfd);

}

void get_books() {
    sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

    message = compute_get_request(HOST, BOOK, NULL, &cookies, 1);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts("\n");
    puts(response);

    close_connection(sockfd);
}

void get_bookid() {
    char *id = (char *) calloc(SIZE, sizeof(char));

    std::cout << "id : ";
    fgets(id, SIZE, stdin);
    strtok(id, "\n");

    char *book = (char *) calloc(SIZE, sizeof(char));
    strcpy(book, BOOK);
    strcat(book, "/");
    strcat(book, id);

    sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

    message = compute_get_request(HOST, book, NULL, &cookies, 1);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts("\n");
    puts(response);

    close_connection(sockfd);

}

void post_book() {
    char *id = (char *) calloc(SIZE, sizeof(char));
    char *title = (char *) calloc(SIZE, sizeof(char));
    char *author = (char *) calloc(SIZE, sizeof(char));
    char *publisher = (char *) calloc(SIZE, sizeof(char));
    char *genre = (char *) calloc(SIZE, sizeof(char));
    char *page_count = (char *) calloc(SIZE, sizeof(char));

    std::cout << "id : ";
    fgets(id, SIZE, stdin);
    strtok(id, "\n");

    std::cout << "title : ";
    fgets(title, SIZE, stdin);
    strtok(title, "\n");

    std::cout << "author : ";
    fgets(author, SIZE, stdin);
    strtok(author, "\n");

    std::cout << "publisher : ";
    fgets(publisher, SIZE, stdin);
    strtok(publisher, "\n");

    std::cout << "genre : ";
    fgets(genre, SIZE, stdin);
    strtok(genre, "\n");

    std::cout << "page_count : ";
    fgets(page_count, SIZE, stdin);
    strtok(page_count, "\n");

    nlohmann::json j {
        {"id", id},
        {"title", title},
        {"author", author},
        {"publisher", publisher},
        {"genre", genre},
        {"page_count", page_count}
    };

    sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

    message = compute_post_request(HOST, BOOK, CONTENT_TYPE, j, 6, &cookies, 1);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts("\n");
    puts(response);

    close_connection(sockfd);
}

void delete_bookid() {
    char *id = (char *) calloc(SIZE, sizeof(char));

    std::cout << "id : ";
    fgets(id, SIZE, stdin);
    strtok(id, "\n");

    char *book = (char *) calloc(SIZE, sizeof(char));
    strcpy(book, BOOK);
    strcat(book, "/");
    strcat(book, id);

    sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

    message = compute_delete_request(HOST, book, NULL, &cookies, 1);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts("\n");
    puts(response);

    close_connection(sockfd);
}

void get_logout() {
    sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

    message = compute_get_request(HOST, GET_LOGOUT, NULL, &cookies, 1);

    cookies.clear();
    cookies.shrink_to_fit();

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts("\n");
    puts(response);

    close_connection(sockfd);
}

void default_case() {
    std::cout << "Bad command \"%s\"\n\n", input;
    std::cout << "Available commands:\n";
    std::cout << "register\nlogin\nenter_library\nget_books\nget_book\nadd_book\ndelete_book\nlogout\nexit\n";
    std::cout << "\nPlease enter one of these\n";
    std::cout << "---------------------\n";
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

                    std::cout << "\nYou can now login\n";
                    std::cout << "---------------------\n";
                } else {
                    default_case();
                }

                break;

            case 'l':
                if (!strncmp(input, "login", 5)) {
                    post_login();

                    std::cout << "\nYou are now logged in\n";
                    std::cout << "---------------------\n";
                } else if (!strncmp(input, "logout", 6)) {
                    get_logout();

                    std::cout << "\nYou are now logged out\n";
                    std::cout << "---------------------\n";
                } else {
                    default_case();
                }

                break;

            case 'e':
                if (!strncmp(input, "enter_library", 13)) {
                    get_access();
                    std::cout << "\nYou have access to the library\n";
                    std::cout << "---------------------\n";
                } else if (!strncmp(input, "exit", 4)) {
                    leave = true;
                    
                    std::cout << "\nYou have exited the application\n";
                    std::cout << "---------------------\n";
                    std::cout << "Te tzuk, paa!\n";
                } else {
                    default_case();
                }

                break;

            case 'g':
                if (!strncmp(input, "get_books", 9)) {
                    get_books();

                    std::cout << "\nYou have received the information about the books\n";
                    std::cout << "---------------------\n";
                } else if (!strncmp(input, "get_book", 8)) {
                    get_bookid();

                    std::cout << "\nYou have received the book\n";
                    std::cout << "---------------------\n";
                } else {
                    default_case();
                }

                break;

            case 'a':
                if (!strncmp(input, "add_book", 8)) {
                    post_book();

                    std::cout << "\nYou have added a book\n";
                    std::cout << "---------------------\n";
                } else {
                    default_case();
                }

                break;

            case 'd':
                if (!strncmp(input, "delete_book", 11)) {
                    delete_bookid();

                    std::cout << "\nYou have deleted a book\n";
                    std::cout << "---------------------\n";
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
