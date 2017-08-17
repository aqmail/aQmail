fqdn="$1"
echo Your fully qualified host name is "$fqdn".

echo Putting "$fqdn" into control/me...
echo "$fqdn" > HOME/control/me
chmod 644 HOME/control/me

( echo "$fqdn" | sed 's/^\([^\.]*\)\.\([^\.]*\)\./\2\./' | (
  read ddom
  echo Putting "$ddom" into control/defaultdomain...
  echo "$ddom" > HOME/control/defaultdomain
  chmod 644 HOME/control/defaultdomain
) )

( echo "$fqdn" | sed 's/^.*\.\([^\.]*\)\.\([^\.]*\)$/\1.\2/' | (
  read pdom
  echo Putting "$pdom" into control/plusdomain...
  echo "$pdom" > HOME/control/plusdomain
  chmod 644 HOME/control/plusdomain
) )

echo Putting "$fqdn" into control/locals...
echo "$fqdn" >> HOME/control/locals
chmod 644 HOME/control/locals

echo Putting "$fqdn" into control/rcpthosts...
echo "$fqdn" >> HOME/control/rcpthosts
chmod 644 HOME/control/rcpthosts
echo "Now qmail will refuse to accept SMTP messages except to $fqdn."
echo 'Make sure to change rcpthosts if you add hosts to locals or virtualdomains!'

echo Enabling TLS  "*:" into control/tlsdestinations ...
echo "*:" >> HOME/control/tlsdestinations
chmod 644 HOME/control/tlsdestinations
echo "Now qmail-remote will send TLS encrypted mails to enabled destinations." 
