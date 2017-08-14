echo 'Recipients

One line per recipient. Information on each line:
* sbytes is the number of bytes successfully delivered to this recipient.
* mess is the number of messages sent to this recipient (success plus failure).
* tries is the number of delivery attempts (success, failure, deferral).
* xdelay is the total xdelay incurred by this recipient.
'
( echo sbytes mess tries xdelay recipient
HOME/bin/recipients | sort -k4 ) | HOME/bin/columnt
