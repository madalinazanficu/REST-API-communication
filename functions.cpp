#include "functions.h"

bool check_page_count(string page_count) {
    for (int i = 0; i < page_count.length(); i++) {
        if (page_count[i] > '9' || page_count[i] < '0') {
            cout << "Error: Page is not respecting the format!\n";
            return false;
        }
    }
    return true;
}

bool check_id(string id) {
    for (int i = 0; i < id.length(); i++) {
        if (id[i] > '9' || id[i] < '0') {
            cout << "Error: Id is not respecting the format!\n";
            return false;
        }
    }
    return true;
}

char *both_register_and_login(int sockfd, char* url, char* type,
                                string username, string password) {

    // Write in json format
    nlohmann::json j  = {{"username", username}, {"password", password}};

    // Convert the Json in string type
    string j_str = j.dump();

    // Convert the string in char * to match compute_post_request parmeter
    char data[j_str.length() + 1];
    strcpy(data, j_str.c_str());

    // Compute the request
    char *to_send = compute_post_request(IP, url, type, data, 1, NULL, 0, NULL);

    // Send and receive
    send_to_server(sockfd, to_send);
    char *to_recv = receive_from_server(sockfd);
    return to_recv;
}

void execute_register(int sockfd, char* url, char* type,
                        string username, string password) {

    // Make a request to the server and get a respons
    char *to_recv = both_register_and_login(sockfd, url, type, username, password);

    // In case there is the error: "username is taken"
    char *payload = basic_extract_json_response(to_recv);
    if (payload != NULL) {
        cout << payload << "\n";
    } else {
        cout << "Utilizatorul " << username << " a fost inregistrat cu succes!\n";
    }

    free(to_recv);
}

char* execute_login(int sockfd, char* url, char* type,
                        string username, string password) {

    // Make a request to the server and get a respons
    char *to_recv = both_register_and_login(sockfd, url, type, username, password);

    // In case there is the error "unmached creditentials":
    char *payload = basic_extract_json_response(to_recv);
    if (payload != NULL) {
        cout << payload << "\n";
        return nullptr;
    } else {
        cout << "User " << username << " login successfully!\n";
    }

    // Cookie stuff is from server respons: "Set-Cookie: cookie"
    char *cookie = extract_json_cookie(to_recv);
    free(to_recv);
    return cookie;
}

char* execute_enter_library(int sockfd, char *url, char *cookie) {
    // Error message: not demonstrating autenthification
    if (cookie == nullptr) {
        cout << "Error: Not logged in yet\n";
        return nullptr;
    }

    // Make a get request to the server
    char *message_get = compute_get_request(IP, url, NULL, cookie, NULL);
    send_to_server(sockfd, message_get);
    char *to_recv = receive_from_server(sockfd);

    
    char *payload = basic_extract_json_response(to_recv);
    if (payload != NULL) {
        cout << "Successfully get access to the library!\n";
    } else {
        cout << "Something went wrong!\n";
        return nullptr;
    }

    // Extract JWT token from to_recv - demonstrate the access to library
    char *token = extract_token(to_recv);

    free(to_recv);
    return token;
}

void execute_get_books(int sockfd, char *url, char *JWT_token, int flag) {
    // Error message: not demonstarting access to library
    if (JWT_token == nullptr) {
        cout << "Error: Don't have access to the library\n";
        return;
    }
    // Make a request to the server to get the books
    char *message_get = compute_get_request(IP, url, NULL, NULL, JWT_token);
    send_to_server(sockfd, message_get);
    char *to_recv = receive_from_server(sockfd);

    // Extract book or books detalis
    char *payload = extract_json_array(to_recv);

    // Check for no books
    if (payload == nullptr) {
        cout << "No book available\n";
    } else {
        cout << payload << endl;
    }
    free(to_recv);
}

void execute_add_book(int sockfd, char *url, char* type, char *JWT_token,
                        string title, string author, string genre,
                        int page_count, string publisher) {

    // Error message: not demonstarting access to library
    if (JWT_token == nullptr) {
        cout << "Error: Don't have access to the library\n";
        return;
    }
    // Write in json format
    nlohmann::json j  = {{"title", title}, {"author", author}, {"genre", genre},
                        {"page_count", page_count}, {"publisher", publisher}};
    
    // Convert the Json in string type
    string j_str = j.dump();

    // Convert the string in char * to match compute_post_request parmeter
    char data[j_str.length() + 1];
    strcpy(data, j_str.c_str());

    // Compute the request
    char *to_send = compute_post_request(IP, url, type, data, 1,
                                            NULL, 0, JWT_token);
    // Send and receive
    send_to_server(sockfd, to_send);
    char *to_recv = receive_from_server(sockfd);
}

void execute_logout(int sockfd, char *url, char* cookie) {
    // Error message: not demonstrating autenthification
    if (cookie == nullptr) {
        cout << "Error: You are not logged in\n";
        return;
    }
    // Make a request to logout
    char *message_get = compute_get_request(IP, url, NULL, cookie, NULL);

    send_to_server(sockfd, message_get);
    char *to_recv = receive_from_server(sockfd);
    char *payload = basic_extract_json_response(to_recv);

    if (payload == nullptr) {
        cout << "User disconnected successfully!\n";
    } else {
        cout << payload << endl;
    }
    free(to_recv);
}

void execute_delete_book(int sockfd, char *url, char *JWT_token, int id) {
    // Error message: not demonstarting access to library
    if (JWT_token == nullptr) {
        cout << "Error: Don't have access to the library\n";
        return;
    }
    // Make a request to the server to get the books
    char *message_get = compute_delete_request(IP, url, JWT_token);
    send_to_server(sockfd, message_get);
    char *to_recv = receive_from_server(sockfd);

    // Extract book or books detalis
    char *payload = basic_extract_json_response(to_recv);
    if (payload != NULL) {
        cout << payload << endl;
    } else {
        cout << "Delete the book with id " << id << " successfully\n";
    }
    free(to_recv);
}
