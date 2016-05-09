#!/bin/bash

##
## Z katalogu $1 wziąć $2 najnowszych plików i spakować do archiwum tar.
##


orginalDir=$pwd
outputTar="archive.tar"

cd $1

filesForArchive=$2
filesInDirectory=$(ls | wc -l)

if [ $filesForArchive -gt $filesInDirectory ]
then
	filesForArchive=${filesInDirectory}
fi

for n in $(seq 1 $filesForArchive)
do
	currentFile=$(ls -t | sed -n "${n}p")
	tar -rf $outputTar $currentFile
	echo "tar -rf $outputTar $currentFile"
	#echo "$(ls -t | sed -n "${n}p")"
done

cd $orginalDir
