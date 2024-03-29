#include <grp.h>
#include <pwd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char *strdup(const char *s);
char *longToStr(long num, int targetLength);
int compare_strings(const void *a, const void *b);
int string_contains(char *str, char element);
char *permission_toString(int permission);
char *mtime_toString(const time_t mtime);
char *getUserNameFromUid(unsigned int uid);
char *getGroupNameFromUid(unsigned int gid);