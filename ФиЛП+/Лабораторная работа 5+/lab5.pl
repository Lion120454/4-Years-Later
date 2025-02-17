% Результат 0
rule(X, 0):-
    (X=0*_);
	(X=_*0);
	(X=(-A)*A);
	(X=A*(-A)).

% Результат 1
rule(X, 1):-
    (X=1+_);
	(X=_+1);
	(X=(-A)+A);
	(X=A+(-A)).

% Результат только A
rule(X, A):- 
    (X=1*A);
	(X=A*1);
	
	(X=0+A);
	(X=A+0);
	
	(X=(-(-A)));
	(X=A*A);
    (X=A+A);

	(X=A+(A*_));
	(X=A+(_*A));
	(X=(A*_)+A);
	(X=(_*A)+A);
	
	(X=(A+_)*A);
	(X=(_+A)*A);
    (X=A*(A+_));
	(X=A*(_+A));
	(X=A*B+A*(-B)).

% Двойное отрицание
rule(-(-A), A). 

%Де Моргана
rule(-(A*B), (-A)+(-B)).
rule(-(A+B), (-A)*(-B)).

% Дистрибутивность
rule(A*(B+C), A*B+A*C). 
rule((A+B)*(C+D), A*C+A*D+B*C+B*D).
rule((A+B)*C, A*C+B*C).

% Свертка
rule(A+(-A)*B, A+B).
rule((-A)+A*B, (-A)+B).

% Расширение
rule(A*B+(-A)*C+B*C, A*B+(-A)*C).

%rule(X, X).

expression(X, A):- rule(X, A).

expression(X+Y, A+B):- 
    simplify(X, A),
    simplify(Y, B).
expression(X*Y, A*B):- 
    simplify(X, A),
    simplify(Y, B).

simplify(X, Y):-
    expression(X, Z),
    (X\=Z -> 
		simplify(Z, Y)).
%simplify(X, X).

%simplify((a+ -a)*b, X). b
%simplify(a*b+a* -b + -a*c, X). a+c
%ф+с+а+п