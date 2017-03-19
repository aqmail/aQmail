#
# This scripts verifies the existance of a
# vpopmail domain dir user (the box) upon receipt
# of SMTP 'Mail From:' information on FD3
# in accordance with the checkpassword API.
#
# Quotas and other obstacles are not
# considered.
#
# Version 0.1 (feh)
#
# Adjust your vpopmail home dir here:

VPOPMAILDIR=/home/vpopmail

#--- done ---

virtualdomains="HOME/control/virtualdomains"
[ -f "$virtualdomains" ] || exit 2
[ -d "$VPOPMAILDIR/domains" ] || exit 2

read -u3 address
[ "x$address" = "x" ] && exit 2

localpart=`echo $address | cut -d'@' -f1 | tr '.' ':'`
domainpart=`echo $address | cut -d'@' -f2`

[ -f "$virtualdomains" ] || exit 2

for line in `grep -v '^#' $virtualdomains`
do
  vdomain=${line%:*}
  if [ "$vdomain" = "$domainpart" ]
  then
    vdomainuser=${line#*:*}
    [ -d "$VPOPMAILDIR/domains/$vdomainuser/$localpart" ] && exit 0
  fi
done

exit 1
