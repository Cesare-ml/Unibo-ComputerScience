/*
 * Esercitazione di Architettura degli elaboratori
 * Data: 6/12/2016
 * Author: Mirko Lancerotto
 *
 * Assebler del linguaggio assembly descritto su nand2tetris.org
 *
 * Per eseguirlo:
 * assembler <nome-file>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "symbol_table.h"
#include "asm_assembler.h"

#define MAX_FILE_NAME_LENGTH 256
#define READ_FILE_BUFF 512

/*
 * Funzione di debug utile per visualizzare il contenuto
 * della symbol table
 */
void symbol_table_print_all(FILE *f, symbol_table *st) {
  symbol_table_node *p = st->head;
  fprintf(f,"table = [\n");
  while(p != NULL) {
    fprintf(f,"\t'%s'=>'%d',\n",p->key,p->val);
    p = p->next;
  }
  fprintf(f,"];\n");
}

int main(int argc, char *argv[]) {
  FILE *fasm, *fhack;
  char *inputFileName, *outputFileName, line[READ_FILE_BUFF];
  symbol_table *st;
  asm_assembler_data *data; //verrá utilizzato per rappresentare le singole istruzioni
  int line_n = 0;
  int i,k;
  int status;

  if(argc < 2 || strlen(argv[1]) > MAX_FILE_NAME_LENGTH) {
    fprintf(stderr,"Argument 1 is needed\n");
    exit(-1);
  }

  inputFileName = (char *)malloc((MAX_FILE_NAME_LENGTH+4) * sizeof(char));
  outputFileName = (char *)malloc((MAX_FILE_NAME_LENGTH+4) * sizeof(char));
  sprintf(inputFileName,"%s",argv[1]);
  if(strcmp( argv[1]+strlen(argv[1])-4 ,".asm") == 0) {
    strncpy(outputFileName,argv[1],strlen(argv[1])-4);
    sprintf(outputFileName,"%s.hack",outputFileName);
  } else {
    sprintf(outputFileName,"%s.hack",argv[1]);
  }

  st = symbol_table_create();

  /* FASE 1:
   * In questa fase viene letto riga per riga il file da compilare,
   * quindi considero solo le linee che rappresentano una label
   * per inserire il loro valore nella symbol table
   */
  fasm = fopen(inputFileName,"r");
  line_n=1;
  i=0;
  while(fgets(line, READ_FILE_BUFF, fasm) != NULL) {
    // Eseguo il parsing della linea senza tenere in considerazione
    // le linee vuote, e A-Instruction e le C-Instruction (Escaped lines)
    status = asm_assembler_parse_line(&data,line,st,i,
      ASM_ASSEMBLER_TYPE_E | ASM_ASSEMBLER_TYPE_AI | ASM_ASSEMBLER_TYPE_CI
    );
    // Se il parser ha computato una label con successo, allora avrá anche aggiunto
    // il suo valore alla symbol table
    if(status == ASM_ASSEMBLER_OK) {
      free(data);
    } else if(status & ASM_ASSEMBLER_ESCAPED_LINE) {
      // Se la linea che non é stata presa in considerazione era una istruzione
      // allora incremento il contatore delle linee di instruzione, utile per
      // poter assegnare il valore alle label che si incontrano
      if(status == ASM_ASSEMBLER_ESCAPED_INSTRUCTION_LINE)
        i++;
    } else if(status == ASM_ASSEMBLER_PARSE_ERROR) {
      fprintf(stderr,"Error on line %d >> '%s'\n",i,line);
    } else if(status == ASM_ASSEMBLER_ESCAPED_NOT_INSTRUCTION_LINE) {
    } else {
      fprintf(stderr,"Unkown error on line %d >> %s\n",line_n,line);
    }
    line_n++;
  }
  fclose(fasm);

  fasm = fopen(inputFileName,"r");
  fhack = fopen(outputFileName,"w");

  /* FASE 2:
   * A questo punto tutte le labels sono state aggiunte alla symbol table,
   * quindi viene letta di nuovo riga per riga il file da compilare, questa
   * volta peró tenendo in considerazione solo le linee che rappresentano un
   * istruzione (A-Instruction o C-Instruction)
   */
  i = 0;
  line_n=1;
  while(fgets(line, READ_FILE_BUFF, fasm) != NULL) {
    status = asm_assembler_parse_line(&data,line,st,i,
      ASM_ASSEMBLER_TYPE_E | ASM_ASSEMBLER_TYPE_L
    );
    if(status == ASM_ASSEMBLER_OK) {
      for(k=15; k>=0; k--) {
        if(data->code & (1<<k) )
          fprintf(fhack,"1");
        else
          fprintf(fhack,"0");
      }
      fprintf(fhack,"\n");
      i++;
      free(data);
    } else if(status & ASM_ASSEMBLER_ESCAPED_LINE) {
    } else if(status == ASM_ASSEMBLER_PARSE_ERROR) {
      fprintf(stderr,"Parse error on line %d >> %s\n",line_n,line);
    } else {
      fprintf(stderr,"Unkown error\n");
    }
    line_n++;
  }

  fclose(fasm);
  fclose(fhack);

  symbol_table_free(st);

  free(inputFileName);
  free(outputFileName);
}
