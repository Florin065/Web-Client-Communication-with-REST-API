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

#define SIZE            1000

#define ZERO            0
#define ONE             1
#define FOUR            4
#define FIVE            5
#define SIX             6
#define EIGHT           8
#define NINE            9
#define ELEVEN          11
#define THIRTEEN        13
#define TWENTY          20

char *message;
char *response;
size_t sockfd;

char *input;

std::string cookie;
char *token;

size_t save_id;

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

    sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, ZERO);

    message = compute_post_request(HOST, POST_REGISTER, CONTENT_TYPE, j, NULL);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts("\n");
    puts(response);

    close_connection(sockfd);
}

void post_login() {
    char *username = (char *) calloc(SIZE, sizeof(char));
    char *password = (char *) calloc(SIZE, sizeof(char));

    std::cout << "\nusername : ";
    fgets(username, SIZE, stdin);
    strtok(username, "\n");

    std::cout << "password : ";
    fgets(password, SIZE, stdin);
    strtok(password, "\n");

    nlohmann::json j {
        {"username", username},
        {"password", password}
    };

    sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, ZERO);

    message = compute_post_request(HOST, POST_LOGIN, CONTENT_TYPE, j, NULL);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts("\n");
    puts(response);

    char *s = strstr(response, "Set-Cookie: ");

    if (s) {
        strtok(s, " ;");
        char *dummy = strtok(NULL, " ;");

        cookie = dummy;
    }

    std::cout << "\nCookie : " << cookie << "\n";

    close_connection(sockfd);
}

void get_access() {
    sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, ZERO);

    message = compute_get_request(HOST, GET_ACCESS, NULL, &cookie, ONE, NULL);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts("\n");
    puts(response);

    char *s = strstr(response, "token");

    if (s) {
        strtok(s, "\":");
        char *dummy = strtok(NULL, "\":");

        token = (char *) calloc(SIZE, sizeof(char));
        strcpy(token, dummy);
    }

    std::cout << "\nToken : " << token << "\n";

    close_connection(sockfd);
}

void get_books() {
    sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, ZERO);

    message = compute_get_request(HOST, BOOK, NULL, NULL, ZERO, token);

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

    save_id = atoi(id);

    char *book = (char *) calloc(SIZE, sizeof(char));
    strcpy(book, BOOK);
    strcat(book, "/");
    strcat(book, id);

    sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, ZERO);

    message = compute_get_request(HOST, book, NULL, NULL, ZERO, token);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts("\n");
    puts(response);

    close_connection(sockfd);

}

void add_book() {
    char *title = (char *) calloc(SIZE, sizeof(char));
    char *author = (char *) calloc(SIZE, sizeof(char));
    char *genre = (char *) calloc(SIZE, sizeof(char));
    char *page_count = (char *) calloc(SIZE, sizeof(char));
    char *publisher = (char *) calloc(SIZE, sizeof(char));

    std::cout << "title : ";
    fgets(title, SIZE, stdin);
    strtok(title, "\n");

    std::cout << "author : ";
    fgets(author, SIZE, stdin);
    strtok(author, "\n");

    std::cout << "genre : ";
    fgets(genre, SIZE, stdin);
    strtok(genre, "\n");

    std::cout << "page_count : ";
    fgets(page_count, SIZE, stdin);
    strtok(page_count, "\n");

    std::cout << "publisher : ";
    fgets(publisher, SIZE, stdin);
    strtok(publisher, "\n");

    sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, ZERO);

    nlohmann::json j {
        {"title", title},
        {"author", author},
        {"genre", genre},
        {"page_count", page_count},
        {"publisher", publisher},
    };

    message = compute_post_request(HOST, BOOK, CONTENT_TYPE, j, token);

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

    save_id = atoi(id);

    char *book = (char *) calloc(SIZE, sizeof(char));
    strcpy(book, BOOK);
    strcat(book, "/");
    strcat(book, id);

    sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, ZERO);

    message = compute_delete_request(HOST, book, NULL, token);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts("\n");
    puts(response);

    close_connection(sockfd);
}

void get_logout() {
    sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, ZERO);

    message = compute_get_request(HOST, GET_LOGOUT, NULL, &cookie, ONE, NULL);

    cookie.clear();
    cookie.shrink_to_fit();
    
    free(token);
    token = NULL;

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts("\n");
    puts(response);

    close_connection(sockfd);
}

void default_case() {
    printf("Bad command \"%s\"\n\n", input);
    std::cout << "Available commands:\n";
    std::cout << "register\nlogin\nenter_library\nget_books\nget_book\nadd_book\ndelete_book\nlogout\nexit\n";
    std::cout << "\nPlease enter one of these\n";
    std::cout << "---------------------\n\n";
}

