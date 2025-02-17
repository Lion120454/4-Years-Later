woman("Mari"). %женщина
woman("Anna"). %женщина
man("Jon"). %мужчина
man("Dik"). %мужчина
lovely("Mari").%милая
kind("Jon"). %добрый
kind("Anna"). %добрая
manly("Jon"). %мужественый
strong("Jon"). %сильный
rich("Dik"). %богатый
manLikes(X) :- woman(X),lovely(X). %нравится мужчинам
womanLike(X, Y) :- man(X),kind(X),(manLikes(Y);strong(X)). %нравится женщинам
happy(X) :- man(X),(rich(X);womanLike(X, _)). ;счастливый
%happy(X).
%womanLike(X, _).