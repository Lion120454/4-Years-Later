;Список студентов
(setq students (list 
                  (list 'Андреев 
                        (list '(Математика 5) 
                              '(Физика 5)
                              '(Программирование 5) 
                              '(Химия 4)))
                  (list 'Петров 
                        (list '(Математика 4) 
                              '(Физика 4)
                              '(Биология 3) 
                              '(Химия 2)))
                  (list 'Сидоров 
                        (list '(Математика 3) 
                              '(Литература 5)
                              '(Программирование 3) 
                              '(Химия 5)))
                  (list 'Иванов 
                        (list '(История 4) 
                              '(Физика 4)
                              '(Программирование 4) 
                              '(Химия 4)))
                  (list 'Пушкин 
                        (list '(Литература 4) 
                              '(История 4)
                              '(Программирование 5) 
                              '(Биология 5)))))

(dolist (student students)
  ; Извлекаем имя студента и список оценок
  (let* ((name (car student))  ; Имя студента
         (grades (cadr student)) ; Список дисциплин с оценками
         ; Считаем общую сумму оценок
         (total (reduce #'+ (mapcar #'cadr grades)))
         ; Получаем количество дисциплин
         (count (length grades))
         ; Вычисляем средний балл, если есть дисциплины
         (average (if (> count 0) (/ total count) 0)))
    
    ; Выводим имя студента
    (format t "Студент: ~a~%" name)
    ; Выводим дисциплины и оценки
    (dolist (subject-and-grade grades)
      (format t "~a: ~a~%" (car subject-and-grade) (cadr subject-and-grade)))
    ; Выводим средний балл
    (format t "Средний балл: ~f~%~%" average)))