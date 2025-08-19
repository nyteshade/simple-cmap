#include <stdio.h>
#include <string.h>

#include "map.h"

int main(int argc, char **argv) {
  Map *map1 = map_create(2, map_compare_string_keys);
  Map *map2 = map_create(2, map_compare_string_keys_ignoring_case);

  printf("Map created using \x1b[1mmap_compare_string_keys\x1b[22m\n");

  map1->set(map1, "lu", "Lu Wang");
  map1->set(map1, "Lu", "Lucy");

  const char *lu1 = (const char *)map1->get(map1, "lu");
  const char *lu2 = (const char *)map1->get(map1, "Lu");

  printf("lu1 = %s\n", lu1);
  printf("lu2 = %s\n", lu2);

  printf("\nThese should not be the same...");
  printf("%s\n", strcmp(lu1, lu2) != 0 ? "they are not equal" : "they are equal");

  /* ------- */

  printf("\nMap created using \x1b[1mmap_compare_string_keys_ignoring_case\x1b[22m\n");

  map2->set(map2, "lu", "Lu Wang");
  map2->set(map2, "Lu", "Lucy");

  const char *lu3 = (const char *)map2->get(map2, "lu");
  const char *lu4 = (const char *)map2->get(map2, "Lu");

  printf("lu3 = %s\n", lu3);
  printf("lu4 = %s\n", lu4);

  printf("\nThese should be the same...");
  printf("%s\n", strcmp(lu3,lu4) != 0 ? "they are not equal" : "they are equal");

  printf("\nThe second set were the same because the keys were case insensitive\n");

  map_free(map1);
  map_free(map2);

  return 0;
}
