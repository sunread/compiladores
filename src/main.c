/* Projeto de Compiladores 2013-2 - Etapa 1
   Fernando Soller Mecking
   Lucas Herbert Jones
   Mateus Cardoso da Silva
*/

#include <stdio.h>
#include "comp_dict.h"
#include "comp_list.h"
#include "comp_tree.h"
#include "comp_graph.h"
#include "tokens.h"


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

  int token;

  while (running)
  {
    if (!(token = yylex()) && feof(getYyin()))
    {
      running = 0;
      break;
    }

    printf ("token <%d> at %d\n", token, getLineNumber());
  }

  dict_print(dictionary);

  return 0;
}
