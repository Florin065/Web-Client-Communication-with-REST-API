#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

int main(int argc, char *argv[])
{
    char *message;
    char *response;
    int sockfd;

    // Ex 1.1: GET dummy from main server

    char *host = "34.254.242.81";
    int port = 8080;
    char *host_port = "34.254.242.81:8080";
    char *url = "/api/v1/dummy";
    char *query_params = NULL;
    char **cookies = NULL;
    int cookies_count = 0;

    sockfd = open_connection(host, port, AF_INET, SOCK_STREAM, 0);

    message = compute_get_request(host_port, url, query_params, cookies, cookies_count);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s\n", response);

    // Ex 1.2: POST dummy and print response from main server

    char *content_type = "application/x-www-form-urlencoded";

    char *body_data[2];
    body_data[0] = calloc(strlen("a=1&b=2") + 1, sizeof(char));
    body_data[1] = calloc(strlen("a=1&b=2") + 1, sizeof(char));
    strcpy(body_data[0], "a=1&b=2");
    strcpy(body_data[1], "a=1&b=2");

    message = compute_post_request(host_port, url, content_type, body_data, 2, NULL, 0);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s\n", response);

    // Ex 2: Login into main server

    char *url_login = "/api/v1/auth/login";

    char *body_data_login[2];
    body_data_login[0] = calloc(strlen("username=student&password=student") + 1, sizeof(char));
    body_data_login[1] = calloc(strlen("username=student&password=student") + 1, sizeof(char));
    strcpy(body_data_login[0], "username=student&password=student");
    strcpy(body_data_login[1], "username=student&password=student");

    message = compute_post_request(host_port, url_login, content_type, body_data_login, 2, NULL, 0);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    printf("%s\n", response);

    // Ex 3: GET weather key from main server



    // Ex 4: GET weather data from OpenWeather API

    

    // Ex 5: POST weather data for verification to main server

    

    // Ex 6: Logout from main server

   

    // BONUS: make the main server return "Already logged in!"

    // free the allocated data at the end!

    free(message);
    free(response);
    close_connection(sockfd);

    return 0;
}
