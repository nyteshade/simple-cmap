#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "map.h"

/*
 * Internal structure for a single key-value entry.
 */
typedef struct {
    void *key;
    void *value;
} MapEntry;

/*
 * The internal map structure.
 */
typedef struct MapImpl {
    struct Map map;
    MapEntry *entries;
    unsigned int size;
    unsigned int capacity;
    MapKeyCompareFunc compare_func;
} MapImpl;

/* --- Private Helper Function --- */

char *str_lower(char *s) {
  while (*s)
    *s++ = tolower(*s);

  return s;
}

/*
 * Finds the index of an entry by its key.
 * Returns -1 if the key is not found.
 */
static int find_entry_index(Map *map, const void *key) {
    MapImpl *impl = (MapImpl*)map;
    unsigned int i;

    for (i = 0; i < impl->size; ++i) {
        if (impl->compare_func(impl->entries[i].key, key) == 0) {
            return i;
        }
    }

    return -1;
}

/* --- Public API Functions --- */

int map_compare_string_keys(const void *key1, const void *key2) {
  const char *string1 = (const char *)key1;
  const char *string2 = (const char *)key2;

  return strcmp(string1, string2);
}

int map_compare_string_keys_ignoring_case(const void *key1, const void *key2) {
  char *string1 = NULL;
  char *string2 = NULL;
  int result = -2;

  string1 = key1 ? str_lower(strdup(key1)) : (char*)key1;
  string2 = key2 ? str_lower(strdup(key2)) : (char*)key2;

  result = map_compare_string_keys(string1, string2);

  if (string1 && key1 && ((void *)string1 != key1)) {
    /* free(string1); */
  }

  if (string2 && key2 && ((void *)string2 != key2)) {
    /* free(string2); */
  }

  return result;
}

int map_compare_int_keys(const void *intPtr1, const void *intPtr2) {
  int int1 = *((const int *)intPtr1);
  int int2 = *((const int *)intPtr2);

  return int1 == int2 ? 0 : -1;
}

int map_compare_uint_keys(const void *uintPtr1, const void *uintPtr2) {
  unsigned int int1 = *((const unsigned int *)uintPtr1);
  unsigned int int2 = *((const unsigned int *)uintPtr2);

  return int1 == int2 ? 0 : -1;

}

int map_compare_float_keys(const void *floatPtr1, const void *floatPtr2) {
  float float1 = *((const float *)floatPtr1);
  float float2 = *((const float *)floatPtr2);

  return float1 == float2 ? 0 : -1;
}

int map_compare_double_keys(const void *doublePtr1, const void *doublePtr2) {
  double double1 = *((const double *)doublePtr1);
  double double2 = *((const double *)doublePtr2);

  return double1 == double2 ? 0 : -1;
}

int map_compare_ptr_keys(const void *ptrKey1, const void *ptrKey2) {
  return ptrKey1 == ptrKey2;
}

void map_free(Map *map) {
    MapImpl *impl = (MapImpl*)map;

    if (!map) {
        return;
    }

    free(impl->entries);
    free(impl);
}

int map_set(Map *map, void *key, void *value) {
    int index;
    MapEntry *new_entries;
    MapImpl *impl;

    if (!map) {
        return -1;
    }

    impl = (MapImpl*)map;

    /* First, check if the key already exists and update it */
    index = find_entry_index(map, key);
    if (index != -1) {
        impl->entries[index].value = value;
        return 0;
    }

    /* If the map is full, resize it */
    if (impl->size >= impl->capacity) {
        unsigned int new_capacity = impl->capacity * 2;
        new_entries = (MapEntry *)realloc(impl->entries, sizeof(MapEntry) * new_capacity);

        if (!new_entries) {
            return -1; /* Allocation failed */
        }
        impl->entries = new_entries;
        impl->capacity = new_capacity;
    }

    /* Add the new key-value pair */
    impl->entries[impl->size].key = key;
    impl->entries[impl->size].value = value;
    impl->size++;

    return 0;
}

void *map_get(Map *map, const void *key) {
    MapImpl *impl = (MapImpl*)map;
    int index;

    if (!map) {
        return NULL;
    }

    index = find_entry_index(map, key);

    if (index != -1) {
        return impl->entries[index].value;
    }

    return NULL;
}

void map_delete(Map *map, const void *key) {
    MapImpl *impl = (MapImpl*)map;
    int index;
    int bytes_to_move;

    if (!map) {
        return;
    }

    index = find_entry_index(map, key);
    if (index != -1) {
        /* Shift all subsequent elements one position to the left */
        if ((unsigned int)index < impl->size - 1) {
            bytes_to_move = (impl->size - index - 1) * sizeof(MapEntry);
            memmove(&impl->entries[index], &impl->entries[index + 1], bytes_to_move);
        }
        impl->size--;
    }
}

int map_get_size(Map *map) {
  MapImpl *impl = (MapImpl *)map;

  if (!map) {
    return -1;
  }

  return (int)impl->size;
}

int map_get_capacity(Map *map) {
  MapImpl *impl = (MapImpl *)map;

  if (!map) {
    return -1;
  }

  return (int)impl->capacity;
}

Map *map_create(unsigned int initial_capacity, MapKeyCompareFunc compare_func) {
    MapImpl *impl;

    /* Defend against a capacity of 0 */
    if (initial_capacity == 0) {
        initial_capacity = 10;
    }

    impl = (MapImpl *)malloc(sizeof(MapImpl));
    if (!impl) {
        return NULL;
    }

    impl->entries = (MapEntry *)malloc(sizeof(MapEntry) * initial_capacity);
    if (!impl->entries) {
        free(impl);
        return NULL;
    }

    impl->size = 0;
    impl->capacity = initial_capacity;
    impl->compare_func = compare_func;
    impl->map.set = map_set;
    impl->map.get = map_get;
    impl->map.delete = map_delete;
    impl->map.getSize = map_get_size;
    impl->map.getCapacity = map_get_capacity;

    return (struct Map*)impl;
}
