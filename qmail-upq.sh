cd QMAIL
cd queue
for dir in mess info local remote todo
do
  ( cd $dir; find . -type f -print ) | (
    cd $dir
    while read path
    do
      id=`basename "$path"`
      sub=`expr "$id" % SPLIT`
      mv "$path" "$sub"/"$id"
    done
  )
done
