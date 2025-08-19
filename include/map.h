#ifndef MAP_H
#define MAP_H

/*
 * The Map structure will have a number of functions within it, in order
 * for relevant answers to be returned or actions to take place, a reference
 * to itself must always be the first parameter.
 */
typedef struct Map {
  int   (*set)(struct Map *self, void *key, void *value);

  void *(*get)(struct Map *self, const void *key);
  int   (*getSize)(struct Map *self);
  int   (*getCapacity)(struct Map *self);

  void  (*delete)(struct Map *self, const void *key);
} Map;

/*
 * Function pointer type for comparing keys.
 * Should return 0 if the keys are equal.
 */
typedef int (*MapKeyCompareFunc)(const void *key1, const void *key2);

/* -- Some definitions of comparators have been provided for easy reuse -- */

/*
 * Conforming to the `MapKeyCompareFunc` type, this comparator assumes the
 * keys in the map are both strings and compares them in a case insensitive
 * manner. Internally it converts a copy to lower-case for each string and
 * then uses `strcmp` to evaluate the result, freeing memory post comparison.
 *
 * @param key1 a pointer to a character string
 * @param key2 a pointer to a character string
 * @return -1 if key1 is less than key2, 0 if they match, 1 if key is greater
 */
int map_compare_string_keys_ignoring_case(const void *key1, const void *key2);

/*
 * Conforming to the `MapKeyCompareFunc` type, this comparator assumes the
 * keys in the map are both strings and compares them in a case sensitive
 * manner.
 *
 * @param key1 a pointer to a character string
 * @param key2 a pointer to a character string
 * @return -1 if key1 is less than key2, 0 if they match, 1 if key is greater
 */
int map_compare_string_keys(const void *key1, const void *key2);

/*
 * Conforming to the `MapKeyCompareFunc` type, this comparator assumes the
 * keys in the map are both integers. The numbers are cast and dereferenced
 * before comparing.
 *
 * @param int1 a pointer to an integer
 * @param int2 a pointer to an integer
 * @return 0 if the dereferenced values match, -1 otherwise
 */
int map_compare_int_keys(const void *intPtr1, const void *intPtr2);

/*
 * Conforming to the `MapKeyCompareFunc` type, this comparator assumes the
 * keys in the map are both unsigned integers. The numbers are cast and
 * dereferenced before comparing.
 *
 * @param uintPtr1 a pointer to an unsigned integer
 * @param uintPtr2 a pointer to an unsigned integer
 * @return 0 if the dereferenced values match, -1 otherwise
 */
int map_compare_uint_keys(const void *uintPtr1, const void *uintPtr2);

/*
 * Conforming to the `MapKeyCompareFunc` type, this comparator assumes the
 * keys in the map are both floats. The numbers are cast and dereferenced
 * before comparing.
 *
 * @param floatPtr1 a pointer to an integer
 * @param floatPtr2 a pointer to an integer
 * @return 0 if the dereferenced values match, -1 otherwise
 */
int map_compare_float_keys(const void *floatPtr1, const void *floatPtr2);

/*
 * Conforming to the `MapKeyCompareFunc` type, this comparator assumes the
 * keys in the map are both doubles. The numbers are cast and dereferenced
 * before comparing.
 *
 * @param doublePtr1 a pointer to a double
 * @param doublePtr2 a pointer to a double
 * @return 0 if the dereferenced values match, -1 otherwise
 */
int map_compare_double_keys(const void *doublePtr1, const void *doublePtr2);

/*
 * Conforming to the `MapKeyCompareFunc` type, this comparator assumes the
 * keys in the map are simply pointers to something (void*). If they
 * match, a 0 is returned.
 *
 * @param ptrKey1 a pointer to anything
 * @param ptrKey2 a pointer to anything
 * @return 0 if the values match, -1 otherwise
 */
int map_compare_ptr_keys(const void *ptrKey1, const void *ptrKey2);

/*
 * Creates and initializes a new map. For ease of use, a number of common
 * comparators are provided by this code. Either choose one of the
 * pre-defined functions:
 *
 *   - map_compare_string_keys_ignoring_case
 *   - map_compare_string_keys
 *   - map_compare_int_keys
 *   - map_compare_uint_keys
 *   - map_compare_float_keys
 *   - map_compare_double_keys
 *   - map_compare_ptr_keys
 *
 * Or implement your own in comformance to `MapKeyCompareFunc`.
 *
 * @param initial_capacity The initial number of elements the map can hold.
 * @param compare_func A pointer to a function used to compare keys.
 * @return A pointer to the newly created map, or NULL if allocation fails.
 */
Map *map_create(unsigned int initial_capacity, MapKeyCompareFunc compare_func);

/*
 * Frees all memory associated with the map.
 *
 * NOTE: This does not free the memory for the keys and values themselves,
 * as the map only stores pointers. That is the responsibility of the user.
 *
 * @param map A pointer to the map to be freed.
 */
void map_free(Map *map);

/*
 * Associates a value with a key. If the key already exists,
 * the old value is replaced with the new value.
 *
 * @param map A pointer to the map.
 * @param key A pointer to the key.
 * @param value A pointer to the value.
 * @return 0 on success, -1 on failure (e.g., memory allocation error).
 */
int map_set(Map *map, void *key, void *value);

/*
 * Retrieves the value associated with a given key.
 *
 * @param map A pointer to the map.
 * @param key A pointer to the key to look up.
 * @return A pointer to the value, or NULL if the key is not found.
 */
void *map_get(Map *map, const void *key);

/*
 * Returns the currently occupied size of the given map.
 *
 * @param map A pointer to the map
 * @return an integer indicating how much is used, or -1 if
 *  map is invalid
 */
int map_get_size(Map *map);

/*
 * Returns the current maximum capacity of the given map.
 *
 * @param map A pointer to the map
 * @return an integer indicating total capacity, or -1 if
 *  map is invalid
 */
int map_get_capacity(Map *map);

/*
 * Removes a key-value pair from the map.
 *
 * @param map A pointer to the map.
 * @param key A pointer to the key to remove.
 */
void map_delete(Map *map, const void *key);

#endif /* MAP_H */
