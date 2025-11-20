#!/bin/sh
##
#  file: qscanMatrix.sh
#  date: 2017.04.10
#
#

#_______________________________________________________________________________
work_dir=$(dirname $0)
bin_dir=$work_dir/bin

#_______________________________________________________________________________
if [ $# != 2 ]; then
    echo "Usage: $(basename $0) [org] [new]"
else
    ${work_dir}/makeConf ${1} ${2}
    cp ../HDPRM/HodoParam_e70_piK_${1}     ../HDPRM/HodoParam_e70_piK_${2}
    cp ../HDPHC/HodoPHCParam_e70_piK_${1}  ../HDPHC/HodoPHCParam_e70_piK_${2}
    cp ../DCTDC/DCTdcCalib_e70_piK_${1}    ../DCTDC/DCTdcCalib_e70_piK_${2}
    cp ../DCDRFT/DCDriftParam_e70_piK_${1} ../DCDRFT/DCDriftParam_e70_piK_${2}
    echo "File copied : run${1} -> run${2}"
fi
