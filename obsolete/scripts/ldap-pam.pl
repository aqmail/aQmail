#
# checkpassword compatible LDAP pam for ADDRESSS (version 0.9.2)
#
# Usage: ldpap-pam -h:host:port -d:DN -w:password -b:base -s:scope -c:certificate -l(ogging) -ll
#
# Warning: This PERL code might not work on all versions of PERL !
#
# Some code taken from: 
# 		ldap_verify.pl by Ted Fines, Jan. 2005.  version 0.1
#
# Customization:
#       Default search attribute is 'Mail'.
#
# Requires:
# 	'Net::LDAP' on http://www.cpan.org 
#
# History: 
#	0.9.2	Fixed bug evalulation reference mailname (tx. Sven)
#
#---------------------------------------------------------------------
use Net::LDAP;
use IO::Handle;
use warnings;
use strict;
#
## Verbose output
#
my $ME='ldap-pam';
my $LOGGING=0;
#
my @INPUT;
my $LDAPHOST='localhost';
my $LDAPPORT='default';
my $LDAPUSR='';
my $LDAPPWD='';
my $LDAPBASE='';
my $LDAPCERT='';
my $TIMEOUT='30';
#
my $LDAPSCOPE='sub';
my $ATTRIBUTE='mail';
my $PROTOCOL_LEN=512;
#
## Check Arguments
#
if ( $#ARGV == -1) {
	print STDERR "[Usage] ldpap-pam -h:host:port -t:timeout -d:DN -w:password -b:base -s:scope -c:certificate -l[l](ogging)\n";
        print STDERR "[Defaults] Host:$LDAPHOST - Port:$LDAPPORT - DN:anonymous - Scope:$LDAPSCOPE - Attribute:$ATTRIBUTE - Timeout:$TIMEOUT \n";
	exit 111;
}

while ( $#ARGV >=  0 ) {
        $_=$ARGV[0];
        s/^-h//        && do { @INPUT = split(/:/,$_); $LDAPHOST=$INPUT[1]; 
				if ( $INPUT[2] ) { $LDAPPORT=$INPUT[2]; } };
        s/^-t//        && do { @INPUT = split(/:/,$_); $TIMEOUT=$INPUT[1]; };
        s/^-d//        && do { @INPUT = split(/:/,$_); $LDAPUSR=$INPUT[1]; };
        s/^-w//        && do { @INPUT = split(/:/,$_); $LDAPPWD=$INPUT[1]; };
        s/^-b//        && do { @INPUT = split(/:/,$_); $LDAPBASE=$INPUT[1]; };
        s/^-s//        && do { @INPUT = split(/:/,$_); $LDAPSCOPE=$INPUT[1]; };
        s/^-c//        && do { @INPUT = split(/:/,$_); $LDAPCERT=$INPUT[1]; };
        s/^-ll//       && do { $LOGGING = 2; };
        s/^-l//        && do { $LOGGING = 1; };
        shift;
}
#
##
my $ldap;
my $mesg;
my $rawinput;
my $verifyaddr;
my $verifyresult;
#
my $num_params = 1;
my $input_descriptor = 3;
#
# These codes from DJB's checkpassword page.
#
my ($verify_ok,$verify_none,$resp_misused,$resp_tempfailure) = (0,1,2,111);
#
my $fhin = new IO::Handle;
my $fherr = new IO::Handle;
$fhin->fdopen($input_descriptor,"r");
$fherr->fdopen(fileno(STDERR),"w");

if (($fhin->opened) && ($fherr->opened)) {
    $fhin->read($rawinput,$PROTOCOL_LEN);
    my @checkfields = split(/\0/,$rawinput);
    if (scalar(@checkfields) != $num_params) {
        if ($LOGGING) { print STDERR "$ME [Error] Wrong format of input address specified.\n"; }
        exit $resp_misused;
    }
    $verifyaddr = $checkfields[0];
    #
    # This section is the 'bottom line' so to speak, where the yea or nay is given.
    #
    $verifyresult = &ldap_mail($verifyaddr);
    if ($verifyresult == $verify_ok) {
        if ($LOGGING) { print STDERR "$ME [Info] Address '$verifyaddr' verified at LDAP Server '$LDAPHOST'.\n"; }
    } elsif ($verifyresult == $verify_none) {
        if ($LOGGING) { print STDERR "$ME [Info] Could not verify address '$verifyaddr' at LDAP Server '$LDAPHOST'.\n"; }
    }
    exit $verifyresult;
}
print STDERR "$ME [Error] Could not connect to LDAP Server '$LDAPHOST:$LDAPPORT'.\n"; 
exit $resp_tempfailure;

sub ldap_mail {
    (my $mailaddr) = @_;

    if ( $LDAPCERT ne "" && $LDAPUSR ne "" && $LDAPPWD ne "" ) {
        if ( $LDAPPORT eq "default" ) { $LDAPPORT='636'; }
        $ldap = Net::LDAPS->new($LDAPHOST, port => $LDAPPORT, timeout => $TIMEOUT) or &mydie();    
        $mesg = $ldap->bind($LDAPUSR, password => $LDAPPWD, version => 3, verify => require, cafile => $LDAPCERT);
    } elsif ( $LDAPUSR ne "" && $LDAPPWD ne "" ) {
        if ( $LDAPPORT eq "default" ) { $LDAPPORT='389'; }
        $ldap = Net::LDAP->new($LDAPHOST, port => $LDAPPORT, timeout => $TIMEOUT) or &mydie();    
        $mesg = $ldap->bind($LDAPUSR, password => $LDAPPWD, version => 3);
    } else {
        if ( $LDAPPORT eq "default" ) { $LDAPPORT='389'; }
        $ldap = Net::LDAP->new($LDAPHOST, port => $LDAPPORT, timeout => $TIMEOUT) or &mydie();    
        $mesg = $ldap->bind(version => 3);
    }
    if ( $mesg->code ) { &mydie($mesg->code) };

    $mesg = $ldap->search (base => $LDAPBASE, scope => $LDAPSCOPE, filter => "$ATTRIBUTE=$mailaddr");
    $mesg->code && &mydie($mesg);
    $ldap->unbind; 

    my $href= $mesg->as_struct;
    my @mailnames = keys %$href;
    foreach (@mailnames) {
      my $DN = $_;
      my $valref = $$href{$_};
      my $mailname = @$valref{$ATTRIBUTE};
      if ( $LOGGING == 2 ) { print STDERR "$ME [Debug] Returned DN '$DN' with '@$mailname' for address '$mailaddr' at LDAP Server '$LDAPHOST'.\n"; }
      if ( "lc($mailaddr)" eq "lc((@$mailname)[0])" ) { return $verify_ok; }
    }
    return $verify_none;
}

sub mydie {
    if (scalar(@_) > 0) {
	print STDERR "$ME [Error] Strange message received from LDAP Server '$LDAPHOST:$LDAPPORT': '$mesg->code', '$mesg->error_name', '$mesg->error->text'.\n"; 
    } else {
	print STDERR "$ME [Error] Could not connect to LDAP Server '$LDAPHOST:$LDAPPORT'.\n"; 
    }
    exit $resp_tempfailure;
}
