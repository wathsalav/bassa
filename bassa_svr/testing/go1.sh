#!/bin/bash

#rm /var/www/html/bassa/data/*

nc 127.0.0.1 8095 << EOF
<?xml version="1.0"?>
<bassa>
  <offline_download_query>
    <name>GADisk</name>
    <url>http://127.0.0.1/test.html</url>
  </offline_download_query>
</bassa>
EOF
