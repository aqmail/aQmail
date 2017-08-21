# Don't edit Makefile! Use ../conf-* for configuration.

SHELL=/bin/sh

COMPILE=./compile
MAKELIB=./makelib
LOAD=./load

default: libs it-analog it-base it-setup it-dns it-control \
it-mbox it-forward it-pop it-queue \
it-clients it-user it-dns it-server it-log

clean:
	rm -f *.a *.o  `cat TARGETS`
	cd ucspissl ; make clean
	cd qlibs ; make clean


libs:
#	cd qlibs ; make
	cd ucspissl ; make
	cp ucspissl/ucspissl.a ucspissl.a
	cp ucspissl/ssl.lib ssl.lib

byte.h:

alloc.a: \
makelib alloc.o alloc_re.o
	./makelib alloc.a alloc.o alloc_re.o

alloc.o: \
compile alloc.c alloc.h error.h
	./compile alloc.c

alloc_re.o: \
compile alloc_re.c alloc.h
	./compile alloc_re.c

auto-ccld.sh:
#../conf-cc conf-ld warn-auto.sh
#	( cat warn-auto.sh; \
#	echo CC=\'`head -1 ../conf-cc`\'; \
#	echo LD=\'`head -1 ../conf-ld`\' \
#	) > auto-ccld.sh

auto-gid: \
load auto-gid.o substdio.a error.a str.a fs.a
	./load auto-gid substdio.a error.a str.a fs.a 

auto-gid.o: \
compile auto-gid.c subfd.h substdio.h readwrite.h exit.h \
scan.h fmt.h
	./compile auto-gid.c

auto-int: \
load auto-int.o substdio.a error.a str.a fs.a
	./load auto-int substdio.a error.a str.a fs.a 

auto-int.o: \
compile auto-int.c substdio.h readwrite.h exit.h scan.h fmt.h
	./compile auto-int.c

auto-int8: \
load auto-int8.o substdio.a error.a str.a fs.a
	./load auto-int8 substdio.a error.a str.a fs.a 

auto-int8.o: \
compile auto-int8.c substdio.h readwrite.h exit.h scan.h fmt.h
	./compile auto-int8.c

auto-str: \
load auto-str.o substdio.a error.a str.a
	./load auto-str substdio.a error.a str.a 

auto-str.o: \
compile auto-str.c substdio.h readwrite.h exit.h
	./compile auto-str.c

auto-uid: \
load auto-uid.o substdio.a error.a str.a fs.a
	./load auto-uid substdio.a error.a str.a fs.a 

auto-uid.o: \
compile auto-uid.c subfd.h substdio.h readwrite.h exit.h \
scan.h fmt.h
	./compile auto-uid.c

auto_break.c: \
auto-str
	./auto-str auto_break \
	"`head -1 conf-break`" > auto_break.c

auto_break.o: \
compile auto_break.c
	./compile auto_break.c

auto_patrn.c: \
auto-int8
	./auto-int8 auto_patrn `head -1 conf-patrn` > auto_patrn.c

auto_patrn.o: \
compile auto_patrn.c
	./compile auto_patrn.c

auto_qmail.c: \
auto-str
	./auto-str auto_qmail `head -1 conf-home` > auto_qmail.c

auto_qmail.o: \
compile auto_qmail.c
	./compile auto_qmail.c

auto_spawn.c: \
auto-int
	./auto-int auto_spawn `head -1 conf-spawn` > auto_spawn.c

auto_spawn.o: \
compile auto_spawn.c
	./compile auto_spawn.c

auto_split.c: \
auto-int
	./auto-int auto_split `head -1 conf-split` > auto_split.c

auto_split.o: \
compile auto_split.c
	./compile auto_split.c

auto_uids.c: \
auto-uid auto-gid
#../conf-users ../conf-groups
	( ./auto-uid auto_uida `head -1 conf-users` \
	&&./auto-uid auto_uidd `head -2 conf-users | tail -1` \
	&&./auto-uid auto_uidl `head -3 conf-users | tail -1` \
	&&./auto-uid auto_uido `head -4 conf-users | tail -1` \
	&&./auto-uid auto_uidp `head -5 conf-users | tail -1` \
	&&./auto-uid auto_uidq `head -6 conf-users | tail -1` \
	&&./auto-uid auto_uidr `head -7 conf-users | tail -1` \
	&&./auto-uid auto_uids `head -8 conf-users | tail -1` \
	&&./auto-gid auto_gidq `head -1 conf-groups` \
	&&./auto-gid auto_gidn `head -2 conf-groups | tail -1` \
	) > auto_uids.c.tmp && mv auto_uids.c.tmp auto_uids.c

auto_uids.o: \
compile auto_uids.c
	./compile auto_uids.c

auto_usera.c: \
auto-str
# ../conf-users
	./auto-str auto_usera `head -1 conf-users` > auto_usera.c

auto_usera.o: \
compile auto_usera.c
	./compile auto_usera.c

base64.o: \
compile base64.c base64.h stralloc.h substdio.h str.h
	./compile base64.c

md5c.o : \
compile md5c.c md5.h
	./compile md5c.c

hmac_md5.o : \
compile hmac_md5.c hmac_md5.h global.h str.h
	./compile hmac_md5.c

bouncesaying: \
load bouncesaying.o strerr.a error.a substdio.a str.a wait.a
	./load bouncesaying strerr.a error.a substdio.a str.a \
	wait.a 

bouncesaying.o: \
compile bouncesaying.c strerr.h error.h wait.h sig.h exit.h
	./compile bouncesaying.c

byte_chr.o: \
compile byte_chr.c
	./compile byte_chr.c

byte_copy.o: \
compile byte_copy.c
	./compile byte_copy.c

byte_cr.o: \
compile byte_cr.c
	./compile byte_cr.c

byte_diff.o: \
compile byte_diff.c
	./compile byte_diff.c

byte_rchr.o: \
compile byte_rchr.c
	./compile byte_rchr.c

byte_zero.o: \
compile byte_zero.c
	./compile byte_zero.c

case.a: \
makelib case_diffb.o case_diffs.o case_lowerb.o case_lowers.o \
case_starts.o case_startb.o case_diffrs.o
	./makelib case.a case_diffb.o case_diffs.o case_lowerb.o \
	case_lowers.o case_starts.o case_startb.o case_diffrs.o

case_diffb.o: \
compile case_diffb.c case.h
	./compile case_diffb.c

case_diffs.o: \
compile case_diffs.c case.h
	./compile case_diffs.c

case_diffrs.o: \
compile case_diffrs.c case.h str.h
	./compile case_diffrs.c

case_lowerb.o: \
compile case_lowerb.c case.h
	./compile case_lowerb.c

case_lowers.o: \
compile case_lowers.c case.h
	./compile case_lowers.c

case_startb.o: \
compile case_startb.c case.h
	./compile case_startb.c

case_starts.o: \
compile case_starts.c case.h
	./compile case_starts.c

cdb.a: \
makelib cdb.o cdb_hash.o cdb_unpack.o cdb_seek.o cdb_make.o
	./makelib cdb.a cdb.o cdb_hash.o cdb_unpack.o cdb_seek.o cdb_make.o

cdb.o: \
compile cdb.c readwrite.h error.h seek.h
# cdb.h uint32.h
	./compile cdb.c

cdb_hash.o: \
compile cdb_hash.c cdb.h uint32.h
	./compile cdb_hash.c

cdb_seek.o: \
compile cdb_seek.c cdb.h uint32.h
	./compile cdb_seek.c

cdb_unpack.o: \
compile cdb_unpack.c cdb.h uint32.h
	./compile cdb_unpack.c

cdbmake.a: \
makelib cdbmake_pack.o cdbmake_hash.o cdbmake_add.o
	./makelib cdbmake.a cdbmake_pack.o cdbmake_hash.o \
	cdbmake_add.o

cdbmake_add.o: \
compile cdbmake_add.c cdbmake.h uint32.h
	./compile cdbmake_add.c

cdbmake_hash.o: \
compile cdbmake_hash.c cdbmake.h uint32.h
	./compile cdbmake_hash.c

cdbmake_pack.o: \
compile cdbmake_pack.c cdbmake.h uint32.h
	./compile cdbmake_pack.c

cdbmss.o: \
compile cdbmss.c readwrite.h seek.h alloc.h cdbmss.h cdb.h cdbmake.h \
uint32.h substdio.h
	./compile cdbmss.c

cdb_make.o: \
compile cdb_make.c readwrite.h seek.h error.h alloc.h cdb.h uint32.h \
cdb_make.h buffer.h uint32.h
	./compile cdb_make.c

chkshsgr: \
load chkshsgr.o
	./load chkshsgr 

chkshsgr.o: \
compile chkshsgr.c exit.h
	./compile chkshsgr.c

chkspawn: \
load chkspawn.o substdio.a error.a str.a fs.a auto_spawn.o
	./load chkspawn substdio.a error.a str.a fs.a auto_spawn.o 

chkspawn.o: \
compile chkspawn.c substdio.h subfd.h fmt.h select.h \
exit.h auto_spawn.h
	./compile chkspawn.c

coe.o: \
compile coe.c coe.h
	./compile coe.c

columnt: \
load columnt.o slurpclose.o strerr.a substdio.a stralloc.a alloc.a \
error.a str.a
	./load columnt slurpclose.o strerr.a substdio.a stralloc.a \
	alloc.a error.a str.a

columnt.o: \
compile columnt.c stralloc.h gen_alloc.h alloc.h slurpclose.h \
strerr.h substdio.h readwrite.h exit.h
	./compile columnt.c

commands.o: \
compile commands.c commands.h substdio.h stralloc.h gen_alloc.h str.h \
case.h
	./compile commands.c

compile:
#make-compile warn-auto.sh systype
#	( cat warn-auto.sh; ./make-compile "`cat systype`" ) > \
#	compile
#	chmod 755 compile

condredirect: \
load condredirect.o qmail.o strerr.a fd.a sig.a wait.a seek.a env.a \
substdio.a error.a str.a fs.a auto_qmail.o
	./load condredirect qmail.o strerr.a fd.a sig.a wait.a \
	seek.a env.a substdio.a error.a str.a fs.a auto_qmail.o 

condredirect.o: \
compile condredirect.c sig.h readwrite.h exit.h env.h error.h \
wait.h seek.h qmail.h substdio.h strerr.h fmt.h
	./compile condredirect.c

config: \
warn-auto.sh config.sh
# ../conf-home ../conf-break ../conf-split
	cat warn-auto.sh config.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	| sed s}BREAK}"`head -1 conf-break`"}g \
	| sed s}SPLIT}"`head -1 conf-split`"}g \
	> config
	chmod 755 config

config-fast:
#warn-auto.sh config-fast.sh
## ../conf-home ../conf-break ../conf-split
#	cat warn-auto.sh config-fast.sh \
#	| sed s}HOME}"`head -1 conf-home`"}g \
#	| sed s}BREAK}"`head -1 conf-break`"}g \
#	| sed s}SPLIT}"`head -1 conf-split`"}g \
#	> config-fast
#	chmod 755 config-fast

constmap.o: \
compile constmap.c constmap.h alloc.h case.h
	./compile constmap.c

