# Don't edit Makefile! Use conf-* for configuration.

SHELL=/bin/sh

default: it-scan it-recipients \
it-x509 it-pam 

clean: \
TARGETS
	rm -f `cat TARGETS`

it-pam: \
ldap-pam qmail-vmailbox qmail-vpopbox

it-recipients: \
qmail-alias2recipients qmail-pwd2recipients \
qmail-users2recipients qmail-vpopmail2recipients 

it-scan: \
qmail-queue-scan

it-x509: \
x509fingerprint

ldap-pam: \
perl-auto.sh ldap-pam.pl ../conf-home
	cat perl-auto.sh ldap-pam.pl \
	| sed s}PERL}"`which perl`"}g \
	| sed s}HOME}"`head -1 ../conf-home`"}g \
	> ldap-pam 
	chmod 755 ldap-pam

qmail-alias2recipients: \
warn-auto.sh qmail-alias2recipients.sh ../conf-home
	cat warn-auto.sh qmail-alias2recipients.sh \
	| sed s}HOME}"`head -1 ../conf-home`"}g \
	> qmail-alias2recipients
	chmod 755 qmail-alias2recipients

qmail-pwd2recipients: \
warn-auto.sh qmail-pwd2recipients.sh ../conf-home
	cat warn-auto.sh qmail-pwd2recipients.sh \
	| sed s}HOME}"`head -1 ../conf-home`"}g \
	> qmail-pwd2recipients
	chmod 755 qmail-pwd2recipients

qmail-mrtg-queue: \
warn-auto.sh qmail-mrtg-queue.sh ../conf-home
	cat warn-auto.sh qmail-mrtg-queue.sh \
	| sed s}HOME}"`head -1 ../conf-home`"}g \
	> qmail-mrtg-queue
	chmod 755 qmail-mrtg-queue

qmail-queue-scan: \
warn-auto.sh qmail-queue-scan.sh ../conf-home
	cat warn-auto.sh qmail-queue-scan.sh \
	| sed s}HOME}"`head -1 ../conf-home`"}g \
	> qmail-queue-scan
	chmod 755 qmail-queue-scan

qmail-users2recipients: \
warn-auto.sh qmail-users2recipients.sh ../conf-home
	cat warn-auto.sh qmail-users2recipients.sh \
	| sed s}HOME}"`head -1 ../conf-home`"}g \
	> qmail-users2recipients
	chmod 755 qmail-users2recipients

qmail-vpopmail2recipients: \
warn-auto.sh qmail-vpopmail2recipients.sh ../conf-home
	cat warn-auto.sh qmail-vpopmail2recipients.sh \
	| sed s}HOME}"`head -1 ../conf-home`"}g \
	> qmail-vpopmail2recipients
	chmod 755 qmail-vpopmail2recipients

qmail-vmailbox: \
ksh-auto.sh qmail-vmailbox.sh ../conf-home
	cat ksh-auto.sh qmail-vmailbox.sh \
	| sed s}KSH}"`which ksh`"}g \
	| sed s}HOME}"`head -1 ../conf-home`"}g \
	> qmail-vmailbox
	chmod 755 qmail-vmailbox

qmail-vpopbox: \
ksh-auto.sh qmail-vpopbox.sh ../conf-home
	cat ksh-auto.sh qmail-vpopbox.sh \
	| sed s}KSH}"`which ksh`"}g \
	| sed s}HOME}"`head -1 ../conf-home`"}g \
	> qmail-vpopbox
	chmod 4755 qmail-vpopbox && \
        chown -f vpopmail:vchkpw qmail-vpopbox

x509fingerprint: \
warn-auto.sh x509fingerprint.sh
	cat warn-auto.sh x509fingerprint.sh \
	> x509fingerprint
	chmod 755 x509fingerprint
