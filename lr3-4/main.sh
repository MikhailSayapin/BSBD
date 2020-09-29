#!/bin/sh

exec 4>&1

#Проходимся по аргументам командной строки
while [ -n "$1" ]
do case "$1" in
-i) exec 0<$2 #Переопределяем stdin
shift;; #Двигаемся дальше
-o) OUT="$2" #output-файл для stdout
exec 1 > $OUT #Переопределяем stdout в файл
shift;; #Двигаемся дальше
-e) ERR="$2" #output-файл для stderr
exec 2 > $ERR #Переопределяем stdout в файл
shift;; #Двигаемся дальше
-) shift #окончание прохождения по аргументам.
break ;;
*) echo "error with $1"
esac shift done

APP_PATH=$1 #путь до приложения
./$APP_PATH #его запуск
