#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "my_string.h"

#define ANSI_COLOR_BLUE "\033[34m"
#define ANSI_COLOR_LIGHT_BLUE "\033[94m"
#define ANSI_COLOR_RESET "\033[0m"

extern char *valideCommands;
extern char *bonusCommands;

void afficher_element(const char *str, long int num, int limitvalue);
void afficher_str(const char *str, int limitvalue);
void afficher_num(long int num, int limitvalue);
void afficher_space(int i);
void afficher_newline();
void afficher_fichier(const struct stat buf, const struct dirent *dir, char *commands, char *realname);