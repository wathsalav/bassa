<html>
<style type='text/css' media='all'>@import '../css/default.css';</style>
<body>
<a href='../doc.php'>Back</a>

<div class='center'>
<h2 align='center'> FAQ </h2>
<p>
<font> What is GADisk? </font>
It is Globally Accessible Disk. That means you can download using our bandwidth and store them for future reference and can access it from anywhere in the world ....
<p>
<font> Why GADidk? </font> 
Please refer <a href='documentation.php'>Documentation file.</a>
<p>
<font> How to implement GADisk? </font>
Please refer <a href='../README.txt'>README file</a>.
<p>
<font> How to use GADisk? </font><br>
-: Administrator :- <br>
you have to provide a user name and password in the configuration file.
login using that user name and password.
you can view and delete downloads and users from the admin area.
if you find BASSA didn't download a particular file, you can resend the request to BASSA.<br>
-: User :- <br>
You need not to configure anything in the browser.
If you exceed the download limit the proxy will automatically redirect you to this system.
You have to create a login account to add your download.
The password will be sent to your email address which was provided by you as username. Please change the password by clicking on 'user preferences' and 'change password' after first login.
You have to login using the username and password to proceed the download.
You have to go to GADisk system website again to get the downloaded file after it completes.
if you forget your password please click on 'forget password' in the login prompt and give your username to email the password.
<p>
<font> What are the Advantages? </font>
<li> Extremely Fast access inside the local network. 
For example, it takes maximum 3 minutes to download an iso image.</li>
<li> Avoiding Repetition of downloads.
The system currently compare exact filename and size to find the file.</li>
<li> No need to wait or lock your machine until it finishes downloading.
We download the file and keep it for you, in our system.</li>
<li> Manage your downloads.
You can add new or remove unwanted downloads and view the status of the download.</li>
<p>
Nowhere, you can find a system like this which has above features for zero cents.
<p>
<font> What are the Disadvantages?</font>
<li> Eventhough it is fast within LAN, it will download in normal speed outside the network.
It can be eleminated if we have a lot of GAD-BASSA system around the world.</li>
<li> Implementation needs some knowladge about servers.
A normal user can't implement this into his/her system. user needs some knowladge about web,mail and proxy servers. But, anybody who knows how to login and check emails can use this system efficiently.</li>
<p> 
<font> TODO </font>
<li> selection of downloader automatically.</li>
<li> search files on all available mirrers.</li>
<li> comment on a download.</li>
<li> implement more security.</li>

<p>
<font> Future Plan </font>
This system is developed to work with BASSA - offline downloader. It has to be enhanced to work with any other downloaders which can be used on a proxy servers.

<?php
include '../files/footer.php';
?>




