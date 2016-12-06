#ifndef ASM_ASSEMBLER_H
#define ASM_ASSEMBLER_H

#include "symbol_table.h"
#include <stdint.h>

/*
 * In linea generale l'assembler é stato pensato per essere composto da una
 * funzione di parsing (asm_assembler_parse_line), la quale si preoccupa di
 * prendere in input una linea di codice, ripulirla dai commenti, trimmarla,
 * quindi capire di quale tipo di istruzione si tratti e delegare la sua
 * interpretazione ad una funzione specializzata, la quale si preoccuperá
 * di interpretarla e di svolgere tutte le azioni richieste
 */

/*
 * Viene utilizzata una mappa binaria per indicare i vari tipi di linee di codice
 * a cui il parser puó venire in contro
 */
#define ASM_ASSEMBLER_TYPE_AI 0x00000001 //A-Instruction
#define ASM_ASSEMBLER_TYPE_CI 0x00000002 //C-Instruction
#define ASM_ASSEMBLER_TYPE_L  0x00000004 //Label
#define ASM_ASSEMBLER_TYPE_E  0x00000008 //Empty line

/*
 * Un'ulteriore mappa binaria viene utilizzata per indicare i vari stati che
 * puó assumere l'esecuzione del parser
 */
#define ASM_ASSEMBLER_OK 0x00000000
#define ASM_ASSEMBLER_PARSE_ERROR 0x00000001
#define ASM_ASSEMBLER_MEMORY_ERROR 0x00000002
#define ASM_ASSEMBLER_ESCAPED_INSTRUCTION_LINE 0x00000004
#define ASM_ASSEMBLER_ESCAPED_NOT_INSTRUCTION_LINE 0x00000008
#define ASM_ASSEMBLER_ESCAPED_LINE 0x0000000C

/*
 * asm_assembler_data descrive un'istruzione
 */
typedef struct asm_assembler_data {
  unsigned int type;
  uint16_t code;
} asm_assembler_data;

/*
 * asm_assembler_match_code descrive a qualche codice fa riferimento la stringa
 * puntata da match
 */
struct asm_assembler_match_code {
  char *match;
  int code;
};

void asm_assembler_trim_comments(char *s);
void asm_assembler_trim(char *s, char *chars);
int asm_assembler_parse_line(asm_assembler_data **data, char *line, symbol_table *st, int index, int escapeTypes);
int asm_assembler_parse_line_type_ai(asm_assembler_data **data, char *l, symbol_table *st);
int asm_assembler_parse_line_type_ci(asm_assembler_data **data, char *l, symbol_table *st);
int asm_assembler_parse_line_type_l(asm_assembler_data **data, char *l, int index, symbol_table *st);
int asm_assembler_parse_line_type_e(asm_assembler_data **data);

#endif