int main()
{
    message  = (char *) calloc(SIZE, sizeof(char));
    response = (char *) calloc(SIZE, sizeof(char));

    while(!leave) {
        input = (char*) calloc(SIZE, sizeof(char));
        fgets(input, SIZE, stdin);
        strtok(input, "\n");

        switch (input[0]) {
            case 'r':
                if (!strncmp(input, "register", EIGHT)) {
                    post_register();

                    char *s = strstr(response, "HTTP/1.1 400 Bad Request");

                    if (s) {
                        std::cout << "\nUsername already exists!\n";
                        std::cout << "---------------------\n\n";
                    } else {
                        std::cout << "\nAccount created successfully!\n";
                        std::cout << "---------------------\n\n";
                    }
                } else {
                    default_case();
                }

                break;

            case 'l':
                if (!strncmp(input, "login", FIVE)) {
                    post_login();

                    if (cookie.size() == ZERO) {
                        std::cout << "\nYou failed to login\n";
                        std::cout << "---------------------\n\n";
                    } else {
                        std::cout << "\nYou are now logged in\n";
                        std::cout << "---------------------\n\n";
                    }
                } else if (!strncmp(input, "logout", SIX)) {
                    get_logout();
                    
                    char *s = strstr(response, "HTTP/1.1 400 Bad Request");

                    if (s) {
                        std::cout << "\nYou are not logged in\n";
                        std::cout << "---------------------\n\n";
                    } else {
                        std::cout << "\nYou successfully logged out\n";
                        std::cout << "---------------------\n\n";
                    }
                } else {
                    default_case();
                }

                break;

            case 'e':
                if (!strncmp(input, "enter_library", THIRTEEN)) {
                    get_access();

                    char *s = strstr(response, "HTTP/1.1 401 Unauthorized");

                    if (s) {
                        std::cout << "\nYou are not logged in\n";
                        std::cout << "---------------------\n\n";
                    } else {
                        std::cout << "\nYou have entered the library\n";
                        std::cout << "---------------------\n\n";
                    }
                } else if (!strncmp(input, "exit", FOUR)) {
                    leave = true;
                    
                    std::cout << "\nYou have exited the application\n";
                    std::cout << "---------------------\n\n";
                    std::cout << "Te tzuk, paa!\n";
                } else {
                    default_case();
                }

                break;

            case 'g':
                if (!strncmp(input, "get_books", NINE)) {
                    get_books();

                    char *s = strstr(response, "HTTP/1.1 403 Forbidden");

                    if (s) {
                        std::cout << "\nYou have no access to the library\n";
                        std::cout << "---------------------\n\n";
                    } else {
                        std::cout << "\nYou have received the books\n";
                        std::cout << "---------------------\n\n";
                    }
                } else if (!strncmp(input, "get_book", EIGHT)) {
                    get_bookid();

                    char *s = strstr(response, "HTTP/1.1 403 Forbidden");

                    if (s) {
                        std::cout << "\nYou have no access to the library\n";
                        std::cout << "---------------------\n\n";
                    } else {
                        std::cout << "\nYou have received the book with id " << save_id << "\n";
                        std::cout << "---------------------\n\n";
                    }
                } else {
                    default_case();
                }

                break;

            case 'a':
                if (!strncmp(input, "add_book", EIGHT)) {
                    add_book();

                    char *s = strstr(response, "HTTP/1.1 403 Forbidden");

                    if (s) {
                        std::cout << "\nYou have no access to the library\n";
                        std::cout << "---------------------\n\n";
                    } else {
                        std::cout << "\nYou have added the book\n";
                        std::cout << "---------------------\n\n";
                    }
                } else {
                    default_case();
                }

                break;

            case 'd':
                if (!strncmp(input, "delete_book", ELEVEN)) {
                    delete_bookid();

                    char *s = strstr(response, "HTTP/1.1");
                    strtok(s, "\n");

                    if (strncmp(s, "HTTP/1.1 403 Forbidden", TWENTY)) {
                        std::cout << "\nYou have no access to the library\n";
                        std::cout << "---------------------\n\n";
                    } else if (strncmp(s, "HTTP/1.1 404 Not Found", TWENTY)) {
                        std::cout << "\nYou have no book with id " << save_id << "\n";
                        std::cout << "---------------------\n\n";
                    } else {
                        std::cout << "\nYou have deleted the book with id " << save_id << "\n";
                        std::cout << "---------------------\n\n";
                    }
                } else {
                    default_case();
                }
                
                break;

            default:
                default_case();

                break;
        }
    }

    return 0;
}
