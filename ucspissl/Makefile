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

haveip6.h: \
tryip6.c choose compile haveip6.h1 haveip6.h2
	./choose c tryip6 haveip6.h1 haveip6.h2 > haveip6.h

hexconversion.o: \
compile hexconversion.c hexconversion.h
	./compile hexconversion.c

https@: home warn-auto.sh https@.sh
	rm -f https@
	cat warn-auto.sh https@.sh \
	| sed s}HOME}"`head -1 home`"}g \
	| sed s}TCPBIN}"`head -1 ../conf-tcpbin`"}g \
	> https@
	chmod 755 https@


ip4_bit.o:
#compile ip4_bit.c byte.h ip4.h ip4_bit.h 
	./compile ip4_bit.c

ip6_compactaddr.o: \
compile ip6_compactaddr.c fmt.h ip6.h
	./compile ip6_compactaddr.c

ip6_expandaddr.o: \
compile ip6_expandaddr.c ip6.h stralloc.h
	./compile ip6_expandaddr.c

#ip6_scan.o: \
#compile ip6_scan.c hexconversion.h ip6.h scan.h
#	./compile ip6_scan.c

ip6_bit.o: \
compile ip6_bit.c byte.h fmt.h hexconversion.h ip6.h str.h strerr.h
	./compile ip6_bit.c

#it: it-sys it-base it-sslperl 

#it-base: sslclient sslserver https@ sslcat sslconnect sslprint 

it-sslperl: sslperl 

it-sys: sysdeps

#load: ../conf-ld print-ld.sh systype warn-auto.sh
#	rm -f load
#	sh print-ld.sh > load
#	chmod 755 load	

perlembed.lib: ../conf-perl ../conf-ldperl print-perlembed.sh
	rm -f perlembed.lib
	sh print-ldperl.sh > perlembed.lib

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

#sslcat: home warn-auto.sh sslcat.sh
#	rm -f sslcat
#	cat warn-auto.sh sslcat.sh \
#	| sed s}HOME}"`head -1 home`"}g \
#	> sslcat
#	chmod 755 sslcat

#sslclient: load sslclient.o remoteinfo6.o timeoutconn6.o ucspissl.a unix.a dns.a \
#auto_cafile.o auto_cadir.o auto_ciphers.o socket.lib ssl.lib
#	./load sslclient remoteinfo6.o timeoutconn6.o ucspissl.a unix.a dns.a \
#	auto_cafile.o auto_cadir.o auto_ciphers.o \
#	`cat socket.lib` `cat ssl.lib`

#sslclient.o: \
#compile sslclient.c auto_cadir.h auto_cafile.h auto_ciphers.h buffer.h \
#byte.h dns.h error.h exit.h fd.h fmt.h gen_alloc.h getln.h iopause.h ip4.h \
#ndelay.h pathexec.h remoteinfo.h scan.h sgetopt.h sig.h socket.h str.h stralloc.h \
#strerr.h subgetopt.h tai.h taia.h timeoutconn.h ucspissl.h uint16.h uint32.h \
#uint64.h wait.h
#	./compile sslclient.c

#sslconnect: home warn-auto.sh sslconnect.sh
#	rm -f sslconnect
#	cat warn-auto.sh sslconnect.sh \
#	| sed s}HOME}"`head -1 home`"}g \
#	| sed s}TCPBIN}"`head -1 ../conf-tcpbin`"}g \
#	> sslconnect
#	chmod 755 sslconnect

#sslhandle.o: \
#compile sslhandle.c alloc.h auto_cadir.h auto_cafile.h auto_ccafile.h \
#auto_certchainfile.h auto_certfile.h auto_ciphers.h auto_dhfile.h auto_keyfile.h buffer.h \
#byte.h coe.h dns.h env.h error.h exit.h fd.h fmt.h gen_alloc.h getln.h iopause.h \
#ip4.h lock.h ndelay.h open.h prot.h remoteinfo.h rules.h scan.h sgetopt.h sig.h \
#socket.h str.h stralloc.h strerr.h subgetopt.h tai.h ucspissl.h uint16.h uint64.h \
#wait.h
#	./compile sslhandle.c

