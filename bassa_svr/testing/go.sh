#!/bin/bash

rm /var/www/html/bassa/data/*

for(( i=1; i<5; i++ )); do
echo "Requesting ted_$i.mp4"
nc 192.248.16.123 8095 << EOF
<?xml version="1.0"?>
<bassa>
    <offline_download_query>
    <name>GADisk</name>
    <url>http://192.248.16.123/pink/ted_$i.mp4</url>
  </offline_download_query>
</bassa>
EOF
done

