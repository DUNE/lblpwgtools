#!/bin/bash

# This script is a near copy of the copyOutFiles
# except that it allows copying of hist files and
# doesn't make hashdirs. I doubt non-production
# users want that. Keep it simple. 

declare -a args

pushd results

count=0
for file in *.root
  do
  echo "Attempting to transfer $file to $DEST"
  case "$file" in
      *%*) 
      echo "$file contains a % - do not copy"
      continue;;
      *\**) 
      echo "$file contains a \* - do not copy"
      continue;;
      *)
      newdest=${DEST}/$(basename ${file})
      echo "Try to ifdh ls ${newdest}"
      if ! ifdh ls ${newdest}; then
	  echo "File does not exist in the output directory - copy out the results"
	  if [ "$count" -eq "0" ]; then 
	      args+=("$file ${newdest}")
	  else
	      args+=(" ; $file ${newdest}")
	  fi
	  let count=(count+1)
      else
	  echo "File already exists in the output directory - do nothing"
      fi	  
      continue;;
  esac
done

echo "Copying out all files at once!"

if [ "${#args[@]}" -gt "0" ]; then
   echo "${#args[@]} files to copy out"
   echo "ifdh cp ${args[*]}"
   ifdh cp ${args[*]}
fi

popd
