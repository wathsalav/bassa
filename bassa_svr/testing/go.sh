#!/bin/bash

rm /var/www/html/bassa/data/*

for(( i=1; i<5; i++ )); do
nc 127.0.0.1 8095 << EOF
<?xml version="1.0"?>
<bassa>
    <offline_download_query>
    <name>GADisk</name>
    <url>http://127.0.0.1/ted_$i.mp4</url>
  </offline_download_query>
</bassa>
EOF
done

