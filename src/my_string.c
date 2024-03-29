#include "../include/my_string.h"

char *strdup(const char *s)
{
  size_t slen = strlen(s);
  char *result = malloc(slen + 1);
  if (result == NULL)
  {
    return NULL;
  }

  memcpy(result, s, slen + 1);
  return result;
}

char *longToStr(long num, int targetLength)
{
  static char str2[21];
  str2[0] = '\0';
  int i = 0;
  for (i = targetLength - 1; i >= 0; i--)
  {
    if (num > 0)
    {
      str2[i] = num % 10 + '0';
      num /= 10;
    }
    else
    {
      str2[i] = ' ';
    }
  }
  str2[targetLength] = '\0';
  return str2;
}

int compare_strings(const void *a, const void *b)
{
  return strcmp(*(const char **)a, *(const char **)b);
}

int string_contains(char *str, char element)
{
  for (int i = 0; i < strlen(str); i++)
  {
    if (str[i] == element)
      return 1;
  }
  return 0;
}

char *mtime_toString(const time_t mtime)
{
  struct tm *tm = localtime(&mtime);
  static char str[13] = "";
  strftime(str, 13, "%b %d %H:%M", tm);
  return str;
}

char *permission_toString(int permission)
{
  char str_permission[8];
  snprintf(str_permission, sizeof(str_permission), "%07o", permission);
  static char result[10];
  result[0] = '\0';
  for (int i = 0; i < 3; i++)
  {
    switch (str_permission[4 + i])
    {
    case '1':
      strcat(result, "--x");
      break;
    case '2':
      strcat(result, "-w-");
      break;
    case '3':
      strcat(result, "-wx");
      break;
    case '4':
      strcat(result, "r--");
      break;
    case '5':
      strcat(result, "r-x");
      break;
    case '6':
      strcat(result, "rw-");
      break;
    case '7':
      strcat(result, "rwx");
      break;

    default:
      strcat(result, "---");
      break;
    }
  }

  return result;
}

char *getUserNameFromUid(unsigned int uid)
{
  struct passwd *pws = getpwuid(uid);
  return pws->pw_name;
}

char *getGroupNameFromUid(unsigned int gid)
{
  struct group *grp = getgrgid(gid);
  return grp->gr_name;
}