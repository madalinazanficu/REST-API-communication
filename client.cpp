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
            int flag = 0;
            cout << "GET BOOKS\n";
            char url[28] = "/api/v1/tema/library/books";
            execute_get_books(sockfd, url, token, flag);
        }
        if (command.compare("get_book") == 0) {
            int flag = 1;
            cout << "GET BOOK\n";
            cout << "Enter id:\n";

            string buffer_id;
            getline(cin, buffer_id);
            if (check_id(buffer_id) == true) {
                char url[40] = "/api/v1/tema/library/books/";
                strcat(url, buffer_id.c_str());
                url[strlen(url)] = '\0';

                execute_get_books(sockfd, url, token, flag);
            }
        }
        if (command.compare("add_book") == 0) {
            cout << "ADD BOOK\n";
            cout << "title:\n";
            string title;
            getline(cin, title);

            cout << "author:\n";
            string author;
            getline(cin, author);

            cout << "genre:\n";
            string genre;
            getline(cin, genre);

            cout << "publisher:\n";
            string publisher;
            getline(cin, publisher);

            cout << "page_count:\n";
            string buff_page_count;
            getline(cin, buff_page_count);

            // Checking the format for the page count
            if (check_page_count(buff_page_count) == true) {
                int page_count = stoi(buff_page_count);

                char url[28] = "/api/v1/tema/library/books";
                char type[18] = "application/json";

                execute_add_book(sockfd, url, type, token, title,
                            author, genre, page_count, publisher);
            }
        }
        if (command.compare("delete_book") == 0) {
            cout << "Enter id:\n";
            string buffer_id;
            getline(cin, buffer_id);
            if (check_id(buffer_id) == true) {
                int id = stoi(buffer_id);
                char url[40] = "/api/v1/tema/library/books/";
                strcat(url, buffer_id.c_str());
                execute_delete_book(sockfd, url, token, id);
            }
        }
        if (command.compare("logout") == 0) {
            cout << "Logout\n";
            char url[28] = "/api/v1/tema/auth/logout";
            execute_logout(sockfd, url, cookie);
            cookie = nullptr;
            token = nullptr;
        }
        if (command.compare("exit") == 0) {
            cout << "Exit\n";
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