control.o: \
compile control.c readwrite.h open.h getln.h stralloc.h gen_alloc.h \
substdio.h error.h control.h alloc.h scan.h
	./compile control.c

crypt.lib: \
trycrypt.c compile load
	( ( ./compile trycrypt.c && \
	./load trycrypt -lcrypt ) >/dev/null 2>&1 \
	&& echo -lcrypt || exit 0 ) > crypt.lib
	rm -f trycrypt.o trycrypt

date822fmt.o: \
compile date822fmt.c datetime.h fmt.h date822fmt.h
	./compile date822fmt.c

datemail: \
warn-auto.sh datemail.sh
# ../conf-home ../conf-break ../conf-split
	cat warn-auto.sh datemail.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	| sed s}BREAK}"`head -1 conf-break`"}g \
	| sed s}SPLIT}"`head -1 conf-split`"}g \
	> datemail
	chmod 755 datemail

datetime.a: \
makelib datetime.o datetime_un.o
	./makelib datetime.a datetime.o datetime_un.o

datetime.o: \
compile datetime.c datetime.h
	./compile datetime.c

datetime_un.o: \
compile datetime_un.c datetime.h
	./compile datetime_un.c

ddist: \
warn-auto.sh ddist.sh
#../conf-home
	cat warn-auto.sh ddist.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> ddist
	chmod 755 ddist

deferrals: \
warn-auto.sh deferrals.sh 
#../conf-home
	cat warn-auto.sh deferrals.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> deferrals
	chmod 755 deferrals

#direntry.h: \
#compile trydrent.c direntry.h1 direntry.h2
#	( ./compile trydrent.c >/dev/null 2>&1 \
#	&& cat direntry.h2 || cat direntry.h1 ) > direntry.h
#	rm -f trydrent.o

dns.lib: \
tryrsolv.c compile load socket.lib dns.o ipalloc.o ip.o strsalloc.o \
stralloc.a alloc.a error.a fs.a str.a
	( ( ./compile tryrsolv.c && ./load tryrsolv dns.o ipalloc.o \
	ip.o strsalloc.o stralloc.a alloc.a error.a fs.a str.a \
	-lresolv `cat socket.lib` ) >/dev/null 2>&1 \
	&& echo -lresolv || exit 0 ) > dns.lib
	rm -f tryrsolv.o tryrsolv

dns.o: \
compile dns.c ip.h ipalloc.h gen_alloc.h fmt.h alloc.h str.h \
stralloc.h strsalloc.h gen_alloc.h dns.h case.h
	./compile dns.c

dnscname: \
load dnscname.o dns.o dnsdoe.o ip.o ipalloc.o strsalloc.o \
stralloc.a alloc.a substdio.a error.a str.a fs.a dns.lib socket.lib
	./load dnscname dns.o dnsdoe.o ip.o ipalloc.o strsalloc.o \
	stralloc.a alloc.a substdio.a error.a str.a fs.a \
	`cat dns.lib` `cat socket.lib`

dnscname.o: \
compile dnscname.c substdio.h subfd.h stralloc.h strsalloc.h \
gen_alloc.h dns.h dnsdoe.h readwrite.h exit.h
	./compile dnscname.c

dnsdoe.o: \
compile dnsdoe.c substdio.h subfd.h exit.h dns.h dnsdoe.h
	./compile dnsdoe.c

dnsfq: \
load dnsfq.o dns.o dnsdoe.o ip.o ipalloc.o strsalloc.o \
stralloc.a alloc.a substdio.a error.a str.a fs.a dns.lib socket.lib
	./load dnsfq dns.o dnsdoe.o ip.o ipalloc.o strsalloc.o \
	stralloc.a alloc.a substdio.a error.a str.a fs.a \
	`cat dns.lib` `cat socket.lib`

dnsfq.o: \
compile dnsfq.c substdio.h subfd.h stralloc.h strsalloc.h gen_alloc.h \
dns.h dnsdoe.h ip.h ipalloc.h gen_alloc.h exit.h
	./compile dnsfq.c

dnsip: \
load dnsip.o dns.o dnsdoe.o ip.o ipalloc.o strsalloc.o \
stralloc.a alloc.a substdio.a error.a str.a fs.a dns.lib socket.lib
	./load dnsip dns.o dnsdoe.o ip.o ipalloc.o strsalloc.o \
	stralloc.a alloc.a substdio.a error.a str.a fs.a \
	`cat dns.lib` `cat socket.lib`

dnsip.o: \
compile dnsip.c substdio.h subfd.h stralloc.h strsalloc.h gen_alloc.h \
dns.h dnsdoe.h ip.h ipalloc.h gen_alloc.h exit.h
	./compile dnsip.c

dnsmxip: \
load dnsmxip.o dns.o dnsdoe.o ip.o ipalloc.o now.o strsalloc.o \
stralloc.a alloc.a substdio.a error.a str.a fs.a dns.lib socket.lib
	./load dnsmxip dns.o dnsdoe.o ip.o ipalloc.o now.o strsalloc.o \
	stralloc.a alloc.a substdio.a error.a str.a fs.a  `cat \
	dns.lib` `cat socket.lib`

dnsmxip.o: \
compile dnsmxip.c substdio.h subfd.h stralloc.h strsalloc.h \
gen_alloc.h fmt.h dns.h dnsdoe.h ip.h ipalloc.h \
now.h datetime.h exit.h
	./compile dnsmxip.c

dnsptr: \
load dnsptr.o dns.o dnsdoe.o ip.o ipalloc.o strsalloc.o \
stralloc.a alloc.a substdio.a error.a str.a fs.a dns.lib socket.lib
	./load dnsptr dns.o dnsdoe.o ip.o ipalloc.o strsalloc.o \
	stralloc.a alloc.a substdio.a error.a str.a fs.a \
	`cat dns.lib` `cat socket.lib`

dnsptr.o: \
compile dnsptr.c substdio.h subfd.h stralloc.h strsalloc.h gen_alloc.h \
str.h scan.h dns.h dnsdoe.h ip.h exit.h
	./compile dnsptr.c

dnstxt: \
load dnstxt.o dns.o dnsdoe.o ip.o ipalloc.o strsalloc.o \
stralloc.a alloc.a substdio.a error.a str.a fs.a dns.lib socket.lib
	./load dnstxt dns.o dnsdoe.o ip.o ipalloc.o strsalloc.o \
	stralloc.a alloc.a substdio.a error.a str.a fs.a \
	`cat dns.lib` `cat socket.lib`

dnstxt.o: \
compile dnstxt.c substdio.h subfd.h stralloc.h strsalloc.h gen_alloc.h \
str.h scan.h dns.h dnsdoe.h ip.h exit.h
	./compile dnstxt.c

env.a: \
makelib env.o envread.o
	./makelib env.a env.o envread.o

env.o: \
compile env.c str.h alloc.h env.h
	./compile env.c

envread.o: \
compile envread.c env.h str.h
	./compile envread.c

error.a: \
makelib error.o error_str.o error_temp.o
	./makelib error.a error.o error_str.o error_temp.o

error.o: \
compile error.c error.h
	./compile error.c

error_str.o: \
compile error_str.c error.h
	./compile error_str.c

error_temp.o: \
compile error_temp.c error.h
	./compile error_temp.c

except: \
load except.o strerr.a error.a substdio.a str.a wait.a
	./load except strerr.a error.a substdio.a str.a wait.a 

except.o: \
compile except.c strerr.h wait.h error.h exit.h
	./compile except.c

failures: \
warn-auto.sh failures.sh
	cat warn-auto.sh failures.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> failures
	chmod 755 failures

fastforward: \
load fastforward.o slurpclose.o coe.o strset.o qmail.o auto_qmail.o \
getopt.a cdb.a env.a strerr.a substdio.a stralloc.a alloc.a error.a \
case.a str.a fs.a sig.a wait.a seek.a open.a fd.a
	./load fastforward slurpclose.o coe.o strset.o qmail.o \
	auto_qmail.o getopt.a cdb.a env.a strerr.a substdio.a \
	stralloc.a alloc.a error.a case.a str.a fs.a sig.a wait.a \
	seek.a open.a fd.a

fastforward.o: \
compile fastforward.c stralloc.h gen_alloc.h substdio.h subfd.h \
substdio.h strset.h uint32.h sgetopt.h subgetopt.h readwrite.h exit.h \
strerr.h env.h sig.h qmail.h substdio.h fmt.h case.h alloc.h coe.h \
seek.h wait.h str.h slurpclose.h open.h cdb.h
	./compile fastforward.c

fd.a: \
makelib fd_copy.o fd_move.o
	./makelib fd.a fd_copy.o fd_move.o

fd_copy.o: \
compile fd_copy.c fd.h
	./compile fd_copy.c

fd_move.o: \
compile fd_move.c fd.h
	./compile fd_move.c

fifo.o: \
compile fifo.c hasmkffo.h fifo.h
	./compile fifo.c

find-systype: \
find-systype.sh auto-ccld.sh
	cat auto-ccld.sh find-systype.sh > find-systype
	chmod 755 find-systype

fmt_str.o: \
compile fmt_str.c fmt.h
	./compile fmt_str.c

fmt_strn.o: \
compile fmt_strn.c fmt.h
	./compile fmt_strn.c

fmt_uint.o: \
compile fmt_uint.c fmt.h
	./compile fmt_uint.c

fmt_uint0.o: \
compile fmt_uint0.c fmt.h
	./compile fmt_uint0.c

fmt_ulong.o: \
compile fmt_ulong.c fmt.h
	./compile fmt_ulong.c

fmt_xlong.o: \
compile fmt_xlong.c fmt.h
	./compile fmt_xlong.c

fmtqfn.o: \
compile fmtqfn.c fmtqfn.h fmt.h auto_split.h
	./compile fmtqfn.c

#fork.h: \
#compile load tryvfork.c fork.h1 fork.h2
#	( ( ./compile tryvfork.c && ./load tryvfork ) >/dev/null \
#	2>&1 \
#	&& cat fork.h2 || cat fork.h1 ) > fork.h
#	rm -f tryvfork.o tryvfork

forward: \
load forward.o qmail.o strerr.a alloc.a fd.a wait.a sig.a env.a \
substdio.a error.a str.a fs.a auto_qmail.o
	./load forward qmail.o strerr.a alloc.a fd.a wait.a sig.a \
	env.a substdio.a error.a str.a fs.a auto_qmail.o 

forward.o: \
compile forward.c sig.h readwrite.h exit.h env.h qmail.h substdio.h \
strerr.h fmt.h
	./compile forward.c

fs.a: \
makelib fmt_str.o fmt_strn.o fmt_uint.o fmt_uint0.o fmt_ulong.o \
fmt_xlong.o scan_ulong.o scan_8long.o scan_xlong.o
	./makelib fs.a fmt_str.o fmt_strn.o fmt_uint.o fmt_uint0.o \
	fmt_ulong.o fmt_xlong.o scan_ulong.o scan_8long.o scan_xlong.o

getln.a: \
makelib getln.o getln2.o
	./makelib getln.a getln.o getln2.o

getln.o: \
compile getln.c
# substdio.h stralloc.h gen_alloc.h getln.h
	./compile getln.c

getln2.o: \
compile getln2.c
# substdio.h stralloc.h gen_alloc.h getln.h
	./compile getln2.c

