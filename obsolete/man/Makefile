# Don't edit Makefile! Use ../conf-* for configuration.

SHELL=/bin/sh

default: modules dev docs

addresses.0: \
addresses.5
	nroff -man addresses.5 > addresses.0

alloc.0: \
alloc.3
	nroff -man alloc.3 > alloc.0

bouncesaying.0: \
bouncesaying.1
	nroff -man bouncesaying.1 > bouncesaying.0

case.0: \
case.3
	nroff -man case.3 > case.0

cdb.0: \
cdb.3
	nroff -man cdb.3 > cdb.0

coe.0: \
coe.3
	nroff -man coe.3 > coe.0

columnt.0: \
columnt.1
	nroff -man columnt.1 > columnt.0

condredirect.0: \
condredirect.1
	nroff -man condredirect.1 > condredirect.0

datetime.0: \
datetime.3
	nroff -man datetime.3 > datetime.0

dev:\
alloc.0 case.0 coe.0 datetime.0 direntry.0 env.0 error.0 error_str.0 \
error_temp.0 fd_copy.0 fd_move.0 fifo_make.0 getln.0 getln2.0 now.0 \
sgetopt.0 stralloc.0 subgetopt.0 wait.0 

direntry.0: \
direntry.3
	nroff -man direntry.3 > direntry.0

docs:\
addresses.0 dot-qmail.0 envelopes.0 cdb.0 forgeries.0 mbox.0 maildir.0 \
qmail-command.0 qmail-header.0 qmail-limits.0 tcp-environ.0

dot-qmail.0: \
dot-qmail.5
	nroff -man dot-qmail.5 > dot-qmail.0

dot-qmail.5: \
dot-qmail.9 ../conf-home ../conf-break ../conf-spawn
	cat dot-qmail.9 \
	| sed s}HOME}"`head -1 ../conf-home`"}g \
	| sed s}BREAK}"`head -1 ../conf-break`"}g \
	| sed s}SPAWN}"`head -1 ../conf-spawn`"}g \
	> dot-qmail.5

env.0: \
env.3
	nroff -man env.3 > env.0

envelopes.0: \
envelopes.5
	nroff -man envelopes.5 > envelopes.0

error.0: \
error.3
	nroff -man error.3 > error.0

error_str.0: \
error_str.3
	nroff -man error_str.3 > error_str.0

error_temp.0: \
error_temp.3
	nroff -man error_temp.3 > error_temp.0

except.0: \
except.1
	nroff -man except.1 > except.0

fastforward.0: \
fastforward.1
	nroff -man fastforward.1 > fastforward.0

fd_copy.0: \
fd_copy.3
	nroff -man fd_copy.3 > fd_copy.0

fd_move.0: \
fd_move.3
	nroff -man fd_move.3 > fd_move.0

fifo_make.0: \
fifo_make.3
	nroff -man fifo_make.3 > fifo_make.0

forgeries.0: \
forgeries.7
	nroff -man forgeries.7 > forgeries.0

forward.0: \
forward.1
	nroff -man forward.1 > forward.0

getln.0: \
getln.3
	nroff -man getln.3 > getln.0

getln2.0: \
getln2.3
	nroff -man getln2.3 > getln2.0

maildir.0: \
maildir.5
	nroff -man maildir.5 > maildir.0

maildir2mbox.0: \
maildir2mbox.1
	nroff -man maildir2mbox.1 > maildir2mbox.0

maildirmake.0: \
maildirmake.1
	nroff -man maildirmake.1 > maildirmake.0

maildirwatch.0: \
maildirwatch.1
	nroff -man maildirwatch.1 > maildirwatch.0

mailsubj.0: \
mailsubj.1
	nroff -man mailsubj.1 > mailsubj.0

matchup.0: \
matchup.1
	nroff -man matchup.1 > matchup.0

mbox.0: \
mbox.5
	nroff -man mbox.5 > mbox.0

