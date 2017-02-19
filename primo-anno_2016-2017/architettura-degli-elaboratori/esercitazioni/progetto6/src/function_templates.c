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
  //printf("%s\n",s);
  return s;
}

char *vm_template_function_call(char *name, int nVars) {
  char *template = "\
    @FUNC_RET_ADDR_%d \n\
    D=A   \n\
    @SP   \n\
    A=M   \n\
    M=D   \n\
    @SP   \n\
    M=M+1 \n\
\
    @LCL  \n\
    D=M   \n\
    @SP   \n\
    A=M   \n\
    M=D   \n\
    @SP   \n\
    M=M+1 \n\
\
    @ARG  \n\
    D=M   \n\
    @SP   \n\
    A=M   \n\
    M=D   \n\
    @SP   \n\
    M=M+1 \n\
\
    @THIS \n\
    D=M   \n\
    @SP   \n\
    A=M   \n\
    M=D   \n\
    @SP   \n\
    M=M+1 \n\
\
    @THAT \n\
    D=M   \n\
    @SP   \n\
    A=M   \n\
    M=D   \n\
    @SP   \n\
    M=M+1 \n\
\
    @SP   \n\
    D=M   \n\
    @%d   \n\
    D=D-A \n\
    @5    \n\
    D=D-A \n\
    @ARG  \n\
    M=D   \n\
\
    @SP   \n\
    D=M   \n\
    @LCL  \n\
    M=D   \n\
\
    @%s   \n\
    0;JMP \n\
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
    @SP    \n\
    A=M-1  \n\
    D=M    \n\
    @5     \n\
    M=D    \n\
\n\
    @ARG   \n\
    D=M    \n\
    @SP    \n\
    M=D    \n\
\n\
    @5     \n\
    D=M    \n\
    @SP    \n\
    A=M    \n\
    M=D    \n\
    @SP    \n\
    M=M+1  \n\
\n\
    @LCL   \n\
    D=M    \n\
    @6     \n\
    M=D    \n\
\n\
    @6     \n\
    M=M-1  \n\
    A=M    \n\
    D=M    \n\
    @THAT  \n\
    M=D    \n\
\n\
    @6     \n\
    M=M-1  \n\
    A=M    \n\
    D=M    \n\
    @THIS  \n\
    M=D    \n\
\n\
    @6     \n\
    M=M-1  \n\
    A=M    \n\
    D=M    \n\
    @ARG   \n\
    M=D    \n\
\n\
    @6     \n\
    M=M-1  \n\
    A=M    \n\
    D=M    \n\
    @LCL   \n\
    M=D    \n\
\n\
    @6     \n\
    M=M-1  \n\
    A=M    \n\
    A=M    \n\
    0;JMP  \n\
  ";

  char *s = (char *)malloc(VM_TEMPLATE_BUFF_CHAR_SIZE * sizeof(char));
  sprintf(s,"%s",template);
  return s;
}
