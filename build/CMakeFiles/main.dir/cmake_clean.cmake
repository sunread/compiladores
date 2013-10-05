FILE(REMOVE_RECURSE
  "CMakeFiles/main.dir/lexer.c.o"
  "CMakeFiles/main.dir/parser.c.o"
  "CMakeFiles/main.dir/src/main.c.o"
  "CMakeFiles/main.dir/src/gv.c.o"
  "CMakeFiles/main.dir/src/comp_dict.c.o"
  "CMakeFiles/main.dir/src/comp_list.c.o"
  "CMakeFiles/main.dir/src/comp_tree.c.o"
  "CMakeFiles/main.dir/src/comp_graph.c.o"
  "CMakeFiles/main.dir/src/semantics.c.o"
  "lexer.c"
  "parser.c"
  "parser.h"
  "parser.output"
  "main.pdb"
  "main"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang C)
  INCLUDE(CMakeFiles/main.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
