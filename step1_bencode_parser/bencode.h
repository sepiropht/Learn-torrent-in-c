#ifndef BENCODE_H
#define BENCODE_H

#include <stddef.h>

// TODO: Define the bencode_type_t enum
// Hint: You need 4 types: STRING, INTEGER, LIST, DICT

// TODO: Forward declare bencode_value_t

// TODO: Define bencode_dict_entry_t struct
// Hint: You need key (string), value (bencode_value_t*), and next pointer

// TODO: Define bencode_list_node_t struct
// Hint: You need value (bencode_value_t*) and next pointer

// TODO: Define bencode_value_t struct
// Hint: Use a tagged union with type field and union of different value types

// TODO: Define bencode_parser_t struct
// Hint: You need data pointer, current position, and total length

// Function prototypes - implement these in bencode.c
bencode_value_t *bencode_parse(const char *data, size_t length);
bencode_value_t *bencode_parse_value(/* TODO: add parameters */);
bencode_value_t *bencode_parse_string(/* TODO: add parameters */);
bencode_value_t *bencode_parse_integer(/* TODO: add parameters */);
bencode_value_t *bencode_parse_list(/* TODO: add parameters */);
bencode_value_t *bencode_parse_dict(/* TODO: add parameters */);

bencode_value_t *bencode_dict_get(bencode_value_t *dict, const char *key);
bencode_value_t *bencode_list_get(bencode_value_t *list, size_t index);

void bencode_free(bencode_value_t *value);
void bencode_print(bencode_value_t *value, int indent);

#endif