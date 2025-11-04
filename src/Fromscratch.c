#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_WORD_COUNT 5000
#define MAX_SUCCESSOR_COUNT MAX_WORD_COUNT / 2
void replace_non_printable_chars_with_space();
char book[] = {
#embed "textfile.txt" /// Stores the content of the file as an array of chars.
    , '\0'};          /// Makes `book` a string.
char *tokenbook[MAX_WORD_COUNT];

char *tokens[MAX_WORD_COUNT];

size_t tokens_size = 0;

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

  bool isitappended = false;
  size_t id = token_id(token);

  size_t *next_empty_index_ptr = &succs_sizes[id];

  if (*next_empty_index_ptr >= MAX_SUCCESSOR_COUNT) {
    printf("Successor array full.");
    exit(EXIT_FAILURE);
  }
  for (size_t i = 0; i < MAX_SUCCESSOR_COUNT; i++) {
    if (succs[id][i] == NULL) {
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

char last_char(char *str) {
  int length = strlen(str);
  return str[length - 1];
}

bool token_ends_a_sentence(char *token) {
  int length = strlen(token);
  if (token[length - 1] == '?' || token[length - 1] == '!') {
    return true;
  }
  return false;
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
}

size_t random_token_id_that_starts_a_sentence() {
  int randonumbo;
  do {
    randonumbo = rand() % tokens_size;
    char *tmp = tokens[randonumbo];

  } while (!isupper(*tokens[randonumbo]));

  return randonumbo;
}

int nonNullElements(size_t tokenid) {
  int innerarraysize = _countof(succs[tokenid]);
  int nonnullelements = 0;
  for (int i = 0; i < innerarraysize; i++) {
    if (succs[tokenid][i] == NULL) {
      break;
    }
    nonnullelements++;
  }
  return nonnullelements;
}

int setnextsuccersor(char *currentsuccs) {
  int lengthoftoken = MAX_WORD_COUNT;
  for (int i = 0; i < lengthoftoken; i++) {
    if (strcmp(currentsuccs, tokens[i]) == 0) {
      return i;
    }
  }
  return 0;
}

char *generate_sentence(char *sentence, size_t sentence_size) {
  size_t current_token_id = random_token_id_that_starts_a_sentence();
  char *token = tokens[current_token_id];
  int randonumbo;

  sentence[0] = '\0';
  strcat(sentence, token);
  if (token_ends_a_sentence(token))
    return sentence;

  size_t sentence_len_next = 0;

  do {
    strcat(sentence, " ");
    randonumbo = rand() % nonNullElements(current_token_id);
    char *tmpsuccersor = succs[current_token_id][randonumbo];
    strcat(sentence, tmpsuccersor);
    if (token_ends_a_sentence(tmpsuccersor)) {
      break;
    } else if (sentence_len_next + strlen(tmpsuccersor) >= sentence_size - 1) {
      break;
    }

    current_token_id = setnextsuccersor(tmpsuccersor);
    if ((sentence_len_next + strlen(tmpsuccersor) >= sentence_size - 1))
      break;
    sentence_len_next = sentence_len_next + strlen(tmpsuccersor);
  } while (sentence_len_next < sentence_size - 1);
  sentence[sentence_size - 1] = '\0';
  return sentence;
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
}

int main() {
  bool RUNNING = true;
  srand(time(NULL));
  replace_non_printable_chars_with_space();

  char delimiter = ' ';
  createtokens(&delimiter);
  char sentence[100];
  memset(sentence, '\0', 100);

  do {
    memset(sentence, '\0', 100);
    char *generatedsentence = generate_sentence(sentence, 100);

  } while (!token_ends_a_sentence(sentence));
  puts(sentence);
  puts(" ");
  return 0;
}