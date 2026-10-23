#!/bin/bash
# usage: ./create_zip.sh <pathToLab>   e.g. ./create_zip.sh /home/stud/src/uas-sbg-cryptography/01LBFA/
# zip lands in ./.submission/ (relative to where the script is called)

studyProgram="ITS-B" # without year
# set in ~/.zshrc: STUD_LASTNAME, STUD_FIRSTNAME, STUD_MATRICULATION
lastName="$STUD_LASTNAME"
firstName="$STUD_FIRSTNAME"
matriculationNumber="$STUD_MATRICULATION"

if [ -z "$lastName" ] || [ -z "$firstName" ] || [ -z "$matriculationNumber" ]; then
    echo "ERROR: set STUD_LASTNAME, STUD_FIRSTNAME, STUD_MATRICULATION"
    exit 1
fi

zipName="${studyProgram}_${lastName}_${firstName}_${matriculationNumber}.zip"
outputDir="$PWD/.submission"

if [ $# -ne 1 ]; then
    echo "ERROR: usage: ./create_zip.sh <pathToLab>"
    exit 1
fi

labPath="${1%/}" # remove trailing slash
if [ ! -d "$labPath" ]; then
    echo "ERROR: $labPath is not a folder"
    exit 1
fi

tempDir=$(mktemp -d) # build folder, nothing touches the original
trap 'rm -rf "$tempDir"' EXIT # cleanup when script ends

shopt -s nullglob # no match -> empty list instead of literal pattern
for taskPath in "$labPath"/[0-9][0-9]; do # only folders 01, 02, ...
    taskName=$(basename "$taskPath")
    mkdir "$tempDir/$taskName"

    # only source files, no Makefile / .exe / .o / args.txt / input
    find "$taskPath" -maxdepth 1 -type f \( -name "*.cpp" -o -name "*.hpp" -o -name "*.h" \) -exec cp {} "$tempDir/$taskName/" \;

    # exactly one .cpp with main, names stay as in the original
    mainFiles=$(grep -l "int main" "$tempDir/$taskName"/*.cpp 2>/dev/null)
    if [ "$(echo "$mainFiles" | wc -w)" -ne 1 ]; then
        echo "ERROR: $taskName needs exactly one .cpp with main, found: $mainFiles"
        exit 1
    fi

    # compile check on exactly what gets zipped
    if ! g++ -std=c++17 -o /dev/null "$tempDir/$taskName"/*.cpp; then
        echo "ERROR: $taskName does not compile"
        exit 1
    fi
done

if [ -z "$(ls -A "$tempDir")" ]; then
    echo "ERROR: no task folders (01, 02, ...) in $labPath"
    exit 1
fi

mkdir -p "$outputDir"
rm -f "$outputDir/$zipName" # else zip adds to the old archive
(cd "$tempDir" && zip -r -X -q "$outputDir/$zipName" .) # cd -> 01, 02 on top level

echo "CREATED: $outputDir/$zipName"
unzip -l "$outputDir/$zipName"
echo "SHA-256 of 01 (compare with hash from E-Learning):"
sha256sum "$tempDir"/01/*
