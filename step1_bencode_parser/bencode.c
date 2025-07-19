#include "bencode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// TODO: Implement helper functions
// Hint: You might want functions like:
// - bencode_value_create(type) - allocate and initialize a bencode_value_t
// - is_at_end(parser) - check if parser reached end of input
// - peek_char(parser) - look at current character without advancing
// - consume_char(parser) - get current character and advance position

bencode_value_t *bencode_parse(const char *data, size_t length) {
    // TODO: Create a parser struct and call bencode_parse_value
    return NULL;
}

bencode_value_t *bencode_parse_value(/* TODO: add parser parameter */) {
    // TODO: Look at the first character to determine type
    // - If digit: parse string (format: "4:spam")
    // - If 'i': parse integer (format: "i42e")
    // - If 'l': parse list (format: "l...e")
    // - If 'd': parse dictionary (format: "d...e")
    return NULL;
}

bencode_value_t *bencode_parse_string(/* TODO: add parser parameter */) {
    // TODO: Parse format "<length>:<string>"
    // 1. Read digits to get length
    // 2. Expect ':' character
    // 3. Read exactly <length> bytes
    // 4. Create bencode_value_t with BENCODE_STRING type
    return NULL;
}

bencode_value_t *bencode_parse_integer(/* TODO: add parser parameter */) {
    // TODO: Parse format "i<number>e"
    // 1. Expect 'i' character
    // 2. Handle optional '-' for negative numbers
    // 3. Read digits
    // 4. Expect 'e' character
    // 5. Create bencode_value_t with BENCODE_INTEGER type
    return NULL;
}

bencode_value_t *bencode_parse_list(/* TODO: add parser parameter */) {
    // TODO: Parse format "l<value1><value2>...e"
    // 1. Expect 'l' character
    // 2. Parse values until 'e' character
    // 3. Create linked list of bencode_list_node_t
    // 4. Create bencode_value_t with BENCODE_LIST type
    return NULL;
}

bencode_value_t *bencode_parse_dict(/* TODO: add parser parameter */) {
    // TODO: Parse format "d<key1><value1><key2><value2>...e"
    // 1. Expect 'd' character
    // 2. Parse key-value pairs until 'e' character
    // 3. Keys must be strings
    // 4. Create linked list of bencode_dict_entry_t
    // 5. Create bencode_value_t with BENCODE_DICT type
    return NULL;
}

bencode_value_t *bencode_dict_get(bencode_value_t *dict, const char *key) {
    // TODO: Search dictionary for key and return corresponding value
    return NULL;
}

bencode_value_t *bencode_list_get(bencode_value_t *list, size_t index) {
    // TODO: Get list element at given index (0-based)
    return NULL;
}

void bencode_free(bencode_value_t *value) {
    // TODO: Recursively free all memory
    // Don't forget to free:
    // - String data
    // - List nodes and their values
    // - Dictionary entries, keys, and values
}

void bencode_print(bencode_value_t *value, int indent) {
    // TODO: Print bencode value with proper indentation
    // This is helpful for debugging
}