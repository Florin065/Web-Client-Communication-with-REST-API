#ifndef _REQUESTS_
#define _REQUESTS_

#include "nlohmann/json.hpp"

// computes and returns a GET request string (query_params
// and cookies can be set to NULL if not needed)
char *compute_get_request(char *host, char *url, char *query_params,
							std::string *cookies, int cookies_count);

// computes and returns a POST request string (cookies can be NULL if not needed)
char *compute_post_request(char *host, char *url, char* content_type, nlohmann::json body_data,
							int body_data_fields_count, std::string *cookies, int cookies_count);

// computes and returns a DELETE request string (cookies can be NULL if not needed)
char *compute_delete_request(char *host, char *url, char *query_params,
							std::string *cookies, int cookies_count);

#endif
