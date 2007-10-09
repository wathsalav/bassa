README File
-----------
This file has step by step installation gudie and specifies the requirements to install GAD system.

Please Read this file before installing GAD.
you need to install BASSA system before installing this.
please read documentation for more details.

REQUIREMENTS
-------------
+ Operating systems
This will work on following operationg systems.
Freebsd 6.1
Linux Fedora 5
Windows

+ Packages
You advised to install following packages before installing GAD.
apache 2
mysql 5
php 5
php5-mysql5
php5-ftp
php5-Extensions (session)
mod_php5
md5

INSTALLATION
------------
Follow the instructions given below.
+MAC OSX
1. download gad.pkg
2. double click on pkg file.
3. follow readme instructions

+source code

1. Download gadisk.tar.gz latest version.

2. Copy the file into the document root of your web server. 
eg:/var/www/html or /usr/local/www/data
cp gadisk.tar.gz /DOCUMENTROOT/ 

3. change directory to document root.
cd /DOCUMENTROOT/

4.Extract files 
tar -xzvf gadisk.tar.gz


5. Modify the configuration file according to your needs. 
cd /DOCUMENTROOT/gadisk/lib/
vi config.php
give your mysql database username and password.
give bassa server ip and port.

6. Create gadisk database.
cd /DOCUMENTROOT/gadisk/database/
vi department
Enter departments name on each line.
php create_db.php

8. Restart webserver and open your web browser, type
http://localhost/gadisk


CONFIGURATION

-------------
+squid

1.give download limit in squid.conf file
cd /etc/squid (or /usr/local/etc/squid if you compile from source code)
vi squid.conf
acl ACLNAME src YOURNETWORK
reply_body_max_size SIZE allow ACLNAME
http_access allow ACLNAME

ACLNAME - name of your acl (eg: staff)
YOURNETWORK - ip range (eg: 192.168.1.0/24)
SIZE - download limit size (eg: 150000 for 150KB)

Additionally add this line to get full url (with options)
strip_query_terms off

2. customize error message
cd error/ (it sholud under error/English folder or the error/language you choose in latest version of squid)
mv ERR_TOO_BIG ERR_TOO_BIG.bak (back up early error message)
vi ERR_TOO_BIG (write new error message)
copy paste the following text
HOSTNAME - where your gad system running
________________________________________________________________________________________
<html>
<head>
<title>Download Too Big</title>
<SCRIPT language="JavaScript">
function submitform()
{
  document.myform.submit();
}
</SCRIPT>
</head>
<body onload="submitform()">
<h2>Please wait .....</h2>
Your request is being directed to offline downloader.
<form name="myform" action="http://HOSTNAME/gadisk/index.php" method="post" >
<INPUT type="hidden" name="url" size="100" value="%U" readonly>
</form>
</body>
</html>
__________________________________________________________________________________________

+sendmail or postfix
you shoud be able to send mail from GAD server using the email given in config.php.

+change Templetes
To change a css select a templete copy it to default.css 
cd images/
cp (prefered css) default.css

you can do the following optional steps by run make file.

+http access control
copy install/apache-gadisk.conf to apache conf.d directory.
modify allow ip range.

+mysql backup
copy install/cron-gadisk to /etc/cron.hourly/

here you are!

Now, try downloading file more than the SIZE you given in squid.conf
you should be redirected to  GAD. .


Thank you for using GAD!

