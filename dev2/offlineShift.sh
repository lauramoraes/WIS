#! /usr/bin/env sh

echo $0
echo $1
echo $2
echo $3

run=$1
type=$2
events=$3
version=$4
dir=$5
jo=$6

tilebeam_home="/afs/cern.ch/user/t/tilebeam"

cd $tilebeam_home/scratch0/Commissioning/reco_new
pwd
source setup.sh
cd TileCalorimeter/TileExample/TileRecEx/TileRecEx-*/cmt
pwd
source setup.sh
cd ../run
pwd
./RecoCommRun $1 $2 $3 $4 $5 $6 $7 $8

