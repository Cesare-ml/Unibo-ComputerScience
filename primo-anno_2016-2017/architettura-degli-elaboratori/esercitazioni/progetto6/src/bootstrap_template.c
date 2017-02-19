#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bootstrap_template.h"
#include "templates_constants.h"

char *vm_template_bootstrap() {
  char *template = "\
    @256 \n\
    D=A  \n\
    @SP  \n\
    M=D  \n\
  ";
  char *s = (char *)malloc(VM_TEMPLATE_BUFF_CHAR_SIZE * sizeof(char));
  sprintf(s,"%s",template);
  return s;
}
