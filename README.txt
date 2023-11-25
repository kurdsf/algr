
This project is meant to be a program
which can simplify expression like 
'x - x + 3' and 'x/x - 1'.
Also it should be able to solve equations like
'x^2 + x = 3' and 'x - 3 = 2*x'. (WIP)

Note:
This project is released under MIT License.
The file utest.h is taken from github.com/sheredom/utest.h.
The files vec.h and vec.c are taken from github.com/mashpoe/c-vector.
See LICENSE for details.

Usage:
This projekt uses Makefile.
Use `make repl` to spawn a REPL in 
which you can enter an expression like 'x / x - y / y'
or '(a * b) / a' and algr will simplify it to '0' and 'b' respectively.

High Level Overview:

Consider the expression 'x + (3 - 2) = 2*x'.
We first tokenize the expression.
After this is should look like this:
Var('x') Add Lparen Integer_Literal(3) Sub Integer_Literal(2) Eq Integer_Literal(2) Mul Var('x');
Invalid chars like '?' will throw an error.

Then we wan't to remove the brackeys of the expression and store the presedence
of each operator in the operator.

For example + has a lower presedence than *, (3*4 + 1 = 13 not 15)
and + in brackeys has a higher presedence than *. (3*(4+1) = 15 not 13)

We store the presedence as an integer.

Consider 'x + (3 - 2) = 2 * x' again.
The first `Add` has a presedence of 0.
The first `Sub` has a presedence of 2 since it is in brackeys.
The first `Mul` has a presedence of 1.

We now remove the brackeys and obtain:
Var('x') Add(1) Integer_Literal(3) Sub(2) Integer_Literal(2) Eq Integer_Literal(2) Mul(1) Var('x');

Then notice a pattern. The first token is always a literal or variable,
the second an operator, the third a literal or variable again and 
the last also a literal or variable.

<operand><operator><operand>...<operator><operand>
    3        +        4           -         x   


In invalid programs, like "+" or "+3" or "3++4" this is 
not the case.
Also remember that brackeys are already removed at this point.

So in the next step we check for this pattern and 
throw an error if it is not there.

Then we wan't to explicitly highlight which operator has which operands.

For example '(3 + 3) * 3', 
(+) has operands 3 and 3,
and (*) has operands 3 and the result of the addition.
We can represent that like this:
    (3 + 3) * 3 = Integer_Literal(3) Add(2) Integer_Literal(3) Mul(1) Integer_Literal(3).
    = (Mul (Add 3 3) 3).
Notice that if there we no brackeys, the final result would be:
(Add 3 (Mul 3 3)).

Some more examples are:

(Notice we omit the explicit Integer_Literal here)

(3 + 3) * (4 + 4) ==> (Mul (Add 3 3) (Add 4 4))
x - x ==> (Sub (Var 'x') (Var 'x')).
x * x - x ==> (Sub (Mul (Var 'x') (Var 'x')) (Var 'x')).

We think of an algorithm to transform these expressions.

Now when making expressions simpler,
we apply rules.

For example:
    x - x + x  (apply rule x - x = 0)
    = 0 + x    (apply rule y + x  = x + y)
    = x + 0    (apply rule x + 0 = x)
    = x.       (No rule known to simplify, Done)

The current idea is to collect as many rules
as possible and then apply them in as many different
combinations as possible.
Notice that we can apply all rules to only parts of the expressions.    
In the future, I want to save some rules by writing rules ordering the expression.
So that I don't have to write extra rules for simplifying 
3*x/x and x*3/x to 3.



    




