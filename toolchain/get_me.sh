#!/bin/bash

echo "Downloading arm-none-eabi-gcc toolchain..."
curl -L https://developer.arm.com/-/media/Files/downloads/gnu-rm/10.3-2021.07/gcc-arm-none-eabi-10.3-2021.07-win32.zip --output toolchain.zip
echo "Extracting..."
unzip toolchain.zip
mv gcc-arm-none-eabi-10.3-2021.07/** .
rm -rf gcc-arm-none-eabi-10.3-2021.07
rm toolchain.zip
echo "Done! TODO: Set PATH here please!"
