#!/usr/bin/expect
set timeout 10
spawn ssh root@ip
expect "password:"
send "xxx\r"
interact
