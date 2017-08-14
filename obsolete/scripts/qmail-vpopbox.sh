#
# This scripts verifies the existance of a
# vpopmail domain dir user (the box) upon receipt
# of SMTP 'Mail From:' information on FD3
# in accordance with the checkpassword API.
#
# Quotas and other obstacles are not
# considered.
#
# Version 0.3 (feh) -- typeset enhanced
#
#-------------------------------------
typeset ulocal
typeset VOPMAILDIR
#
# Adjust your vpopmail home dir here:

VPOPMAILDIR=/home/vpopmail

#--- done ---

[ -d "${VPOPMAILDIR}/domains" ] || exit 2

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
    vdomainuser=${line#*:*}
    [ -d "${VPOPMAILDIR}/domains/${vdomainuser}/${ulocal}" ] && exit 0
  fi
done

exit 1
