######################################################################
## very simple script to avoid hand scanning of incomplete jobs     ##
## some job outputs have a consistent file size                     ##
## therefore any output file with size < x comes from a failed job  ##
##                                                                  ##
## loop1 makes a list of existent files                             ##
## loop2 removes small size files                                   ##
## meant for use before running submit_failed_jobs.sh               ##
######################################################################

location="/my_long_path/"
directory="my_directory_"
suffixes=(suffix1 sufix2 sufix3)
filesize_min=1000 # minimum file size in bytes to be considered complete

# make a list of existent root files with their full path
for index in {0..2}; do
	dir $location$directory${suffixes[$index]}/files/*root > filelist__$directory${suffixes[$index]}.txt
done

# loop over the file list and remove those that are incomplete
for index in {0..2}; do
	while read p; do
		filesize=$(stat -c%s "$p")
		#echo $filesize
		if [ $filesize -lt $filesize_min ]; then 
			rm "$p"
			#echo "removed file" $p "with size " $filesize
		fi
	done < filelist_$directory${suffixes[$index]}.txt
done