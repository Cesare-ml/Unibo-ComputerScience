#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include "asm_assembler.h"
#include "symbol_table.h"

/*
 * Inizializzazione dei vari match a cui dovrá attenersi una C-Instruction
 */
const int ASM_ASSEMBLER_DEST_CODES_N = 8;
struct asm_assembler_match_code asm_assembler_dest_codes[] = {
  {.match = "M",    .code = 1},
  {.match = "D",    .code = 2},
  {.match = "MD",   .code = 3},
  {.match = "A",    .code = 4},
  {.match = "AM",   .code = 5},
  {.match = "AD",   .code = 6},
  {.match = "AMD",  .code = 7},
};

const int ASM_ASSEMBLER_JMP_CODES_N = 8;
struct asm_assembler_match_code asm_assembler_jmp_codes[] = {
  {.match = "JGT",  .code = 1},
  {.match = "JEQ",  .code = 2},
  {.match = "JGE",  .code = 3},
  {.match = "JLT",  .code = 4},
  {.match = "JNE",  .code = 5},
  {.match = "JLE",  .code = 6},
  {.match = "JMP",  .code = 7},
};

const int ASM_ASSEMBLER_COMP_CODES_N = 28;
struct asm_assembler_match_code asm_assembler_comp_codes[] = {
  {.match = "0",    .code = 0x2A},
  {.match = "1",    .code = 0x3F},
  {.match = "-1",   .code = 0x3A},
  {.match = "D",    .code = 0x0C},
  {.match = "A",    .code = 0x30},  {.match = "M",    .code = 0x70},
  {.match = "!D",   .code = 0x0D},
  {.match = "!A",   .code = 0x31},  {.match = "!M",   .code = 0x71},
  {.match = "-D",   .code = 0x0F},
  {.match = "-A",   .code = 0x33},  {.match = "-M",   .code = 0x73},
  {.match = "D+1",  .code = 0x1F},
  {.match = "A+1",  .code = 0x37},  {.match = "M+1",  .code = 0x77},
  {.match = "D-1",  .code = 0x0E},
  {.match = "A-1",  .code = 0x32},  {.match = "M-1",  .code = 0x72},
  {.match = "D+A",  .code = 0x02},  {.match = "D+M",  .code = 0x42},
  {.match = "D-A",  .code = 0x13},  {.match = "D-M",  .code = 0x53},
  {.match = "A-D",  .code = 0x07},  {.match = "M-D",  .code = 0x47},
  {.match = "D&A",  .code = 0x00},  {.match = "D&M",  .code = 0x40},
  {.match = "D|A",  .code = 0x15},  {.match = "D|M",  .code = 0x55},
};

void asm_assembler_trim_comments(char *s) {
  while(*s && *s != '/')
    s++;
  *s = 0;
}

/*
 * Modifica s rimuovendo tutti i caratteri consecutivi in testa ed in coda che
 * sono presenti anche in chars
 */
void asm_assembler_trim(char *s, char *chars) {
  char *buff;
  char *first, *last, *p, *c;
  int ok = 1;

  //Rimozione in testa
  p=s;
  while(*p && ok) {
    ok = 0;
    for(c=chars; *c; c++) {
      ok = ok || *p == *c;
    }
    if(ok) p++;
  }
  first = p;

  //Rimozione in coda
  for(p=first, last=first; *p; p++) {
    ok = 0;
    for(c=chars; *c; c++) {
      ok = ok || *p == *c;
    }
    if(ok == 0)
      last=p;
  }

  //strcpy non puó utilizzare la stessa stringa sia come primo che come secondo
  //parametro, quindi viene utilizzato un buffer di supporto
  buff = (char *)malloc((last-first+2)*sizeof(char));
  strncpy(buff,first,last-first+1);
  buff[last-first+1]=0;
  strcpy(s,buff);
  free(buff);
}

