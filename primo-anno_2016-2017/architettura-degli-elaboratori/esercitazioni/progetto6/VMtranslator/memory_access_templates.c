#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "memory_access_templates.h"
#include "templates_constants.h"

char *vm_template_memory_access_push_lcl_arg(char *x,char* memBlock) {
  char *template = "\
    @%s   \n\
    D=A   \n\
    @%s   \n\
    A=M   \n\
    A=D+A \n\
    D=M   \n\
    @SP   \n\
    A=M   \n\
    M=D   \n\
    @SP   \n\
    M=M+1 \n\
  ";
  char *s = (char *)malloc(VM_TEMPLATE_BUFF_CHAR_SIZE * sizeof(char));
  sprintf(s,template,x,memBlock);
  return s;
}

char *vm_template_memory_access_push_local(char *x) {
  vm_template_memory_access_push_lcl_arg(x,"LCL");
}

char *vm_template_memory_access_push_argument(char *x) {
  vm_template_memory_access_push_lcl_arg(x,"ARG");
}

char *vm_template_memory_access_push_static(char *x) {
  char *template = "\
    @StaticVar_%s \n\
    D=M   \n\
    @SP   \n\
    A=M   \n\
    M=D   \n\
    @SP   \n\
    M=M+1 \n\
  ";
  char *s = (char *)malloc(VM_TEMPLATE_BUFF_CHAR_SIZE * sizeof(char));
  sprintf(s,template,x);
  return s;
}

char *vm_template_memory_access_push_constant(char *x) {
  char *template = "\
    @%s   \n\
    D=A   \n\
    @SP   \n\
    A=M   \n\
    M=D   \n\
    @SP   \n\
    M=M+1 \n\
  ";
  char *s = (char *)malloc(VM_TEMPLATE_BUFF_CHAR_SIZE * sizeof(char));
  sprintf(s,template,x);
  return s;
}

char *vm_template_memory_access_pop_lcl_arg(char *x,char* memBlock) {
  char *template = "\
    @%s   \n\
    D=A   \n\
    @%s   \n\
    A=M   \n\
    D=D+A \n\
    @5    \n\
    M=D   \n\
    @0    \n\
    M=M-1 \n\
    A=M   \n\
    D=M   \n\
    @5    \n\
    A=M   \n\
    M=D   \n\
  ";
  char *s = (char *)malloc(VM_TEMPLATE_BUFF_CHAR_SIZE * sizeof(char));
  sprintf(s,template,x,memBlock);
  return s;
}

char *vm_template_memory_access_pop_local(char *x) {
  vm_template_memory_access_pop_lcl_arg(x,"LCL");
}

char *vm_template_memory_access_pop_argument(char *x) {
  vm_template_memory_access_pop_lcl_arg(x,"ARG");
}

char *vm_template_memory_access_pop_static(char *x) {
  char *template = "\
    @SP   \n\
    M=M-1 \n\
    A=M   \n\
    D=M   \n\
    @StaticVar_%s \n\
    M=D   \n\
  ";
  char *s = (char *)malloc(VM_TEMPLATE_BUFF_CHAR_SIZE * sizeof(char));
  sprintf(s,template,x);
  return s;
}
