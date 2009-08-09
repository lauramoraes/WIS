#!/usr/bin/expect 

set timeout 20
spawn  /usr/bin/ssh -q tilecom@lxplus.cern.ch
expect {*yes*}
send "yes\n"
expect {*password:}
send "TileCom0\n"
#
#
expect {*tilecom*} 
send "$argv\n"
expect {*tilecom*} exit;
