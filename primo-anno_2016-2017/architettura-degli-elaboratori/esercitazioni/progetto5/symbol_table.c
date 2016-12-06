#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"

/*
 * Crea ed inizializza una symbol table con i simboli predefiniti dell'assembly
 */
symbol_table* symbol_table_create() {
  char *init_keys[] = {
    "R0","R1","R2","R3","R4","R5","R6","R7","R8","R9","R10","R11","R12","R13","R14","R15",
    "SCREEN","KBD","SP","LCL","ARG","THIS","THAT"
  };
  int init_vals[] = {
    0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
    16384,24576,0,1,2,3,4
  };
  int i;
  symbol_table *st = (symbol_table *)malloc(sizeof(symbol_table));
  if(st != NULL) {
    st->count_mem_locations = 16; // a 16 poiché sono 16 i diversi tipi di registri inseriti
    st->head = NULL;

    for(i=0; i<23; i++) {
      symbol_table_add(st,init_keys[i],init_vals[i]);
    }
  }
  return st;
}

/*
 * Aggiunge un nodo in testa alla lista.
 */
void symbol_table_add(symbol_table *st, char *key, int val) {
  symbol_table_node *n,*p;

  if(symbol_table_has(st,key) == 0) {
    n = (symbol_table_node *)malloc(sizeof(symbol_table_node));

    if(n != NULL) {
      n->key = (char *)malloc((1+strlen(key))*sizeof(char));
      if(n->key != NULL) {
        strcpy(n->key,key);
        n->val = val;
        n->next = st->head;
        st->head = n;
      } else {
        free(n);
      }
    }
  }
}

/*
 * Aggiunge una variabile alla lista.
 * A differenza delle labels che puntano ad un'istruzione, le variabili
 * puntano alla prima area di memoria libera, quindi ad ogni inserimento
 * viene incrementato il numero di variabili inserite per poter sempre sapere
 * dove inserire la prossima
 */
void symbol_table_add_mem_location(symbol_table *st, char *key) {
  symbol_table_add(st,key,st->count_mem_locations);
  st->count_mem_locations++;
}

int symbol_table_has(symbol_table *st, char *key) {
  symbol_table_node *p = st->head;
  while(p!=NULL && strcmp(p->key,key) != 0)
    p = p->next;
  return p!=NULL;
}

void symbol_table_free(symbol_table *st) {
  symbol_table_node *p, *prev;

  p = st->head;
  while(p!=NULL) {
    prev = p;
    p = p->next;
    free(prev->key);
    free(prev);
  }

  free(st);
}

symbol_table_node* symbol_table_get(symbol_table *st, char *key) {
  symbol_table_node *p = st->head;
  while(p!=NULL && strcmp(p->key,key) != 0)
    p = p->next;
  return p;
}

int symbol_table_get_val(symbol_table *st, char *key) {
  symbol_table_node *p = symbol_table_get(st,key);
  if(p != NULL) {
    return p->val;
  }
  return -1;
}
