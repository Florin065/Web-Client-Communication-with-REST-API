#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include <iostream>
#include "helpers.hpp"
#include "requests.hpp"
#include "nlohmann/json.hpp"

char *compute_get_request(char *host, const char *url, char *query_params,
                            std::string *cookies, size_t cookies_count, char *token)
{
    char *message = new char[BUFLEN];
    char *line    = new char[LINELEN];

    // write the method name, URL and protocol type
    if (query_params) {
        sprintf(line, "GET %s?%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "GET %s HTTP/1.1", url);
    }

    compute_message(message, line);

    // add the host
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // add headers and/or cookies, according to the protocol format
    if (cookies) {
        for (size_t i = 0; i < cookies_count; ++i) {
            sprintf(line, "Cookie: %s", cookies[i].c_str());
            compute_message(message, line);
        }
    }

    if (token) {
        sprintf(line, "Authorization: Bearer %s", token);
        compute_message(message, line);
    }

    // add final new line
    compute_message(message, "");
    return message;
}

char *compute_post_request(char *host, const char *url, char *content_type, nlohmann::json body_data, char *token)
{
    char *message = (char *) calloc(BUFLEN, sizeof(char));
    char *line = (char *) calloc(LINELEN, sizeof(char));

    // write the method name, URL and protocol type
    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);

    // add the host
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // add necessary headers (Content-Type and Content-Length are mandatory)
    sprintf(line, "Content-Type: %s", content_type);
    compute_message(message, line);

	sprintf(line, "Content-Length: %ld", body_data.dump().length());
    compute_message(message, line);

    // add token
    if (token) {
        sprintf(line, "Authorization: Bearer %s", token);
        compute_message(message, line); 
    }

    // add new line at end of header
    compute_message(message, "");
    
    // add the actual payload data
    memset(line, 0, LINELEN);
    compute_message(message, body_data.dump().c_str());

    free(line);
    return message;
}

char *compute_delete_request(char *host, const char *url, char *query_params, char *token)
{
    char *message = (char *) calloc(BUFLEN, sizeof(char));
    char *line    = (char *) calloc(LINELEN, sizeof(char));

    // write the method name, URL and protocol type
    if (query_params) {
        sprintf(line, "DELETE %s?%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "DELETE %s HTTP/1.1", url);
    }

    compute_message(message, line);

    // add the host
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // add token
    if (token) {
        sprintf(line, "Authorization: Bearer %s", token);
        compute_message(message, line);
    }

    // add final new line
    compute_message(message, "");
    return message;
}
