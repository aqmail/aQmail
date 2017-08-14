echo 'Recipient hosts

One line per recipient host. Information on each line:
* sbytes is the number of bytes successfully delivered to this host.
* mess is the number of messages sent to this host (success plus failure).
* tries is the number of delivery attempts (success, failure, deferral).
* xdelay is the total xdelay incurred by this host.
'
( echo sbytes mess tries xdelay host
HOME/bin/rhosts | sort -k4 ) | HOME/bin/columnt
