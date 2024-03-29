#include "../include/afficher.h"
#include <dirent.h>

void afficher_element(const char *str, long int num, int limitvalue)
{
  if (str != NULL)
  {
    if (limitvalue)
    {
      printf("%*s", limitvalue, str);
    }
    else
    {
      printf("%s", str);
    }
  }
  else if (num != 0)
  {
    printf("%ld", num);
  }
}

void afficher_str(const char *str, int limitvalue)
{
  afficher_element(str, 0, limitvalue);
}

void afficher_num(long int num, int limitvalue)
{
  afficher_element(NULL, num, limitvalue);
}

void afficher_space(int i) { afficher_element(" ", 0, i); }

void afficher_newline() { afficher_element("\n", 0, 1); }

void afficher_fichier(const struct stat buf, const struct dirent *dir, char *commands, char *realname) {
  if ((dir->d_name[0] == '.' || strlen(dir->d_name) < 1) &&
      !string_contains(commands, 'a') && !string_contains(commands, 'f') &&
      !string_contains(commands, 'd') && !string_contains(commands, 'A'))
    return;

  if ((strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) && string_contains(commands, 'A') && !string_contains(commands, 'f')) return;

  char file_name[255];
  if (strlen(realname) > 0) {
    strcpy(file_name, realname);
  } else {
    strcpy(file_name, dir->d_name);
  }

  char *type = "-";
  if (dir->d_type == 4) {
    type = "d";
    strcpy(file_name, ANSI_COLOR_LIGHT_BLUE);
    if (strlen(realname) > 0) strcat(file_name, realname);
    else strcat(file_name, dir->d_name);
    strcat(file_name, ANSI_COLOR_RESET);
  }

  if (string_contains(commands, 'l')) afficher_str(type, 1);

  if (string_contains(commands, 'l')) {
    afficher_str(permission_toString(buf.st_mode), 0);
    afficher_space(1);
  }

  if (string_contains(commands, 'l')) {
    afficher_str(longToStr(buf.st_nlink, 1), 0);
    afficher_space(1);
  }

  if (string_contains(commands, 'l')) {
    afficher_str(getUserNameFromUid(buf.st_uid), 0);
    afficher_space(1);
  }

  if (string_contains(commands, 'l')) {
    afficher_str(getGroupNameFromUid(buf.st_gid), 7);
    afficher_space(1);
  }

  if (string_contains(commands, 'l')) {
    afficher_str(longToStr(buf.st_size, 6), 9);
    afficher_space(1);
  }

  if (string_contains(commands, 'l')) {
    afficher_str(mtime_toString(buf.st_mtime), 0);
    afficher_space(1);
  }

  afficher_str(file_name, 0);

  if (string_contains(commands, 'l'))
    afficher_newline();
  else
    afficher_space(2);
}
