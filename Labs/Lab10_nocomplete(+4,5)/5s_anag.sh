#!/bin/bash

# task 1: unscramble yxusonlia
#grep -iP '^(?:([yxusonlia])(?!.*\1)){9}$' /usr/share/dict/words
# negative lookahead: not take anything that follows by \1 ???
# WHY DOES THIS WORK???

# task 2
read word
size=${#word}
grep -iP "^(?:([$word])(?!.*\1)){$size}$" /usr/share/dict/words

# task 3

