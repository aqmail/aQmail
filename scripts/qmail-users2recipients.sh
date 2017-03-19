LANG=C
grep "^=" HOME/users/assign | awk -F: '{print $1"@localhost"}' | cut -c2- | sort -u >> HOME/users/recipients
