#!/bin/bash
echo "LZMA 500kb"
time 7z a 500kB.7z 500kB.data -m1=LZMA >> /dev/null

echo "LZMA 3MB"
time 7z a 3MB.7z 3MB.data -m1=LZMA >> /dev/null

echo "LZMA 10MB"
time 7z a 10MB.7z 10MB.data -m1=LZMA >> /dev/null


echo "BZip2 500kb"
time 7z a 500kBbzip2.7z 500kB.data -m1=BZip2 >> /dev/null

echo "BZip2 3MB"
time 7z a 3MBbzip2.7z 3MB.data -m1=BZip2 >> /dev/null

echo "BZip2 10MB"
time 7z a 10MBbzip2.7z 10MB.data -m1=BZip2 >> /dev/null


echo "LZO 500kB"
time lzop 500kB.data >> /dev/null

echo "LZO 3MB"
time lzop 3MB.data >> /dev/null

echo "LZO 10MB"
time lzop 10MB.data >> /dev/null