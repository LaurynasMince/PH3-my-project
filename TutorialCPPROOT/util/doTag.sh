#!/bin/bash
#Adrian Oct 2012
#Commit a tag version to SVN

if [ $# -ne 4 ]; then
    cat<<EOF
Usage: $0 package number do_list do_tag  
Usage: $0 TutorialCPPROOT 00-00-99 0 0
EOF
    exit 1
fi

package=$1
number=$2
do_list=$3
do_tag=$4


path="$SVNUSR/${package}"
trunk="${path}/trunk"
tags="${path}/tags"
tag="${tags}/${package}-${number}"
#echo "trunk=${trunk}"
#echo "tags=${tags}"
#echo "tag=${tag}"

command_list="svn ls ${tags}"
echo ${command_list}

command_tag="svn cp ${trunk} ${tag}"
echo ${command_tag}

if [ ${do_list} == 1 ]; then
    `echo ${command_list}`
fi

if [ ${do_tag} == 1 ]; then
    `echo ${command_tag}`
fi