getopt.a: \
makelib subgetopt.o sgetopt.o
	./makelib getopt.a subgetopt.o sgetopt.o

gfrom.o: \
compile gfrom.c str.h gfrom.h
	./compile gfrom.c

hasflock.h: \
tryflock.c compile load
	( ( ./compile tryflock.c && ./load tryflock ) >/dev/null \
	2>&1 \
	&& echo \#define HASFLOCK 1 || exit 0 ) > hasflock.h
	rm -f tryflock.o tryflock

hasmkffo.h: \
trymkffo.c compile load
	( ( ./compile trymkffo.c && ./load trymkffo ) >/dev/null \
	2>&1 \
	&& echo \#define HASMKFIFO 1 || exit 0 ) > hasmkffo.h
	rm -f trymkffo.o trymkffo

hasspnam.h: \
tryspnam.c compile load
	( ( ./compile tryspnam.c && ./load tryspnam ) >/dev/null \
	2>&1 \
	&& echo \#define HASGETSPNAM 1 || exit 0 ) > hasspnam.h
	rm -f tryspnam.o tryspnam

hasuserpw.h: \
tryuserpw.c s.lib compile load
	( ( ./compile tryuserpw.c \
	&& ./load tryuserpw `cat s.lib` ) >/dev/null 2>&1 \
	&& echo \#define HASGETUSERPW 1 || exit 0 ) > hasuserpw.h
	rm -f tryuserpw.o tryuserpw

hasnpbg1.h: \
trynpbg1.c compile load open.h open.a fifo.h fifo.o select.h
	( ( ./compile trynpbg1.c \
	&& ./load trynpbg1 fifo.o open.a && ./trynpbg1 ) \
	>/dev/null 2>&1 \
	&& echo \#define HASNAMEDPIPEBUG1 1 || exit 0 ) > \
	hasnpbg1.h
	rm -f trynpbg1.o trynpbg1

hassalen.h: \
trysalen.c compile
	( ./compile trysalen.c >/dev/null 2>&1 \
	&& echo \#define HASSALEN 1 || exit 0 ) > hassalen.h

hassgact.h: \
trysgact.c compile load
	( ( ./compile trysgact.c && ./load trysgact ) >/dev/null \
	2>&1 \
	&& echo \#define HASSIGACTION 1 || exit 0 ) > hassgact.h
	rm -f trysgact.o trysgact

hassgprm.h: \
trysgprm.c compile load
	( ( ./compile trysgprm.c && ./load trysgprm ) >/dev/null \
	2>&1 \
	&& echo \#define HASSIGPROCMASK 1 || exit 0 ) > hassgprm.h
	rm -f trysgprm.o trysgprm

hasshsgr.h: \
chkshsgr warn-shsgr tryshsgr.c compile load
	./chkshsgr || ( cat warn-shsgr; exit 1 )
	( ( ./compile tryshsgr.c \
	&& ./load tryshsgr && ./tryshsgr ) >/dev/null 2>&1 \
	&& echo \#define HASSHORTSETGROUPS 1 || exit 0 ) > \
	hasshsgr.h
	rm -f tryshsgr.o tryshsgr

hasutmp.h: \
tryutmp.c compile
	( ./compile tryutmp.c >/dev/null 2>&1 \
	&& echo \#define HASUTMP 1 || exit 0 ) > hasutmp.h
	rm -f tryutmp.o

haswaitp.h: \
trywaitp.c compile load
	( ( ./compile trywaitp.c && ./load trywaitp ) >/dev/null \
	2>&1 \
	&& echo \#define HASWAITPID 1 || exit 0 ) > haswaitp.h
	rm -f trywaitp.o trywaitp

headerbody.o: \
compile headerbody.c stralloc.h gen_alloc.h substdio.h getln.h \
hfield.h headerbody.h
	./compile headerbody.c

hfield.o: \
compile hfield.c hfield.h
	./compile hfield.c

hier.o: \
compile hier.c auto_qmail.h auto_split.h auto_uids.h fmt.h fifo.h
	./compile hier.c

hostname: \
load hostname.o substdio.a error.a str.a dns.lib socket.lib
	./load hostname substdio.a error.a str.a `cat dns.lib` \
	`cat socket.lib`

hostname.o: \
compile hostname.c substdio.h subfd.h readwrite.h exit.h
	./compile hostname.c

idn2.lib: \
tryidn2.c compile load
	( (./compile tryidn2.c && \
	./load tryidn2 `head -1 conf.tmp/conf-idn2` -lidn2 ) >/dev/null 2>&1 \
	&& echo "`head -1 conf.tmp/conf-idn2` -lidn2" || exit 0 ) > idn2.lib
#	rm -f tryind2.o tryidn2

install: \
load install.o fifo.o hier.o auto_qmail.o auto_split.o auto_uids.o \
strerr.a substdio.a open.a error.a str.a fs.a
	./load install fifo.o hier.o auto_qmail.o auto_split.o \
	auto_uids.o strerr.a substdio.a open.a error.a str.a fs.a 

install.o: \
compile install.c substdio.h strerr.h error.h open.h readwrite.h \
exit.h
	./compile install.c

instcheck: \
load instcheck.o fifo.o hier.o auto_qmail.o auto_split.o auto_uids.o \
strerr.a substdio.a error.a str.a fs.a
	./load instcheck fifo.o hier.o auto_qmail.o auto_split.o \
	auto_uids.o strerr.a substdio.a error.a str.a fs.a 

instcheck.o: \
compile instcheck.c strerr.h error.h readwrite.h exit.h
	./compile instcheck.c

ip.o: \
compile ip.c
# fmt.h scan.h ip.h stralloc.h
	./compile ip.c

ipalloc.o: \
compile ipalloc.c alloc.h gen_allocdefs.h ip.h ipalloc.h \
gen_alloc.h
	./compile ipalloc.c

ipme.o: \
compile ipme.c hassalen.h
# ip.h ipme.h ipalloc.h \
#gen_alloc.h stralloc.h readwrite.h
	./compile ipme.c

ipmeprint: \
load ipmeprint.o ipme.o ip.o ipalloc.o auto_qmail.o open.a \
getln.a stralloc.a alloc.a substdio.a \
error.a str.a fs.a socket.lib
	./load ipmeprint ipme.o ip.o auto_qmail.o ipalloc.o \
	open.a getln.a stralloc.a alloc.a \
	substdio.a error.a str.a fs.a  `cat socket.lib`

ipmeprint.o: \
compile ipmeprint.c subfd.h substdio.h ip.h ipme.h \
ipalloc.h gen_alloc.h exit.h auto_qmail.h
	./compile ipmeprint.c

it-analog: \
columnt matchup \
ddist deferrals failures senders successes suids \
recipients rhosts rhosts rxdelay \
xqp xrecipient xsender \
zddist zdeferrals zfailures zrecipients zrhosts \
zrxdelay zsenders zsendmail zsuccesses zsuids zoverall

it-base: \
qmail-local qmail-rspawn qmail-lspawn qmail-send \
qmail-clean qmail-start qmail-queue qmail-inject qmail-todo

it-mbox: \
forward predate preline condredirect bouncesaying except \
datemail maildirmake maildir2mbox maildirwatch qreceipt
#datemail maildirmake maildir2mbox maildirwatch qbiff qreceipt

it-clients: \
mailsubj qmail-remote qmail-qmqpc qmail-smtpam sendmail

it-dns: \
dnscname dnsptr dnsip dnsmxip dnsfq dnstxt \
hostname ipmeprint spfquery

it-pop: \
qmail-popup qmail-pop3d

it-forward: \
fastforward forward printforward setforward newaliases \
printmaillist setmaillist newinclude

it-control: \
qmail-badmimetypes qmail-badloadertypes \
qmail-mfrules qmail-recipients qmail-showctl \
qmail-authuser qmail-vmailuser

it-log: \
splogger qmail-mrtg qmail-mrtg-queue tai64nfrac

it-queue: \
qmail-qread qmail-qstat qmail-tcpto qmail-tcpok
# qmail-upq

it-server: \
qmail-qmtpd qmail-qmqpd qmail-smtpd dnsip

it-setup: \
config config-fast install instcheck

it-user: \
qmail-getpw qmail-newu qmail-pw2u qmail-newmrh 

load:
#make-load warn-auto.sh systype
#	( cat warn-auto.sh; ./make-load "`cat systype`" ) > load
#	chmod 755 load

lock.a: \
makelib lock_ex.o lock_exnb.o lock_un.o
	./makelib lock.a lock_ex.o lock_exnb.o lock_un.o

lock_ex.o: \
compile lock_ex.c hasflock.h lock.h
	./compile lock_ex.c

lock_exnb.o: \
compile lock_exnb.c hasflock.h lock.h
	./compile lock_exnb.c

lock_un.o: \
compile lock_un.c hasflock.h lock.h
	./compile lock_un.c

maildir.o: \
compile maildir.c prioq.h datetime.h gen_alloc.h env.h stralloc.h \
datetime.h now.h datetime.h str.h maildir.h \
strerr.h
	./compile maildir.c

maildir2mbox: \
load maildir2mbox.o maildir.o prioq.o now.o myctime.o gfrom.o lock.a \
getln.a env.a open.a strerr.a stralloc.a alloc.a substdio.a error.a \
str.a fs.a datetime.a
	./load maildir2mbox maildir.o prioq.o now.o myctime.o \
	gfrom.o lock.a getln.a env.a open.a strerr.a stralloc.a \
	alloc.a substdio.a error.a str.a fs.a datetime.a 

maildir2mbox.o: \
compile maildir2mbox.c readwrite.h prioq.h datetime.h gen_alloc.h \
env.h stralloc.h subfd.h substdio.h getln.h \
error.h open.h lock.h gfrom.h str.h exit.h myctime.h maildir.h \
strerr.h
	./compile maildir2mbox.c

maildirmake: \
load maildirmake.o strerr.a substdio.a error.a str.a
	./load maildirmake strerr.a substdio.a error.a str.a 

maildirmake.o: \
compile maildirmake.c strerr.h exit.h
	./compile maildirmake.c

maildirwatch: \
load maildirwatch.o hfield.o headerbody.o maildir.o prioq.o now.o \
getln.a env.a open.a strerr.a stralloc.a alloc.a substdio.a error.a \
str.a
	./load maildirwatch hfield.o headerbody.o maildir.o \
	prioq.o now.o getln.a env.a open.a strerr.a stralloc.a \
	alloc.a substdio.a error.a str.a 

maildirwatch.o: \
compile maildirwatch.c getln.h substdio.h subfd.h prioq.h \
datetime.h gen_alloc.h stralloc.h str.h exit.h hfield.h \
readwrite.h open.h headerbody.h maildir.h strerr.h
	./compile maildirwatch.c

mailsubj: \
warn-auto.sh mailsubj.sh
# ../conf-home ../conf-break ../conf-split
	cat warn-auto.sh mailsubj.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	| sed s}BREAK}"`head -1 conf-break`"}g \
	| sed s}SPLIT}"`head -1 conf-split`"}g \
	> mailsubj
	chmod 755 mailsubj

#make-compile: \
#make-compile.sh auto-ccld.sh
#	cat auto-ccld.sh make-compile.sh > make-compile
#	chmod 755 make-compile

#make-load: \
#make-load.sh auto-ccld.sh
#	cat auto-ccld.sh make-load.sh > make-load
#	chmod 755 make-load

#make-makelib: \
#make-makelib.sh auto-ccld.sh
#	cat auto-ccld.sh make-makelib.sh > make-makelib
#	chmod 755 make-makelib

makelib:
#make-makelib warn-auto.sh systype
#	( cat warn-auto.sh; ./make-makelib "`cat systype`" ) > \
#	makelib
#	chmod 755 makelib

matchup: \
load matchup.o strerr.a getln.a substdio.a stralloc.a alloc.a error.a \
str.a fs.a case.a
	./load matchup strerr.a getln.a substdio.a stralloc.a \
	alloc.a error.a str.a fs.a case.a 

matchup.o: \
compile matchup.c stralloc.h gen_alloc.h gen_alloc.h gen_allocdefs.h \
strerr.h getln.h substdio.h subfd.h substdio.h readwrite.h exit.h \
str.h fmt.h scan.h case.h
	./compile matchup.c

myctime.o: \
compile myctime.c datetime.h fmt.h myctime.h
	./compile myctime.c

mfrules.o: \
compile mfrules.c
# stralloc.h gen_alloc.h open.h cdb.h uint32.h \
#case.h mfrules.h str.h
	./compile mfrules.c

ndelay.a: \
makelib ndelay.o ndelay_off.o
	./makelib ndelay.a ndelay.o ndelay_off.o

ndelay.o: \
compile ndelay.c ndelay.h
	./compile ndelay.c

ndelay_off.o: \
compile ndelay_off.c ndelay.h
	./compile ndelay_off.c

newaliases: \
load newaliases.o auto_qmail.o token822.o control.o cdbmss.o \
cdbmake.a strerr.a getln.a substdio.a stralloc.a alloc.a error.a \
str.a fs.a seek.a open.a case.a
	./load newaliases auto_qmail.o token822.o control.o \
	cdbmss.o cdbmake.a strerr.a getln.a substdio.a stralloc.a \
	alloc.a error.a str.a fs.a seek.a open.a case.a

newaliases.o: \
compile newaliases.c substdio.h strerr.h stralloc.h gen_alloc.h \
getln.h open.h readwrite.h token822.h gen_alloc.h control.h \
auto_qmail.h case.h cdbmss.h cdbmake.h uint32.h substdio.h
	./compile newaliases.c

newinclude: \
load newinclude.o auto_qmail.o token822.o control.o getln.a strerr.a \
stralloc.a env.a alloc.a substdio.a error.a str.a fs.a open.a wait.a \
fd.a
	./load newinclude auto_qmail.o token822.o control.o \
	getln.a strerr.a stralloc.a env.a alloc.a substdio.a \
	error.a str.a fs.a open.a wait.a fd.a

newinclude.o: \
compile newinclude.c substdio.h strerr.h stralloc.h gen_alloc.h \
getln.h open.h readwrite.h token822.h gen_alloc.h control.h \
auto_qmail.h env.h
	./compile newinclude.c

newfield.o: \
compile newfield.c fmt.h datetime.h stralloc.h gen_alloc.h \
date822fmt.h newfield.h stralloc.h
	./compile newfield.c

now.o: \
compile now.c datetime.h now.h datetime.h
	./compile now.c

open.a: \
makelib open_append.o open_excl.o open_read.o open_trunc.o \
open_write.o
	./makelib open.a open_append.o open_excl.o open_read.o \
	open_trunc.o open_write.o

open_append.o: \
compile open_append.c open.h
	./compile open_append.c

open_excl.o: \
compile open_excl.c open.h
	./compile open_excl.c

open_read.o: \
compile open_read.c open.h
	./compile open_read.c

open_trunc.o: \
compile open_trunc.c open.h
	./compile open_trunc.c

open_write.o: \
compile open_write.c open.h
	./compile open_write.c

pathexec_env.o: \
compile pathexec_env.c
# stralloc.h gen_alloc.h alloc.h str.h \
#env.h pathexec.h
	./compile pathexec_env.c

pathexec_run.o: \
compile pathexec_run.c error.h stralloc.h gen_alloc.h str.h env.h \
pathexec.h
	./compile pathexec_run.c

predate: \
load predate.o datetime.a strerr.a sig.a fd.a wait.a substdio.a \
error.a str.a fs.a
	./load predate datetime.a strerr.a sig.a fd.a wait.a \
	substdio.a error.a str.a fs.a 

predate.o: \
compile predate.c datetime.h wait.h fd.h fmt.h strerr.h \
substdio.h subfd.h readwrite.h exit.h
	./compile predate.c

preline: \
load preline.o strerr.a fd.a wait.a sig.a env.a getopt.a substdio.a \
error.a str.a
	./load preline strerr.a fd.a wait.a sig.a env.a getopt.a \
	substdio.a error.a str.a 

preline.o: \
compile preline.c fd.h sgetopt.h subgetopt.h readwrite.h strerr.h \
substdio.h exit.h wait.h env.h sig.h error.h
	./compile preline.c

printforward: \
load printforward.o cdb.a strerr.a substdio.a stralloc.a alloc.a \
error.a str.a
	./load printforward cdb.a strerr.a substdio.a stralloc.a \
	alloc.a error.a str.a

printforward.o: \
compile printforward.c substdio.h subfd.h substdio.h strerr.h \
stralloc.h gen_alloc.h cdb.h uint32.h
	./compile printforward.c

printmaillist: \
load printmaillist.o getln.a strerr.a substdio.a stralloc.a alloc.a \
error.a str.a
	./load printmaillist getln.a strerr.a substdio.a \
	stralloc.a alloc.a error.a str.a
        
printmaillist.o: \
compile printmaillist.c substdio.h subfd.h substdio.h strerr.h \
stralloc.h gen_alloc.h getln.h
	./compile printmaillist.c

prioq.o: \
compile prioq.c alloc.h gen_allocdefs.h prioq.h datetime.h \
gen_alloc.h
	./compile prioq.c

prot.o: \
compile prot.c hasshsgr.h prot.h
	./compile prot.c

qmail-authuser: \
load qmail-authuser.o auto_qmail.o alloc.a case.a control.o constmap.o \
env.a error.a fd.a fs.a getln.a hmac_md5.o md5c.o substdio.a open.a seek.a \
stralloc.a str.a pathexec_env.o pathexec_run.o prot.o shadow.lib crypt.lib \
s.lib sha1.o sha256.o sig.a wait.a
	./load qmail-authuser auto_qmail.o control.o constmap.o \
	pathexec_env.o pathexec_run.o prot.o \
	hmac_md5.o md5c.o sha1.o sha256.o \
	stralloc.a alloc.a case.a error.a env.a fd.a fs.a getln.a \
	sig.a substdio.a open.a seek.a str.a substdio.a stralloc.a \
	wait.a `cat shadow.lib` `cat crypt.lib` `cat s.lib`

qmail-authuser.o: \
compile qmail-authuser.c auto_qmail.h case.h constmap.h control.h \
error.h exit.h fd.h fmt.h gen_alloc.h getln.h global.h hmac_md5.h md5.h \
open.h pathexec.h prot.h readwrite.h scan.h stralloc.h str.h substdio.h \
hasspnam.h hasuserpw.h stralloc.h sha1.h sha256.h sig.h wait.h
	./compile qmail-authuser.c 

qmail-clean: \
load qmail-clean.o fmtqfn.o now.o getln.a sig.a stralloc.a alloc.a \
substdio.a error.a str.a fs.a auto_qmail.o auto_split.o
	./load qmail-clean fmtqfn.o now.o getln.a sig.a stralloc.a \
	alloc.a substdio.a error.a str.a fs.a auto_qmail.o \
	auto_split.o 

qmail-clean.o: \
compile qmail-clean.c readwrite.h sig.h now.h datetime.h str.h \
getln.h stralloc.h gen_alloc.h substdio.h subfd.h \
scan.h fmt.h error.h exit.h fmtqfn.h auto_qmail.h
	./compile qmail-clean.c

qmail-getpw: \
load qmail-getpw.o case.a substdio.a error.a str.a fs.a auto_break.o \
auto_usera.o
	./load qmail-getpw case.a substdio.a error.a str.a fs.a \
	auto_break.o auto_usera.o 

qmail-getpw.o: \
compile qmail-getpw.c readwrite.h substdio.h subfd.h  \
error.h exit.h str.h case.h fmt.h auto_usera.h auto_break.h \
qlx.h
	./compile qmail-getpw.c

qmail-inject: \
load qmail-inject.o headerbody.o hfield.o newfield.o quote.o now.o \
control.o date822fmt.o constmap.o qmail.o case.a fd.a wait.a open.a \
getln.a sig.a getopt.a datetime.a token822.o env.a stralloc.a alloc.a \
substdio.a error.a str.a fs.a auto_qmail.o
	./load qmail-inject headerbody.o hfield.o newfield.o \
	quote.o now.o control.o date822fmt.o constmap.o qmail.o \
	case.a fd.a wait.a open.a getln.a sig.a getopt.a datetime.a \
	token822.o env.a stralloc.a alloc.a substdio.a error.a \
	str.a fs.a auto_qmail.o 

qmail-inject.o: \
compile qmail-inject.c sig.h substdio.h stralloc.h gen_alloc.h \
subfd.h sgetopt.h subgetopt.h getln.h alloc.h str.h fmt.h \
hfield.h token822.h control.h env.h  \
gen_allocdefs.h error.h qmail.h substdio.h now.h datetime.h exit.h \
quote.h headerbody.h auto_qmail.h newfield.h stralloc.h constmap.h
	./compile qmail-inject.c

qmail-local: \
load qmail-local.o qmail.o quote.o now.o gfrom.o myctime.o \
slurpclose.o case.a getln.a getopt.a sig.a open.a seek.a lock.a fd.a \
wait.a env.a stralloc.a alloc.a strerr.a substdio.a error.a str.a \
fs.a datetime.a auto_qmail.o auto_patrn.o socket.lib
	./load qmail-local qmail.o quote.o now.o gfrom.o myctime.o \
	slurpclose.o case.a getln.a getopt.a sig.a open.a seek.a \
	lock.a fd.a wait.a env.a stralloc.a alloc.a strerr.a \
	substdio.a error.a str.a fs.a datetime.a auto_qmail.o \
	auto_patrn.o `cat socket.lib`

qmail-local.o: \
open.h wait.h lock.h seek.h substdio.h getln.h strerr.h subfd.h \
sgetopt.h subgetopt.h alloc.h error.h stralloc.h \
gen_alloc.h fmt.h str.h now.h datetime.h case.h quote.h qmail.h \
slurpclose.h myctime.h gfrom.h auto_patrn.h
	./compile qmail-local.c

qmail-lspawn: \
load qmail-lspawn.o spawn.o prot.o slurpclose.o coe.o sig.a wait.a \
case.a cdb.a fd.a open.a stralloc.a alloc.a substdio.a error.a str.a \
fs.a auto_qmail.o auto_uids.o auto_spawn.o
	./load qmail-lspawn spawn.o prot.o slurpclose.o coe.o \
	sig.a wait.a case.a cdb.a fd.a open.a stralloc.a alloc.a \
	substdio.a error.a str.a fs.a auto_qmail.o auto_uids.o \
	auto_spawn.o 

qmail-lspawn.o: \
compile qmail-lspawn.c fd.h wait.h prot.h substdio.h stralloc.h \
gen_alloc.h scan.h exit.h error.h cdb.h uint32.h case.h \
slurpclose.h auto_qmail.h auto_uids.h qlx.h
	./compile qmail-lspawn.c

qmail-badmimetypes: \
load qmail-badmimetypes.o cdbmss.o getln.a open.a cdbmake.a seek.a case.a \
stralloc.a alloc.a strerr.a substdio.a error.a str.a auto_qmail.o
	./load qmail-badmimetypes cdbmss.o getln.a open.a cdbmake.a \
	seek.a case.a stralloc.a alloc.a strerr.a substdio.a \
	error.a str.a auto_qmail.o

qmail-badmimetypes.o: \
compile qmail-badmimetypes.c strerr.h stralloc.h gen_alloc.h substdio.h \
getln.h exit.h readwrite.h open.h auto_qmail.h cdbmss.h cdbmake.h \
uint32.h
	./compile qmail-badmimetypes.c

qmail-badloadertypes: \
load qmail-badloadertypes.o cdbmss.o getln.a open.a cdbmake.a seek.a case.a \
stralloc.a alloc.a strerr.a substdio.a error.a str.a auto_qmail.o
	./load qmail-badloadertypes cdbmss.o getln.a open.a cdbmake.a \
	seek.a case.a stralloc.a alloc.a strerr.a substdio.a \
	error.a str.a auto_qmail.o

qmail-badloadertypes.8: \
qmail-badloadertypes.9 
#../conf-break ../conf-spawn
	cat qmail-badloadertypes.9 \
	| sed s}HOME}"`head -1 conf-home`"}g \
	| sed s}BREAK}"`head -1 conf-break`"}g \
	| sed s}SPAWN}"`head -1 conf-spawn`"}g \
	> qmail-badloadertypes.8

