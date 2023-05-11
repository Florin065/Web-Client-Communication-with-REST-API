#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include <stdbool.h>
#include "helpers.h"
#include "requests.h"

#define HOST "34.254.242.81"
#define PORT 8080

#define POST_REGISTER   "/api/v1/tema/auth/register"
#define POST_LOGIN      "/api/v1/tema/auth/login"
#define GET_ACCESS      "/api/v1/tema/library/access"
#define GET_BOOKS       "/api/v1/tema/library/books"
#define GET_BOOKID      "/api/v1/tema/library/books/"
#define POST_BOOK       "/api/v1/tema/library/books"
#define DELETE_BOOKID   "/api/v1/tema/library/books/"
#define GET_LOGOUT      "/api/v1/tema/auth/logout"

#define BUFFER_SIZE     100

char *message;
char *response;
int sockfd;

char *buffer;

// Exit condition
bool leave = false;
bool login = false;

char *token;

bool check_num(char *string) {
    for (size_t i = 0; i < strlen(string) - 1; i++) {
        if (string[i] < '0' || string[i] > '9') {
            return false;
        }
    }

    return true;
}

char *json_input_parsing(char *serialized_string) {
    char *username = calloc(BUFFER_SIZE, sizeof(char));
    char *password = calloc(BUFFER_SIZE, sizeof(char));

    user:
    printf("username=");
    fgets(username, BUFFER_SIZE, stdin);

    if (strncmp(username, "\n", strlen("\n")) == 0) {
        printf("User does not exist\n");
        goto user;
    }

    printf("password=");
    fgets(password, BUFFER_SIZE, stdin);

    if (strncmp(password, "\n", strlen("\n")) == 0) {
        printf("Password does not exist\n");
        goto user;
    }

    strtok(username, "\n");
    strtok(password, "\n");

    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);

    json_object_set_string(root_object, "username", username);
    json_object_set_string(root_object, "password", password);

    serialized_string = json_serialize_to_string_pretty(root_value);

    return serialized_string;
}

void json_add_parsing(char *serialized_string) {
    id:
    printf("id=");
    char *id = calloc(BUFFER_SIZE, sizeof(char));
    fgets(id, BUFFER_SIZE, stdin);
    strtok(id, "\n");

    if (!check_num(id)) {
        printf("Id :(\n");
        goto id;
    }

    title:
    printf("title=");
    char *title = calloc(BUFFER_SIZE, sizeof(char));
    fgets(title, BUFFER_SIZE, stdin);

    if (strncmp(title, "\n", strlen("\n")) == 0) {
        printf("Title :(\n");
        goto title;
    }

    author:
    printf("author=");
    char *author = calloc(BUFFER_SIZE, sizeof(char));
    fgets(author, BUFFER_SIZE, stdin);

    if (strncmp(author, "\n", strlen("\n")) == 0) {
        printf("Author :(\n");
        goto author;
    }

    publisher:
    printf("publisher=");
    char *publisher = calloc(BUFFER_SIZE, sizeof(char));
    fgets(publisher, BUFFER_SIZE, stdin);

    if (strncmp(publisher, "\n", strlen("\n")) == 0) {
        printf("Publisher :(\n");
        goto publisher;
    }

    genre:
    printf("genre=");
    char *genre = calloc(BUFFER_SIZE, sizeof(char));
    fgets(genre, BUFFER_SIZE, stdin);

    if (strncmp(genre, "\n", strlen("\n")) == 0) {
        printf("Genre :(\n");
        goto genre;
    }

    page_count:
    printf("page_count=");
    char *page_count = calloc(BUFFER_SIZE, sizeof(char));
    fgets(page_count, BUFFER_SIZE, stdin);
    strtok(page_count, "\n");

    if (!check_num(page_count)) {
        printf("Page count :(\n");
        goto page_count;
    }

    strtok(id, "\n");
    strtok(title, "\n");
    strtok(author, "\n");
    strtok(publisher, "\n");
    strtok(genre, "\n");
    strtok(page_count, "\n");

    JSON_Value *root_value = json_value_init_object();
    JSON_Object *root_object = json_value_get_object(root_value);

    json_object_set_string(root_object, "id", id);
    json_object_set_string(root_object, "title", title);
    json_object_set_string(root_object, "author", author);
    json_object_set_string(root_object, "publisher", publisher);
    json_object_set_string(root_object, "genre", genre);
    json_object_set_string(root_object, "page_count", page_count);

    serialized_string = json_serialize_to_string_pretty(root_value);

    json_value_free(root_value);
}

void post_register() {
    sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

    char *serialized_string = calloc(BUFFER_SIZE, sizeof(char));

    printf("salut\n");

    json_input_parsing(serialized_string);

        printf("salut1\n");

    printf("%s\n", serialized_string);
        printf("%s\n", serialized_string);
            printf("%s\n", serialized_string);

    message = compute_post_request(HOST, POST_REGISTER, "application/json", serialized_string, 0, NULL, 0);

            printf("salut1\n");

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    printf("%s\n", response);

    close_connection(sockfd);
}

