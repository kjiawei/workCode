#!/bin/bash

echo "Packing......"
./pack -P JH5058 -C pack.cfg -o update.pkg
echo "update.pkg done!"

echo "Unpacking......"
./unpack update.pkg
echo "done!"