modules: \
qmail-local.0 qmail-lspawn.0 qmail-getpw.0 qmail-remote.0 qmail-smtpam.0 \
qmail-rspawn.0 qmail-clean.0 qmail-send.0 qmail-start.0 splogger.0 spfquery.0 \
qmail-queue.0 qmail-inject.0 mailsubj.0 qmail-showctl.0 qmail-newu.0 \
qmail-badmimetypes.0 qmail-badloadertypes.0 qmail-recipients.0 qmail-mfrules.0 \
qmail-pw2u.0 qmail-qread.0 qmail-qstat.0 qmail-tcpto.0 qmail-tcpok.0 \
qmail-pop3d.0 qmail-popup.0 qmail-qmqpc.0 qmail-qmqpd.0 qmail-qmtpd.0 \
qmail-smtpd.0 qmail-newmrh.0 qmail-mrtg.0 qmail-users.0 qreceipt.0 qbiff.0 \
forward.0 preline.0 condredirect.0 bouncesaying.0 except.0 maildirmake.0 \
maildir2mbox.0 maildirwatch.0 sqmail.0 qmail-authuser.0 tai64nfrac.0 \
columnt.0 matchup.0 xqp.0 xrecipient.0 xsender.0 newaliases.0 newinclude.0 \
fastforward.0 printforward.0 printmaillist.0 setforward.0 setmaillist.0 

newaliases.0: \
newaliases.1
	nroff -man newaliases.1 > newaliases.0

newinclude.0: \
newinclude.1
	nroff -man newinclude.1 > newinclude.0

now.0: \
now.3
	nroff -man now.3 > now.0

preline.0: \
preline.1
	nroff -man preline.1 > preline.0

printforward.0: \
printforward.1
	nroff -man printforward.1 > printforward.0

printmaillist.0: \
printmaillist.1
	nroff -man printmaillist.1 > printmaillist.0

qbiff.0: \
qbiff.1
	nroff -man qbiff.1 > qbiff.0

qmail-clean.0: \
qmail-clean.8
	nroff -man qmail-clean.8 > qmail-clean.0

qmail-authuser.0: \
qmail-authuser.8
	nroff -man qmail-authuser.8 > qmail-authuser.0

qmail-authuser.8: \
qmail-authuser.9 ../conf-home  ../conf-break ../conf-spawn
	cat qmail-authuser.9 \
	| sed s}HOME}"`head -1 ../conf-home`"}g \
	> qmail-authuser.8

qmail-badmimetypes.0: \
qmail-badmimetypes.8
	nroff -man qmail-badmimetypes.8 > qmail-badmimetypes.0

qmail-badmimetypes.8: \
qmail-badmimetypes.9 ../conf-home ../conf-break ../conf-spawn
	cat qmail-badmimetypes.9 \
	| sed s}HOME}"`head -1 ../conf-home`"}g \
	| sed s}BREAK}"`head -1 ../conf-break`"}g \
	| sed s}SPAWN}"`head -1 ../conf-spawn`"}g \
	> qmail-badmimetypes.8

qmail-badloadertypes.0: \
qmail-badloadertypes.8
	nroff -man qmail-badloadertypes.8 > qmail-badloadertypes.0

qmail-badloadertypes.8: \
qmail-badloadertypes.9 ../conf-home ../conf-break ../conf-spawn
	cat qmail-badloadertypes.9 \
	| sed s}HOME}"`head -1 ../conf-home`"}g \
	| sed s}BREAK}"`head -1 ../conf-break`"}g \
	| sed s}SPAWN}"`head -1 ../conf-spawn`"}g \
	> qmail-badloadertypes.8

qmail-command.0: \
qmail-command.8
	nroff -man qmail-command.8 > qmail-command.0

qmail-control.0: \
qmail-control.5
	nroff -man qmail-control.5 > qmail-control.0

qmail-control.5: \
qmail-control.9 ../conf-home ../conf-break ../conf-spawn
	cat qmail-control.9 \
	| sed s}HOME}"`head -1 ../conf-home`"}g \
	| sed s}BREAK}"`head -1 ../conf-break`"}g \
	| sed s}SPAWN}"`head -1 ../conf-spawn`"}g \
	> qmail-control.5

qmail-getpw.0: \
qmail-getpw.8
	nroff -man qmail-getpw.8 > qmail-getpw.0

qmail-getpw.8: \
qmail-getpw.9 ../conf-home ../conf-break ../conf-spawn
	cat qmail-getpw.9 \
	| sed s}HOME}"`head -1 ../conf-home`"}g \
	| sed s}BREAK}"`head -1 ../conf-break`"}g \
	| sed s}SPAWN}"`head -1 ../conf-spawn`"}g \
	> qmail-getpw.8

qmail-header.0: \
qmail-header.5
	nroff -man qmail-header.5 > qmail-header.0

