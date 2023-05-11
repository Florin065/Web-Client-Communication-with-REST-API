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
#include "parson.h"

#define HOST "34.254.242.81"
#define PORT 8080

#define POST_REGISTER   "/api/v1/tema/auth/register"
#define POST_LOGIN      "/api/v1/tema/auth/login"
#define GET_ACCESS      "/api/v1/tema/library/access"
#define GET_BOOKS       "/api/v1/tema/library/books"
// #define GET_BOOKID      "/api/v1/tema/library/books/:bookId."
#define POST_BOOK       "/api/v1/tema/library/books"
// #define DELETE_BOOKID   "/api/v1/tema/library/books/:bookId."
#define GET_LOGOUT      "/api/v1/tema/auth/logout"

#define BUFFER_SIZE     100

char *message;
char *response;
int sockfd;

char *buffer;

// Exit condition
bool leave = false;
bool login = false;

char **cookie;
char *token;

char *json_input_parsing(char *serialized_string) {
    char *username = calloc(BUFFER_SIZE, sizeof(char));
    char *password = calloc(BUFFER_SIZE, sizeof(char));

    user:
    printf("username=");
    fgets(username, BUFFER_SIZE, stdin);

    if (strncmp(username, "\n", 1) == 0) {
        printf("User does not exist\n");
        goto user;
    }

    printf("password=");
    fgets(password, BUFFER_SIZE, stdin);

    if (strncmp(password, "\n", 1) == 0) {
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
}

void post_register() {
    char *serialized_string = NULL;
    json_input_parsing(serialized_string);

    message = compute_post_request(HOST, POST_REGISTER, "application/json", serialized_string, 0, NULL, 0);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    printf("%s\n", response);
}

void post_login() {
    if (login) {
        printf("You are already logged in\n");

        return;
    }

    char *serialized_string = NULL;
    json_input_parsing(serialized_string);

    message = compute_post_request(HOST, POST_LOGIN, "application/json", &serialized_string, NULL, NULL, 0);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    printf("%s\n", response);

    char **cookies;
    char *rest = response;

    while ((cookies = strtok_r(rest, "\n", &rest))) {
        if (strstr(cookies, "Set-Cookie:")) {
            cookie = strtok(cookies, ";");
            cookie = strtok(cookie, ":");
            cookie = strtok(0, " ");

            break;
        }
    }

    login = true;
}

void get_access() {

}

void get_books() {

}

void get_bookid() {

}

void post_book() {

}

void delete_bookid(int id) {

}

void get_logout() {
    message = compute_get_request(HOST, GET_LOGOUT, NULL, cookie, 0);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    printf("%s\n", response);

    cookie = NULL;
    token = NULL;

    login = false;
}

void default_case() {
    printf("Bad command \"%s\"\n\n", buffer);
    printf("Available commands:\n");
    printf("register\nlogin\nenter_library\nget_books\nget_book\nadd_book\ndelete_book\nlogout\nexit\n");
    printf("\nPlease enter one of these\n");
}

int main(int argc, char *argv[])
{
    sockfd = open_connection(HOST, PORT, AF_INET, SOCK_STREAM, 0);

    while(!leave) {
        buffer = calloc(BUFFER_SIZE, sizeof(char));
        fgets(buffer, BUFFER_SIZE, stdin);
        strtok(buffer, "\n");

        switch (buffer[0]) {
        case 'r':
            if (strncmp(buffer, "register", 8) == 0) post_register();
            else default_case;
            
            break;
        case 'l':
            if (strncmp(buffer, "login", 5) == 0) post_login();
            else if (strncmp(buffer, "logout", 6) == 0) get_logout();
            else default_case;

            break;
        case 'e':
            if (strncmp(buffer, "enter_library", 13) == 0) get_access();
            else if (strncmp(buffer, "exit", 4) == 0) leave = true;
            else default_case;
            
            break;
        case 'g':
            if (strncmp(buffer, "get_books", 9) == 0) get_books();
            else if (strncmp(buffer, "get_book", 8) == 0) get_bookid();
            else default_case;
            
            break;
        case 'a':
            if (strncmp(buffer, "add_book", 8) == 0) post_book();
            else default_case;
            
            break;
        case 'd':
            if (strncmp(buffer, "delete_book", 11) == 0) {
                char *id = strtok(buffer, " ");
                delete_bookid(id);
            } else default_case;

            break;
        default:
            default_case;

            break;
        }

        close_connection(sockfd);
    }

    free(buffer);
    free(message);
    free(response);
    close_connection(sockfd);

    return 0;
}
