#!/bin/bash

tmp=$(mktemp -d)
cd $tmp
touch -d '1918-11-11 11:00 GMT' wwi-armistice-cs35L
touch now
sleep 1
touch now1
TZ=UTC0 ~/cs35l/assign4/lab4coreutils/bin/ls -lt --full-time wwi-armistice-cs35L now now1
cd ~/cs35l/assign4
rm -rf $tmp