qmail-inject.0: \
qmail-inject.8
	nroff -man qmail-inject.8 > qmail-inject.0

qmail-limits.0: \
qmail-limits.7
	nroff -man qmail-limits.7 > qmail-limits.0

qmail-limits.7: \
qmail-limits.9 ../conf-home ../conf-break ../conf-spawn
	cat qmail-limits.9 \
	| sed s}HOME}"`head -1 ../conf-home`"}g \
	| sed s}BREAK}"`head -1 ../conf-break`"}g \
	| sed s}SPAWN}"`head -1 ../conf-spawn`"}g \
	> qmail-limits.7

qmail-local.0: \
qmail-local.8
	nroff -man qmail-local.8 > qmail-local.0

qmail-log.0: \
qmail-log.5
	nroff -man qmail-log.5 > qmail-log.0

qmail-lspawn.0: \
qmail-lspawn.8
	nroff -man qmail-lspawn.8 > qmail-lspawn.0

qmail-mfrules.0: \
qmail-mfrules.8
	nroff -man qmail-mfrules.8 > qmail-mfrules.0

qmail-mfrules.8: \
qmail-mfrules.9 ../conf-home ../conf-break ../conf-spawn
	cat qmail-mfrules.9 \
	| sed s}HOME}"`head -1 ../conf-home`"}g \
	| sed s}BREAK}"`head -1 ../conf-break`"}g \
	| sed s}SPAWN}"`head -1 ../conf-spawn`"}g \
	> qmail-mfrules.8

qmail-mrtg.0: \
qmail-mrtg.8
	nroff -man qmail-mrtg.8 > qmail-mrtg.0

qmail-newmrh.0: \
qmail-newmrh.8
	nroff -man qmail-newmrh.8 > qmail-newmrh.0

qmail-newmrh.8: \
qmail-newmrh.9 ../conf-home ../conf-break ../conf-spawn
	cat qmail-newmrh.9 \
	| sed s}HOME}"`head -1 ../conf-home`"}g \
	| sed s}BREAK}"`head -1 ../conf-break`"}g \
	| sed s}SPAWN}"`head -1 ../conf-spawn`"}g \
	> qmail-newmrh.8

qmail-newu.0: \
qmail-newu.8
	nroff -man qmail-newu.8 > qmail-newu.0

qmail-newu.8: \
qmail-newu.9 ../conf-home ../conf-break ../conf-spawn
	cat qmail-newu.9 \
	| sed s}HOME}"`head -1 ../conf-home`"}g \
	| sed s}BREAK}"`head -1 ../conf-break`"}g \
	| sed s}SPAWN}"`head -1 ../conf-spawn`"}g \
	> qmail-newu.8

qmail-pop3d.0: \
qmail-pop3d.8
	nroff -man qmail-pop3d.8 > qmail-pop3d.0

qmail-popup.0: \
qmail-popup.8
	nroff -man qmail-popup.8 > qmail-popup.0

qmail-pw2u.0: \
qmail-pw2u.8
	nroff -man qmail-pw2u.8 > qmail-pw2u.0

qmail-pw2u.8: \
qmail-pw2u.9 ../conf-home ../conf-break ../conf-spawn
	cat qmail-pw2u.9 \
	| sed s}HOME}"`head -1 ../conf-home`"}g \
	| sed s}BREAK}"`head -1 ../conf-break`"}g \
	| sed s}SPAWN}"`head -1 ../conf-spawn`"}g \
	> qmail-pw2u.8

qmail-qmqpc.0: \
qmail-qmqpc.8
	nroff -man qmail-qmqpc.8 > qmail-qmqpc.0

qmail-qmqpd.0: \
qmail-qmqpd.8
	nroff -man qmail-qmqpd.8 > qmail-qmqpd.0

qmail-qmtpd.0: \
qmail-qmtpd.8
	nroff -man qmail-qmtpd.8 > qmail-qmtpd.0

qmail-qread.0: \
qmail-qread.8
	nroff -man qmail-qread.8 > qmail-qread.0

qmail-qstat.0: \
qmail-qstat.8
	nroff -man qmail-qstat.8 > qmail-qstat.0

qmail-queue.0: \
qmail-queue.8
	nroff -man qmail-queue.8 > qmail-queue.0

qmail-recipients.0: \
qmail-recipients.8
	nroff -man qmail-recipients.8 > qmail-recipients.0

