#!/bin/sh
bindir=$(pwd)
cd /mnt/DATA-UNC/school/georgia_tech_university/electrical_engineering/advanced_programming_techniques/labs/ece6122/lab3/tutorial09_vbo_indexing/
export 

if test "x$1" = "x--debugger"; then
	shift
	if test "xYES" = "xYES"; then
		echo "r  " > $bindir/gdbscript
		echo "bt" >> $bindir/gdbscript
		/usr/bin/gdb -batch -command=$bindir/gdbscript --return-child-result /mnt/DATA-UNC/school/georgia_tech_university/electrical_engineering/advanced_programming_techniques/labs/ece6122/lab3/build/tutorial09_vbo_indexing 
	else
		"/mnt/DATA-UNC/school/georgia_tech_university/electrical_engineering/advanced_programming_techniques/labs/ece6122/lab3/build/tutorial09_vbo_indexing"  
	fi
else
	"/mnt/DATA-UNC/school/georgia_tech_university/electrical_engineering/advanced_programming_techniques/labs/ece6122/lab3/build/tutorial09_vbo_indexing"  
fi
