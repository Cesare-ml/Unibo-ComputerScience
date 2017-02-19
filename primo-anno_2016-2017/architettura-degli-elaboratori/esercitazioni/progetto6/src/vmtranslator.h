#ifndef VMTRANSLATOR_H
#define VMTRANSLATOR_H

#include <stdio.h>

#define VM_READ_FILE_BUFF 512

#define VM_SYNTAX_ERROR -1

#define VM_PARSE_OK 0

#define VM_ARITHMETIC_ADD 1
#define VM_ARITHMETIC_SUB 2
#define VM_ARITHMETIC_NEG 3
#define VM_ARITHMETIC_EQ  4
#define VM_ARITHMETIC_GT  5
#define VM_ARITHMETIC_LT  6
#define VM_ARITHMETIC_AND 7
#define VM_ARITHMETIC_OR  8
#define VM_ARITHMETIC_NOT 9

#define VM_MEM_ACC_POP  10
#define VM_MEM_ACC_PUSH 11

#define VM_MEM_ACC_STATIC   12
#define VM_MEM_ACC_LOCAL    13
#define VM_MEM_ACC_ARGUMENT 14
#define VM_MEM_ACC_CONSTANT 15

#define VM_FLOW_LABEL   16
#define VM_FLOW_GOTO    17
#define VM_FLOW_IF_GOTO 18

#define VM_FUNCTION_DECLARATION 19
#define VM_FUNCTION_CALL        20
#define VM_FUNCTION_RETURN      21

typedef struct{
  char *match;
  int id;
} link;

void vm_translate_file(FILE *, FILE *);
int vm_check_linker(link [], int, char *);
void vm_clear_translated_line(char *);
int vm_translate_line(char *,FILE *);
char *vm_translate_arithmetic_line(char *);
char *vm_translate_mem_acc_line(char *);
char *vm_translate_flow_line(char *);
char *vm_translate_function_line(char *);
#endif
