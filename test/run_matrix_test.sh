#!/usr/bin/bash
stage=0
# test row col major
if [ $stage == 0 ]; then
    for d in 256 512 1024; do 
        echo dim $d
        ./matrix-major-test --dim=$d
    done
fi


# test naive/openblas mul 
if [ $stage == 1 ]; then
    for d in 128 256 512 1024; do 
        echo dim $d
        ./matrix-blas-test --dim=$d
    done
fi
