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
bld_dir=bld/env
irrlicht_arc=irrlicht-${irrlicht_version}.zip

log INFO "downloading irrlicht source package from ${irrlicht_url}"
mkdir -p ${bld_dir}
qpushd ${bld_dir}
if [[ ! -e ${irrlicht_arc} ]]; then
    wget ${irrlicht_url} -o ${irrlicht_arc}
fi

log INFO "unpacking irrlicht source package ${irrlicht_arc}"

unzip ${irrlicht_arc}

log INFO "building irrlicht source"

qpushd `basename ${irrlicht_arc} .zip`
qpushd source/Irrlicht
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
cp lib/Linux/* ${target_lib}

qpopd
qpopd
