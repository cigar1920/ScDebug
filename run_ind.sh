#!/bin/sh

suf0="_p.c"
suf1="_p.cpp"
suf2="_u.c"
suf3="_e.c"
suf4="_tac.cpp"
# modify gccinclulde to point to your gcc include directory
# The label for computation increases from the number in tac.log. 
exarg=-I/usr/lib/gcc/x86_64-linux-gnu/4.8/include/


rm $1_*
if [ -e $1${suf1} ]
then
	rm $1${suf1}
fi
./bin/insp $1 -extra-arg=$exarg --
mv $1$suf0 $1
./bin/ext $1 -extra-arg=c++11 --

clang-format -style=LLVM $1$suf3 > $1$suf3$suf3 2>&1
mv $1$suf3$suf3 $1$suf3
if [ $2 -eq 0 ];then
echo -e “Default Choice is on the ops with same level\n”
./bin/tac $1$suf3 -extra-arg=c++11 --
suf2="ops"
elif [ $2 -eq 1 ];then
echo -e "You have chose the level of op\n"
./bin/tacop $1$suf3 -extra-arg=c++11 --
suf2="op"
elif [ $2 -eq 2 ];then
echo -e "You have chose the level of statement\n"
./bin/tacstmt $1$suf3 -extra-arg=c++11 --
suf2="stmt"
else
echo -e "You have chose the level of block\n"
./bin/partion $1$suf3 -extra-arg=c++11 --
./bin/transblock $1$suf3 -extra-arg=c++11 --
suf2="block"
fi

clang-format -style=LLVM $1$suf3$suf4 > $1$suf2$suf1 2>&1 

rm $1${suf3}
rm $1$suf3$suf4
