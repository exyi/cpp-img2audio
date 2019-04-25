#!/usr/bin/fish

# neznám jiný programovací jazyk, kde by chybové hlášky nešly číst normálně z command line, ale tento kokotský bazmek vždycky odroluje deset obrazovek při jedné debilní chybě...

cat (clang++ main.cpp  -std=c++17 -Wall -Wextra -pedantic -fsanitize=address -fno-omit-frame-pointer -g -fdiagnostics-color=always 2>&1| psub) (echo \n\n"GCC:"\n | psub) (g++ main.cpp -o /tmp/kokotskabinarka  -std=c++17 -Wall -Wextra -pedantic -fdiagnostics-color=always 2>&1 | psub) | less -r
