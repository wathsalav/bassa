<?php
#################################
### File  : opendb.php 	      ###
### Author: Gowri Ananthan    ###
### Email : gna@ucsc.cmb.ac.lk###
### Licence: BSD              ###
#################################
include 'config.php';
$conn = mysql_connect($dbhost, $dbuser, $dbpass) or die ("Unable to connect to $dbhost using username:$dbuser and given password! Please read README file<br/>".mysql_error());
mysql_select_db($dbname) or die ("Database $dbname not exist. Please read README file.<br/>" .mysql_error());
?>
