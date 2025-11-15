#!/bin/bash
echo "cleaning up before"
rm -r release
make clean
echo "building"
make -j4
result=$?

if [ $result -eq 0 ]
then
  echo "creating package"
  mkdir -p "release/switch/.overlays"
  mv "fastCFWswitch.ovl" "release/switch/.overlays"
  echo "done"
else
  echo "build failed" >&2
  exit $result
fi
