#include "functions.h"

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
    char *to_send = compute_post_request(IP, url, type, data, 1, NULL, 0);

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
        cout << "Not logged in yet\n";
        return nullptr;
    }

    // Make a get request to the server
    char *message_get = compute_get_request(IP, url, NULL, cookie);
    send_to_server(sockfd, message_get);
    char *to_recv = receive_from_server(sockfd);

    
    char *payload = basic_extract_json_response(to_recv);
    if (payload != NULL) {
        cout << "Successfully get acces to the library!\n";
    } else {
        cout << "Something went wrong!\n";
        return nullptr;
    }

    // Extract the JWT token from to_recv - demonstrate the access to the library
    char *token = extract_token(to_recv);

    free(to_recv);
    return token;
}

void execute_get_books() {

}
void execute_get_book() {

}
void execute_add_book() {

}
void execute_delete_book() {

}
void execute_logout() {

}
void execute_exit() {

}