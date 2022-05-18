#ifndef _FUNCTIONS_
#define _FUNCTIONS_
#include "requests.h"
#include "helpers.h"
#define IP "34.241.4.235"
using namespace std;
#include <string>
#include <nlohmann/json.hpp>
#include <iostream>

char *both_register_and_login(int sockfd, char* url, char* type,
                                string username, string password);

void execute_register(int sockfd, char* url, char* type,
                        string username, string password);

char* execute_login(int sockfd, char* url, char* type,
                        string username, string password);

char* execute_enter_library(int sockfd, char *url, char *cookie);

void execute_get_books();
void execute_get_book();
void execute_add_book();
void execute_delete_book();
void execute_logout();
void execute_exit();

#endif