#!/bin/bash

if [ -z $1 ]
then
  clear
  echo "---------------------------------------------------------------"
  echo "-----------------skrypt do testowania rozwiazan----------------"
  echo "-uzycie : ./testerka.sh <directory> <nazwaProgramu> <valgrind>-"
  echo "--jesli ostatni argument to '-val' to sprawdzana jest rowniez--"
  echo "--------------zarzadzanie pamiecia przez valgrind--------------"
  echo "---------------------------------------------------------------"
  echo "-----a, dodaj se prawa 755 na niego bo tworzy troche syfu------"
  echo "---------------------------------------------------------------"
  echo "------- > > > made by: https://github.com/fiffeek < < < -------"
  echo "---------------------------------------------------------------"
else
  echo "Testuje wszystkie pliki .in w ${1}."
  failed=0
  count=0

  for f in "$1"/*.in; do
    alreadyFailed=0
    spaces=35
    (( count++ ))

    echo "">valgrind.txt
    echo "">temp.out
    printf "Test [${count}] from [${f::-3}]: "

    tosubstract=${#f}
    spaces=$(( spaces - tosubstract ))

    if [ $count -gt 9 ] && [ $count -lt 100 ]; then
      spaces=$(( spaces - 1 ))
    fi

    if [ $count -gt 99 ] && [ $count -lt 1000 ]; then
      spaces=$(( spaces - 2 ))
    fi

    if [ "${3}" == "-val" ]; then
      valgrind --error-exitcode=15 --leak-check=full \
      --show-leak-kinds=all --errors-for-leak-kinds=all -q \
      --log-file="valgrind.txt" ./${2} <${f} 1>temp.out
    else
      ./${2} <${f} 1>temp.out
    fi

    CODE=$?

    for (( i = 0; i < ${spaces}; i++ )); do
      printf " "
    done

    if [ "$(diff -q temp.out ${f%.in}.out)" != "" ]; then
      (( failed++ ))
      (( alreadyFailed++ ))
      printf "\e[31m[failed]\e[39m with return code = \e[94m[${CODE}]\e[39m."
    else
      printf "\e[32m[passed]\e[39m with return code = \e[94m[${CODE}]\e[39m."
    fi

    if [ "$3" != "-val" ]; then
      echo ""
    fi

    if [[ $(tail --lines=1 valgrind.txt) != "" ]] && [ "$3" == "-val" ]; then
      echo -e " Valgrind returned \e[91m[ERROR]\e[39m."
      if [ $alreadyFailed == 0 ]; then
        (( failed++ ))
      fi
    elif [ "$3" == "-val" ]; then
      echo -e " Valgrind returned \e[92m[NO ERRORS]\e[39m".
    fi
  done

  echo ""
  echo "Liczba testow: [${count}]"
  echo "Failures: [${failed}]"
fi

rm -f valgrind.txt temp.out
