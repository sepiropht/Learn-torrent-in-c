# Step 1: Bencode Parser Exercise

## Objective
Implement a parser for the Bencode format, which is the encoding scheme used by BitTorrent for .torrent files and peer communication.

## BitTorrent Protocol: Bencode Format

### What is Bencode?
Bencode (pronounced "bee-encode") is a simple encoding format used throughout the BitTorrent protocol. It was designed to be easy to parse and generate, making it ideal for peer-to-peer communication where efficiency matters.

### Bencode Data Types

Bencode supports four data types:

#### 1. Strings
- Format: `<length>:<string>`
- Example: `4:spam` represents the string "spam"
- Length is in decimal ASCII
- Strings can contain any bytes, including binary data

#### 2. Integers  
- Format: `i<integer>e`
- Example: `i42e` represents the integer 42
- Negative numbers: `i-42e` represents -42
- Zero: `i0e` represents 0

#### 3. Lists
- Format: `l<contents>e`
- Example: `l4:spami42ee` represents the list ["spam", 42]
- Can contain any bencode values (strings, integers, lists, dictionaries)
- Empty list: `le`

#### 4. Dictionaries
- Format: `d<key1><value1><key2><value2>...e`
- Example: `d3:cow3:moo4:spami42ee` represents {"cow": "moo", "spam": 42}
- Keys must be strings and appear in lexicographical order
- Empty dictionary: `de`

## Implementation Guide

### Step 1: Define Data Structures
In `bencode.h`, complete the TODOs to define:
- `bencode_type_t` enum with 4 values
- `bencode_value_t` struct using a tagged union
- `bencode_list_node_t` and `bencode_dict_entry_t` structs for linked lists
- `bencode_parser_t` struct to track parsing state

### Step 2: Helper Functions
Create these utility functions in `bencode.c`:
- `bencode_value_create(type)` - allocate and initialize
- `is_at_end(parser)` - check if at end of input
- `peek_char(parser)` - look at current char without advancing
- `consume_char(parser)` - get current char and advance

### Step 3: Parsing Functions
Implement each parsing function:

#### String Parsing (`bencode_parse_string`)
1. Read digits to calculate length
2. Expect ':' separator
3. Read exactly `length` bytes
4. Allocate memory and copy data

#### Integer Parsing (`bencode_parse_integer`)
1. Expect 'i' prefix
2. Handle optional '-' for negative numbers
3. Read digit characters
4. Expect 'e' suffix

#### List Parsing (`bencode_parse_list`)
1. Expect 'l' prefix
2. Parse values until 'e' suffix
3. Build linked list of nodes
4. Handle empty lists

#### Dictionary Parsing (`bencode_parse_dict`)
1. Expect 'd' prefix
2. Parse key-value pairs until 'e' suffix
3. Keys must be strings
4. Build linked list of entries

### Step 4: Memory Management
Implement `bencode_free()` to recursively free all allocated memory:
- String data buffers
- List node chains
- Dictionary entry chains and keys

## Key C Programming Concepts

### Tagged Unions
Use `type` field to determine which union member is valid:
```c
switch (value->type) {
    case BENCODE_STRING: /* use value->value.string */ break;
    case BENCODE_INTEGER: /* use value->value.integer */ break;
    // ...
}
```

### Linked Lists
For dynamic collections without fixed size limits:
```c
typedef struct node {
    data_type *data;
    struct node *next;
} node_t;
```

### Recursive Parsing
Each function calls `bencode_parse_value()` for nested structures.

### Error Handling
Return `NULL` for any parsing error or memory allocation failure.

## Testing Your Implementation

The provided `test_bencode.c` has 57 comprehensive tests. To run:

```bash
cd step1_bencode_parser
gcc -o test_bencode test_bencode.c bencode.c
./test_bencode
```

Expected output: "All tests PASSED!"

## Common Pitfalls

1. **Memory leaks** - Always free what you allocate
2. **Buffer overruns** - Check bounds when reading string data
3. **Invalid input** - Handle malformed Bencode gracefully
4. **Null terminators** - Strings may contain binary data
5. **Recursive cleanup** - Free nested structures properly

## Testing Strategy

Start with simple cases and build up:
1. Parse individual strings and integers
2. Parse empty lists and dictionaries  
3. Parse simple containers with one element
4. Parse nested structures
5. Test error conditions

When you can parse `d3:cow3:moo4:spami42ee` correctly, you're ready for the next step!