#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../include/ressource.h"

char *valideCommands = "lRrdtaAL";
char *bonusCommands = "fFgG";

int main(int argc, char *argv[])
{
  char commands[256];
  char *folders[argc];
  int folders_index = 0;

  for (int i = 1; i < argc; i++)
  {
    if (argv[i][0] != '-')
    {
      folders[folders_index] = strdup(argv[i]);
      folders[folders_index] = argv[i];
      folders_index++;
    }
    else if (strlen(argv[i]) > 1)
    {
      for (int y = 1; y < strlen(argv[i]); y++)
      {
        if ((string_contains(valideCommands, argv[i][y]) ||
             string_contains(bonusCommands, argv[i][y])) &&
            !string_contains(commands, argv[i][y]))
          strncat(commands, &argv[i][y], 1);
        else
        {
          printf("Command : '%c' is not valid !\n", argv[i][y]);
          return 1;
        }
      }
    }
  }

  if (!string_contains(commands, 'f'))
    qsort(folders, folders_index, sizeof(char *), compare_strings);

  if (folders_index == 0)
  {
    ressource_folder_process(".", commands);
  }
  else if (folders_index == 1)
  {
    ressource_folder_process(folders[folders_index - 1], commands);
  }
  else
  {
    for (int i = 0; i < folders_index; i++)
    {
      printf("%s : \n", folders[i]);
      ressource_folder_process(folders[i], commands);
      printf("\n");
    }
  }

  return 0;
}