/*
  @input data: punta ad un puntatore giá inizializzato al quale verrá assegnato
    l'indirizzo di memoria di una variabile che descriverá la linea di codice di
    cui si sta eseguendo il parsing
  @input line: la linea di codice che si vuole parsare
  @input st: la symbol_table sulla quale faranno riverimento le varie istruzioni
  @input index: contatore delle istruzioni di tipo AI e CI trattate fino ad ora,
    utili per poter assegnare il valore corretto alle labels
  @input escapeTypes: mappa binaria i cui bit a 1 rappresentano i tipi di istruzioni
    da non tenere in considerazione durante il parsing della linea corrente
*/
int asm_assembler_parse_line(asm_assembler_data **data, char *line, symbol_table *st, int index, int escapeTypes){
  int status;
  char *l = (char *)malloc((1+strlen(line))*sizeof(char));

  if(l != NULL) {
    strcpy(l,line);
    asm_assembler_trim_comments(l);
    asm_assembler_trim(l," \n\r\t");

    // Cerco il tipo di istruzione, quindi richiamo una funzione specializzata
    // per eseguirne il parsing
    if(strlen(l) == 0) {
      if(escapeTypes & ASM_ASSEMBLER_TYPE_E)
        status = ASM_ASSEMBLER_ESCAPED_NOT_INSTRUCTION_LINE;
      else
        status = asm_assembler_parse_line_type_e(data);

    } else if(l[0] == '(') {
      if(escapeTypes & ASM_ASSEMBLER_TYPE_L)
        status = ASM_ASSEMBLER_ESCAPED_INSTRUCTION_LINE;
      else
        status = asm_assembler_parse_line_type_l(data,l,index,st);

    } else if(l[0] == '@') {
      if(escapeTypes & ASM_ASSEMBLER_TYPE_AI)
        status = ASM_ASSEMBLER_ESCAPED_INSTRUCTION_LINE;
      else
        status = asm_assembler_parse_line_type_ai(data,l,st);

    } else {
      if(escapeTypes & ASM_ASSEMBLER_TYPE_CI)
        status = ASM_ASSEMBLER_ESCAPED_INSTRUCTION_LINE;
      else
        status = asm_assembler_parse_line_type_ci(data,l,st);
    }

    free(l);
  } else {
    status = ASM_ASSEMBLER_MEMORY_ERROR;
  }

  return status;
}

/*
 * Parsing di una A-Instruction (senza controllo degli errori)
 */
int asm_assembler_parse_line_type_ai(asm_assembler_data **data, char *l, symbol_table *st) {
  if(l==NULL || strlen(l)<2)
    return ASM_ASSEMBLER_PARSE_ERROR;

  *data = (asm_assembler_data *)malloc(sizeof(asm_assembler_data));
  if(*data != NULL) {
    (*data)->type = ASM_ASSEMBLER_TYPE_AI;

    //Se inizia con un numero, allora non puó essere una variabile
    if(l[1]>='0' && l[1]<='9') {
      (*data)->code = atoi(l+1);
    //Se invece é una variabile, allora controllo se il suo valore é giá presente
    //nella symbol_table, in tal caso lo recupero e lo uso
    } else if(symbol_table_has(st, l+1)){
      (*data)->code = symbol_table_get_val(st, l+1);
    //altrimenti registro la variabile nella symbol_table, il suo valore verrá
    //assegnato in automatico puntando alla prima area di memoria non ancora
    //utilizzata
    } else {
      symbol_table_add_mem_location(st, l+1);
      (*data)->code = symbol_table_get_val(st, l+1);
    }
  } else {
    return ASM_ASSEMBLER_MEMORY_ERROR;
  }
  return ASM_ASSEMBLER_OK;
}

/*
 * Parsing di una C-Instruction
 * in generale ha forma "dest=comp" oppure "comp;jmp"
 */