#sslperl: load sslperl.o rules.o ip4_bit.o remoteinfo6.o timeoutconn6.o \
#sslhandle.o cdb.a ucspissl.a unix.a dns.a \
#auto_cafile.o auto_ccafile.o auto_cadir.o \
#auto_dhfile.o auto_certfile.o auto_keyfile.o \
#auto_ciphers.o auto_certchainfile.o \
#socket.lib ssl.lib socket.lib perlembed.lib
#	./load sslperl auto_cafile.o auto_ccafile.o auto_cadir.o auto_dhfile.o \
#	auto_certfile.o auto_keyfile.o auto_ciphers.o auto_certchainfile.o \
#	rules.o ip4_bit.o remoteinfo6.o timeoutconn6.o sslhandle.o \
#	cdb.a ucspissl.a unix.a dns.a  \
#	`cat socket.lib` `cat ssl.lib` `cat socket.lib` `cat perlembed.lib`

#sslperl.o: \
#compile ccperl sslperl.c gen_alloc.h sslperl.c str.h stralloc.h strerr.h
#	./compile `cat ccperl` sslperl.c

#sslprint: load sslprint.o auto_cafile.o auto_ccafile.o auto_cadir.o \
#auto_dhfile.o auto_certfile.o auto_keyfile.o auto_ciphers.o auto_certchainfile.o \
#remoteinfo6.o timeoutconn6.o sslhandle.o ucspissl.a cdb.a unix.a rules.o ip4_bit.o \
#socket.lib ssl.lib socket.lib
#	./load sslprint auto_cafile.o auto_ccafile.o auto_cadir.o auto_dhfile.o \
#	auto_certfile.o auto_keyfile.o auto_ciphers.o auto_certchainfile.o \
#	rules.o ip4_bit.o remoteinfo6.o timeoutconn6.o sslhandle.o \
#	cdb.a ucspissl.a unix.a dns.a \
#	`cat socket.lib` `cat ssl.lib` 

#sslprint.o: \
#compile sslprint.c buffer.h env.h 
#	./compile sslprint.c

#socket.lib ssl.lib

sslserver: sslserver.o auto_cafile.o auto_ccafile.o auto_cadir.o \
auto_certchainfile.o auto_dhfile.o auto_certfile.o auto_keyfile.o auto_ciphers.o \
rules.o ip4_bit.o remoteinfo6.o timeoutconn6.o
#ucspissl.a 
	./load sslserver auto_cafile.o auto_ccafile.o auto_cadir.o auto_dhfile.o \
	auto_certchainfile.o auto_certfile.o auto_keyfile.o auto_ciphers.o \
	rules.o ip4_bit.o remoteinfo6.o timeoutconn6.o cdb.a ucspissl.a dns.a \
	`cat socket.lib` `cat ssl.lib` \
	byte.o fmt.o pathexec.o prot.o readclose.o qlibs/scan.o qlibs/uint16p.o \
	alloc.a buffer.a case.a dns.a ip.a sig.a str.a stralloc.a socket.a time.a \
	env.a getopt.a fd.a getln.a open.a ndelay.a wait.a \
	strerr.a error.a
#cdb.a ucspissl.a unix.a dns.a socket.lib ssl.lib

sslserver.o: \
auto_cadir.h auto_cafile.h auto_ccafile.h \
auto_certchainfile.h auto_certfile.h auto_ciphers.h auto_dhfile.h auto_keyfile.h
#compile sslserver.c alloc.h 
# buffer.h \
#byte.h dns.h env.h error.h exit.h fd.h fmt.h gen_alloc.h getln.h iopause.h ip4.h \
#ndelay.h open.h pathexec.h prot.h remoteinfo.h rules.h scan.h sgetopt.h sig.h \
#socket.h str.h stralloc.h strerr.h subgetopt.h tai.h taia.h ucspissl.h uint16.h \
#wait.h
	./compile sslserver.c

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
	./makelib ucspissl.a ssl_ca.o ssl_cca.o ssl_certkey.o ssl_chainfile.o ssl_ciphers.o \
	ssl_context.o ssl_env.o ssl_error.o ssl_io.o ssl_new.o ssl_params.o ssl_timeoutaccept.o \
	ssl_timeoutconn.o ssl_verify.o ucspissl.o

ucspissl.o:
	./compile ucspissl.c

uint64.h: choose compile load tryulong64.c uint64.h1 uint64.h2
	./choose clr tryulong64 uint64.h1 uint64.h2 >uint64.h

