#ifndef MEMORY_ACCESS_TEMPLATES_H
#define MEMORY_ACCESS_TEMPLATES_H

char *vm_template_memory_access_push_lcl_arg(char *,char *);
char *vm_template_memory_access_push_local(char *);
char *vm_template_memory_access_push_argument(char *);
char *vm_template_memory_access_push_static(char *);
char *vm_template_memory_access_push_constant(char *);
char *vm_template_memory_access_pop_lcl_arg(char *,char *);
char *vm_template_memory_access_pop_local(char *);
char *vm_template_memory_access_pop_argument(char *);
char *vm_template_memory_access_pop_static(char *);

#endif
