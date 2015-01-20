#!/bin/bash

Folder="TutorialCPPROOT"

mkdir -p ../temp
cd ../temp

if [[ -d ${Folder} ]]; then
    echo "${Folder} exists, so we remove it"
    rm -rf ${Folder}
fi
if [[ -f ${Folder}.tgz ]]; then
    echo "${Folder}.tgz exists, so we remove it"
    rm -f ${Folder}.tgz
fi
mkdir -p ${Folder}
# copy the folders
cp -r ../${Folder}/* ${Folder}
# uncompile
make clean -C ${Folder}
# for each folder we remove the .svn inside
for dir in ${Folder}/*/
do
    dir=${dir%*/}
    folder=${dir##*/}
    echo ${folder}
    rm -rf ${folder}/.svn
done
# now ready to tar it
tar cvzf ${Folder}.tgz ${Folder}
rm -rf ${Folder}
mv ${Folder}.tgz ../.
cd ..
rm -rf temp
scp ${Folder}.tgz ppelx.physics.gla.ac.uk:~/public_html/${Folder}/.
