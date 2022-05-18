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
#include <string>
#include <iostream>
#include "functions.h"
#include <nlohmann/json.hpp>

using namespace std;


void read_commands() {
    char ip[14] = "34.241.4.235";
    int sockfd;
    bool exit = false;
    char *cookie = nullptr;
    char *token = nullptr;

    while (exit == false) {
        // Conectrea la server
        sockfd = open_connection(ip, 8080, AF_INET, SOCK_STREAM, 0);

        // Citirea comenzii curente
        string command;
        getline(cin, command);

        if (command.compare("register") == 0) {
            cout << "REGISTER\n";

            cout << "username:\n";
            string username;
            getline(cin, username);

            cout << "password:\n";
            string password;
            getline(cin, password);
            
            char url[28] = "/api/v1/tema/auth/register";
            char type[18] = "application/json";
            execute_register(sockfd, url, type, username, password);
        }
        if (command.compare("login") == 0) {
            cout << "LOGIN\n";

            cout << "username:\n";
            string username;
            getline(cin, username);

            cout << "password:\n";
            string password;
            getline(cin, password);

            char url[28] = "/api/v1/tema/auth/login";
            char type[18] = "application/json";

            // Received the cookie
            cookie = execute_login(sockfd, url, type, username, password);
        }
        if (command.compare("enter_library") == 0) {
            cout << "ENTER LIBRARY\n";
            char url[28] = "/api/v1/tema/library/access";

            // Received the access token
            token = execute_enter_library(sockfd, url, cookie);
        }
        if (command.compare("get_books") == 0) {
            execute_get_books();
        }
        if (command.compare("get_book") == 0) {
            execute_get_book();
        }
        if (command.compare("add_book") == 0) {
            execute_add_book();
        }
        if (command.compare("delete_book") == 0) {
            execute_delete_book();
        }
        if (command.compare("logout") == 0) {
            execute_logout();
        }
        if (command.compare("exit") == 0) {
            cout << "Exit\n";
            execute_exit();
            exit = true;
            break;
        }
        // Deconectarea se realizeaza dupa fiecare operatie
        close_connection(sockfd);        
    }
}


int main(int argc, char *argv[])
{
    read_commands();
    return 0;
}
