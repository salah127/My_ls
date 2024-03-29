#include "afficher.h"

struct RessourceList {
  char *path;
  size_t length;
  size_t capacity;
  struct dirent **base;
};

struct RessourceList *ressource_load(const char *path);
struct dirent *ressource_getFileInFolder(char *path, char *filename);
int ressource_name_cmp(const void *dir1, const void *dir2);
int ressource_time_cmp(const void *a, const void *b);
void ressource_sort(struct RessourceList *list);
void ressource_sortbytime(struct RessourceList *list);
void ressource_reverse(struct RessourceList *list);
void ressource_print(const struct RessourceList *list, char *commands);
void ressource_folder_process(char *path, char *commands);
long ressource_getTotal(char *path);