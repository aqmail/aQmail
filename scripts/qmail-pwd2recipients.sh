#!/bin/sh
grep "/home/" /etc/passwd | awk -F: '{print $1"@localhost"}' | sort -u >> HOME/users/recipients
