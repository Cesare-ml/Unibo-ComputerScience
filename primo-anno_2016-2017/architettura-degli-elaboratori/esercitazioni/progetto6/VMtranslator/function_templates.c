#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "function_templates.h"
#include "templates_constants.h"

int vm_template_function_ret_add_counter = 0;

char *vm_template_function_declaration(char *name, int nVars) {
  int i;

  char *template = "\
    (%s) \n\
  ";
  char *s = (char *)malloc(VM_TEMPLATE_BUFF_CHAR_SIZE * sizeof(char));
  sprintf(s,template,name);

  for(i=0; i<nVars; i++) {
    strcat(s,"\
      @0    \n\
      D=A   \n\
      @SP   \n\
      A=M   \n\
      M=D   \n\
      @SP   \n\
      M=M+1 \n\
    ");
  }
  printf("%s\n",s);
  return s;
}

char *vm_template_function_call(char *name, int nVars) {
  char *template = "\
    @FUNC_RET_ADDR_%d \n\
\
    @LCL  \n\
    D=A   \n\
    @SP   \n\
    A=M   \n\
    M=D   \n\
    @SP   \n\
    M=M+1 \n\
\
    @ARG  \n\
    D=A   \n\
    @SP   \n\
    A=M   \n\
    M=D   \n\
    @SP   \n\
    M=M+1 \n\
\
    @SP   \n\
    D=A   \n\
    @%d   \n\
    D=D-A \n\
    @3    \n\
    D=D-A \n\
    @ARG  \n\
    M=D   \n\
\
    @SP   \n\
    D=A   \n\
    @LCL  \n\
    M=D   \n\
\
    @%s   \n\
    ;JMP  \n\
    (FUNC_RET_ADDR_%d) \n\
  ";
  char *s = (char *)malloc(VM_TEMPLATE_BUFF_CHAR_SIZE * sizeof(char));
  sprintf(s,template,
    vm_template_function_ret_add_counter,
    nVars,
    name,
    vm_template_function_ret_add_counter
  );
  vm_template_function_ret_add_counter++;
  return s;
}


char *vm_template_function_return() {
  char *template = "\
    @LCL  \n\
    D=A   \n\
    @5    \n\
    M=D   \n\
\
    @5    \n\
    D=M   \n\
    @1    \n\
    A=D-A \n\
    D=M   \n\
    @ARG  \n\
    M=D   \n\
\
    @5    \n\
    D=M   \n\
    @2    \n\
    A=D-A \n\
    D=M   \n\
    @LCL  \n\
    M=D   \n\
\
    @5    \n\
    D=M   \n\
    @3    \n\
    A=D-A \n\
    A=M   \n\
    ;JMP  \n\
  ";
  
  char *s = (char *)malloc(VM_TEMPLATE_BUFF_CHAR_SIZE * sizeof(char));
  sprintf(s,template);
  return s;
}
