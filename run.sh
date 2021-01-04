#!/bin/sh

# Our scripts for instrumentation for Fdlibm,
# also an example of how to use run_ind.sh

if [ $1 -eq 0 ];then
echo -e “Default Choice is on the ops with same level\n”
elif [ $1 -eq 1 ];then
echo -e "You have chose the level of op\n"
elif [ $1 -eq 2 ];then
echo -e "You have chose the level of statement\n"
else
echo -e "You have chose the level of block\n"
fi
cd /home/shijia/Public/testprogram/
files=`ls *.c`
cd -

rm -rf *_e.c*
for file in $files
do
start_time=$(date +%s%N)
	./run_ind.sh /home/shijia/Public/testprogram/$file$suffix $1  > ./log/output$file.txt 2>&1
end_time=$(date +%s%N)
echo $(($end_time-$start_time))
done
