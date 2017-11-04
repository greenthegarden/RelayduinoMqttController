#!/usr/bin/env bash

WIDTH=288

if [ ! -d small ]; then
  mkdir small
fi

shopt -s nullglob
for file in *.png
do
	echo "Resizing png image file ${file}"
#  magick mogrify -size ${WIDTH} ${file}
  convert ${file} -resize ${WIDTH} small/${file}
done
