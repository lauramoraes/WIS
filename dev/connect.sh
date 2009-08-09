#!/usr/bin/expect 

set prompt {*~* }
set timeout -1
spawn  /usr/bin/ssh -q tilebeam@lxplus.cern.ch
expect {
	{*yes/no*}  {send "yes\n" ; exp_continue }
	{*password:} {send "datarun95\n" ; exp_continue }
	"Last login" {puts Conectado.}
}
#
#
expect $prompt  {puts "Got Prompt!!"}
send "$argv\n"
expect $prompt {puts "Got Prompt!!"}
send logout
puts "End of the expect script."
