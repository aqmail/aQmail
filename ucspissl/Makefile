# Don't edit Makefile!  Use conf-* for configuration.

SHELL=/bin/sh

COMPILE=./compile
MAKELIB=./makelib

#default: it
default: ucspissl.a

clean:
	rm -f *.o *.a

obj:
	$(COMPILE) ssl_*.c ucspissl.c

auto_cadir.o:
	./compile auto_cadir.c

auto_cafile.o: compile auto_cafile.c
	./compile auto_cafile.c

auto_ccafile.o: compile auto_ccafile.c
	./compile auto_ccafile.c

auto_certchainfile.o: compile auto_certchainfile.c
	./compile auto_certchainfile.c

auto_certfile.o: compile auto_certfile.c
	./compile auto_certfile.c

auto_ciphers.o: compile auto_ciphers.c
	./compile auto_ciphers.c

auto_dhfile.o: compile auto_dhfile.c
	./compile auto_dhfile.c

auto_keyfile.o: compile auto_keyfile.c
	./compile auto_keyfile.c

#haveip6.h: \
#tryip6.c choose compile haveip6.h1 haveip6.h2
#	./choose c tryip6 haveip6.h1 haveip6.h2 > haveip6.h

hexconversion.o: \
compile hexconversion.c hexconversion.h
	./compile hexconversion.c

#https@: home warn-auto.sh https@.sh
#	rm -f https@
#	cat warn-auto.sh https@.sh \
#	| sed s}HOME}"`head -1 home`"}g \
#	| sed s}TCPBIN}"`head -1 ../conf-tcpbin`"}g \
#	> https@
#	chmod 755 https@

#ip4_bit.o:
#compile ip4_bit.c byte.h ip4.h ip4_bit.h 
#	./compile ip4_bit.c

#ip6_compactaddr.o: \
#compile ip6_compactaddr.c fmt.h ip6.h
#	./compile ip6_compactaddr.c

#ip6_expandaddr.o: \
#compile ip6_expandaddr.c ip6.h stralloc.h
#	./compile ip6_expandaddr.c

#ip6_scan.o: \
#compile ip6_scan.c hexconversion.h ip6.h scan.h
#	./compile ip6_scan.c

#ip6_bit.o: \
#compile ip6_bit.c byte.h fmt.h hexconversion.h ip6.h str.h strerr.h
#	./compile ip6_bit.c

#it: it-sys it-base it-sslperl 

#it-base: sslclient sslserver https@ sslcat sslconnect sslprint 

it-sslperl: sslperl 

it-sys: sysdeps

#load: ../conf-ld print-ld.sh systype warn-auto.sh
#	rm -f load
#	sh print-ld.sh > load
#	chmod 755 load	

#perlembed.lib: ../conf-perl ../conf-ldperl print-perlembed.sh
#	rm -f perlembed.lib
#	sh print-ldperl.sh > perlembed.lib

remoteinfo6.o:
	$(COMPILE) remoteinfo6.c

rules.o:
	./compile rules.c

socket.lib: trylsock.c compile load
	( ( ./compile trylsock.c && \
	./load trylsock -lsocket -lnsl ) >/dev/null 2>&1 \
	&& echo -lsocket -lnsl || exit 0 ) > socket.lib
	rm -f trylsock.o trylsock

ssl.lib: ../conf-ssllib print-dl.sh
	rm -f ssl.lib
	sh print-dl.sh > ssl.lib
	chmod 755 ssl.lib

ssl_ca.o:
	./compile ssl_ca.c

ssl_cca.o:
	./compile ssl_cca.c

ssl_chainfile.o:
	./compile ssl_chainfile.c

ssl_certkey.o:
	./compile ssl_certkey.c

ssl_ciphers.o:
	./compile ssl_ciphers.c

ssl_context.o:
	./compile ssl_context.c

ssl_env.o:
	./compile ssl_env.c

ssl_error.o:
	./compile ssl_error.c

ssl_io.o:
	./compile ssl_io.c

ssl_new.o:
	./compile ssl_new.c

ssl_params.o:
	./compile ssl_params.c

