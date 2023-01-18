#!/bin/bash

echo "C: "
for f in $(find . -name "*.c"); do cat $f; done | wc -l

echo "ASM: "
for f in $(find . -name "*.asm"); do cat $f; done | wc -l

echo "H: "
for f in $(find . -name "*.h"); do cat $f; done | wc -l
