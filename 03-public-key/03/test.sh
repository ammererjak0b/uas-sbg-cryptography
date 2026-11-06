#!/bin/bash
# local test only, not part of the submission

# keys.txt = output of ../02/main.exe: line 1 "Public key: (e, N)", line 2 "Private key: (d, N)"
# sed -n 1p   -> -n = print nothing by default, 1p = print only line 1
# tr -d '(),' -> -d = delete these chars: ( ) ,
# awk '{print $3, $4}' -> split line into words, print word 3 and 4 (word 1 = Public, 2 = key:, 3 = e, 4 = N)
# < <( )      -> feeds the command output into read
# read -r a b -> -r = take backslashes literally, 1st word -> a, 2nd word -> b
read -r e N < <(sed -n 1p keys.txt | tr -d '(),' | awk '{print $3, $4}') # public key: e, N
read -r d _ < <(sed -n 2p keys.txt | tr -d '(),' | awk '{print $3, $4}') # private key: d, _ = N thrown away (same N)

# main.exe args: <inputFile> <outputFile> <e or d> <N>
# "$e" in quotes -> number stays one single argument
./main.exe message.txt cipher.bin "$e" "$N" # encrypt: message.txt -> cipher.bin
./main.exe cipher.bin decrypted.txt "$d" "$N" # decrypt: cipher.bin -> decrypted.txt

# cmp A B -> compares bytes, silent if equal, prints first difference if not
# && -> run echo only if cmp found no difference
cmp message.txt decrypted.txt && echo "OK: identical"
