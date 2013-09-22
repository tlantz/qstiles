#!/bin/bash

function log()
{
    ts=`date '+%d-%b-%y %H:%M:%S,%N'`
    echo "INSTALL   ${ts}     ${1}    ${2}"
}

function qpushd()
{
    pushd $1 > /dev/null
}

function qpopd()
{
    popd $1 > /dev/null
}

if [[ 2 > $#* ]]; then
    echo "error: not enough arguments"
    echo "Usage: $0 <target> [irrlicht_version]"
    exit 1
fi

target_dir=$1
shift
irrlicht_version=$1
shift
if [[ -z ${irrlicht_version} ]]; then
    irrlicht_version=1.8
fi
irrlicht_url=http://downloads.sourceforge.net/irrlicht/irrlicht-${irrlicht_version}.zip

log INFO "downloading irrlicht source package"
mkdir tmp
qpushd tmp
wget ${irrlicht_url}

log INFO "unpacking irrlicht source package"

unzip irrlicht-${irrlicht_version}.zip

log INFO "building irrlicht source"

qpushd irrlicht-${irrlicht_version}/source/Irrlicht
make -j8
qpopd

log INFO "installing irrlicht build in ${target_dir}"
target_inc=${target_dir}/include
target_lib=${target_dir}/lib
if [[ ! -d ${target_inc} ]]; then
    mkdir -p ${target_inc}
fi
rm -rf ${target_inc}/*
cp -R include/* ${target_inc}

if [[ ! -d ${target_lib} ]]; then
    mkdir -p ${target_lib} 
fi
rm -rf ${target_lib}/*
cp source/Irrlicht/libIrrlicht.a ${target_lib}

qpopd

qpopd
log INFO "cleaning up tmp contents"
rm -rf tmp