qmail-badloadertypes.o: \
compile qmail-badloadertypes.c strerr.h stralloc.h gen_alloc.h substdio.h \
getln.h exit.h readwrite.h open.h auto_qmail.h cdbmss.h cdbmake.h \
uint32.h 
	./compile qmail-badloadertypes.c

qmail-newmrh: \
load qmail-newmrh.o cdbmss.o getln.a open.a cdbmake.a seek.a case.a \
stralloc.a alloc.a strerr.a substdio.a error.a str.a auto_qmail.o
	./load qmail-newmrh cdbmss.o getln.a open.a cdbmake.a \
	seek.a case.a stralloc.a alloc.a strerr.a substdio.a \
	error.a str.a auto_qmail.o 

qmail-newmrh.o: \
compile qmail-newmrh.c strerr.h stralloc.h gen_alloc.h substdio.h \
getln.h exit.h readwrite.h open.h auto_qmail.h cdbmss.h cdbmake.h \
uint32.h case.h
	./compile qmail-newmrh.c

qmail-recipients: \
load qmail-recipients.o cdbmss.o getln.a open.a cdbmake.a seek.a case.a \
stralloc.a alloc.a strerr.a substdio.a error.a str.a auto_qmail.o
	./load qmail-recipients cdbmss.o getln.a open.a cdbmake.a \
	seek.a case.a stralloc.a alloc.a strerr.a substdio.a \
	error.a str.a auto_qmail.o

