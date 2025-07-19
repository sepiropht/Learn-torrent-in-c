#include "bencode.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

static int tests_run = 0;
static int tests_passed = 0;

#define TEST_ASSERT(condition, message) do { \
    tests_run++; \
    if (condition) { \
        tests_passed++; \
        printf("PASS: %s\n", message); \
    } else { \
        printf("FAIL: %s\n", message); \
    } \
} while(0)

void test_string_parsing() {
    printf("\n=== Testing String Parsing ===\n");
    
    bencode_value_t *result = bencode_parse("4:spam", 6);
    TEST_ASSERT(result != NULL, "Parse simple string");
    TEST_ASSERT(result->type == BENCODE_STRING, "String type correct");
    TEST_ASSERT(result->value.string.length == 4, "String length correct");
    TEST_ASSERT(strcmp(result->value.string.data, "spam") == 0, "String content correct");
    bencode_free(result);
    
    result = bencode_parse("0:", 2);
    TEST_ASSERT(result != NULL, "Parse empty string");
    TEST_ASSERT(result->value.string.length == 0, "Empty string length correct");
    bencode_free(result);
    
    result = bencode_parse("12:hello world!", 15);
    TEST_ASSERT(result != NULL, "Parse string with spaces");
    TEST_ASSERT(strcmp(result->value.string.data, "hello world!") == 0, "String with spaces content correct");
    bencode_free(result);
}

void test_integer_parsing() {
    printf("\n=== Testing Integer Parsing ===\n");
    
    bencode_value_t *result = bencode_parse("i42e", 4);
    TEST_ASSERT(result != NULL, "Parse positive integer");
    TEST_ASSERT(result->type == BENCODE_INTEGER, "Integer type correct");
    TEST_ASSERT(result->value.integer == 42, "Positive integer value correct");
    bencode_free(result);
    
    result = bencode_parse("i-42e", 5);
    TEST_ASSERT(result != NULL, "Parse negative integer");
    TEST_ASSERT(result->value.integer == -42, "Negative integer value correct");
    bencode_free(result);
    
    result = bencode_parse("i0e", 3);
    TEST_ASSERT(result != NULL, "Parse zero");
    TEST_ASSERT(result->value.integer == 0, "Zero value correct");
    bencode_free(result);
}

void test_list_parsing() {
    printf("\n=== Testing List Parsing ===\n");
    
    bencode_value_t *result = bencode_parse("le", 2);
    TEST_ASSERT(result != NULL, "Parse empty list");
    TEST_ASSERT(result->type == BENCODE_LIST, "List type correct");
    TEST_ASSERT(result->value.list == NULL, "Empty list is NULL");
    bencode_free(result);
    
    result = bencode_parse("l4:spami42ee", 12);
    TEST_ASSERT(result != NULL, "Parse list with string and integer");
    TEST_ASSERT(result->type == BENCODE_LIST, "List type correct");
    
    bencode_value_t *first = bencode_list_get(result, 0);
    TEST_ASSERT(first != NULL, "First list element exists");
    TEST_ASSERT(first->type == BENCODE_STRING, "First element is string");
    TEST_ASSERT(strcmp(first->value.string.data, "spam") == 0, "First element content correct");
    
    bencode_value_t *second = bencode_list_get(result, 1);
    TEST_ASSERT(second != NULL, "Second list element exists");
    TEST_ASSERT(second->type == BENCODE_INTEGER, "Second element is integer");
    TEST_ASSERT(second->value.integer == 42, "Second element value correct");
    
    bencode_value_t *third = bencode_list_get(result, 2);
    TEST_ASSERT(third == NULL, "Third element doesn't exist");
    
    bencode_free(result);
}

