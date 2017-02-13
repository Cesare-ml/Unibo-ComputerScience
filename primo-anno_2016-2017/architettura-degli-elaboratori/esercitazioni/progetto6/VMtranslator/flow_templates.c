#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "flow_templates.h"
#include "templates_constants.h"


char *vm_template_flow_label(char *label) {
  char *template = "\
    (%s) \n\
  ";
  char *s = (char *)malloc(VM_TEMPLATE_BUFF_CHAR_SIZE * sizeof(char));
  sprintf(s,template,label);
  return s;
}

char *vm_template_flow_goto(char *label) {
  char *template = "\
    @%s  \n\
    ;JMP \n\
  ";
  char *s = (char *)malloc(VM_TEMPLATE_BUFF_CHAR_SIZE * sizeof(char));
  sprintf(s,template,label);
  return s;
}

char *vm_template_flow_if_goto(char *label) {
  char *template = "\
    @SP  \n\
    M=M-1\n\
    A=M  \n\
    D=M  \n\
    @%s  \n\
    D;JNE\n\
  ";
  char *s = (char *)malloc(VM_TEMPLATE_BUFF_CHAR_SIZE * sizeof(char));
  sprintf(s,template,label);
  return s;
}