ssl_timeoutaccept.o:
	./compile ssl_timeoutaccept.c

ssl_timeoutconn.o:
	./compile ssl_timeoutconn.c

ssl_verify.o:
	./compile ssl_verify.c


sslserver: sslserver.o auto_cafile.o auto_ccafile.o auto_cadir.o \
auto_certchainfile.o auto_dhfile.o auto_certfile.o auto_keyfile.o auto_ciphers.o \
rules.o remoteinfo6.o timeoutconn6.o
#ucspissl.a  ip4_bit.o
	./load sslserver auto_cafile.o auto_ccafile.o auto_cadir.o auto_dhfile.o \
	auto_certchainfile.o auto_certfile.o auto_keyfile.o auto_ciphers.o \
	rules.o remoteinfo6.o timeoutconn6.o cdb.a ucspissl.a dns.a \
	`cat socket.lib` `cat ssl.lib` \
	byte.o fmt.o pathexec.o prot.o readclose.o qlibs/scan.o qlibs/uint16p.o \
	alloc.a buffer.a case.a dns.a ip.a sig.a str.a stralloc.a socket.a time.a \
	env.a getopt.a fd.a getln.a open.a ndelay.a wait.a \
	strerr.a error.a
#	rules.o ip4_bit.o remoteinfo6.o timeoutconn6.o cdb.a ucspissl.a dns.a \

sslserver.o:
#auto_cadir.h auto_cafile.h auto_ccafile.h \
#auto_certchainfile.h auto_certfile.h auto_ciphers.h auto_dhfile.h auto_keyfile.h
#compile sslserver.c alloc.h 
# buffer.h \
#byte.h dns.h env.h error.h exit.h fd.h fmt.h gen_alloc.h getln.h iopause.h ip4.h \
#ndelay.h open.h pathexec.h prot.h remoteinfo.h rules.h scan.h sgetopt.h sig.h \
#socket.h str.h stralloc.h strerr.h subgetopt.h tai.h taia.h ucspissl.h uint16.h \
#wait.h
	$(COMPILE) sslserver.c

#sysdeps: systype compile load hasflock.h hassgact.h hassgprm.h \
#haswaitp.h iopause.h select.h uint64.h
#	rm -f sysdeps
#	cat systype compile load >> sysdeps
#	grep sysdep hasflock.h >> sysdeps
#	grep sysdep hassgact.h >> sysdeps
#	grep sysdep hassgprm.h >> sysdeps
#	grep sysdep haswaitp.h >> sysdeps
#	grep sysdep iopause.h >> sysdeps
#	grep sysdep select.h >> sysdeps
#	grep sysdep uint64.h >> sysdeps

#systype: find-systype.sh trycpp.c x86cpuid.c
#	sh find-systype.sh > systype

timeoutconn.o: \
compile timeoutconn.c error.h iopause.h ndelay.h socket.h tai.h \
taia.h timeoutconn.h uint16.h uint64.h
	./compile timeoutconn.c

timeoutconn6.o:
	$(COMPILE) timeoutconn6.c

libqssl.a: ucspissl.a
	@cp ucspissl.a libqssl.a

ucspissl.a: obj
#makelib  ssl_ca.o ssl_cca.o ssl_certkey.o ssl_chainfile.o ssl_ciphers.o \
#ssl_context.o ssl_env.o ssl_error.o ssl_io.o ssl_new.o ssl_params.o ssl_timeoutaccept.o \
#ssl_timeoutconn.o ssl_verify.o ucspissl.o
	$(MAKELIB) ucspissl.a ssl_ca.o ssl_cca.o ssl_certkey.o ssl_chainfile.o ssl_ciphers.o \
	ssl_context.o ssl_env.o ssl_error.o ssl_io.o ssl_new.o ssl_params.o ssl_timeoutaccept.o \
	ssl_timeoutconn.o ssl_verify.o ucspissl.o

ucspissl.o:
	./compile ucspissl.c

uint64.h: choose compile load tryulong64.c uint64.h1 uint64.h2
	./choose clr tryulong64 uint64.h1 uint64.h2 >uint64.h
