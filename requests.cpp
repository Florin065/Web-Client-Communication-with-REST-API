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

char *compute_get_request(char *host, char *url, char *query_params,
                            std::string *cookies, int cookies_count)
{
    char *message = (char *) calloc(BUFLEN, sizeof(char));
    char *line    = (char *) calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL, request params (if any) and protocol type

    if (query_params) {
        sprintf(line, "GET %s?%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "GET %s HTTP/1.1", url);
    }

    compute_message(message, line);

    // Step 2: add the host

    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // Step 3 (optional): add headers and/or cookies, according to the protocol format

    if (cookies) {
        sprintf(line, "Cookie: %s", *cookies);
        compute_message(message, line);
    }
    // Step 4: add final new line

    sprintf(line, "");
    compute_message(message, "");
    return message;
}

char *compute_post_request(char *host, char *url, char* content_type, nlohmann::json body_data,
                            int body_data_fields_count, std::string *cookies, int cookies_count)
{
    char *message = (char *) calloc(BUFLEN, sizeof(char));
    char *line = (char *) calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL and protocol type

    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);
    
    // Step 2: add the host

    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    /* Step 3: add necessary headers (Content-Type and Content-Length are mandatory)
            in order to write Content-Length you must first compute the message size
    */

    sprintf(line, "Content-Type: %s", content_type);
    compute_message(message, line);

	sprintf(line, "Content-Length: %ld", body_data.dump().length());
    compute_message(message, line);

    // Step 4 (optional): add cookies

    if (cookies) {
        sprintf(line, "Cookies: %s", *cookies);
        compute_message(message, line); 
    }

    // Step 5: add new line at end of header

    compute_message(message, "");

    // Step 6: add the actual payload data
    
    memset(line, 0, LINELEN);
    compute_message(message, body_data.dump().c_str());

    free(line);
    return message;
}

char *compute_delete_request(char *host, char *url, char *query_params,
                            std::string *cookies, int cookies_count)
{
    char *message = (char *) calloc(BUFLEN, sizeof(char));
    char *line    = (char *) calloc(LINELEN, sizeof(char));

    // Step 1: write the method name, URL, request params (if any) and protocol type

    if (query_params) {
        sprintf(line, "DELETE %s?%s HTTP/1.1", url, query_params);
    } else {
        sprintf(line, "DELETE %s HTTP/1.1", url);
    }

    compute_message(message, line);

    // Step 2: add the host

    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    // Step 3 (optional): add headers and/or cookies, according to the protocol format

    if (cookies) {
        sprintf(line, "Cookie: %s", *cookies);
        compute_message(message, line);
    }
    // Step 4: add final new line

    sprintf(line, "");
    compute_message(message, "");
    return message;
}
