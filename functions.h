#ifndef _FUNCTIONS_
#define _FUNCTIONS_
#include "requests.h"
#include "helpers.h"
#define IP "34.241.4.235"
using namespace std;
#include <string>
#include <nlohmann/json.hpp>
#include <iostream>

bool check_page_count(string page_count);
bool check_id(string id);

char *both_register_and_login(int sockfd, char* url, char* type,
                                string username, string password);

void execute_register(int sockfd, char* url, char* type,
                        string username, string password);

char* execute_login(int sockfd, char* url, char* type,
                        string username, string password);

char* execute_enter_library(int sockfd, char *url, char *cookie);

void execute_get_books(int sockfd, char *url, char *token, int flag);

void execute_add_book(int sockfd, char *url, char* type, char *JWT_token,
                        string title, string author, string genre,
                        int page_count, string publisher);


void execute_delete_book(int sockfd, char *url, char *JWT_token, int id);
void execute_logout(int sockfd, char *url, char *cookie);

#endif