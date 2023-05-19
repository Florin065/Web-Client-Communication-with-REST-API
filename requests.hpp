#ifndef _REQUESTS_
#define _REQUESTS_

#include "nlohmann/json.hpp"

// computes and returns a GET request string (query_params
// and cookies can be set to NULL if not needed)
char *compute_get_request(char *host, const char *url, char *query_params,
							std::string *cookies, size_t cookies_count, char *token);

// computes and returns a POST request string (cookies can be NULL if not needed)
char *compute_post_request(char *host, const char *url, char *content_type, nlohmann::json body_data, char *token);

// computes and returns a DELETE request string (cookies can be NULL if not needed)
char *compute_delete_request(char *host, const char *url, char *query_params, char *token);

#endif