qmail-recipients.o: \
compile qmail-recipients.c strerr.h stralloc.h gen_alloc.h substdio.h \
getln.h exit.h readwrite.h open.h auto_qmail.h cdbmss.h cdbmake.h \
uint32.h 
	./compile qmail-recipients.c

qmail-vmailuser: \
load qmail-vmailuser.o auto_qmail.o alloc.a case.a control.o constmap.o \
env.a error.a fd.a fs.a  getln.a substdio.a open.a seek.a \
stralloc.a str.a 
	./load qmail-vmailuser auto_qmail.o control.o constmap.o \
	getln.a stralloc.a alloc.a case.a error.a env.a fd.a fs.a  \
	substdio.a open.a seek.a str.a substdio.a stralloc.a 

qmail-vmailuser.o: \
compile qmail-vmailuser.c auto_qmail.h case.h constmap.h control.h \
error.h exit.h fd.h fmt.h gen_alloc.h global.h getln.h \
open.h readwrite.h scan.h stralloc.h str.h substdio.h stralloc.h 
	./compile qmail-vmailuser.c

qmail-smtpam: \
load qmail-smtpam.o control.o constmap.o strsalloc.o timeoutread.o timeoutwrite.o \
timeoutconn.o tcpto.o now.o dns.o ip.o ipalloc.o ipme.o quote.o socket6_if.o \
ndelay.a sig.a open.a lock.a seek.a getln.a stralloc.a alloc.a case.a error.a \
substdio.a error.a str.a fs.a auto_qmail.o dns.lib socket.lib ssl.lib idn2.lib \
tls_timeoutio.o tls_errors.o tls_remote.o
	./load qmail-smtpam control.o constmap.o strsalloc.o timeoutread.o \
	timeoutwrite.o timeoutconn.o tcpto.o now.o dns.o ip.o ipalloc.o \
	ipme.o quote.o socket6_if.o ndelay.a sig.a open.a lock.a seek.a \
	getln.a stralloc.a alloc.a substdio.a error.a str.a fs.a auto_qmail.o \
	tls_errors.o tls_remote.o tls_timeoutio.o case.a ucspissl.a \
	`cat ssl.lib` `cat dns.lib` `cat socket.lib`

qmail-smtpam.o: \
compile qmail-smtpam.c sig.h stralloc.h gen_alloc.h substdio.h \
subfd.h scan.h case.h error.h auto_qmail.h control.h dns.h \
alloc.h quote.h ip.h ipalloc.h gen_alloc.h ipme.h \
gen_alloc.h gen_allocdefs.h str.h now.h datetime.h exit.h constmap.h \
tcpto.h readwrite.h timeoutconn.h timeoutread.h timeoutwrite.h \
tls_start.h tls_remote.h tls_errors.h
	./compile qmail-smtpam.c

qmail-mfrules: \
load qmail-mfrules.o case.a cdbmss.o getln.a open.a cdbmake.a seek.a  \
fs.a stralloc.a alloc.a strerr.a substdio.a error.a str.a auto_qmail.o
	./load qmail-mfrules case.a cdbmss.o getln.a open.a cdbmake.a \
	seek.a fs.a stralloc.a alloc.a strerr.a substdio.a \
	error.a str.a auto_qmail.o

qmail-mfrules.o: \
compile qmail-mfrules.c auto_qmail.h case.h strerr.h stralloc.h \
gen_alloc.h readwrite.h open.h auto_qmail.h cdbmss.h cdbmake.h \
getln.h substdio.h scan.h fmt.h error.h 
	./compile qmail-mfrules.c

qmail-mrtg: \
load qmail-mrtg.o getln.a open.a now.o \
fs.a stralloc.a alloc.a strerr.a substdio.a error.a str.a case.a
	./load qmail-mrtg open.a getln.a now.o \
	fs.a stralloc.a alloc.a strerr.a substdio.a \
	error.a str.a case.a

qmail-mrtg.o: \
compile qmail-mfrules.c strerr.h stralloc.h gen_alloc.h case.h \
readwrite.h open.h getln.h scan.h open.h fmt.h now.h datetime.h \
substdio.h scan.h fmt.h error.h str.h
	./compile qmail-mrtg.c

qmail-mrtg-queue: \
warn-auto.sh qmail-mrtg-queue.sh
	cat warn-auto.sh qmail-mrtg-queue.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> qmail-mrtg-queue
	chmod 755 qmail-mrtg-queue

qmail-newu: \
load qmail-newu.o cdbmss.o getln.a open.a seek.a cdbmake.a case.a \
stralloc.a alloc.a substdio.a error.a str.a auto_qmail.o
	./load qmail-newu cdbmss.o getln.a open.a seek.a cdbmake.a \
	case.a stralloc.a alloc.a substdio.a error.a str.a \
	auto_qmail.o 

qmail-newu.o: \
compile qmail-newu.c stralloc.h gen_alloc.h subfd.h substdio.h \
getln.h cdbmss.h cdbmake.h uint32.h exit.h \
readwrite.h open.h error.h case.h auto_qmail.h
	./compile qmail-newu.c

qmail-pop3d: \
load qmail-pop3d.o commands.o case.a timeoutread.o timeoutwrite.o \
maildir.o prioq.o now.o env.a strerr.a sig.a open.a getln.a \
stralloc.a alloc.a substdio.a error.a str.a fs.a socket.lib
	./load qmail-pop3d commands.o case.a timeoutread.o \
	timeoutwrite.o maildir.o prioq.o now.o env.a strerr.a sig.a \
	open.a getln.a stralloc.a alloc.a substdio.a error.a str.a \
	fs.a  `cat socket.lib`

qmail-pop3d.o: \
compile qmail-pop3d.c commands.h sig.h getln.h stralloc.h gen_alloc.h \
substdio.h alloc.h open.h prioq.h datetime.h gen_alloc.h scan.h fmt.h \
str.h exit.h maildir.h strerr.h readwrite.h timeoutread.h \
timeoutwrite.h
	./compile qmail-pop3d.c

qmail-popup: \
load qmail-popup.o commands.o timeoutread.o timeoutwrite.o now.o \
envread.o tls_start.o env.a \
case.a fd.a sig.a wait.a stralloc.a alloc.a substdio.a error.a str.a \
fs.a socket.lib
	./load qmail-popup commands.o timeoutread.o timeoutwrite.o \
	envread.o tls_start.o env.a \
	now.o case.a fd.a sig.a wait.a stralloc.a alloc.a \
	substdio.a error.a str.a fs.a  `cat socket.lib`

qmail-popup.o: \
compile qmail-popup.c commands.h fd.h sig.h stralloc.h gen_alloc.h \
substdio.h alloc.h wait.h str.h now.h datetime.h fmt.h exit.h \
readwrite.h timeoutread.h timeoutwrite.h
	./compile qmail-popup.c

qmail-pw2u: \
load qmail-pw2u.o constmap.o control.o open.a getln.a case.a getopt.a \
stralloc.a alloc.a substdio.a error.a str.a fs.a auto_usera.o \
auto_break.o auto_qmail.o
	./load qmail-pw2u constmap.o control.o open.a getln.a \
	case.a getopt.a stralloc.a alloc.a substdio.a error.a str.a \
	fs.a auto_usera.o auto_break.o auto_qmail.o 

qmail-pw2u.o: \
compile qmail-pw2u.c substdio.h readwrite.h subfd.h \
sgetopt.h subgetopt.h control.h constmap.h stralloc.h gen_alloc.h \
fmt.h str.h scan.h open.h error.h getln.h auto_break.h auto_qmail.h \
auto_usera.h
	./compile qmail-pw2u.c

qmail-qmqpc: \
load qmail-qmqpc.o slurpclose.o socket6_if.o timeoutread.o timeoutwrite.o \
timeoutconn.o constmap.o case.a ip.o control.o auto_qmail.o sig.a ndelay.a open.a \
getln.a substdio.a stralloc.a alloc.a error.a str.a fs.a socket.lib
	./load qmail-qmqpc slurpclose.o socket6_if.o timeoutread.o \
	timeoutwrite.o timeoutconn.o constmap.o case.a ip.o control.o auto_qmail.o \
	sig.a ndelay.a open.a getln.a substdio.a stralloc.a alloc.a \
	error.a str.a fs.a  `cat socket.lib`

