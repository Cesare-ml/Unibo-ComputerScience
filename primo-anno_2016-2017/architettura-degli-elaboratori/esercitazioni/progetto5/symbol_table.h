#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

/*
 * La symbol table viene trattata come una generica lista composta
 * da una struttura dati che tiene traccia della testa e del numero
 * di variabili assegnate ad un'area di memoria che sono state inserite.
 *
*/

typedef struct symbol_table_node {
  char *key;
  int val;
  struct symbol_table_node *next;
} symbol_table_node;

typedef struct symbol_table {
  int count_mem_locations;
  struct symbol_table_node *head;
} symbol_table;

symbol_table* symbol_table_create();
void symbol_table_add(symbol_table *st, char *key, int val);
void symbol_table_add_mem_location(symbol_table *st, char *key);
int symbol_table_has(symbol_table *st, char *key);
void symbol_table_free(symbol_table *st);
int symbol_table_exist(symbol_table *st, char *key);
symbol_table_node* symbol_table_get(symbol_table *st, char *key);
int symbol_table_get_val(symbol_table *st, char *key);

#endif
