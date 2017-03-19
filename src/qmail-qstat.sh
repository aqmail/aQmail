cd HOME
messdirs=`echo queue/mess/* | wc -w`
messfiles=`find queue/mess/* -print | wc -w`
tododirs=`echo queue/todo/* | wc -w`
todofiles=`find queue/todo/* -print 2>/dev/null | wc -w`
echo messages in queue: `expr $messfiles - $messdirs`
if [ $tododirs -gt 1 ]
then
 echo messages in queue but not yet preprocessed: `expr $todofiles - $tododirs`
else
 echo messages in queue but not yet preprocessed: `expr $todofiles - $tododirs + 1`
fi