int asm_assembler_parse_line_type_ci(asm_assembler_data **data, char *l, symbol_table *st){
  int i,k,ok,len;
  char *p,*t;

  *data = (asm_assembler_data *)malloc(sizeof(asm_assembler_data));

  if(*data != NULL) {
    (*data)->type = ASM_ASSEMBLER_TYPE_CI;
    //Inizializzo code a 0xE000 poiché i primi 3 bit a sinistra di una
    //C-Instruction sono sempre a 1
    (*data)->code = 0xE000;

    p = l;
    //Converto tutte le lettere della stringa in maiuscolo, in questo modo
    //rendo le C-Instruction Case-insensitive
    for ( ; *p; p++) *p = toupper((unsigned char)*p);

    p = l;
    //Se é presente l'= allora si tratta di un'assegnazione, quindi
    //controllo che il registro di destinazione (dest) faccia matching con le possibili
    //sintassi
    if ( (t=strchr(p,'=')) != NULL ) {
      len = t-p;
      ok = 0;
      for(i=0; !ok && i<ASM_ASSEMBLER_DEST_CODES_N; i++) {
        if (0 == strncmp(p,asm_assembler_dest_codes[i].match,len)){
          (*data)->code = (*data)->code | (asm_assembler_dest_codes[i].code << 3);
          ok = 1;
          p += len + 1;
        }
      }
      if(!ok) {
        free(*data);
        return ASM_ASSEMBLER_PARSE_ERROR;
      }
    }

    //La parte centrale (Comp) é sempre presente
    if((t=strchr(p,';')) != NULL) {
      len = t-p;
    } else {
      len = strlen(p);
    }
    ok = 0;
    for(i=0; !ok && i<ASM_ASSEMBLER_COMP_CODES_N; i++) {
      if (0 == strncmp(p,asm_assembler_comp_codes[i].match,len)){
        (*data)->code = (*data)->code | (asm_assembler_comp_codes[i].code << 6);
        ok = 1;
        p += len;
      }
    }
    if(!ok) {
      free(*data);
      return ASM_ASSEMBLER_PARSE_ERROR;
    }

    //Se é presente il ; allora é presente un jump
    if(*p == ';') {
      p++;
      ok = 0;
      for(i=0; !ok && i<ASM_ASSEMBLER_JMP_CODES_N; i++) {
        if (0 == strncmp(p,asm_assembler_jmp_codes[i].match,strlen(asm_assembler_jmp_codes[i].match))){
          (*data)->code = (*data)->code | asm_assembler_jmp_codes[i].code;
          ok = 1;
          p += strlen(asm_assembler_jmp_codes[i].match);
        }
      }
      if(!ok) {
        free(*data);
        return ASM_ASSEMBLER_PARSE_ERROR;
      }
    } else if(*p != 0) {
      free(*data);
      return ASM_ASSEMBLER_PARSE_ERROR;
    }

  } else {
    return ASM_ASSEMBLER_MEMORY_ERROR;
  }
  return ASM_ASSEMBLER_OK;
}

/*
 * Tipo di linea con label
 */
int asm_assembler_parse_line_type_l(asm_assembler_data **data, char *l, int index, symbol_table *st){
  char *label;

  // se la linea ha meno di 3 caratteri, sicuramente non é ben formattata,
  // infatti le label hanno come minimo le due parentesi '(',')'
  if(l==NULL || strlen(l)<3)
    return ASM_ASSEMBLER_PARSE_ERROR;

  label = (char *)malloc((strlen(l)-2+1)*sizeof(char));
  if(label != NULL) {
    *data = (asm_assembler_data *)malloc(sizeof(asm_assembler_data));

    if(*data != NULL) {
      (*data)->type = ASM_ASSEMBLER_TYPE_L;
      // variabile di supporto a cui viene assegnata la stringa della label
      // senza parentesi
      strncpy(label,l+1,strlen(l+1)-1);
      label[strlen(l)-2] = 0;

      // se la label é giá presente nella symbol table vuol dire che la label
      // é stata dichiarata due volte nel codice del programma
      if(symbol_table_has(st, label)){
        free (label);
        return ASM_ASSEMBLER_PARSE_ERROR;
      } else {
        symbol_table_add(st, label, index);
        (*data)->code = index;
      }
    } else {
      free (label);
      return ASM_ASSEMBLER_MEMORY_ERROR;
    }
  } else {
    return ASM_ASSEMBLER_MEMORY_ERROR;
  }
  free (label);
  return ASM_ASSEMBLER_OK;
}

/*
 * Tipo di linea vuota
 */
int asm_assembler_parse_line_type_e(asm_assembler_data **data) {
  *data = (asm_assembler_data *)malloc(sizeof(asm_assembler_data));
  if(*data != NULL) {
    (*data)->type = ASM_ASSEMBLER_TYPE_E;
  } else {
    return ASM_ASSEMBLER_MEMORY_ERROR;
  }
  return ASM_ASSEMBLER_OK;
}
