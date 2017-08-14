#
# Notes:
#   chmod 1755 qmail-queue.scan; chown qmailq:qmail qmail-queue.scan
#   SQMAIL/tmp has to exist prior with owner qmaill:qmail 
#
# Usage:
#   Generate: control/spamdomains
#   Input:    recipient-domain:spam-threshold
#   Sample:   example.com:8
#             allspams:0
#             #testdomain:11 	# entries with leading '#' are disregarded
#
# Caution:
#   This script is a sample. Depending on your anti-virus and/or anti-spam
#   software, some heavy tweaking is required.
#
#
# Dependencies:
#    Korn shell
#    DJB's MESS822 package (or not)
#    SPAMCONTROL > 2.5 (return codes)
#
# Environment variables used:
#
# $MAILFROM -- set by qmail-smtpd
# $RCPTTO -- set by qmail-smtpd
#
# Virus scanner:
#    The AV scanner returns '0' if ok -- or '1'/'2' if virus -- anything else = error
#
# Spam scanner:
#    SpamAssassin > 3.0 (spamc/spamd)
#
# Output:
#    RC=0; if ok
#    RC=32; if virus 
#    RC=33; if spam
#    RC=81; if error
#
# Performance:
#    Put SQMAIL/tmp on a RAMDISK
#
# License:
#    Public Domain
#
# Author:
#    Dr. Erwin Hoffmann - FEHCom
#
# Version:
#    0.9.6
#
#------------------------------------------------------------------------------------------------
SQMAIL=HOME
#
alias -x SCANNER='/usr/local/bin/clamdscan'
#alias -x SCANNER='/etc/iscan/vscan'
alias -x SPAMMER='/usr/bin/spamc'
alias -x 822FIELD='/usr/local/bin/822field'
# alias -x 822FIELD='/usr/bin/grep'
#
VERBOSE=0
#
SCANNERARGS="--no-summary"
SPAMMERARGS=""
#
## No code change necessary from here 
#
typeset SPAM
integer SPAMC=0
integer SPAMTHRESHOLD=-1
integer SPAMTH
typeset SPAMDOMAINS

ID="${RANDOM}$$"
MESSAGE="${SQMAIL}/tmp/msg.${ID}"
export DTLINE="spam-queue"
#
[[ ! -d ${SQMAIL}/tmp ]] && exit 53
cat > ${MESSAGE} || exit 53
#
## Virus scanning
#
if [[ "x${QHPSI}" = "x" ]]; then
	VIRUS=$(SCANNER ${SCANNERARGS} ${MESSAGE})
	VRC=$?
	[[ ${VERBOSE} -gt 0 ]] && print -u2 "AV Scanner info [`SCANNER` -V]: ${VIRUS}"

#	VIRUS=$(echo "${VIRUS}" | grep -e "\*\*")       ## for TrendMicro only
	case ${VRC} in
		(0)     RC=0;;
		(1|2)   exec 1>&2; echo "Infected email not delivered (${VIRUS})"; RC=32;;
		(*)     exec 1>&2; echo "`SCANNER -V` internal error (${VRC})"; RC=81;;
	esac
fi
#
## Check Spamlevel for each domain
#
if [[ -f ${SQMAIL}/control/spamdomains && ${RC} -eq 0 ]]; then
	SPAMDOMAINS=$(grep -v "^#" ${SQMAIL}/control/spamdomains) 
	for LINE in ${SPAMDOMAINS}
	do
		DOMAIN=${LINE%:*}
		SPAMTH=${LINE#*:*}
		[[ $(echo "${RCPTTO}" | grep -ci "${DOMAIN}") -gt 0 ]] && SPAMTHRESHOLD=${SPAMTH}
	done 

	[[ ${VERBOSE} -gt 0 ]] && print -u2 "Rcptdomain: ${RCPTTO#*@} -- Threshold: ${SPAMTHRESHOLD}"

	if [[ ${SPAMTHRESHOLD} -ge 0 ]]; then
#
## Spam recognition -- the following codes is only useful for SpamAssassins spamc version 3.x
#
		SPAM=$(SPAMMER ${SPAMMERARGS} < ${MESSAGE} > ${MESSAGE}_$$ && mv ${MESSAGE}_$$ ${MESSAGE} || exit 53)
		SPAM=$(822FIELD "X-Spam-Level" < ${MESSAGE} | head -n 1)
		SPAM=${SPAM# } 

		[[ ${VERBOSE} -gt 0 ]] && print -u2 echo "[$(echo `SPAMMER -V` | tr -d '\n')]: ${SPAMTHRESHOLD}"

		if [[ "x${SPAM}" != "x" ]]; then
			if [[ $(echo "${SPAM}" | grep -c "X-Spam-Level") -gt 0 ]]; then
				SPAMC=$(echo "${SPAM##X-Spam-Level:}" tr -d ' ' | tr -d '\n' | wc -c)
			fi
		else
			SPAMC=$(echo "${SPAM}" | awk -F"/" '{print $1}' | awk -F"." '{print $1}')
		fi
		[[ ${VERBOSE} -gt 0 ]] && print -u2 "Spam: ${SPAM} - Spamc: ${SPAMC}"
#
## Spam rejection
#
		if [[ ${SPAMC} -gt 0 && ${SPAMC} -gt ${SPAMTHRESHOLD} ]]; then
#
# If you enable one of the next lines, the sender receives a rejection, while the email is let thru
#
#                       ${SQMAIL}/bin/forward ${MAILFROM} "${RCPTTO}" < ${MESSAGE}
#                       ${SQMAIL}/bin/forward ${MAILFROM} "${DELIVERTO}" < ${MESSAGE}
			export SPAMSCORE="${SPAMC}"
			RC=33
		fi
	fi
fi

[[ ${RC} -eq 0 ]] && ${SQMAIL}/bin/qmail-queue < ${MESSAGE}

rm ${MESSAGE}
exit ${RC}
