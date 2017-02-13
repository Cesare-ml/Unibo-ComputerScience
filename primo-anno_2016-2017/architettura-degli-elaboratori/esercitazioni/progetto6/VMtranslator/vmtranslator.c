#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vmtranslator.h"
#include "utils.h"
#include "templates_constants.h"
#include "bootstrap_template.h"
#include "arithmetic_templates.h"
#include "memory_access_templates.h"
#include "flow_templates.h"
#include "function_templates.h"

link vm_arithmetics_command[] = {
  {"add",VM_ARITHMETIC_ADD},
  {"sub",VM_ARITHMETIC_SUB},
  {"neg",VM_ARITHMETIC_NEG},
  {"eq",VM_ARITHMETIC_EQ},
  {"gt",VM_ARITHMETIC_GT},
  {"lt",VM_ARITHMETIC_LT},
  {"and",VM_ARITHMETIC_AND},
  {"or",VM_ARITHMETIC_OR},
  {"not",VM_ARITHMETIC_NOT}
};

link vm_mem_acc_command_arg_1[] = {
  {"pop",VM_MEM_ACC_POP},
  {"push",VM_MEM_ACC_PUSH},
};

link vm_mem_acc_command_arg_2[] = {
  {"static",VM_MEM_ACC_STATIC},
  {"local",VM_MEM_ACC_LOCAL},
  {"argument",VM_MEM_ACC_ARGUMENT},
  {"constant",VM_MEM_ACC_CONSTANT},
};

link vm_flow_command[] = {
  {"label",VM_FLOW_LABEL},
  {"goto",VM_FLOW_GOTO},
  {"if-goto",VM_FLOW_IF_GOTO},
};

link vm_function_command[] = {
  {"function", VM_FUNCTION_DECLARATION},
  {"call", VM_FUNCTION_CALL},
  {"return", VM_FUNCTION_RETURN},
};

void vm_translate_file(FILE *fin, FILE *fout) {
  int status,i=0;
  char *line = (char *)malloc(VM_READ_FILE_BUFF*sizeof(char));
  char *bootstrap;

  bootstrap = vm_template_bootstrap();
  vm_clear_translated_line(bootstrap);
  fprintf(fout,bootstrap);
  free(bootstrap);

  while(fgets(line, VM_READ_FILE_BUFF, fin) != NULL) {
    //printf("%s",line);
    status = vm_translate_line(line,fout);
    if(status == VM_SYNTAX_ERROR) {
      printf("Syntax Error on line: %d\n",i);
    }
    else if(status == VM_PARSE_OK){
      printf("%d: ok\n",i);
    }
    i++;
  }
}

int vm_check_linker(link linker[], int N, char *line) {
  int i,len;
  int type = VM_SYNTAX_ERROR;
  for(i=0; i<N; i++) {
    len = strlen(linker[i].match);
    if(strncmp(line,linker[i].match,len) == 0)
      type = linker[i].id;
  }
  return type;
}

void vm_clear_translated_line(char *s) {
  char *t = (char *)malloc(VM_TEMPLATE_BUFF_CHAR_SIZE*sizeof(char));
  char *p;

  t[0] = 0;
  p = strtok (s,"\n");
  while (p != NULL)
  {
    trim(p," \r\t\0");
    strcat(t,p);
    strcat(t,"\n");
    p = strtok (NULL, "\n");
  }
  strcpy(s,t);
}

int vm_translate_line(char *line,FILE *fout) {
  int type = VM_SYNTAX_ERROR;
  char *translatedLine;

  trim(line," \n\r\t\0");

  //controllo se la linea é vuota o se é un commento
  if(line[0]==0 || line[0]=='/' && line[1]=='/')
    return VM_PARSE_OK;

  //controllo se é un calcolo aritmetico
  if((type=vm_check_linker(vm_arithmetics_command,9,line)) != VM_SYNTAX_ERROR) {
    translatedLine = vm_translate_arithmetic_line(line);
  }

  //controllo se é un'accesso alla memoria
  else if((type=vm_check_linker(vm_mem_acc_command_arg_1,2,line)) != VM_SYNTAX_ERROR) {
    translatedLine = vm_translate_mem_acc_line(line);
  }

  //controllo se é un comando di flow
  else if((type=vm_check_linker(vm_flow_command,3,line)) != VM_SYNTAX_ERROR) {
    translatedLine = vm_translate_flow_line(line);
  }

  //controllo se é un comando di funzione
  else if((type=vm_check_linker(vm_function_command,3,line)) != VM_SYNTAX_ERROR) {
    translatedLine = vm_translate_function_line(line);
  }

  if(type != VM_SYNTAX_ERROR) {
    vm_clear_translated_line(translatedLine);
    fprintf(fout,"%s\n",translatedLine);
    free(translatedLine);
  } else {
    return VM_SYNTAX_ERROR;
  }
  return VM_PARSE_OK;
}

