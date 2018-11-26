#ifndef __WSET_H
#define __WSET_H

typedef struct WordSet WordSet;

WordSet *wset_new ();

int  wset_size     (WordSet *wset);
void wset_free     (WordSet *wset);
int  wset_add      (WordSet *wset, char *word);
int  wset_fast_add (WordSet *wset, char *word);
int  wset_remove   (WordSet *wset, char *word);
int  wset_exists   (WordSet *wset, char *word);
int  wset_freq     (WordSet *wset, char *word);
void wset_print    (WordSet *wset);

#endif