qmail-qmqpc.o: \
compile qmail-qmqpc.c substdio.h getln.h readwrite.h exit.h str.h \
stralloc.h gen_alloc.h slurpclose.h error.h sig.h ip.h timeoutconn.h \
timeoutread.h timeoutwrite.h auto_qmail.h control.h fmt.h
	./compile qmail-qmqpc.c

qmail-qmqpd: \
load qmail-qmqpd.o received.o now.o date822fmt.o qmail.o auto_qmail.o \
env.a substdio.a sig.a error.a wait.a fd.a str.a datetime.a fs.a
	./load qmail-qmqpd received.o now.o date822fmt.o qmail.o \
	auto_qmail.o env.a substdio.a sig.a error.a wait.a fd.a \
	str.a datetime.a fs.a case.a

qmail-qmqpd.o: \
compile qmail-qmqpd.c auto_qmail.h qmail.h substdio.h received.h \
sig.h readwrite.h exit.h now.h datetime.h fmt.h env.h case.h
	./compile qmail-qmqpd.c

qmail-qmtpd: \
load qmail-qmtpd.o rcpthosts.o control.o constmap.o received.o \
date822fmt.o now.o qmail.o cdb.a fd.a wait.a datetime.a open.a \
getln.a sig.a case.a env.a stralloc.a alloc.a substdio.a error.a \
str.a fs.a auto_qmail.o
	./load qmail-qmtpd rcpthosts.o control.o constmap.o \
	received.o date822fmt.o now.o qmail.o cdb.a fd.a wait.a \
	datetime.a open.a getln.a sig.a case.a env.a stralloc.a \
	alloc.a substdio.a error.a str.a fs.a auto_qmail.o 

qmail-qmtpd.o: \
compile qmail-qmtpd.c stralloc.h gen_alloc.h substdio.h qmail.h \
now.h datetime.h str.h fmt.h env.h sig.h rcpthosts.h \
auto_qmail.h readwrite.h control.h received.h 
	./compile qmail-qmtpd.c

qmail-qread: \
load qmail-qread.o fmtqfn.o readsubdir.o date822fmt.o datetime.a \
open.a getln.a stralloc.a alloc.a substdio.a error.a str.a fs.a \
auto_qmail.o auto_split.o
	./load qmail-qread fmtqfn.o readsubdir.o date822fmt.o \
	datetime.a open.a getln.a stralloc.a alloc.a substdio.a \
	error.a str.a fs.a auto_qmail.o auto_split.o 

qmail-qread.o: \
compile qmail-qread.c stralloc.h gen_alloc.h substdio.h subfd.h \
fmt.h str.h getln.h fmtqfn.h readsubdir.h \
auto_qmail.h open.h datetime.h date822fmt.h readwrite.h error.h \
exit.h
	./compile qmail-qread.c

qmail-qstat: \
warn-auto.sh qmail-qstat.sh
# ../conf-home ../conf-break ../conf-split
	cat warn-auto.sh qmail-qstat.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	| sed s}BREAK}"`head -1 conf-break`"}g \
	| sed s}SPLIT}"`head -1 conf-split`"}g \
	> qmail-qstat
	chmod 755 qmail-qstat

qmail-queue: \
load qmail-queue.o triggerpull.o fmtqfn.o now.o date822fmt.o \
datetime.a seek.a ndelay.a open.a sig.a alloc.a substdio.a error.a \
env.a wait.a \
str.a fs.a auto_qmail.o auto_split.o auto_uids.o
	./load qmail-queue triggerpull.o fmtqfn.o now.o \
	date822fmt.o datetime.a seek.a ndelay.a open.a sig.a \
	env.a wait.a alloc.a substdio.a error.a str.a fs.a \
	auto_qmail.o auto_split.o auto_uids.o 

qmail-queue.o: \
compile qmail-queue.c readwrite.h sig.h exit.h open.h seek.h fmt.h \
env.h wait.h \
alloc.h substdio.h datetime.h now.h datetime.h triggerpull.h extra.h \
auto_qmail.h auto_uids.h date822fmt.h fmtqfn.h
	./compile qmail-queue.c

qmail-remote: \
load qmail-remote.o control.o constmap.o timeoutread.o timeoutwrite.o \
timeoutconn.o tcpto.o now.o dns.o ip.o ipalloc.o ipme.o quote.o strsalloc.o \
ndelay.a sig.a open.a lock.a seek.a getln.a stralloc.a alloc.a case.a \
tls_timeoutio.o tls_errors.o tls_remote.o base64.o md5c.o hmac_md5.o \
substdio.a error.a str.a fs.a auto_qmail.o socket6_if.o dns.lib socket.lib idn2.lib
	./load qmail-remote control.o constmap.o timeoutread.o \
	timeoutwrite.o timeoutconn.o tcpto.o now.o case.a ip.o \
	tls_errors.o tls_remote.o tls_timeoutio.o \
	base64.o md5c.o hmac_md5.o socket6_if.o strsalloc.o \
	ipalloc.o ipme.o quote.o dns.o ndelay.a sig.a open.a \
	lock.a seek.a getln.a stralloc.a alloc.a substdio.a \
	str.a fs.a ucspissl.a case.a error.a auto_qmail.o \
	`cat ssl.lib` `cat dns.lib` `cat socket.lib` `cat idn2.lib`

qmail-remote.o: \
compile qmail-remote.c sig.h stralloc.h gen_alloc.h substdio.h \
subfd.h scan.h case.h error.h auto_qmail.h control.h dns.h \
alloc.h quote.h ip.h ipalloc.h gen_alloc.h ipme.h \
gen_alloc.h gen_allocdefs.h str.h now.h datetime.h exit.h constmap.h \
tcpto.h readwrite.h timeoutconn.h timeoutread.h timeoutwrite.h \
tls_start.h tls_remote.h tls_errors.h
	./compile qmail-remote.c

qmail-rspawn: \
load qmail-rspawn.o spawn.o tcpto_clean.o now.o coe.o sig.a open.a \
seek.a lock.a wait.a fd.a stralloc.a alloc.a substdio.a error.a str.a \
auto_qmail.o auto_uids.o auto_spawn.o
	./load qmail-rspawn spawn.o tcpto_clean.o now.o coe.o \
	sig.a open.a seek.a lock.a wait.a fd.a stralloc.a alloc.a \
	substdio.a error.a str.a auto_qmail.o auto_uids.o \
	auto_spawn.o 

qmail-rspawn.o: \
compile qmail-rspawn.c fd.h wait.h substdio.h exit.h error.h \
tcpto.h uint32.h
	./compile qmail-rspawn.c

qmail-send: \
load qmail-send.o qsutil.o control.o constmap.o newfield.o prioq.o \
trigger.o fmtqfn.o quote.o now.o readsubdir.o qmail.o date822fmt.o \
datetime.a case.a ndelay.a getln.a wait.a seek.a fd.a sig.a open.a \
lock.a stralloc.a alloc.a substdio.a error.a str.a fs.a auto_qmail.o \
auto_split.o env.a
	./load qmail-send qsutil.o control.o constmap.o newfield.o \
	prioq.o trigger.o fmtqfn.o quote.o now.o readsubdir.o \
	qmail.o date822fmt.o datetime.a case.a ndelay.a getln.a \
	wait.a seek.a fd.a sig.a open.a lock.a stralloc.a alloc.a \
	substdio.a error.a str.a fs.a auto_qmail.o auto_split.o env.a

qmail-send.o: \
compile qmail-send.c readwrite.h sig.h control.h select.h \
open.h seek.h exit.h lock.h ndelay.h now.h datetime.h getln.h \
substdio.h alloc.h error.h stralloc.h gen_alloc.h str.h fmt.h \
scan.h case.h auto_qmail.h trigger.h newfield.h quote.h \
qmail.h qsutil.h prioq.h datetime.h gen_alloc.h constmap.h \
fmtqfn.h readsubdir.h
	./compile qmail-send.c

qmail-showctl: \
load qmail-showctl.o auto_uids.o control.o open.a getln.a stralloc.a \
alloc.a substdio.a error.a str.a fs.a auto_qmail.o auto_break.o \
auto_patrn.o auto_spawn.o auto_split.o
	./load qmail-showctl auto_uids.o control.o open.a getln.a \
	stralloc.a alloc.a substdio.a error.a str.a fs.a \
	auto_qmail.o auto_break.o auto_patrn.o auto_spawn.o \
	auto_split.o 

qmail-showctl.o: \
compile qmail-showctl.c substdio.h subfd.h exit.h fmt.h \
str.h control.h constmap.h stralloc.h gen_alloc.h \
auto_uids.h auto_qmail.h auto_break.h auto_patrn.h auto_spawn.h \
auto_split.h
	./compile qmail-showctl.c

qmail-smtpd: \
load qmail-smtpd.o auto_break.o rcpthosts.o commands.o timeoutread.o \
timeoutwrite.o ip.o ipme.o ipalloc.o control.o constmap.o received.o \
recipients.o mfrules.o uint32_unpack.o tls_start.o smtpdlog.o dns.o \
date822fmt.o now.o qmail.o wildmat.o spf.o spfdnsip.o strsalloc.o \
cdb.a fd.a wait.a datetime.a getln.a open.a sig.a case.a env.a stralloc.a \
alloc.a substdio.a error.a str.a seek.a fs.a auto_qmail.o base64.o socket.lib
	./load qmail-smtpd rcpthosts.o recipients.o commands.o timeoutread.o \
	mfrules.o uint32_unpack.o tls_start.o auto_break.o smtpdlog.o \
	timeoutwrite.o ip.o ipme.o ipalloc.o control.o constmap.o dns.o \
	spf.o spfdnsip.o received.o date822fmt.o now.o qmail.o wildmat.o \
	base64.o strsalloc.o cdb.a seek.a fd.a wait.a datetime.a getln.a \
	open.a sig.a case.a  env.a stralloc.a alloc.a substdio.a str.a \
	fs.a error.a auto_qmail.o `cat dns.lib` `cat socket.lib`

qmail-smtpd.o: \
compile qmail-smtpd.c sig.h readwrite.h stralloc.h gen_alloc.h \
substdio.h alloc.h auto_qmail.h control.h received.h constmap.h \
error.h ipme.h ip.h ipalloc.h gen_alloc.h qmail.h spf.h \
str.h fmt.h scan.h case.h env.h now.h datetime.h \
exit.h rcpthosts.h timeoutread.h timeoutwrite.h commands.h wait.h \
fd.h base64.h recipients.h mfrules.h smtpdlog.h
	./compile qmail-smtpd.c

qmail-start: \
load qmail-start.o prot.o fd.a auto_uids.o
	./load qmail-start prot.o fd.a auto_uids.o 

qmail-start.o: \
compile qmail-start.c fd.h prot.h exit.h auto_uids.h
	./compile qmail-start.c

qmail-tcpok: \
load qmail-tcpok.o open.a lock.a strerr.a substdio.a error.a str.a \
auto_qmail.o
	./load qmail-tcpok open.a lock.a strerr.a substdio.a \
	error.a str.a auto_qmail.o 