char *vm_translate_arithmetic_line(char *line) {
  char *out;
  int op = vm_check_linker(vm_arithmetics_command,9,line);
  switch(op) {
    case VM_ARITHMETIC_ADD : out = vm_template_arithmetic_basic_2_register('+'); break;
    case VM_ARITHMETIC_SUB : out = vm_template_arithmetic_basic_2_register('-'); break;
    case VM_ARITHMETIC_NEG : out = vm_template_arithmetic_neg_operator(); break;
    case VM_ARITHMETIC_EQ  : out = vm_template_arithmetic_eq_operator(); break;
    case VM_ARITHMETIC_GT  : out = vm_template_arithmetic_gt_operator(); break;
    case VM_ARITHMETIC_LT  : out = vm_template_arithmetic_lt_operator(); break;
    case VM_ARITHMETIC_AND : out = vm_template_arithmetic_basic_2_register('&'); break;
    case VM_ARITHMETIC_OR  : out = vm_template_arithmetic_basic_2_register('|'); break;
    case VM_ARITHMETIC_NOT : out = vm_template_arithmetic_not_operator(); break;
  }

  return out;
}


char *vm_translate_mem_acc_line(char *line) {
  int type = VM_SYNTAX_ERROR;
  int mem_block = VM_SYNTAX_ERROR;
  int val = -1;
  char *p,*out;

  trim(line," \n\r\t\0");
  // Splitto la linea nelle tre parti in cui é suddiviso un comando di mem_acc
  p = strtok (line," \t");

  //determino se é un push o un pop
  if(p != NULL) {
    type = vm_check_linker(vm_mem_acc_command_arg_1,2,p);
  } else {
    return NULL;
  }

  //determino se é costant/local/argument/static
  do {
    p = strtok (NULL, " \t/");
  } while(
        p != NULL
    &&  VM_SYNTAX_ERROR == (mem_block = vm_check_linker(vm_mem_acc_command_arg_2,4,p))
  );

  if(p == NULL) {
    return NULL;
  }

  //determino il valore/puntatore
  do {
    p = strtok (NULL, " \t/");
  } while(p != NULL && p[0]==0);

  if(p == NULL) {
    return NULL;
  }

  //ottengo il template relativo ai parametri determinati
  if(type == VM_MEM_ACC_PUSH) {
    switch(mem_block) {
      case VM_MEM_ACC_LOCAL   : out = vm_template_memory_access_push_local(p); break;
      case VM_MEM_ACC_ARGUMENT: out = vm_template_memory_access_push_argument(p); break;
      case VM_MEM_ACC_STATIC  : out = vm_template_memory_access_push_static(p); break;
      case VM_MEM_ACC_CONSTANT: out = vm_template_memory_access_push_constant(p); break;
    }

  } else {
    switch(mem_block) {
      case VM_MEM_ACC_LOCAL   : out = vm_template_memory_access_pop_local(p); break;
      case VM_MEM_ACC_ARGUMENT: out = vm_template_memory_access_pop_argument(p); break;
      case VM_MEM_ACC_STATIC  : out = vm_template_memory_access_pop_static(p); break;
      case VM_MEM_ACC_CONSTANT: return NULL;
    }
  }

  return out;
}

char *vm_translate_flow_line(char *line) {
  int type = VM_SYNTAX_ERROR;
  char *p, *label, *out;

  trim(line," \n\r\t\0");
  // Splitto la linea nelle 2 parti in cui é suddiviso un comando di flow
  p = strtok (line," \t");

  //determino se é una label/goto/if-goto
  if(p != NULL) {
    type = vm_check_linker(vm_flow_command,3,p);
  } else {
    return NULL;
  }

  //determino la label
  do {
    p = strtok (NULL, " \t/");
  } while(p != NULL && p[0]==0);

  if(p == NULL) {
    return NULL;
  }
  label = p;

  switch(type) {
    case VM_FLOW_LABEL    : out = vm_template_flow_label(label); break;
    case VM_FLOW_GOTO     : out = vm_template_flow_goto(label); break;
    case VM_FLOW_IF_GOTO  : out = vm_template_flow_if_goto(label); break;
  }

  return out;
}

char *vm_translate_function_line(char *line) {
  int type = VM_SYNTAX_ERROR;
  char *p, *name, *out;
  int nVals;

  trim(line," \n\r\t\0");
  // Splitto la linea nelle 2 parti in cui é suddiviso un comando di flow
  p = strtok (line," \t");

  //determino se é una declaration/return/call
  if(p != NULL) {
    type = vm_check_linker(vm_function_command,3,p);
  } else {
    return NULL;
  }

  if(type == VM_FUNCTION_CALL || type == VM_FUNCTION_DECLARATION) {
    //determino il nome
    do {
      p = strtok (NULL, " \t/");
    } while(p != NULL && p[0]==0);

    if(p == NULL) {
      return NULL;
    }
    name = p;

    //determino il numero di parametri
    do {
      p = strtok (NULL, " \t/");
    } while(p != NULL && 0==sscanf(p,"%d",&nVals));

    if(p == NULL) {
      return NULL;
    }
  }

  switch(type) {
    case VM_FUNCTION_DECLARATION  : out = vm_template_function_declaration(name,nVals); break;
    case VM_FUNCTION_CALL         : out = vm_template_function_call(name,nVals); break;
    case VM_FUNCTION_RETURN       : out = vm_template_function_return(); break;
  }

  return out;
}
