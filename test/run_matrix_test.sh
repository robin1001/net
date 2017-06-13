#!/usr/bin/bash

for d in 256 512 1024; do 
    echo dim $d
    ./matrix-test --dim=$d
done
