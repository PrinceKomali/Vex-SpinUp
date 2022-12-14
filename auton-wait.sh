grep -r --color=none delay src/auton.c | tr -d ' \tdelay();' | awk '{s+=$1} END {print s/1000}' 
