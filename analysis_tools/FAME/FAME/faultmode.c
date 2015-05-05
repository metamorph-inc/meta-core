/*
 * USAGERIGHTS fame
 *
 * This file provides two functions, FAME_get_fault_operating_mode and FAME_get_fault_amount,
 * which take the name of a potential fault and return either the mode or the amount of
 * the fault.  They get this info by reading a file called "FAME_fault_settings.txt", which
 * should contain one line per fault, giving the name of the fault, the integer mode for the
 * the fault, and a floating-point value for the fault (which value should be between 0 and 1).
 *
 * If the named fault is not in the file, the specified default value will be returned.
 */

#include <stdio.h>      // for fopen, fclose, fscanf, and ferror
#include <string.h>     // for strcmp and strdup
#include <stdlib.h>     // for malloc and free and perror

typedef struct _setting * setting;

struct _setting {
  const char *name;
  int mode;
  float amount;
  setting next;
};

static setting settings = NULL;

static const char *settings_filename = "FAME_fault_settings.txt";

static void free_settings() {
  setting next;
  while (settings != NULL) {
    next = settings->next;
    settings->next = NULL;
    free((void *) settings->name);
    free(settings);
    settings = next;
  }
}

static void read_settings(const char *filename) {
  FILE *fp = NULL;
  char name[4096];
  unsigned int mode;
  float amount;
  setting new_setting;

  free_settings();

  if (filename == NULL) {
    fp = fopen(settings_filename, "r");
  } else {
    fp = fopen(filename, "r");
  }
  if (fp == NULL) {
    perror((filename == NULL) ? settings_filename : filename);
    exit(1);
  }
  while (fscanf(fp, "%4095s %u %f\n", name, &mode, &amount) == 3) {
    new_setting = malloc(sizeof(*new_setting));
    new_setting->name = strdup(name);
    new_setting->mode = mode;
    new_setting->amount = amount;
    new_setting->next = settings;
    settings = new_setting;
  }
  if (ferror(fp)) {
    perror((filename == NULL) ? settings_filename : filename);
    exit(1);
  }
  fclose(fp);
}

int FAME_get_fault_operating_mode (const char *component_name, int defalt) {
  setting list_pointer;

  if (settings == NULL) {
    read_settings(NULL);
  }

  for (list_pointer = settings;  list_pointer != NULL;  list_pointer = list_pointer->next) {
    if (strcmp(list_pointer->name, component_name) == 0) {
      return list_pointer->mode;
    }
  }

  return defalt;
}

double FAME_get_fault_amount (const char *component_name, double defalt) {
  setting list_pointer;

  if (settings == NULL) {
    read_settings(NULL);
  }

  for (list_pointer = settings;  list_pointer != NULL;  list_pointer = list_pointer->next) {
    if (strcmp(list_pointer->name, component_name) == 0) {
      return list_pointer->amount;
    }
  }

  return defalt;
}

