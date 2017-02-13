/*
 * Esercitazione 6 di Architettura degli elaboratori
 * Data: 12/02/2017
 * Author: Mirko Lancerotto
 *
 * Traduttore da linguaggio Virtual Machine ad Assembly della macchina
 * descritta su nand2tetris.org
 *
 * Per eseguirlo:
 * VMtranslator <nome-file>
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vmtranslator.h"

#define MAX_FILE_NAME_LENGTH 256


int main(int argc, char *argv[]) {
  FILE *fvm, *fasm;
  char *inputFileName, *outputFileName;

  if(argc < 2 || strlen(argv[1]) > MAX_FILE_NAME_LENGTH) {
    fprintf(stderr,"Argument 1 is needed\n");
    exit(-1);
  }

  inputFileName = (char *)malloc((MAX_FILE_NAME_LENGTH+3) * sizeof(char));
  outputFileName = (char *)malloc((MAX_FILE_NAME_LENGTH+3) * sizeof(char));

  sprintf(inputFileName,"%s",argv[1]);
  if(strcmp( argv[1]+strlen(argv[1])-3 ,".vm") == 0) {
    strncpy(outputFileName,argv[1],strlen(argv[1])-3);
    sprintf(outputFileName,"%s.asm",outputFileName);
  } else {
    sprintf(outputFileName,"%s.asm",argv[1]);
  }

  fvm = fopen(inputFileName,"r");
  fasm = fopen(outputFileName,"w");
  vm_translate_file(fvm,fasm);
  fclose(fvm);
  fclose(fasm);

  free(inputFileName);
  free(outputFileName);
}
