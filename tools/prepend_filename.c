#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
  * struct node - a node structure for a linked list
  * Description: This is needed to store each path variable
  * for later reference. It will be needed to be able to
  * modify the actual filename.
  * @data: it holds a string containing each path components
  * @next: a  pointer to the next node.
*/
typedef struct node {
    char *data;
    struct node *next;
} node;

/**
  * print_linked_list - prints the data a linked list
  * contains node by node to the stdout.
  * @head: a pointer to the head of the linked list.
*/
void print_linked_list(node *head)
{
    node *current = head;

    while (current != NULL)
    {
        printf("Data: %s\n", current->data);
        current = current->next;
    }
}
/**
  * confirm_memory_allocation - after allocating memory dynamically this
  * function checks if NULL was returned to confirm the success of the
  * operation.
  * @pointer: the pointer memory was allocated for.
*/
void confirm_memory_allocation(void *pointer)
{
    if (pointer == NULL)
    {
        perror("Memory allocation failed");
        exit(-1);
    }
}
/**
  * populate_list - populates the linked list with the path components.
  * @head: a pointer to the memory address of the head pointer of the
  * linked list. The head pointer needs to be modified here.
  * @filename: a pointer to the memory address of a variable (filename).
  * It is supposed to hold the actual filename without the path.
  * Note that the actual filename might be gotten in this function, that
  * is why we passed a pointer to its memory address.
*/
void populate_list(node **head, char **filename)
{
    char *token;
    node *current = NULL;
    node *new;

    while (token != NULL)
    {
        token = strtok(NULL, "/\\");
        if (token != NULL)
        {
            if (current == NULL)
            {
                current = malloc(sizeof(node));
                confirm_memory_allocation(current);
                current->data = token;
                current->next = NULL;
                (*head)->next = current;
            }
            else
            {
                new = malloc(sizeof(node));
                confirm_memory_allocation(new);
                new->data = token;
                new->next = NULL;
                current->next = new;
                current = new;
            }
            *filename = token;
        }
    }
}
/**
  * prepend - adds @prependie to the filename, This is the final stage needed.
  * @head: a pointer to the head of the linked list containing the file path
  * components.
  * @fp_mod: a pointer to the address of the fp_mod variable. This variable holds
  * the string|filepath that will be returned. It needs to be modified here since we prepend
  * to the filename here.
  * @prependie: the string that needs to be prepended.
  * @filename: the name of the file itself. This does not contain any path.
*/
void prepend(node *head, char **fp_mod, char *prependie, char *filename)
{
    node *current;

    current = head;
    while (current != NULL)
    {
        if (current->next != NULL)
        {
            strcat(*fp_mod, current->data);
            strcat(*fp_mod, "/");
        }
        else if (current->next == NULL)
        {
            strcat(*fp_mod, prependie);
            strcat(*fp_mod, filename);
        }
        current = current->next;
    }
}
/**
  * prepend_filename - prepends a string to a filename.
  * Description: The filename may come with a full path
  * so the function cuts the path till it gets to the filename.
  * Then it modifies the filename and returns the whole path.
  * @filepath: a string containing the filepath to modify.
  * @prependie: the string that needs to be prepended to the
  * filename.
  * Return: the modified file path.
*/
char *prepend_filename(char *filepath, char *prependie)
{
    char *fp = malloc(sizeof(char) * (strlen(filepath) + 1));
    char *fp_mod = malloc(sizeof(char) * (strlen(filepath) + strlen(prependie) + 1));
    char *filename, *token;

    /*
     * Linked list variable declarations:
     */
    node *head = malloc(sizeof(node));

    /* Confirms that there was no problem with dynamic memory allocation */
    confirm_memory_allocation(fp);
    confirm_memory_allocation(fp_mod);
    confirm_memory_allocation(head);
    /* Confirms that there was no problem with dynamic memory allocation */

    strcpy(fp, filepath);
    strcpy(fp_mod, ""); /* This is required or else fp_mod will access memory it isn't supposed to. */
    
    /**
      * Copy a slash(/) to the filepath string to
      * be returned if @filepath begins with a slash(/).
    */
    if (fp[0] == '/' || fp[0] == '\\')
        strcpy(fp_mod, "/");

    token = strtok(fp, "/\\");
    filename = token; /* This is needed in case only a filename is sent not a whole path. */
    head->data = token;
    head->next = NULL;

    populate_list(&head, &filename);
    prepend(head, &fp_mod, prependie, filename);

    /* Clean up */
    free(fp);
    return (fp_mod);
}

// int main(void)
// {
//     prepend_filename("/test_file.txt", "DEC_");
// }
