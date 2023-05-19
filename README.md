# Web-Client-Communication-with-REST-API

## Homework structure

* nlohmann/ - json for modern c++ (https://github.com/nlohmann/json)
    * json_fwd.hpp
    * json.hpp
* buffer.cpp
* buffer.hpp
* client.cpp
* helper.cpp
* helper.hpp
* Makefile
* requests.cpp
* requests.hpp

## Implementation
* The code includes several header files for standard input/output, networking, and JSON parsing.
* It defines constants for the host, port, API endpoints, and other values.
* Global variables are declared for storing the request message, response, socket file descriptor, input, and
authentication tokens.
* Functions are defined for each operation:
    * post_register(): Allows a user to register a new account by sending a POST request to the server.
    * post_login(): Handles user login by sending a POST request with username and password.
    * get_access(): Requests access to the library by sending a GET request to the server.
    * get_books(): Retrieves a list of books from the library by sending a GET request.
    * get_bookid(): Retrieves details of a specific book by its ID using a GET request.
    * add_book(): Adds a new book to the library by sending a POST request with book details.
    * delete_bookid(): Deletes a book from the library by its ID using a DELETE request.
    * get_logout(): Performs user logout by sending a GET request.
    * default_case(): Displays an error message for invalid commands.
* The main function sets up a loop that reads user input and performs the corresponding operation based on the
command.
* The code handles authentication tokens, cookies, and server responses to determine the success or failure of
each operation.

## About client
* The program waits for commands from the user in an infinite loop. Once a command is received, the program checks
the first letter of the word, entering a specific case in the switch. If the command is not correct, an appropriate
message will be displayed.
* When the register command is received, we open a connection to the server, after which the username and password
are read from the keyboard and the json object is created in which the username and password are stored. Then, the
request message is created and sent, then the response is awaited, displayed and the connection with the server is
closed.
* When the login command is received, we open a connection to the server, after which the username and password are
read from the keyboard and the json object is created. Then, the request message is created and sent, then the response
is awaited, it is displayed. If the request was successful, the cookie is saved for the current user. At the end, the
connection with the server is closed.
* When the enter_library command is received, a connection to the server is opened, the message for the server is
created, the message is sent and the response from the server is awaited. If there were no problems, the token for
permission to access the library is saved, the answer is displayed and the connection to the server is closed.
* When the get_books command is received, we open a connection to the server, creates the message for the server,
sends the message, displays the response and closes the connection.
* When the get_book command is received, we open the connection to the server, read the id, create the message for
the server, open the connection, send the message, display the answer and the connection is closed.
* When the add_book command is received, we open a connection to the server, read the book data, create the json
object, create the message to the server, send, display the response and close the connection.
* When the delete_book command is received, a connection to the server is opened, the id is read, the message for
the server is created, it is sent, the response is displayed and the connection is closed.
* When the logout command is received, a connection to the server is opened, the message for the server is created,
the request is sent to the server and the response is displayed. The data for cookies and authorizations are cleared
and the connection is closed
* When the exit command is received, the loop is exited and the application is closed.

## More infos
* I used the functions given in lab 9 as a starting point for solving the homework. I modified the functions to
add the token for permission to modify the library. I also added a function for creating a request for DELETE
requests. This creates the add method DELETE, the host and the header for the authorization token.

## Account
username: adrianelicopterdelupta
password: tiktok