qmail-tcpok.o: \
compile qmail-tcpok.c strerr.h substdio.h lock.h open.h readwrite.h \
auto_qmail.h exit.h
	./compile qmail-tcpok.c

qmail-tcpto: \
load qmail-tcpto.o ip.o now.o open.a lock.a substdio.a error.a str.a \
stralloc.a alloc.a fs.a auto_qmail.o
	./load qmail-tcpto ip.o now.o open.a lock.a substdio.a \
	error.a str.a stralloc.a alloc.a fs.a auto_qmail.o 

qmail-tcpto.o: \
compile qmail-tcpto.c substdio.h subfd.h auto_qmail.h stralloc.h \
fmt.h ip.h lock.h error.h exit.h datetime.h now.h datetime.h
	./compile qmail-tcpto.c

qmail-todo: \
load qmail-todo.o control.o constmap.o trigger.o fmtqfn.o now.o qsutil.o \
readsubdir.o case.a ndelay.a getln.a sig.a open.a stralloc.a alloc.a \
substdio.a error.a str.a fs.a auto_qmail.o auto_split.o
	./load qmail-todo control.o constmap.o trigger.o fmtqfn.o now.o \
	readsubdir.o case.a ndelay.a getln.a sig.a open.a stralloc.a qsutil.o \
	alloc.a substdio.a error.a str.a fs.a auto_qmail.o auto_split.o

qmail-todo.o: \
compile alloc.h auto_qmail.h constmap.h control.h error.h \
exit.h fmt.h fmtqfn.h getln.h open.h ndelay.h now.h readsubdir.h readwrite.h \
scan.h select.h str.h stralloc.h substdio.h trigger.h qsutil.h
	./compile qmail-todo.c

qmail-upq: warn-auto.sh qmail-upq.sh
#conf-home ../conf-break ../conf-split
	cat warn-auto.sh qmail-upq.sh \
	| sed s}QMAIL}"`head -1 conf-home`"}g \
	| sed s}BREAK}"`head -1 conf-break`"}g \
	| sed s}SPLIT}"`head -1 conf-split`"}g \
	> qmail-upq
	chmod 755 qmail-upq

qmail.o: \
compile qmail.c substdio.h readwrite.h wait.h exit.h fd.h \
qmail.h auto_qmail.h
	./compile qmail.c

qreceipt: \
load qreceipt.o headerbody.o hfield.o quote.o token822.o qmail.o \
getln.a fd.a wait.a sig.a env.a stralloc.a alloc.a substdio.a error.a \
str.a auto_qmail.o
	./load qreceipt headerbody.o hfield.o quote.o token822.o \
	qmail.o getln.a fd.a wait.a sig.a env.a stralloc.a alloc.a \
	substdio.a error.a str.a auto_qmail.o 

qreceipt.o: \
compile qreceipt.c sig.h env.h substdio.h stralloc.h gen_alloc.h \
subfd.h getln.h alloc.h str.h hfield.h token822.h \
gen_alloc.h error.h gen_alloc.h gen_allocdefs.h headerbody.h exit.h \
open.h quote.h qmail.h 
	./compile qreceipt.c

qsutil.o: \
compile qsutil.c stralloc.h gen_alloc.h readwrite.h substdio.h \
qsutil.h
	./compile qsutil.c

quote.o: \
compile quote.c stralloc.h gen_alloc.h str.h quote.h
	./compile quote.c

rcpthosts.o: \
compile rcpthosts.c cdb.h uint32.h open.h error.h control.h \
constmap.h stralloc.h gen_alloc.h rcpthosts.h
	./compile rcpthosts.c

recipients.o: \
compile recipients.c cdb.h uint32.h open.h error.h control.h str.h \
case.h constmap.h stralloc.h gen_alloc.h recipients.h substdio.h auto_break.h
	./compile recipients.c

recipients: \
warn-auto.sh recipients.sh
	cat warn-auto.sh recipients.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> recipients
	chmod 755 recipients

rhosts: \
warn-auto.sh rhosts.sh
	cat warn-auto.sh rhosts.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> rhosts
	chmod 755 rhosts

rxdelay: \
warn-auto.sh rxdelay.sh
	cat warn-auto.sh rxdelay.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> rxdelay
	chmod 755 rxdelay

senders: \
warn-auto.sh senders.sh
	cat warn-auto.sh senders.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> senders
	chmod 755 senders

smtpdlog.o: \
compile smtpdlog.c stralloc.h gen_alloc.h open.h uint32.h env.h fmt.h \
substdio.h smtpdlog.h str.h
	./compile smtpdlog.c 

s.lib: \
tryslib.c compile load
	( ( ./compile tryslib.c && \
        ./load tryslib -ls ) >/dev/null 2>&1 \
	&& echo -ls || exit 0 ) > s.lib
	rm -f tryslib.o tryslib

shadow.lib: \
tryshadow.c compile load
	( ( ./compile tryshadow.c && \
	./load tryshadow -lshadow ) >/dev/null 2>&1 \
	&& echo -lshadow || exit 0 ) > shadow.lib
	rm -f tryshadow.o tryshadow

successes: \
warn-auto.sh successes.sh
	cat warn-auto.sh successes.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> successes
	chmod 755 successes

suids: \
warn-auto.sh suids.sh
	cat warn-auto.sh suids.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> suids
	chmod 755 suids

readsubdir.o: \
compile readsubdir.c readsubdir.h fmt.h scan.h str.h \
auto_split.h
	./compile readsubdir.c

received.o: \
compile received.c fmt.h qmail.h substdio.h now.h datetime.h \
datetime.h date822fmt.h received.h str.h stralloc.h str.h
	./compile received.c

scan_8long.o: \
compile scan_8long.c scan.h
	./compile scan_8long.c

scan_ulong.o: \
compile scan_ulong.c scan.h
	./compile scan_ulong.c

scan_xlong.o: \
compile scan_xlong.c scan.h
	./compile scan_xlong.c

seek.a: \
makelib seek_cur.o seek_end.o seek_set.o seek_trunc.o
	./makelib seek.a seek_cur.o seek_end.o seek_set.o \
	seek_trunc.o

seek_cur.o: \
compile seek_cur.c seek.h
	./compile seek_cur.c

seek_end.o: \
compile seek_end.c seek.h
	./compile seek_end.c

seek_set.o: \
compile seek_set.c seek.h
	./compile seek_set.c

seek_trunc.o: \
compile seek_trunc.c seek.h
	./compile seek_trunc.c

select.h: \
compile trysysel.c select.h1 select.h2
	( ./compile trysysel.c >/dev/null 2>&1 \
	&& cat select.h2 || cat select.h1 ) > select.h
	rm -f trysysel.o trysysel

sendmail: \
load sendmail.o env.a getopt.a alloc.a substdio.a error.a str.a \
auto_qmail.o
	./load sendmail env.a getopt.a alloc.a substdio.a error.a \
	str.a auto_qmail.o 

sendmail.o: \
compile sendmail.c sgetopt.h subgetopt.h substdio.h subfd.h \
alloc.h auto_qmail.h exit.h env.h str.h
	./compile sendmail.c

setforward: \
load setforward.o cdbmss.o cdbmake.a strerr.a substdio.a stralloc.a \
alloc.a error.a str.a seek.a open.a case.a
	./load setforward cdbmss.o cdbmake.a strerr.a substdio.a \
	stralloc.a alloc.a error.a str.a seek.a open.a case.a

setforward.o: \
compile setforward.c substdio.h subfd.h substdio.h strerr.h \
stralloc.h gen_alloc.h open.h case.h readwrite.h cdbmss.h cdbmake.h \
uint32.h substdio.h
	./compile setforward.c

setmaillist: \
load setmaillist.o getln.a strerr.a substdio.a stralloc.a alloc.a \
error.a str.a open.a
	./load setmaillist getln.a strerr.a substdio.a stralloc.a \
	alloc.a error.a str.a open.a

setmaillist.o: \
compile setmaillist.c substdio.h subfd.h substdio.h strerr.h \
stralloc.h gen_alloc.h getln.h open.h readwrite.h
	./compile setmaillist.c

sgetopt.o: \
compile sgetopt.c substdio.h subfd.h sgetopt.h subgetopt.h \
subgetopt.h
	./compile sgetopt.c

sha1.o : \
compile sha1.c sha1.h
	./compile sha1.c

sha256.o : \
compile sha256.c sha256.h
	./compile sha256.c

sig.a: \
makelib sig_alarm.o sig_block.o sig_catch.o sig_pause.o sig_pipe.o \
sig_child.o sig_hup.o sig_term.o sig_bug.o sig_misc.o
	./makelib sig.a sig_alarm.o sig_block.o sig_catch.o \
	sig_pause.o sig_pipe.o sig_child.o sig_hup.o sig_term.o \
	sig_bug.o sig_misc.o

sig_alarm.o: \
compile sig_alarm.c sig.h
	./compile sig_alarm.c

sig_block.o: \
compile sig_block.c sig.h hassgprm.h
	./compile sig_block.c

sig_bug.o: \
compile sig_bug.c sig.h
	./compile sig_bug.c

sig_catch.o: \
compile sig_catch.c sig.h hassgact.h
	./compile sig_catch.c

sig_child.o: \
compile sig_child.c sig.h
	./compile sig_child.c

sig_hup.o: \
compile sig_hup.c sig.h
	./compile sig_hup.c

sig_misc.o: \
compile sig_misc.c sig.h
	./compile sig_misc.c

sig_pause.o: \
compile sig_pause.c sig.h hassgprm.h
	./compile sig_pause.c

sig_pipe.o: \
compile sig_pipe.c sig.h
	./compile sig_pipe.c

sig_term.o: \
compile sig_term.c sig.h
	./compile sig_term.c

slurpclose.o: \
compile slurpclose.c stralloc.h gen_alloc.h readwrite.h slurpclose.h \
error.h
	./compile slurpclose.c

socket.lib: \
trylsock.c compile load
	( ( ./compile trylsock.c && \
	./load trylsock -lsocket -lnsl ) >/dev/null 2>&1 \
	&& echo -lsocket -lnsl || exit 0 ) > socket.lib
	rm -f trylsock.o trylsock

socket6_if.o: \
compile socket6_if.c socket6_if.h uint32.h
	./compile socket6_if.c

spawn.o: \
compile chkspawn spawn.c sig.h wait.h substdio.h str.h \
stralloc.h gen_alloc.h select.h exit.h coe.h open.h error.h \
auto_qmail.h auto_uids.h auto_spawn.h
	./chkspawn
	./compile spawn.c

spfdinsip.o: \
compile spfdnsip.c spf.h
	./compile spfdnsip.c

spf.o: \
compile spf.c stralloc.h gen_alloc.h alloc.h ipme.h ip.h ipalloc.h \
stralloc.h strsalloc.h str.h fmt.h scan.h now.h case.h
	./compile spf.c

spfquery: \
load spfquery.o spf.o spfdnsip.o ip.o ipme.o ipalloc.o strsalloc.o now.o dns.o \
datetime.a stralloc.a alloc.a str.a substdio.a error.a fs.a case.a dns.lib
	./load spfquery spf.o spfdnsip.o ip.o ipme.o ipalloc.o strsalloc.o \
	now.o dns.o datetime.a stralloc.a alloc.a str.a substdio.a \
	case.a error.a fs.a `cat dns.lib` `cat socket.lib`

