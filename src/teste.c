#include <stdio.h>

void cleanString(char *s)
{
   int i, j = 0;

   for ( i = 0; s[i] != '\0'; i++ )
   {

      if(s[i] == ',' )
      {
          s[j++] = ' ';
      }
      else if (s[i] != '=' && s[i] != '>')
      {
         s[j++] = s[i];
      }


   }
   s[j] = '\0';
}

int main()
{
   char text[] = "add x,x => x";
   char ins[20];
   char op1[20];
   char op2[20];
   char res[20];
   printf("before : %s\n", text);
   cleanString(text);
   printf("after  : %s\n", text);
   sscanf(text, "%s %s %s %s", ins, op1, op2, res);
   printf("%s", ins);
   return 0;
}