qmail-recipients.8: \
qmail-recipients.9 ../conf-home ../conf-break ../conf-spawn
	cat qmail-recipients.9 \
	| sed s}HOME}"`head -1 ../conf-home`"}g \
	| sed s}BREAK}"`head -1 ../conf-break`"}g \
	| sed s}SPAWN}"`head -1 ../conf-spawn`"}g \
	> qmail-recipients.8

qmail-remote.0: \
qmail-remote.8
	nroff -man qmail-remote.8 > qmail-remote.0

qmail-rspawn.0: \
qmail-rspawn.8
	nroff -man qmail-rspawn.8 > qmail-rspawn.0

qmail-send.0: \
qmail-send.8
	nroff -man qmail-send.8 > qmail-send.0

qmail-send.8: \
qmail-send.9 ../conf-home ../conf-break ../conf-spawn
	cat qmail-send.9 \
	| sed s}HOME}"`head -1 ../conf-home`"}g \
	| sed s}BREAK}"`head -1 ../conf-break`"}g \
	| sed s}SPAWN}"`head -1 ../conf-spawn`"}g \
	> qmail-send.8

qmail-showctl.0: \
qmail-showctl.8
	nroff -man qmail-showctl.8 > qmail-showctl.0

qmail-smtpam.0: \
qmail-smtpam.8
	nroff -man qmail-smtpam.8 > qmail-smtpam.0

qmail-smtpd.0: \
qmail-smtpd.8
	nroff -man qmail-smtpd.8 > qmail-smtpd.0

qmail-start.0: \
qmail-start.8
	nroff -man qmail-start.8 > qmail-start.0

qmail-start.8: \
qmail-start.9 ../conf-home ../conf-break ../conf-spawn
	cat qmail-start.9 \
	| sed s}HOME}"`head -1 ../conf-home`"}g \
	| sed s}BREAK}"`head -1 ../conf-break`"}g \
	| sed s}SPAWN}"`head -1 ../conf-spawn`"}g \
	> qmail-start.8

qmail-tcpok.0: \
qmail-tcpok.8
	nroff -man qmail-tcpok.8 > qmail-tcpok.0

qmail-tcpto.0: \
qmail-tcpto.8
	nroff -man qmail-tcpto.8 > qmail-tcpto.0

qmail-users.0: \
qmail-users.5
	nroff -man qmail-users.5 > qmail-users.0

qmail-users.5: \
qmail-users.9 ../conf-home ../conf-break ../conf-spawn
	cat qmail-users.9 \
	| sed s}HOME}"`head -1 ../conf-home`"}g \
	| sed s}BREAK}"`head -1 ../conf-break`"}g \
	| sed s}SPAWN}"`head -1 ../conf-spawn`"}g \
	> qmail-users.5

qreceipt.0: \
qreceipt.1
	nroff -man qreceipt.1 > qreceipt.0

sgetopt.0: \
sgetopt.3
	nroff -man sgetopt.3 > sgetopt.0

setforward.0: \
setforward.1
	nroff -man setforward.1 > setforward.0

setmaillist.0: \
setmaillist.1
	nroff -man setmaillist.1 > setmaillist.0

spfquery.0: \
spfquery.8
	nroff -man spfquery.8 > spfquery.0

splogger.0: \
splogger.8
	nroff -man splogger.8 > splogger.0

sqmail.0: \
sqmail.7
	nroff -man sqmail.7 > sqmail.0

sqmail.7: \
sqmail.9 ../package/version
	cat sqmail.9 \
	| sed s}VERSION}"`head -1 ../package/version`"}g \
	> sqmail.7

stralloc.0: \
stralloc.3
	nroff -man stralloc.3 > stralloc.0

subgetopt.0: \
subgetopt.3
	nroff -man subgetopt.3 > subgetopt.0

tai64nfrac.0: \
tai64nfrac.5
	nroff -man tai64nfrac.5 > tai64nfrac.0

tcp-environ.0: \
tcp-environ.5
	nroff -man tcp-environ.5 > tcp-environ.0

wait.0: \
wait.3
	nroff -man wait.3 > wait.0

xqp.0: \
xqp.1
	nroff -man xqp.1 > xqp.0

xrecipient.0: \
xrecipient.1
	nroff -man xrecipient.1 > xrecipient.0

xsender.0: \
xsender.1
	nroff -man xsender.1 > xsender.0

clean: \
TARGETS
	rm -f `cat TARGETS`
