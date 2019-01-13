// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents number of children for each node in a trie
#define N 27

// Represents a node in a trie
typedef struct node
{
    bool is_word;
    struct node *children[N];
}
node;

// Represents a trie
node *root = NULL;

// Prototypes
node *new_node(node *ptr_to_new_node);
int get_letter_pos(char c);
unsigned int count_words(node *node_ptr);
node *free_nodes(node *node_ptr);


// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize trie
    root = new_node(root);
    if (root == NULL)
    {
        return false;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into trie
    while (fscanf(file, "%s", word) != EOF)
    {
        // Create a traversal pointer for looking through trie
        node *trav = root;

        // Loop through letters of word and insert into trie
        for (int j = 0, word_len = strlen(word); j < word_len; j++)
        {
            int letter_pos_in_alphabet = get_letter_pos(word[j]);

            // Check if this is last letter of word
            if (j == word_len - 1)
            {
                trav->is_word = true;
                break; //end for loop here
            }

            // If there's no node for the next letter then add one
            if (trav->children[letter_pos_in_alphabet] == NULL)
            {
                trav->children[letter_pos_in_alphabet] = new_node(trav->children[letter_pos_in_alphabet]);
                if (trav->children[letter_pos_in_alphabet] == NULL)
                {
                    return false;
                }
            }

            // Move traversal pointer on to next node
            trav = trav->children[letter_pos_in_alphabet];
        }
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}


// Return pointer to a newly initialised node or NULL if malloc failed
node *new_node(node *ptr_to_new_node)
{
    // Initialize new node
    ptr_to_new_node = malloc(sizeof(node));
    if (ptr_to_new_node == NULL)
    {
        return NULL;
    }
    ptr_to_new_node->is_word = false;
    for (int i = 0; i < N; i++)
    {
        ptr_to_new_node->children[i] = NULL;
    }

    return ptr_to_new_node;
}


// Returns position of a letter in the alphabet counting from 0. Apostophe will return N-1
int get_letter_pos(char c)
{
    // Make sure character is lower case
    char c_lower = tolower(c);

    int letter_pos_in_alphabet = 0;

    // If this letter is an apostrophe put it at the end of the array
    if (c_lower == '\'')
    {
        letter_pos_in_alphabet = N - 1;
    }
    else
    {
        // Will give letter position counting from 0 due to ascii conversion
        letter_pos_in_alphabet = c_lower - 'a';
    }

    return letter_pos_in_alphabet;
}


// Returns count of words in a trie
unsigned int count_words(node *node_ptr)
{
    unsigned int words_count = 0;

    // If this node ptr is NULL return 0
    if (!node_ptr)
    {
        return 0;
    }
    else if (node_ptr->is_word)
    {
        words_count++;
    }

    // Recursively add up the words that are down every branch of this node
    for (int i = 0; i < N; i++)
    {
        words_count += count_words(node_ptr->children[i]);
    }

    return words_count;
}


// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // Not making this size() function the same as count_words as project brief says
    // that size()'s function definition can't be changed
    return count_words(root);
}


// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Create a traversal pointer for looking through trie
    node *trav = root;

    // Loop through letters of word
    // Looping until word_len - 1 as don't want to move trav ptr past last letter
    for (int j = 0, word_len = strlen(word); j < word_len - 1; j++)
    {
        int letter_pos_in_alphabet = get_letter_pos(word[j]);

        //if there's no node for the next letter then it musn't be a word
        if (trav->children[letter_pos_in_alphabet] == NULL)
        {
            return false;
        }
        else
        {
            //move traversal pointer on to next node
            trav = trav->children[letter_pos_in_alphabet];
        }
    }

    // If whole for loop has been gone through successfully the end of the word has been reached
    // If it is a word or not depends on this
    return trav->is_word;
}


// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    root = free_nodes(root);
    if (!root)
    {
        return true;
    }
    else
    {
        return false;
    }
}


// Free memory used by nodes recursively
node *free_nodes(node *node_ptr)
{
    // If this ptr isn't null then free it's children first
    if (node_ptr)
    {
        for (int i = 0; i < N; i++)
        {
            // If the ptr to a child isn't NULL then go down that branch
            if (node_ptr->children[i])
            {
                node_ptr->children[i] = free_nodes(node_ptr->children[i]);
            }
        }
    }

    // There should now be no children to this node so it's safe to free it and reset ptr to NULL
    free(node_ptr);
    return NULL;
}