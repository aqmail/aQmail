#
# This script generate whitelist for all your vpopmail domains, alias
# and ezmlm mailing-list for spamcontrol RECIPIENT feature.
# http://www.fehcom.de/qmail/spamcontrol
# David du SERRE-TELMON <david@xinus.net> 06/30/2004
# Fixed by Zoltan Frombach <zoltan@frombach.com> 11/19/2004

dstfile="HOME/users/recipients"

rm -f $dstfile.tmp

for vdomain in `cat HOME/users/assign | cut -d: -f 5 | grep -v -w -e '^\.$' | sort | uniq`; do
  if [ -d ${vdomain} ]; then
    domain=`echo "${vdomain}" | sed 's/.*\///'`;
    for mail in `cat ${vdomain}/vpasswd | cut -d: -f 1`; do
      echo "${mail}@${domain}" >> ${dstfile}.tmp
    done
    cd ${vdomain}
    for fwd in `ls -a -1 .qmail-* | sed 's/\:/\./' | sed 's/\.qmail-//g' | grep -v -w default`; do
      if [ $fwd != "default" ]; then
        if echo ${fwd} | grep "\-owner$" > /dev/null; then
          echo "${fwd}-owner@${domain}" | sed 's/\-owner//' >> ${dstfile}.tmp
          echo "${fwd}-help@${domain}" | sed 's/\-owner//' >> ${dstfile}.tmp
          echo "${fwd}-subscribe@${domain}" | sed 's/\-owner//' >> ${dstfile}.tmp
          echo "${fwd}-unsubscribe@${domain}" | sed 's/\-owner//' >> ${dstfile}.tmp
        else
          echo "${fwd}@${domain}" >> ${dstfile}.tmp
        fi
      fi
    done
  fi
done

if [ -e ${dstfile}.tmp ]; then
  if [ -e ${dstfile} ]; then
    mv ${dstfile} ${dstfile}.old
  fi
  cat ${dstfile}.tmp | sort -t @ -k 2 -k 1 | uniq > ${dstfile}
  rm -f ${dstfile}.tmp
fi

HOME/bin/qmail-recipients

exit $?
