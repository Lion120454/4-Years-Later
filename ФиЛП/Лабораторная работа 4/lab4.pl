woman(Mari).
woman(Anna).

man(Jon).
man(Dik).

lovely(Mari).

kind(Jon).
kind(Anna).

manly(Jon).

strong(Jon).

rich(Dik).

manLikes(X) :- woman(X),lovely(X).

womanLike(X, Y) :- man(X),kind(X),(manLikes(Y);strong(X)).

happy(X) :- man(X),(rich(X);womanLike(X, _)).

%happy(X).
%womanLike(X, _).

