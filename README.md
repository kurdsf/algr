# Algr is a C library for working with computer algebra.
# Currently implemented is the solution of simple equations.
  (For example x^2 = 0; x + 2 = -1; x + x = 2;)
  The only method exposed to users is ```float* find_solutions(char* lhs, char* rhs, size_t* numsolutions);```
  The method gets the left hand side of an expression (like "x + 3")
  and the right hand side (like " 3 - x*x") and returns the number of solutions.
  Example:
  ```  
  #include<stdio.h> // for printf
  #include<stddef.h> // for size_t
  #include"algr.h" // for find_solutions
  
  int main(void){

          float* solutions;
          size_t numsolutions;

          solutions = find_solutions("x + 3","3",&numsolutions);  

          for(size_t i = 0; i < numsolutions; i++)
                  printf("\t%f\n",solutions[i]);

         


          return 0;
  }

```  

Outputs: 
        0.0
  
 (Since x + 3 = x <=> | - 3
        x = 0)


Currently supported operations: +,-,*,/,^ 
 where x^y is pow(x,y).

See documentation: <docs> // TODO

