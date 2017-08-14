echo 'Senders

One line per sender. Information on each line:
* mess is the number of messages sent by this sender.
* bytes is the number of bytes sent by this sender.
* sbytes is the number of bytes successfully received from this sender.
* rbytes is the number of bytes from this sender, weighted by recipient.
* recips is the number of recipients (success plus failure).
* tries is the number of delivery attempts (success, failure, deferral).
* xdelay is the total xdelay incurred by this sender.
'
( echo mess bytes sbytes rbytes recips tries xdelay sender
HOME/bin/senders | sort -n -k7 ) | HOME/bin/columnt
