#!/bin/bash
DATE=`date +%Y%m%d-%H%M%S`
for i in ./*; do
	if [ -d $i ]; then
		7z a -mx=9 -ms=on "${i}_$DATE.7z" "$i";
		zip -ry1 "${i}_$DATE.zip" "$i";
	fi;
done;
exit 0;
