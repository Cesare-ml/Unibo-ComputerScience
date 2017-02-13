#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "arithmetic_templates.h"
#include "templates_constants.h"

int vm_template_arithmetic_eq_counter = 0;
int vm_template_arithmetic_lt_counter = 0;
int vm_template_arithmetic_gt_counter = 0;

// + - & | operators
char *vm_template_arithmetic_basic_2_register(char op) {
  char *template = "\
    @SP      \n\
    M=M-1    \n\
    A=M      \n\
    D=M      \n\
    @SP      \n\
    M=M-1    \n\
    A=M      \n\
    A=M      \n\
    D=A %c D \n\
    @SP      \n\
    A=M      \n\
    M=D      \n\
    @SP      \n\
    M=M+1    \n\
  ";
  char *s = (char *)malloc(VM_TEMPLATE_BUFF_CHAR_SIZE * sizeof(char));
  sprintf(s,template,op);
  return s;
}

char *vm_template_arithmetic_neg_operator() {
  char *template = "\
    @SP    \n\
    A=M-1  \n\
    M=!M   \n\
    M=M+1  \n\
  ";
  char *s = (char *)malloc(VM_TEMPLATE_BUFF_CHAR_SIZE * sizeof(char));
  strcpy(s,template);
  return s;
}

char *vm_template_arithmetic_not_operator() {
  char *template = "\
    @SP   \n\
    A=M-1 \n\
    M=!M  \n\
  ";
  char *s = (char *)malloc(VM_TEMPLATE_BUFF_CHAR_SIZE * sizeof(char));
  strcpy(s,template);
  return s;
}

char *vm_template_arithmetic_eq_operator() {
  char *template = "\
    @SP    \n\
    M=M-1  \n\
    A=M    \n\
    D=M    \n\
    @SP    \n\
    M=M-1  \n\
    A=M    \n\
    D=D-M  \n\
    @EQ_JUMP_TRUE_%c \n\
    D;JEQ  \n\
    @0     \n\
    D=A    \n\
    @EQ_JUMP_END_%c \n\
    ;JMP   \n\
    (EQ_JUMP_TRUE_%c) \n\
    @1     \n\
    D=A    \n\
    (EQ_JUMP_END_%c) \n\
    @SP    \n\
    A=M    \n\
    M=D    \n\
    @SP    \n\
    M=M+1  \n\
  ";
  char *s = (char *)malloc(VM_TEMPLATE_BUFF_CHAR_SIZE * sizeof(char));
  sprintf(
    s,template,
    vm_template_arithmetic_eq_counter,
    vm_template_arithmetic_eq_counter,
    vm_template_arithmetic_eq_counter,
    vm_template_arithmetic_eq_counter
  );
  vm_template_arithmetic_eq_counter++;
  return s;
}


char *vm_template_arithmetic_lt_operator() {
  char *template = "\
    @SP    \n\
    A=M-1  \n\
    D=M    \n\
    @LT_JUMP_SUB_CHECK_%c \n\
    D;JLE  \n\
    @SP    \n\
    A=M-1  \n\
    A=A-1  \n\
    D=M    \n\
    @LT_JUMP_TRUE_%c \n\
    D;JLT  \n\
    (LT_JUMP_SUB_CHECK_%c) \n\
    @SP    \n\
    A=M-1  \n\
    D=M    \n\
    @SP    \n\
    A=M-1  \n\
    A=A-1  \n\
    D=M-D  \n\
    @LT_JUMP_TRUE_%c \n\
    A;JLT  \n\
    @0     \n\
    D=A    \n\
    @LT_JUMP_END_%c \n\
    ;JMP   \n\
    (LT_JUMP_TRUE_%c) \n\
    @1     \n\
    D=A    \n\
    (LT_JUMP_END_%c) \n\
    @SP    \n\
    M=M-1  \n\
    M=M-1  \n\
    A=M    \n\
    M=D    \n\
    @SP    \n\
    M=M+1  \n\
  ";
  char *s = (char *)malloc(VM_TEMPLATE_BUFF_CHAR_SIZE * sizeof(char));
  sprintf(
    s,template,
    vm_template_arithmetic_lt_counter,
    vm_template_arithmetic_lt_counter,
    vm_template_arithmetic_lt_counter,
    vm_template_arithmetic_lt_counter,
    vm_template_arithmetic_lt_counter,
    vm_template_arithmetic_lt_counter,
    vm_template_arithmetic_lt_counter
  );
  vm_template_arithmetic_lt_counter++;
  return s;
}

char *vm_template_arithmetic_gt_operator() {
  char *template = "\
    @SP    \n\
    A=M-1  \n\
    D=M    \n\
    @GT_JUMP_SUB_CHECK_%c \n\
    D;JGE  \n\
    @SP    \n\
    A=M-1  \n\
    A=A-1  \n\
    D=M    \n\
    @GT_JUMP_TRUE_%c \n\
    D;JGT  \n\
    (GT_JUMP_SUB_CHECK_%c) \n\
    @SP    \n\
    A=M-1  \n\
    D=M    \n\
    @SP    \n\
    A=M-1  \n\
    A=A-1  \n\
    D=M-D  \n\
    @GT_JUMP_TRUE_%c \n\
    A;JGT  \n\
    @0     \n\
    D=A    \n\
    @GT_JUMP_END_%c \n\
    ;JMP   \n\
    (GT_JUMP_TRUE_%c) \n\
    @1     \n\
    D=A    \n\
    (GT_JUMP_END_%c) \n\
    @SP    \n\
    M=M-1  \n\
    M=M-1  \n\
    A=M    \n\
    M=D    \n\
    @SP    \n\
    M=M+1  \n\
  ";
  char *s = (char *)malloc(VM_TEMPLATE_BUFF_CHAR_SIZE * sizeof(char));
  sprintf(
    s,template,
    vm_template_arithmetic_gt_counter,
    vm_template_arithmetic_gt_counter,
    vm_template_arithmetic_gt_counter,
    vm_template_arithmetic_gt_counter,
    vm_template_arithmetic_gt_counter,
    vm_template_arithmetic_gt_counter,
    vm_template_arithmetic_gt_counter
  );
  vm_template_arithmetic_gt_counter++;
  return s;
}