#unix.a: \
#makelib  alloc.o alloc_re.o buffer.o buffer_0.o buffer_1.o buffer_2.o \
#buffer_copy.o buffer_get.o buffer_put.o buffer_read.o buffer_write.o byte_chr.o \
#byte_copy.o byte_cr.o byte_diff.o byte_zero.o case_diffb.o case_diffs.o coe.o \
#dns_dfd.o dns_domain.o dns_dtda.o dns_ip.o dns_ipq.o dns_name.o dns_nd.o dns_packet.o \
#dns_random.o dns_rcip.o dns_rcrw.o dns_resolve.o dns_sortip.o dns_transmit.o dns_txt.o \
#env.o error.o error_str.o fd_copy.o fd_move.o fmt_ulong.o fmt_xlong.o getln.o \
#getln2.o hexconversion.o iopause.o ip4_bit.o ip4_fmt.o ip4_scan.o ip6_bit.o ip6_compactaddr.o \
#ip6_expandaddr.o ip6_scan.o lock_ex.o lock_un.o ndelay_off.o ndelay_on.o open_append.o \
#open_read.o open_trunc.o openreadclose.o pathexec_env.o pathexec_run.o prot.o \
#readclose.o scan_uint.o scan_ulong.o scan_xlong.o seek_set.o sgetopt.o sig.o \
#sig_block.o sig_catch.o sig_pause.o socket_accept.o socket_accept6.o socket_bind.o \
#socket_bind6.o socket_conn.o socket_conn6.o socket_delay.o socket_getifidx.o socket_getifname.o \
#socket_ip4loopback.o socket_listen.o socket_local.o socket_local6.o socket_opts.o \
#socket_opts6.o socket_recv6.o socket_remote.o socket_remote6.o socket_send6.o socket_tcp.o \
#socket_tcp6.o socket_udp.o socket_udp6.o socket_v4mappedprefix.o socket_v6any.o socket_v6loopback.o \
#str_chr.o str_diff.o str_len.o str_start.o stralloc_cat.o stralloc_catb.o stralloc_cats.o \
#stralloc_copy.o stralloc_eady.o stralloc_opyb.o stralloc_opys.o stralloc_pend.o strerr_die.o \
#strerr_sys.o subgetopt.o tai_pack.o taia_add.o taia_approx.o taia_frac.o taia_less.o \
#taia_now.o taia_pack.o taia_sub.o taia_uint.o uint16_pack.o uint16_unpack.o uint32_pack.o \
#uint32_unpack.o wait_nohang.o wait_pid.o
#	./makelib unix.a  alloc.o alloc_re.o buffer.o buffer_0.o buffer_1.o buffer_2.o \
#	buffer_copy.o buffer_get.o buffer_put.o buffer_read.o buffer_write.o byte_chr.o \
#	byte_copy.o byte_cr.o byte_diff.o byte_zero.o case_diffb.o case_diffs.o coe.o \
#	dns_dfd.o dns_domain.o dns_dtda.o dns_ip.o dns_ipq.o dns_name.o dns_nd.o dns_packet.o \
#	dns_random.o dns_rcip.o dns_rcrw.o dns_resolve.o dns_sortip.o dns_transmit.o dns_txt.o \
#	env.o error.o error_str.o fd_copy.o fd_move.o fmt_ulong.o fmt_xlong.o getln.o \
#	getln2.o hexconversion.o iopause.o ip4_bit.o ip4_fmt.o ip4_scan.o ip6_bit.o ip6_compactaddr.o \
#	ip6_expandaddr.o ip6_scan.o lock_ex.o lock_un.o ndelay_off.o ndelay_on.o open_append.o \
#	open_read.o open_trunc.o openreadclose.o pathexec_env.o pathexec_run.o prot.o \
#	readclose.o scan_uint.o scan_ulong.o scan_xlong.o seek_set.o sgetopt.o sig.o \
#	sig_block.o sig_catch.o sig_pause.o socket_accept.o socket_accept6.o socket_bind.o \
#	socket_bind6.o socket_conn.o socket_conn6.o socket_delay.o socket_getifidx.o socket_getifname.o \
#	socket_ip4loopback.o socket_listen.o socket_local.o socket_local6.o socket_opts.o \
#	socket_opts6.o socket_recv6.o socket_remote.o socket_remote6.o socket_send6.o socket_tcp.o \
#	socket_tcp6.o socket_udp.o socket_udp6.o socket_v4mappedprefix.o socket_v6any.o socket_v6loopback.o \
#	str_chr.o str_diff.o str_len.o str_start.o stralloc_cat.o stralloc_catb.o stralloc_cats.o \
#	stralloc_copy.o stralloc_eady.o stralloc_opyb.o stralloc_opys.o stralloc_pend.o strerr_die.o \
#	strerr_sys.o subgetopt.o tai_pack.o taia_add.o taia_approx.o taia_frac.o taia_less.o \
#	taia_now.o taia_pack.o taia_sub.o taia_uint.o uint16_pack.o uint16_unpack.o uint32_pack.o \
#	uint32_unpack.o wait_nohang.o wait_pid.o