void test_dict_parsing() {
    printf("\n=== Testing Dictionary Parsing ===\n");
    
    bencode_value_t *result = bencode_parse("de", 2);
    TEST_ASSERT(result != NULL, "Parse empty dictionary");
    TEST_ASSERT(result->type == BENCODE_DICT, "Dictionary type correct");
    TEST_ASSERT(result->value.dict == NULL, "Empty dictionary is NULL");
    bencode_free(result);
    
    result = bencode_parse("d3:cow3:moo4:spami42ee", 22);
    TEST_ASSERT(result != NULL, "Parse dictionary with entries");
    TEST_ASSERT(result->type == BENCODE_DICT, "Dictionary type correct");
    
    bencode_value_t *cow_value = bencode_dict_get(result, "cow");
    TEST_ASSERT(cow_value != NULL, "Dictionary key 'cow' exists");
    TEST_ASSERT(cow_value->type == BENCODE_STRING, "Dictionary value is string");
    TEST_ASSERT(strcmp(cow_value->value.string.data, "moo") == 0, "Dictionary value content correct");
    
    bencode_value_t *spam_value = bencode_dict_get(result, "spam");
    TEST_ASSERT(spam_value != NULL, "Dictionary key 'spam' exists");
    TEST_ASSERT(spam_value->type == BENCODE_INTEGER, "Dictionary value is integer");
    TEST_ASSERT(spam_value->value.integer == 42, "Dictionary integer value correct");
    
    bencode_value_t *missing = bencode_dict_get(result, "missing");
    TEST_ASSERT(missing == NULL, "Non-existent key returns NULL");
    
    bencode_free(result);
}

void test_nested_structures() {
    printf("\n=== Testing Nested Structures ===\n");
    
    bencode_value_t *result = bencode_parse("ll4:spamee", 10);
    TEST_ASSERT(result != NULL, "Parse nested list");
    TEST_ASSERT(result->type == BENCODE_LIST, "Outer structure is list");
    
    bencode_value_t *inner_list = bencode_list_get(result, 0);
    TEST_ASSERT(inner_list != NULL, "Inner list exists");
    TEST_ASSERT(inner_list->type == BENCODE_LIST, "Inner structure is list");
    
    bencode_value_t *inner_string = bencode_list_get(inner_list, 0);
    TEST_ASSERT(inner_string != NULL, "Inner string exists");
    TEST_ASSERT(strcmp(inner_string->value.string.data, "spam") == 0, "Inner string content correct");
    
    bencode_free(result);
    
    result = bencode_parse("d4:listl4:spamee", 16);
    TEST_ASSERT(result != NULL, "Parse dictionary with list value");
    TEST_ASSERT(result->type == BENCODE_DICT, "Outer structure is dictionary");
    
    bencode_value_t *list_value = bencode_dict_get(result, "list");
    TEST_ASSERT(list_value != NULL, "List value exists in dictionary");
    TEST_ASSERT(list_value->type == BENCODE_LIST, "Value is list");
    
    bencode_value_t *list_string = bencode_list_get(list_value, 0);
    TEST_ASSERT(list_string != NULL, "String exists in nested list");
    TEST_ASSERT(strcmp(list_string->value.string.data, "spam") == 0, "Nested string content correct");
    
    bencode_free(result);
}

void test_invalid_input() {
    printf("\n=== Testing Invalid Input ===\n");
    
    bencode_value_t *result = bencode_parse("", 0);
    TEST_ASSERT(result == NULL, "Empty input returns NULL");
    
    result = bencode_parse("invalid", 7);
    TEST_ASSERT(result == NULL, "Invalid input returns NULL");
    
    result = bencode_parse("i42", 3);
    TEST_ASSERT(result == NULL, "Incomplete integer returns NULL");
    
    result = bencode_parse("4:ab", 4);
    TEST_ASSERT(result == NULL, "Incomplete string returns NULL");
    
    result = bencode_parse("l4:spam", 7);
    TEST_ASSERT(result == NULL, "Incomplete list returns NULL");
    
    result = bencode_parse("d3:cow", 6);
    TEST_ASSERT(result == NULL, "Incomplete dictionary returns NULL");
}

int main() {
    printf("Running Bencode Parser Tests\n");
    printf("============================\n");
    
    test_string_parsing();
    test_integer_parsing();
    test_list_parsing();
    test_dict_parsing();
    test_nested_structures();
    test_invalid_input();
    
    printf("\n============================\n");
    printf("Test Results: %d/%d tests passed\n", tests_passed, tests_run);
    
    if (tests_passed == tests_run) {
        printf("All tests PASSED!\n");
        return 0;
    } else {
        printf("Some tests FAILED!\n");
        return 1;
    }
}