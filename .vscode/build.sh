#!/usr/bin/env bash
# walks up from the focused file's folder to the nearest Makefile and builds there
dir="$1"
while [ "$dir" != "/" ] && [ ! -f "$dir/Makefile" ]; do
    dir=$(dirname "$dir")
done

if [ -f "$dir/Makefile" ]; then
    make -C "$dir" CFLAGS="-g -Wall -Wextra"
else
    echo "no Makefile found in this or any parent folder"
    exit 1
fi
