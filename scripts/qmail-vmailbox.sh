#
# This scripts verifies the existance of a
# VMailMgr user dir (the box) upon receipt 
# of SMTP 'Mail From:' information on FD3
# in accordance with the checkpassword API.
#
# Quotas and other obstacles are not 
# considered.
#
# Version 0.3 (feh) -- typeset enhanced
#
#------------------------------------------
typeset -l ulocal  # lowercase!

virtualdomains="HOME/control/virtualdomains"
[ -f "${virtualdomains}" ] || exit 2

read -u3 address
[ "x${address}" = "x" ] && exit 2

ulocal=`echo ${address} | cut -d'@' -f1 | tr '.' ':'`
domain=`echo ${address} | cut -d'@' -f2`

for line in `grep -v '^#' ${virtualdomains}`
do
  vdomain=${line%:*}
  if [ "${vdomain}" = "${domain}" ]
  then
    vuser=${line#*:*}
    homedir=`grep ^${vuser} /etc/passwd | cut -d':' -f6`
    [ -d "${homedir}/users/${ulocal}" ] && exit 0
  fi
done

exit 1
