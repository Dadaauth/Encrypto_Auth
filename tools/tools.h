#ifndef TOOLS_H
#define TOOLS_H

typedef struct KeyGen{
    unsigned char *key;
    unsigned char *iv;
} KEYGEN;

KEYGEN generate_secrets(char *password);
void drawProgressBar(double percentage);
char *prepend_filename(char *filepath, char *prependie);


#endif