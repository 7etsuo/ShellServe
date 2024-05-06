#include <stdio.h>
#include <stdlib.h> // NULL
#include <errno.h>  // perror
#include <unistd.h> // for exit() 
#include <string.h> // for strcat()

// array of commands for the pipeline
// Come up with a context free gramar for the command line
// a command line is a PLS | CL 
// or maybe a commandline is a PipeLine [&] 
// a PipeLine -> PLS 
//            -> PLS | PL
// PLS        -> Token *

#define MAXCOMMANDS 256

int g_parent;

struct Command { 
  char *argv[MAXCOMMANDS];
  int argc;
}; 

struct Node {
  int value;
  struct Node *left;
  struct Node *right;
};

struct Node* AddNode(struct Node *node, int value);
// void PreOrderTraverse(int depth, struct Node* const tree, int isGraphiz);
void PreOrderTraverse(int depth, struct Node* const tree, int parentValue, int isGraphiz); 

int main(int argc, char **argv) 
{
  struct Node root = {rand() % 100, NULL, NULL};


  for (size_t i = 0; i < 100; i++)
    AddNode(&root, rand() % 100);

  printf("digraph TreeGraph {\n");
  PreOrderTraverse(0, &root, root.value, 1);
  printf("}\n");

  return 0;
}

void PrintTree(int depth, int value)
{
  int tmp = depth;
  for (; tmp > 0; tmp--)
    putchar('\t');
  printf("Depth: %d Value %d\n", depth, value);
}

void PreOrderTraverse(int depth, struct Node* const tree, int parentValue, int isGraphiz) 
{ 
  if (tree && tree->value != parentValue)
    printf("\t%d -> %d;\n", parentValue, tree->value); 

  if (tree->left)
    PreOrderTraverse(depth + 1, tree->left, tree->value, isGraphiz);

  if (tree->right)
    PreOrderTraverse(depth + 1, tree->right, tree->value, isGraphiz);

  return;
}

struct Node* AddNode(struct Node *node, int value)
{
  if (node == NULL) { 
    node = (struct Node *) malloc(sizeof(struct Node));
    if (node == NULL) {
      perror("malloc() failed");
      exit(1);
    } 
    node->value = value; 
    node->left = node->right = NULL;

    return node;
  }

  if (value < node->value) { 
    node->left = AddNode(node->left, value);  
  } else if (value > node->value) { 
    node->right = AddNode(node->right, value);
  } else { 
    return node;
  } 

  return node;
}
