# This script sets constants needed to run root
# author:       Fernando Guimarães Ferreira
#               Federal University of Rio de Janeiro
#               Coppe - LPS
# advisor:      Carmen Lúcia Lodi Maidantchik
# Rio de Janeiro, March 14th 2006
#/bin/zsh

if test "${ROOT_CONSTS_ALREADY_INIT:-}" != "true"; then
	echo "Setting constants ..."
        unset ROOTSYS
        . /etc/profile.d/root.sh
#	export ROOTSYS=/afs/cern.ch/sw/lcg/external/root/5.06.00/slc3_gcc323/root
#	export LD_LIBRARY_PATH=/afs/cern.ch/sw/lcg/external/root/5.06.00/slc3_gcc323/root/lib/:$LD_LIBRARY_PATH
#	export PATH=/afs/cern.ch/sw/lcg/external/root/5.06.00/slc3_gcc323/root/bin:$PATH
	export ROOT_CONSTS_ALREADY_INIT="true"
	echo "Set up"
else
        echo "Already setup..."
fi

