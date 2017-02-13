#include <string.h>
#include <stdlib.h>
#include "utils.h"

/*
 * Modifica s rimuovendo tutti i caratteri consecutivi in testa ed in coda che
 * sono presenti anche in chars
 */
void trim(char *s, char *chars) {
  char *buff;
  char *first, *last, *p, *c;
  int ok = 1;

  if(s==NULL || s[0]==0 || chars==NULL || chars[0]==0)
    return;

  //Rimozione in testa
  p=s;
  while(*p && ok) {
    ok = 0;
    for(c=chars; *c; c++) {
      ok = ok || *p == *c;
    }
    if(ok) p++;
  }
  first = p;

  //Rimozione in coda
  for(p=first, last=first; *p; p++) {
    ok = 0;
    for(c=chars; *c; c++) {
      ok = ok || *p == *c;
    }
    if(ok == 0)
      last=p;
  }

  //strcpy non puó utilizzare la stessa stringa sia come primo che come secondo
  //parametro, quindi viene utilizzato un buffer di supporto
  buff = (char *)malloc((last-first+2)*sizeof(char));
  strncpy(buff,first,last-first+1);
  buff[last-first+1]=0;
  strcpy(s,buff);
  free(buff);
}
