#!/bin/sh
#set -o xtrace

SQMAIL=HOME
QMAIL_LOGS="/var/log"
SVC_HOME="/service"

SKELETON_SOURCE="${SQMAL}/source"
SKELETON_ME="mail.example.com"
SKELETON_CONCURRENCYREMOTE="120"
SKELETON_QUEUELIFETIME="1440"
SKELETON_PORT="1000"

SKELETON_DIR="${SQMAL}/skeleton"
SKELETON_QMAIL_SEND="start_run"
SKELETON_QMAIL_LOGS="log_start_run"
SKELETON_QMAIL_SMTP="smtpd_run"
SKELETON_SMTP_LOGS="log_smtpd_run"
SKELETON_QMAIL_QMTPD="qmtpd_run"
SKELETON_QMAIL_LOGS="log_qmtpd_run"

if [[ -f conf-qmq ]]; then
	MYINSTANCES=$(grep -v "^#" conf-qmq | cut -d"#" -f1)
	echo "The follwing qmail instances are defined:"
	echo ""
	grep -v "^#" conf-qmq
	echo ""
	echo "--> Use '$0 build' to setup the instances."
	echo "--> Use '$0 conf' to deploy the instances."
	echo "--> Use '$0 all' to setup and deploy the instances."
	echo ""
	echo "Note (1): qmail will be installed at '${SQMAL}'." 
	echo "Note (2): qmail-logs will be installed at '${QMAIL_LOGS}/qmail-send-INSTANCEID' ...."
	echo "Note (3): 'service' base directory is '${SVC_HOME}'."
	echo "Note (4): 'qmail-send' will be initially touched 'down' at every instance."
	echo "Note (5): Initial configuration is: 'queuelifetime=${SKELETON_QUEUELIFETIME}', concurrencyremote=${SKELETON_CONCURRENCYREMOTE}'."
	echo "Note (6): Communication from the primary qmail instance to the secondaries is bsed on 'QMTP'.
	echo ""
	echo "Enter 'ctl-c' to abort; or continue installation."
else
	echo "Configuration file 'conf-qmq' not available."
	exit 1
fi

set -A INSTANCES ${MYINSTANCES}

if [[ "$1" = "build" || "$1" = "all" ]]; then
	for MAPPING in ${INSTANCES[@]}
	do
		INSTANCE=$(echo "${MAPPING}" | awk -F: '{print $1}')
		NAME=$(echo "${MAPPING}" | awk -F: '{print $2}')
		if [[ "x${NAME}" != "x" ]]; then
			mkdir ${SQMAL}-${INSTANCE}
			mkdir ${QMAL_LOGS}/qmail-${INSTANCE}
			chown qmaill ${QMAL_LOGS}/qmail-${INSTANCE}
			mkdir ${QMAL_LOGS}/qmtp-${INSTANCE}
			chown qmaill ${QMAL_LOGS}/qmtp-${INSTANCE}
			cd ${QMAIL_SOURCE}
			echo "${SQMAL}-${INSTANCE}" > conf-qmail
			make
			make setup check
			echo "${SKELETON_ME}" > ${SQMAL}-${INSTANCE}/control/me
			echo "${SKELETON_CONCURRENCYREMOTE}" > ${SQMAL}-${INSTANCE}/control/concurrencyremote
			echo "${SKELETON_QUEUELIFETIME}" > ${SQMAL}-${INSTANCE}/control/queuelifetime
		fi
	done
elif [[ "$1" == "conf" || "$1" == "all" ]]; then
	for MAPPING in ${INSTANCES[@]}
	do
		INSTANCE=$(echo "${MAPPING}" | awk -F: '{print $1}')
		NAME=$(echo "${MAPPING}" | awk -F: '{print $2}')
		if [[ "x${NAME}" != "x" ]]; then
			integer PORT=$((${SKELETON_PORT}+${INSTANCE}))
			echo "Selecting ${PORT} for instance ${INSTANCE} ..."
#
## qmail-send/qmail-start
#
			mkdir -p ${SQMAL}-${INSTANCE}/svc/qmail-${INSTANCE}-start/log
			touch ${SQMAL}-${INSTANCE}/svc/qmail-${INSTANCE}-start/down

			sed s/INSTANCE/${INSTANCE}/g ${SKELETON_DIR}/${SKELETON_QMAIL_LOGS} > \
				${SQMAL}-${INSTANCE}/svc/qmail-${INSTANCE}-start/log/run
			chmod +x ${SQMAL}-${INSTANCE}/svc/qmail-${INSTANCE}-start/log/run

			sed s/INSTANCE/${INSTANCE}/g ${SKELETON_DIR}/${SKELETON_QMAIL_SEND} > \
				${SQMAL}-${INSTANCE}/svc/qmail-${INSTANCE}-start/run
			chmod +x ${SQMAL}-${INSTANCE}/svc/qmail-${INSTANCE}-start/run
			ln -s ${SQMAL}-${INSTANCE}/svc/qmail-${INSTANCE}-start ${SVC_HOME}/qmail-${INSTANCE}-start
#
## qmail-qmtpd
#
			mkdir -p ${SQMAL}-${INSTANCE}/svc/qmail-${INSTANCE}-qmtpd/log

			sed s/INSTANCE/${INSTANCE}/g ${SKELETON_DIR}/ ${SKELETON_QMTPD_LOGS} > \
				${SQMAL}-${INSTANCE}/svc/qmail-${INSTANCE}-qmtpd/log/run
			chmod +x ${SQMAL}-${INSTANCE}/svc/qmail-${INSTANCE}-qmtpd/log/run

			sed s/INSTANCE/${INSTANCE}/g ${SKELETON_DIR}/ ${SKELETON_QMAIL_QMTPD} | \
				sed s/PORT/${PORT}/g > ${SQMAL}-${INSTANCE}/svc/qmail-${INSTANCE}-qmtpd/run
			chmod +x ${SQMAL}-${INSTANCE}/svc/qmail-${INSTANCE}-qmtpd/run
#
## link to /svc
# 
			ln -s ${SQMAL}-${INSTANCE}/svc/qmail-${INSTANCE}-qmtpd ${SVC_HOME}/qmail-${INSTANCE}-qmtpd
			ln -s ${SQMAL}-${INSTANCE}/svc/qmail-${INSTANCE}-start ${SVC_HOME}/qmail-${INSTANCE}-start
		fi
	done
else
	echo "Please provide either 'build' and 'conf' for individual steps; or 'all' for all-inclusive."
fi

exit 0
