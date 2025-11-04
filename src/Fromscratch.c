#include <ctype.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORD_COUNT 10000
#define MAX_SUCCESSOR_COUNT MAX_WORD_COUNT / 2
void replace_non_printable_chars_with_space();
char book[] = {
#embed "textfile.txt" /// Stores the content of the file as an array of chars.
    , '\0'};          /// Makes `book` a string.
char *tokenbook[MAX_WORD_COUNT];
/// Array of tokens registered so far.
/// No duplicates are allowed.
char *tokens[MAX_WORD_COUNT];
/// `tokens`'s current size
size_t tokens_size = 0;

/// Array of successor tokens
/// One token can have many successor tokens. `succs[x]` corresponds to
/// `token[x]`'s successors.
/// We store directly tokens instead of token_ids, because we will directly
/// print them. If we wanted to delete the book, then it would make more sense
/// to store `token_id`s
char *succs[MAX_WORD_COUNT][MAX_SUCCESSOR_COUNT];
/// `succs`'s current size
size_t succs_sizes[MAX_WORD_COUNT];

size_t token_id(char *token) {
  size_t id;
  for (id = 0; id < tokens_size; ++id) {
    if (strcmp(tokens[id], token) == 0) {
      return id;
    }
  }
  tokens[id] = token;
  ++tokens_size;
  return id;
}

void append_to_succs(char *token, char *succ) {
  //   if (strcmp(succ, "Modern") == 0) {
  //     puts("Modern");
  //   }
  bool isitappended = false;
  size_t id = token_id(token);

  size_t *next_empty_index_ptr = &succs_sizes[id];

  if (*next_empty_index_ptr >= MAX_SUCCESSOR_COUNT) {
    printf("Successor array full.");
    exit(EXIT_FAILURE);
  }
  for (size_t i = 0; i < MAX_SUCCESSOR_COUNT; i++) {
    if (succs[id][i] == nullptr) {
      break;
    }
    if (strcmp(succ, succs[id][i]) == 0) {
      isitappended = true;
    }
  }
  if (!isitappended) {
    succs[id][(*next_empty_index_ptr)++] = succ;
  }
}

void createtokens(char *delimiters) {

  puts(delimiters);
  int booksize = strlen(book);
  char tempbook[booksize];
  strcpy(tempbook, book);

  char *tokenpointer = strtok(tempbook, " ");
  char *succerserpointer = strtok(NULL, " ");
  append_to_succs(tokenpointer, succerserpointer);
  for (size_t word = 0; word < MAX_WORD_COUNT; word++) {
    tokenpointer = succerserpointer;
    succerserpointer = strtok(NULL, " ");
    append_to_succs(tokenpointer, succerserpointer);
  }
  //   append_to_succs(tokenpointer, succerserpointer);
  //   for (size_t word = 0; word < booksize; word++) {
  //     tokenpointer = succerserpointer;
  //     succerserpointer = strtok(NULL, " ");

  //     bool appenddone = append_to_succs(tokenpointer, succerserpointer);
  //     if (appenddone) {
  //       break;
  //     }
  //   }
}

void replace_non_printable_chars_with_space() {
  int booklength = strlen(book);
  int count = 0;
  for (size_t character = 0; character < booklength; character++) {
    char tmp = book[character];
    int inttemp = tmp;

    if (!isprint(book[character])) {
      book[character] = ' ';
    }
    count++;
  }
  // printf("%d", count);
}

int main() {
  replace_non_printable_chars_with_space();
  char delimiter = ' ';
  createtokens(&delimiter);
}