# Tx. Warren Odom (28 Apr 2005)

cd HOME/alias
ls -l .qmail-* | grep -v .qmail-default | tr -s " " | awk '{print $9}' | sed s/^\.qmail-// | awk '{print $2"@localhost"}' | sort -u >> HOME/users/recipients
