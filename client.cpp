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
#define TWENTYTWO       22

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
    sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, ZERO);

    std::string username, password;

    std::cout << "username : ";
    std::getline(std::cin, username);

    std::cout << "password : ";
    std::getline(std::cin, password);

    nlohmann::json j {
        {"username", username},
        {"password", password}
    };

    message = compute_post_request(HOST, POST_REGISTER, CONTENT_TYPE, j, NULL);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts("\n");
    puts(response);

    close_connection(sockfd);
}

void post_login() {
    if (!cookie.empty()) {
        cookie.clear();
        cookie.shrink_to_fit();
        free(token);
        token = NULL;

        std::cout << "\nYou logged in with another account!\n";
        std::cout << "---------------------\n\n";
    }

    sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, ZERO);

    std::string username, password;

    std::cout << "username : ";
    std::getline(std::cin, username);

    std::cout << "password : ";
    std::getline(std::cin, password);

    nlohmann::json j {
        {"username", username},
        {"password", password}
    };

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
    sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, ZERO);
    std::string id;
    std::cout << "id : ";
    std::getline(std::cin, id);

    save_id = std::stoi(id);

    std::string book = BOOK;
    book += "/";
    book += id;

    message = compute_get_request(HOST, book.c_str(), NULL, NULL, ZERO, token);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts("\n");
    puts(response);

    close_connection(sockfd);

}

void add_book() {
    sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, ZERO);

    std::string title, author, genre, page_count, publisher;

    std::cout << "title : ";
    std::getline(std::cin, title);

    std::cout << "author : ";
    std::getline(std::cin, author);

    std::cout << "genre : ";
    std::getline(std::cin, genre);

    std::cout << "page_count : ";
    std::getline(std::cin, page_count);

    std::cout << "publisher : ";
    std::getline(std::cin, publisher);

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
    sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, ZERO);

    std::string id;
    std::cout << "id : ";
    std::getline(std::cin, id);

    save_id = std::stoi(id);

    std::string book = BOOK;
    book += "/";
    book += id;

    message = compute_delete_request(HOST, book.c_str(), NULL, token);

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
        input = (char *) calloc(SIZE, sizeof(char));
        fgets(input, SIZE, stdin);
        strtok(input, "\n");

        switch (input[0]) {
            case 'r':
                if (!strncmp(input, "register", EIGHT) && strlen(input) == EIGHT) {
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
                if (!strncmp(input, "login", FIVE) && strlen(input) == FIVE) {
                    post_login();

                    if (cookie.size() == ZERO) {
                        std::cout << "\nYou failed to login\n";
                        std::cout << "---------------------\n\n";
                    } else {
                        std::cout << "\nYou are now logged in\n";
                        std::cout << "---------------------\n\n";
                    }
                } else if (!strncmp(input, "logout", SIX) && strlen(input) == SIX) {
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
                if (!strncmp(input, "enter_library", THIRTEEN) && strlen(input) == THIRTEEN) {
                    get_access();

                    char *s = strstr(response, "HTTP/1.1 401 Unauthorized");

                    if (s) {
                        std::cout << "\nYou are not logged in\n";
                        std::cout << "---------------------\n\n";
                    } else {
                        std::cout << "\nYou have entered the library\n";
                        std::cout << "---------------------\n\n";
                    }
                } else if (!strncmp(input, "exit", FOUR) && strlen(input) == FOUR) {
                    leave = true;
                    
                    std::cout << "\nYou have exited the application\n";
                    std::cout << "---------------------\n\n";
                    std::cout << "Te tzuk, paa!\n";
                } else {
                    default_case();
                }

                break;

            case 'g':
                if (!strncmp(input, "get_books", NINE) && strlen(input) == NINE) {
                    get_books();

                    char *s = strstr(response, "HTTP/1.1 403 Forbidden");

                    if (s) {
                        std::cout << "\nYou have no access to the library\n";
                        std::cout << "---------------------\n\n";
                    } else {
                        std::cout << "\nYou have received the books\n";
                        std::cout << "---------------------\n\n";
                    }
                } else if (!strncmp(input, "get_book", EIGHT) && strlen(input) == EIGHT) {
                    get_bookid();

                    char *s = strstr(response, "HTTP/1.1");
                    strtok(s, "\n");

                    if (strncmp(s, "HTTP/1.1 403 Forbidden", TWENTYTWO) == ZERO) {
                        std::cout << "\nYou have no access to the library\n";
                        std::cout << "---------------------\n\n";
                    } else if (strncmp(s, "HTTP/1.1 404 Not Found", TWENTYTWO) == ZERO) {
                        std::cout << "\nYou have no book with id " << save_id << "\n";
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
                if (!strncmp(input, "add_book", EIGHT) && strlen(input) == EIGHT) {
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
                if (!strncmp(input, "delete_book", ELEVEN) && strlen(input) == ELEVEN) {
                    delete_bookid();

                    char *s = strstr(response, "HTTP/1.1");
                    strtok(s, "\n");

                    if (strncmp(s, "HTTP/1.1 403 Forbidden", TWENTYTWO) == ZERO) {
                        std::cout << "\nYou have no access to the library\n";
                        std::cout << "---------------------\n\n";
                    } else if (strncmp(s, "HTTP/1.1 404 Not Found", TWENTYTWO) == ZERO) {
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
