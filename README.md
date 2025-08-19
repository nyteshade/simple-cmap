# C Map Library

A lightweight, header‑only key/value map implementation written in plain C.  Think of it as a tiny, zero‑dependency collection that you can drop into any C project that needs a simple associative array.

## Highlights
- **No heavy dependencies** – only the C standard library.
- **Minimal build** – a single `make` target that produces `o/map.o`.  Include `include/map.h` in your project and link against the object file.
- **Extensible key type** – a user‑supplied comparison function lets you store strings, integers, floats, pointers or any custom type.
- **Class‑style API** – the public `struct Map` contains function pointers so you can use `map->get(map, key)` just like an OOP method.
- **Small example** – demonstrates case‑sensitive and case‑insensitive string keys.

> **Note:** The `SMakefile` (SAS/C for Amiga) is provided for historical reasons and is **not** used for modern builds.

## Directory Layout
```
├── Makefile      # Builds `o/map.o`
├── README.md     # *You are reading it*
├── SMakefile     # (ignored on modern systems)
├── examples/
│   └── string_keys/
│       ├── Makefile  # Builds a demo executable
│       └── main.c    # Shows usage of the map
├── include/
│   └── map.h     # Public header
├── o/            # Where the object file is created
└── src/
    └── map.c     # Core implementation
```

## Building the Library
```bash
# From the repository root
make
```
This compiles `src/map.c` into `o/map.o`.  The resulting object can then be linked into other programs.

### Building the Example
The example showcases the map in action and verifies both case‑sensitive and case‑insensitive behaviour.
```bash
# 1️⃣ Build the library object
make

# 2️⃣ Build the demo executable
cd examples/string_keys
make

# 3️⃣ Run the demo
./main
```
You should see output similar to:
```
Insert: "Hello" => 42
Insert: "hello" => 99
Case‑sensitive lookup: Hello -> 42
Case‑insensitive lookup: hello -> 99
```

## API Reference
The header `include/map.h` declares everything you need.

### Map Structure
```c
typedef struct Map {
    int   (*set)(struct Map *self, void *key, void *value);
    void *(*get)(struct Map *self, const void *key);
    int   (*getSize)(struct Map *self);
    int   (*getCapacity)(struct Map *self);
    void  (*delete)(struct Map *self, const void *key);
} Map;
```
Because the struct contains function pointers, you can call them either directly (`map_set(map, k, v)`) or via the object (`map->set(map, k, v)`), providing a lightweight OO‑style syntax.

### Functions
| Function | Purpose |
|----------|---------|
| `map_create` | Allocate a map with a given initial capacity and a key‑comparison callback. |
| `map_free` | Release all resources held by the map. |
| `map_set` | Insert or update a key‑value pair. |
| `map_get` | Retrieve the value associated with a key. |
| `map_delete` | Remove a key/value pair from the map. |
| `map_get_size` | Current number of stored entries. |
| `map_get_capacity` | Size of the internal table. |

### Built‑in Comparators
| Comparator | Types Supported | Description |
|------------|-----------------|-------------|
| `map_compare_string_keys` | `char*` | Case‑sensitive string comparison (uses `strcmp`). |
| `map_compare_string_keys_ignoring_case` | `char*` | Case‑insensitive comparison – copies string, lower‑cases, then compares. |
| `map_compare_int_keys` | `int*` | Compare integer values. |
| `map_compare_uint_keys` | `unsigned int*` | Compare unsigned integers. |
| `map_compare_float_keys` | `float*` | Compare float values. |
| `map_compare_double_keys` | `double*` | Compare double values. |
| `map_compare_ptr_keys` | `void*` | Pointer comparison. |

### Example Usage
```c
#include "../include/map.h"
#include <stdio.h>

int main(void) {
    // Create a map that treats strings in a case‑insensitive way
    Map *case_insensitive_map = map_create(8, map_compare_string_keys_ignoring_case);

    char *k1 = "Hello";
    int   v1 = 42;

    case_insensitive_map->set(case_insensitive_map, k1, &v1);

    int *found = (int*)case_insensitive_map->get(case_insensitive_map, "hello");
    if(found) printf("Found: %d\n", *found);

    case_insensitive_map->delete(case_insensitive_map, "HELLO");
    printf("Size after delete: %d\n", case_insensitive_map->getSize(case_insensitive_map));

    map_free(case_insensitive_map);
    return 0;
}
```

## Extending the Map
If you need a key type not covered by the built‑ins, supply your own comparison function that matches the signature:
```c
typedef int (*MapKeyCompareFunc)(const void *key1, const void *key2);
```
Return `0` for equality; any non‑zero value signals a mismatch.

## Contributing
Feel free to open issues or pull requests. The code aims to stay small, clear, and portable.

## License
MIT
