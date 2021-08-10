#!/bin/bash
# empty_io.sh: Overwrite output.txt and input.txt with ('\0'?)
# 21/08/10 Erik Karlsson >> Use under GPLv3
printf "\e[1m\e[;31m>input.txt and output.txt is now empty.\e[0m\n"
(> output.txt)&&(> input.txt)
