#!/bin/bash

#rm /var/www/html/bassa/data/*

nc 192.248.16.15 8095 << EOF
<?xml version="1.0"?>
<bassa>
  <offline_download_query>
    <name>GADisk</name>
    <!--<url>http://bouncer.gentoo.org/fetch/gentoo-2007.0-livecd/x86/</url>-->
    <url>http://download.microsoft.com/download/8/5/6/856bfc39-fa48-4315-a2b3-e6697a54ca88/32 BIT/setup.exe</url>
  </offline_download_query>
</bassa>
EOF
