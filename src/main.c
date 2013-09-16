/* Projeto de Compiladores 2013-2 - Etapa 1
   Fernando Soller Mecking
   Mateus Cardoso da Silva
*/

#include <stdio.h>
#include "main.h"


int main (int argc, char **argv)
{
  int running = 1;
  setYyin(stdin);

  if (argc > 0) {
    if (fopen(argv[1], "r"))
    {
        setYyin(fopen(argv[1], "r"));
    }
  }

  dictionary = dict_new();

  gv_init(NULL);

  int resultado = yyparse();

  gv_close();

  dict_print(dictionary);

  return resultado;
}
