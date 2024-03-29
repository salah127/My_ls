#include "../include/ressource.h"

#include <dirent.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

// Assuming struct RessourceList and other declarations are correctly defined in
// "ressource.h"

char *globalSortPath = NULL;

struct RessourceList *ressource_load(const char *path) {
  DIR *dir;
  struct dirent *entry;
  struct RessourceList *list = malloc(sizeof(struct RessourceList));
  if (list == NULL) {
    perror("Failed to allocate memory for resource list");
    return NULL;
  }

  list->path = strdup(path);
  list->length = 0;
  list->capacity = 10;  // Define the capacity
  list->base = malloc(list->capacity * sizeof(struct dirent *));
  if (list->base == NULL) {
    perror("Failed to allocate memory for base");
    free(list->path);
    free(list);
    return NULL;
  }

  dir = opendir(path);
  if (dir == NULL) {
    perror("Failed to open directory");
    free(list->path);
    free(list->base);
    free(list);
    return NULL;
  }

  while ((entry = readdir(dir)) != NULL) {
    if (list->length == list->capacity) {
      list->capacity *= 2;
      struct dirent **new_base =
          realloc(list->base, list->capacity * sizeof(struct dirent *));
      if (new_base == NULL) {
        perror("Failed to reallocate memory for base");
        for (size_t i = 0; i < list->length; i++) {
          free(list->base[i]);
        }
        free(list->base);
        free(list->path);
        free(list);
        closedir(dir);
        return NULL;
      }
      list->base = new_base;
    }
    list->base[list->length] = malloc(sizeof(struct dirent));
    if (list->base[list->length] == NULL) {
      perror("Failed to allocate memory for dirent");
      for (size_t i = 0; i < list->length; i++) {
        free(list->base[i]);
      }
      free(list->base);
      free(list->path);
      free(list);
      closedir(dir);
      return NULL;
    }
    memcpy(list->base[list->length], entry, sizeof(struct dirent));
    list->length++;
  }
  closedir(dir);
  return list;
}

int ressource_name_cmp(const void *a, const void *b) {
  struct dirent *entryA = *(struct dirent **)a;
  struct dirent *entryB = *(struct dirent **)b;
  return strcmp(entryA->d_name, entryB->d_name);
}

int ressource_time_cmp(const void *a, const void *b) {
  struct dirent *entryA = *(struct dirent **)a;
  struct dirent *entryB = *(struct dirent **)b;
  char fullPathA[PATH_MAX];
  char fullPathB[PATH_MAX];

  snprintf(fullPathA, PATH_MAX, "%s/%s", globalSortPath, entryA->d_name);
  snprintf(fullPathB, PATH_MAX, "%s/%s", globalSortPath, entryB->d_name);

  struct stat statA, statB;
  if (stat(fullPathA, &statA) != 0 || stat(fullPathB, &statB) != 0) {
    return 0;
  }

  return statA.st_mtime - statB.st_mtime;
}

void ressource_sort(struct RessourceList *list) {
  qsort(list->base, list->length, sizeof(struct dirent *), ressource_name_cmp);
}

void ressource_sortbytime(struct RessourceList *list) {
  globalSortPath =
      list->path;  // Ensure globalSortPath is set before calling qsort
  qsort(list->base, list->length, sizeof(struct dirent *), ressource_time_cmp);
}

void ressource_reverse(struct RessourceList *list) {
  for (size_t i = 0; i < list->length / 2; i++) {
    struct dirent *temp = list->base[i];
    list->base[i] = list->base[list->length - 1 - i];
    list->base[list->length - 1 - i] = temp;
  }
}

void ressource_print(const struct RessourceList *list, char *commands) {
  struct dirent **dir = list->base;  // Corrected declaration

  char *recursiveFolders[list->length];
  int rf_index = 0;

  if (string_contains(commands, 'l')) {
    printf("total %ld\n", ressource_getTotal(list->path));
  }

  for (size_t i = 0; i < list->length; i++, dir++) {
    struct stat buf;
    size_t size = strlen((*dir)->d_name) + strlen(list->path) + 2;
    char newpath[size];
    snprintf(newpath, size, "%s/%s", list->path, (*dir)->d_name);
    stat(newpath, &buf);
    afficher_fichier(buf, *dir, commands, "");

    if (string_contains(commands, 'R') && (*dir)->d_type == 4 &&
        strlen((*dir)->d_name) > 0 &&
        ((*dir)->d_name[0] != '.' || strlen((*dir)->d_name) > 1) &&
        ((*dir)->d_name[1] != '.' || strlen((*dir)->d_name) > 2)) {
      recursiveFolders[rf_index] = strdup(newpath);
      snprintf(recursiveFolders[rf_index], strlen(newpath) + 1, "%s", newpath);
      rf_index++;
    }
  }
  afficher_newline();

  for (size_t i = 0; i < rf_index; i++) {
    printf("\n%s: \n", recursiveFolders[i]);
    ressource_folder_process(recursiveFolders[i], commands);
  }
}

void ressource_folder_process(char *path, char *commands) {
  struct stat path_stat;
  int fileExist = stat(path, &path_stat);
  if (fileExist == -1) {
    printf("Unable to read folder/file : %s\n", path);
    exit(1);
  }

  int isDir = S_ISDIR(path_stat.st_mode);

  if (string_contains(commands, 'd')) {
    struct dirent *directory = ressource_getFileInFolder(".", path);
    afficher_fichier(path_stat, directory, commands, path);
    printf("\n");
    return;
  }

  if (!isDir) {
    struct dirent *directory = ressource_getFileInFolder(".", path);
    afficher_fichier(path_stat, directory, commands, path);
    printf("\n");
    return;
  }

  struct RessourceList *list = ressource_load(path);
  if (list) {
    if (!string_contains(commands, 'f')) {
      ressource_sort(list);
    }

    if (string_contains(commands, 't')) {
      ressource_sortbytime(list);
    }

    if (string_contains(commands, 'r')) {
      ressource_reverse(list);
    }

    ressource_print(list, commands);
    free(list->base);
    free(list);
  }
}

long ressource_getTotal(char *path) {
  int total = 0;
  DIR *dir = opendir(path);
  struct dirent *d;
  if (dir) {
    while ((d = readdir(dir)) != NULL) {
      if (strcmp(d->d_name, ".") == 0 || strcmp(d->d_name, "..") == 0) continue;
      struct stat buf;
      int size = strlen(path) + strlen(d->d_name) + 2;
      char newPath[size];
      snprintf(newPath, size, "%s/%s", path, d->d_name);
      stat(newPath, &buf);
      total += buf.st_blocks;
    }
  } else
    return 0;

  return total / 2;
}

struct dirent *ressource_getFileInFolder(char *path, char *filename) {
  DIR *dir;
  struct dirent *entry;
  dir = opendir(path);
  if (!dir) return NULL;
  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(filename, entry->d_name) == 0) {
      return entry;
    }
  }
  return NULL;
}