spfquery.o: \
compile spfquery.c substdio.h subfd.h stralloc.h gen_alloc.h alloc.h \
spf.h exit.h dns.h str.h ipalloc.h
	./compile spfquery.c 

splogger: \
load splogger.o substdio.a error.a str.a fs.a syslog.lib socket.lib
	./load splogger substdio.a error.a str.a fs.a  `cat \
	syslog.lib` `cat socket.lib`

splogger.o: \
compile splogger.c error.h substdio.h subfd.h exit.h str.h \
scan.h fmt.h
	./compile splogger.c

str.a: \
makelib str_len.o str_diff.o str_diffn.o str_cpy.o str_chr.o \
str_rchr.o str_start.o byte_chr.o byte_rchr.o byte_diff.o \
byte_copy.o byte_cr.o byte_zero.o
	./makelib str.a str_len.o str_diff.o str_diffn.o str_cpy.o \
	str_chr.o str_rchr.o str_start.o byte_chr.o byte_rchr.o \
	byte_diff.o byte_copy.o byte_cr.o byte_zero.o

str_chr.o: \
compile str_chr.c str.h
	./compile str_chr.c

str_cpy.o: \
compile str_cpy.c str.h
	./compile str_cpy.c

str_cpyb.o: \
compile str_cpyb.c str.h
	./compile str_cpyb.c

str_diff.o: \
compile str_diff.c str.h
	./compile str_diff.c

str_diffn.o: \
compile str_diffn.c str.h
	./compile str_diffn.c

str_len.o: \
compile str_len.c str.h
	./compile str_len.c

str_rchr.o: \
compile str_rchr.c str.h
	./compile str_rchr.c

str_start.o: \
compile str_start.c str.h
	./compile str_start.c

stralloc.a: \
makelib stralloc_eady.o stralloc_pend.o stralloc_copy.o \
stralloc_opys.o stralloc_opyb.o stralloc_cat.o stralloc_cats.o \
stralloc_catb.o stralloc_arts.o
	./makelib stralloc.a stralloc_eady.o stralloc_pend.o \
	stralloc_copy.o stralloc_opys.o stralloc_opyb.o \
	stralloc_cat.o stralloc_cats.o stralloc_catb.o \
	stralloc_arts.o

stralloc_arts.o: \
compile stralloc_arts.c str.h stralloc.h gen_alloc.h
	./compile stralloc_arts.c

stralloc_cat.o: \
compile stralloc_cat.c stralloc.h gen_alloc.h
	./compile stralloc_cat.c

stralloc_catb.o: \
compile stralloc_catb.c stralloc.h gen_alloc.h
	./compile stralloc_catb.c

stralloc_cats.o: \
compile stralloc_cats.c str.h stralloc.h gen_alloc.h
	./compile stralloc_cats.c

stralloc_copy.o: \
compile stralloc_copy.c stralloc.h gen_alloc.h
	./compile stralloc_copy.c

stralloc_eady.o: \
compile stralloc_eady.c alloc.h stralloc.h gen_alloc.h \
gen_allocdefs.h
	./compile stralloc_eady.c

stralloc_opyb.o: \
compile stralloc_opyb.c stralloc.h gen_alloc.h
	./compile stralloc_opyb.c

stralloc_opys.o: \
compile stralloc_opys.c str.h stralloc.h gen_alloc.h
	./compile stralloc_opys.c

stralloc_pend.o: \
compile stralloc_pend.c alloc.h stralloc.h gen_alloc.h \
gen_allocdefs.h
	./compile stralloc_pend.c

strerr.a: \
makelib strerr_sys.o strerr_die.o
	./makelib strerr.a strerr_sys.o strerr_die.o

strerr_die.o: \
compile strerr_die.c substdio.h subfd.h exit.h strerr.h
	./compile strerr_die.c

strerr_sys.o: \
compile strerr_sys.c error.h strerr.h
	./compile strerr_sys.c

strsalloc.o: \
compile strsalloc.c alloc.h gen_allocdefs.h stralloc.h strsalloc.h \
gen_alloc.h
	./compile strsalloc.c

subfderr.o: \
compile subfderr.c readwrite.h substdio.h subfd.h 
	./compile subfderr.c

subfdin.o: \
compile subfdin.c readwrite.h substdio.h subfd.h 
	./compile subfdin.c

subfdins.o: \
compile subfdins.c readwrite.h substdio.h subfd.h 
	./compile subfdins.c

subfdout.o: \
compile subfdout.c readwrite.h substdio.h subfd.h 
	./compile subfdout.c

subfdouts.o: \
compile subfdouts.c readwrite.h substdio.h subfd.h 
	./compile subfdouts.c

subgetopt.o: \
compile subgetopt.c subgetopt.h
	./compile subgetopt.c

substdi.o:
#compile substdi.c substdio.h error.h
	./compile substdi.c

substdio.a: \
makelib substdio.o substdi.o substdo.o subfderr.o subfdout.o \
subfdouts.o subfdin.o subfdins.o substdio_copy.o
	./makelib substdio.a substdio.o substdi.o substdo.o \
	subfderr.o subfdout.o subfdouts.o subfdin.o subfdins.o \
	substdio_copy.o

substdio.o: \
compile substdio.c substdio.h
	./compile substdio.c

substdio_copy.o: \
compile substdio_copy.c substdio.h
	./compile substdio_copy.c

substdo.o:
#compile substdo.c substdio.h str.h error.h
	./compile substdo.c

syslog.lib: \
trysyslog.c compile load
	( ( ./compile trysyslog.c && \
	./load trysyslog -lgen ) >/dev/null 2>&1 \
	&& echo -lgen || exit 0 ) > syslog.lib
	rm -f trysyslog.o trysyslog

systype: \
find-systype trycpp.c
	./find-systype > systype

strset.o: \
compile strset.c strset.h uint32.h str.h alloc.h
	./compile strset.c

tai64nfrac: \
load tai64nfrac.o getln.a open.a \
fs.a stralloc.a alloc.a strerr.a substdio.a error.a str.a
	./load tai64nfrac open.a getln.a \
	fs.a stralloc.a alloc.a strerr.a substdio.a \
	error.a str.a 

tai64nfrac.o: \
compile tai64nfrac.c strerr.h stralloc.h gen_alloc.h \
readwrite.h open.h getln.h open.h fmt.h \
substdio.h scan.h error.h
	./compile tai64nfrac.c

tcpto.o: \
compile tcpto.c tcpto.h open.h lock.h seek.h now.h datetime.h ip.h \
datetime.h readwrite.h
	./compile tcpto.c

tcpto_clean.o: \
compile tcpto_clean.c tcpto.h open.h substdio.h readwrite.h
	./compile tcpto_clean.c

timeoutconn.o: \
compile timeoutconn.c ndelay.h select.h error.h readwrite.h ip.h \
timeoutconn.h control.h constmap.h stralloc.h str.h socket6_if.h
	./compile timeoutconn.c

timeoutread.o: \
compile timeoutread.c timeoutread.h select.h error.h readwrite.h
	./compile timeoutread.c

timeoutwrite.o: \
compile timeoutwrite.c timeoutwrite.h select.h error.h readwrite.h
	./compile timeoutwrite.c

tls_errors.o: \
compile tls_errors.c tls_errors.h stralloc.h
	./compile tls_errors.c

tls_remote.o: \
compile tls_remote.c tls_remote.h select.h error.h ndelay.h str.h case.h
	./compile tls_remote.c

tls_start.o: \
compile tls_start.c tls_start.h
	./compile tls_start.c

tls_timeoutio.o: \
compile tls_timeoutio.c tls_timeoutio.h select.h error.h ndelay.h
	./compile tls_timeoutio.c

token822.o: \
compile token822.c stralloc.h gen_alloc.h alloc.h str.h token822.h \
gen_alloc.h gen_allocdefs.h
	./compile token822.c

trigger.o: \
compile trigger.c select.h open.h trigger.h hasnpbg1.h
	./compile trigger.c

triggerpull.o: \
compile triggerpull.c ndelay.h open.h triggerpull.h
	./compile triggerpull.c

uint32_unpack.o: \
compile uint32_unpack.c uint32.h
	./compile uint32_unpack.c

uint32.h: \
tryulong32.c compile load uint32.h1 uint32.h2
	( ( ./compile tryulong32.c && ./load tryulong32 && \
	./tryulong32 ) >/dev/null 2>&1 \
	&& cat uint32.h2 || cat uint32.h1 ) > uint32.h
	rm -f tryulong32.o tryulong32

wait.a: \
makelib wait_pid.o wait_nohang.o
	./makelib wait.a wait_pid.o wait_nohang.o

wait_nohang.o: \
compile wait_nohang.c haswaitp.h
	./compile wait_nohang.c

wait_pid.o: \
compile wait_pid.c error.h haswaitp.h
	./compile wait_pid.c

wildmat.o: \
compile wildmat.c
	./compile wildmat.c

xqp: \
warn-auto.sh xqp.sh
	cat warn-auto.sh xqp.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> xqp
	chmod 755 xqp

xrecipient: \
warn-auto.sh xrecipient.sh
	cat warn-auto.sh xrecipient.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> xrecipient
	chmod 755 xrecipient

xsender: \
warn-auto.sh xsender.sh
	cat warn-auto.sh xsender.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> xsender
	chmod 755 xsender

zddist: \
warn-auto.sh zddist.sh
	cat warn-auto.sh zddist.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> zddist
	chmod 755 zddist

zdeferrals: \
warn-auto.sh zdeferrals.sh
	cat warn-auto.sh zdeferrals.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> zdeferrals
	chmod 755 zdeferrals

zfailures: \
warn-auto.sh zfailures.sh
	cat warn-auto.sh zfailures.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> zfailures
	chmod 755 zfailures

zoverall: \
warn-auto.sh zoverall.sh
	cat warn-auto.sh zoverall.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> zoverall
	chmod 755 zoverall

zrecipients: \
warn-auto.sh zrecipients.sh
	cat warn-auto.sh zrecipients.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> zrecipients
	chmod 755 zrecipients

zrhosts: \
warn-auto.sh zrhosts.sh
	cat warn-auto.sh zrhosts.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> zrhosts
	chmod 755 zrhosts

zrxdelay: \
warn-auto.sh zrxdelay.sh
	cat warn-auto.sh zrxdelay.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> zrxdelay
	chmod 755 zrxdelay

zsenders: \
warn-auto.sh zsenders.sh
	cat warn-auto.sh zsenders.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> zsenders
	chmod 755 zsenders

zsendmail: \
warn-auto.sh zsendmail.sh
	cat warn-auto.sh zsendmail.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> zsendmail
	chmod 755 zsendmail

zsuccesses: \
warn-auto.sh zsuccesses.sh
	cat warn-auto.sh zsuccesses.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> zsuccesses
	chmod 755 zsuccesses

zsuids: \
warn-auto.sh zsuids.sh
	cat warn-auto.sh zsuids.sh \
	| sed s}HOME}"`head -1 conf-home`"}g \
	> zsuids
	chmod 755 zsuids