void post_login() {
    sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

    if (login) {
        printf("You are already logged in\n");

        return;
    }

    char *serialized_string = NULL;
    json_input_parsing(serialized_string);

    message = compute_post_request(HOST, POST_LOGIN, "application/json", serialized_string, 0, NULL, 0);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    printf("%s\n", response);

    char *cookies;
    char *rest = response;

    while ((cookies = strtok_r(rest, "\n", &rest))) {
        if (strstr(cookies, "Set-Cookie:")) {
            token = strtok(cookies, ";");
            token = strtok(token, ":");
            token = strtok(0, " ");

            break;
        }
    }

    login = true;

    close_connection(sockfd);
}

void get_access() {
    sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

    message = compute_get_request(HOST, GET_ACCESS, NULL, &token, 0);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    printf("%s\n", response);

    char *tokens;
    char *rest = response;

    while ((tokens = strtok_r(rest, "\n", &rest))) {
        if (strstr(tokens, "token:")) {
            token = strtok(tokens, ":");
            token = strtok(0, " ");

            break;
        }
    }

    close_connection(sockfd);
}

void get_books() {
    sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

    message = compute_get_request(HOST, GET_BOOKS, NULL, &token, 0);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    printf("%s\n", response);

    close_connection(sockfd);
}

void get_bookid() {
    sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

    id:
    printf("id=");
    char *id = calloc(BUFFER_SIZE, sizeof(char));
    fgets(id, BUFFER_SIZE, stdin);
    strtok(id, "\n");

    if (!check_num(id)) {
        printf("Invalid id\n");
        goto id;
    }

    char book[60];

    strcpy(book, GET_BOOKID);
    strcat(book, id);

    message = compute_get_request(HOST, book, NULL, &token, 0);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    printf("%s\n", response);

    free(id);

    close_connection(sockfd);
}

void post_book() {
    sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);
    
    char *serialized_string = NULL;
    json_add_parsing(serialized_string);

    message = compute_post_request(HOST, POST_BOOK, "application/json", serialized_string, 0, &token, 1);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    printf("%s\n", response);

    free(serialized_string);

    close_connection(sockfd);
}

void delete_bookid() {
    sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

    id:
    printf("id=");
    char *id = calloc(BUFFER_SIZE, sizeof(char));
    fgets(id, BUFFER_SIZE, stdin);
    strtok(id, "\n");

    if (!check_num(id)) {
        printf("Invalid id\n");
        goto id;
    }

    char book[60];

    strcpy(book, DELETE_BOOKID);
    strcat(book, id);

    message = compute_delete_request(HOST, book, NULL, &token, 0);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    printf("%s\n", response);

    free(id);

    close_connection(sockfd);
}

void get_logout() {
    sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

    message = compute_get_request(HOST, GET_LOGOUT, NULL, &token, 0);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    printf("%s\n", response);

    token = NULL;
    token = NULL;

    login = false;

    close_connection(sockfd);
}

void default_case() {
    printf("Bad command \"%s\"\n\n", buffer);
    printf("Available commands:\n");
    printf("register\nlogin\nenter_library\nget_books\nget_book\nadd_book\ndelete_book\nlogout\nexit\n");
    printf("\nPlease enter one of these\n");
}

int main(int argc, char *argv[])
{
    message = calloc(BUFFER_SIZE, sizeof(char));
    response = calloc(BUFFER_SIZE, sizeof(char));

    while(!leave) {
        buffer = calloc(BUFFER_SIZE, sizeof(char));
        fgets(buffer, BUFFER_SIZE, stdin);
        strtok(buffer, "\n");

        switch (buffer[0]) {
        case 'r':
            if (strncmp(buffer, "register", 8) == 0) post_register();
            else default_case();
            
            break;
        case 'l':
            if (strncmp(buffer, "login", 5) == 0) post_login();
            else if (strncmp(buffer, "logout", 6) == 0) get_logout();
            else default_case();

            break;
        case 'e':
            if (strncmp(buffer, "enter_library", 13) == 0) get_access();
            else if (strncmp(buffer, "exit", 4) == 0) leave = true;
            else default_case();
            
            break;
        case 'g':
            if (strncmp(buffer, "get_books", 9) == 0) get_books();
            else if (strncmp(buffer, "get_book", 8) == 0) get_bookid();
            else default_case();
            
            break;
        case 'a':
            if (strncmp(buffer, "add_book", 8) == 0) post_book();
            else default_case();
            
            break;
        case 'd':
            if (strncmp(buffer, "delete_book", 11) == 0) delete_bookid();
            else default_case();

            break;
        default:
            default_case();

            break;
        }

    }

    free(buffer);
    free(message);
    free(response);

    return 0;
}
