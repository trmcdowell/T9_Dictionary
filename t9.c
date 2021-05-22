// Thomas McDowell
//
// t9.c is program that takes an input txt file and builds a trie using
// words in the txt file. It then creates an interactive prompt that a
// user can use to search for existing words.

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_LEN 100

typedef struct TrieNode {
  char *data;
  struct TrieNode *children[9];
} TrieNode;

typedef struct Trie {
  TrieNode *root;
} Trie;

// funtions used in main and tree operations
Trie *convertText(int argc, char **argv);
void insert(Trie *trie, char word[BUFFER_LEN]);
int getChildIdx(char c);
TrieNode *newTrieNode();
Trie *newTrie();
TrieNode *search(Trie *trie, char *numStr);
void interactiveSearch();
void freeNode(TrieNode *node);
void freeSubTrie(TrieNode *node);
void freeTrie(Trie *trie);

/* main function of t9. Constructs a trie from an input file of lowercase
 * words and constructs an interactive search interface that allows a user to
 * search the constructed trie for input words. */
int main(int argc, char **argv) {
  Trie *trie = convertText(argc, argv);
  interactiveSearch(trie);
  freeTrie(trie);
  return 0;
}

/* connvertText creates a root node and then constructs a trie from an input
 * text file of lowercase words */
Trie *convertText(int argc, char **argv) {
  if (argc < 2) {
    printf("missing FILE argument\n");
    exit(1);
  }
  // get file and initialize trie
  char *filename = argv[1];
  FILE *file;
  char buffer[BUFFER_LEN];
  file = fopen(filename, "r");
  Trie *trie = newTrie();

  // read all lines and insert them into the trie
  while (fgets(buffer, BUFFER_LEN, file)) {
    insert(trie, buffer);
  }
  fclose(file);
  return trie;
}

// insert adds a word into a trie
void insert(Trie *trie, char *word) {
  char *wordRef = word;
  TrieNode *curr = trie->root;
  int childIdx;
  // while not at end of word
  while (*word != '\n' && *word != '\0') {
    childIdx = getChildIdx(*word);
    // if the child doesn't exist
    if (curr->children[childIdx] == NULL) {
      curr->children[childIdx] = newTrieNode();
      curr = curr->children[childIdx];
      word++;
    } else {
      curr = curr->children[childIdx];
      word++;
    }
  }
  // now curr is the last node for the input word
  if (curr->data != NULL) {
    while (curr->children[8] != NULL) {
      curr = curr->children[8];
    }
    // create # nodes if necessary
    curr->children[8] = newTrieNode();
    curr = curr->children[8];
  }
  curr->data = malloc(BUFFER_LEN * sizeof(char));
  curr->data = strncpy(curr->data, wordRef, BUFFER_LEN);
}

// getChildIdx returns the correct child index for a new trie node
int getChildIdx(char c) {
  // comparison value to help get t9 representation for each char
  int compareValue = c - 'a';
  /* Possible keys use digits 2 - 9, but we have to subtract two for proper
   * indexing in the trie*/
  if (compareValue >= 0 && compareValue < 3) {
    return 0;
  } else if (compareValue >= 3 && compareValue < 6) {
    return 1;
  } else if (compareValue >= 6 && compareValue < 9) {
    return 2;
  } else if (compareValue >= 9 && compareValue < 12) {
    return 3;
  } else if (compareValue >= 12 && compareValue < 15) {
    return 4;
  } else if (compareValue >= 15 && compareValue < 19) {
    return 5;
  } else if (compareValue >= 19 && compareValue < 22) {
    return 6;
  } else if (compareValue >= 22 && compareValue <= 26) {
    return 7;
  }
  return 0;
}

// newTrieNode creates and returns a new trie node
TrieNode *newTrieNode() {
  TrieNode *node = malloc(1 * sizeof(TrieNode));
  node->data = NULL;
  for (int i = 0; i < 9; i++)
    node->children[i] = NULL;
  return node;
}

// newTrie creates and returns a new trie
Trie *newTrie() {
  Trie *trie = malloc(1 * sizeof(Trie));
  trie->root = newTrieNode();
  return trie;
}

// search looks for a T9onym in the trie and returns it if found
TrieNode *search(Trie *trie, char *numStr) {
  TrieNode *curr = trie->root;
  int childIdx;
  while (*numStr != '\0' && *numStr != '\n') {
    // catch invalid search input (only numbers and #s allowed)
    if ((((int)*numStr - '0' < 2) || ((int)*numStr - '0' > 9)) &&
        (*numStr != '#')) {
      return NULL;
    }
    childIdx = (int)(*numStr - '0' - 2); // correction for indexing
    if (*numStr == '#') childIdx = 8;
    if (curr->children[childIdx] != NULL) {
      curr = curr->children[childIdx];
    } else {
      return NULL;
    }
    numStr++;
  }
  return curr;
}

// interactive search calls search interactively and prints data found by
// the search
void interactiveSearch(Trie *trie) {
  char numStr[BUFFER_LEN];
  TrieNode *searchNode = trie->root;
  char *numStrRef = NULL;
  bool printed; // tracks whether or not a print has been made
  printf("Enter \"exit\" to quit.\n");
  while (1) {
    printed = false;
    printf("Enter a char sequence (or # for next word):\n");
    numStrRef = fgets(numStr, BUFFER_LEN, stdin);
    // Exit condition
    if (strstr(numStr, "exit") != NULL) break;
    // Look for strings that only contain #
    if ((strncmp(numStr, "#", 1) != 0) && (strlen(numStr) > 1)) {
      searchNode = search(trie, numStrRef);
    }
    else if ((searchNode !=NULL) && (searchNode->children[8] != NULL)) {
      searchNode = searchNode->children[8];
    } else {
      printf("There are no more T9onyms\n");
      printed = true;
    }
    if (searchNode == NULL) printf("Not found in current dictionary\n");
    else if (printed == false) printf("%s\n", searchNode->data);
  }
}

// free node frees a node from memory
void freeNode(TrieNode *node) {
  if (node->data != NULL)
    free(node->data);
  free(node);
}

// freeSubTrie frees nodes and their children from memory
void freeSubTrie(TrieNode *node) {
  if (node != NULL) {
    for (int i = 0; i < 9; i++) {
      if (node->children[i] != NULL) {
        freeSubTrie(node->children[i]);
      }
    }
    freeNode(node);
  }
}

// freeTrie frees a trie from memory
void freeTrie(Trie *trie) {
  freeSubTrie(trie->root);
  free(trie);
}
