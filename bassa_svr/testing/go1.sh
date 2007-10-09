#!/bin/bash

rm /var/www/html/bassa/data/*

nc 192.248.16.123 8095 << EOF
<?xml version="1.0"?>
<bassa>
  <offline_download_query>
    <name>GADisk</name>
    <url>http://192.248.16.123/ted_1.mp4</url>
  </offline_download_query>
</bassa>
EOF
