<?php
include 'lib/config.php';
include 'lib/opendb.php';
require_once 'lib/functions.php';
include 'files/header.php';
### variable declaration ###
if (isset ($_POST['url']) && $_POST['url']!=''){
  $_SESSION['url']=$_POST['url'];
 }
$url=$_SESSION['url'];

echo '<div class="logo">';

echo "<table><tr><td bgcolor='#CDD8E7'>";
if (!isset($email)){
  include 'loginbox.php';
  echo 'Dont have login? <a href="regform.php">Signup Here!</a>';
 }
 else {
   echo "<img src='images/gad-bassa.png'>";
 }
echo "</td></tr></table>";

echo '<div class="title">';

/* search file*/
if ($url!=''){
  echo "<hr>";
  $size=remotefsize($url);
  if ($size!='0'){
    $_SESSION['size']=$size;
    search_file($url,$size);
    if ($_SESSION['email']!='' && $_SESSION['passwd']!=''){
      echo "<head><meta http-equiv='refresh' content='0;URL=login.php'></head>";
    }
    echo "<hr>";
  }
  else {
    echo "<font class='red'>Zero size reply.</font><br/>";
    echo "Unable to get the size of the file. Please try again later.<br/>";
    $_SESSION[url]='';
  }

 }
 else {
   echo "<h3><font class='red'>*</font> Please <font class='red'>do not</font> use this system to get paid Downloads.</h3><br/>";
 }
echo "</div></div>";
include 'files/navigation.php';
echo "<hr>";

echo "<h2>Message Board</h2>";

echo "<font class='red'>10th May 2007</font>";
echo "<p>";
echo "Due to internal mail server problem, we were unable to deliver the mails on time. Those who didn't receive the password yet (please allow 10min), please click on forget password and provide your email address. you will get the password as soon as possible. Thanks for ackasanga and chamlyindunil for pointing this problem. ";
echo "</p>";
echo "<a href='message.php'>more ...</a>";

echo "<hr>";

echo "<h2>Introduction</h2>";
echo "<p>";
echo " Each second spend on Internet is valuable as we are paying for it. At present, we are sharing about 2 MBps among 1000 users in our Institution. During the peak time the users find it difficult to surf Internet due to heavy download, as it consumes lot of time to finish it task. As a result the users get frustrated due to the time taken for downloading. During the off time the bandwidth get wasted due to non-operation.";
echo " <a href='doc/documentation.php'>more ...</a><br/>";
echo "<hr>";

/* Display Latest 5 downloads*/
$query="SELECT * FROM downloads where status='queue'";
$result=mysql_query($query);
$num=mysql_numrows($result);
$i=($num>5)?$num-5:0;
if ($num!=0) {
  echo "<h2>Last Five Requests</h2>";
  echo "<table>";
  echo "<tr><th>File Name</th><th>Origin Server URL</th></tr>";
  while ($i < $num) {
    $my_file=mysql_result($result,$i,'file');
    $my_url=mysql_result($result,$i,'url');
    echo "<tr><td>$my_file</td>";

    $strlen=strlen($my_url);
    if ($strlen>30){
      $parts = explode("/", "$my_url");
      $site=$parts[2];
      echo "<td>$site</td></tr>";
    }
    else {
      echo "<td>$my_url</td></tr>";
    }
    $i++;
  }
  echo "</table>";

 }
 else {
   echo " No requests to list.<br/>";
 }
echo "<hr>";

/* Display Statistics*/
$query="SELECT * FROM downloads where status like '%completed%'";
$result=mysql_query($query);
$num=mysql_numrows($result);
if ($num!=0) {
  $i=0;
  while ($i < $num) {
    $my_hits=mysql_result($result,$i,hits);
    $my_size=mysql_result($result,$i,size);
    $cache_size+=$my_size;
    $total_size+=($my_size*$my_hits);
    $total_hits+=$my_hits;
    $i++;
  }
  //calculating total download files
  $cgb=round($cache_size/1073741824,2);
  $tgb=round($total_size/1073741824,2);
  $hits=$total_hits-$num;

  // Top requested file
  $h_query="SELECT file FROM downloads where hits=(SELECT MAX(hits) FROM downloads) ";
  $h_result=mysql_query($h_query);
  $max_hits=mysql_result($h_result,file);

  //Number of users
  $u_query="SELECT COUNT(email) from users";
  $u_result=mysql_query($u_query);
  $num_users=mysql_result($u_result,email);

  //Displaying all statistics
  echo "<h2>Bassa statistics</h2>";
  echo "<table>";
  echo "<tr><td>Total Downloads</td><td>$cgb GB</td></tr>";
  echo "<tr><td>Served Size</td><td>$tgb GB</td></tr>";
  echo "<tr><td>Total Cache Hits</td><td>$hits</td></tr>";
  echo "<tr><td>Top Requested File</td><td>$max_hits</td></tr>";
  echo "<tr><td>Number of Files</td><td>$num</td></tr>";
  echo "<tr><td>Number of Users</td><td>$num_users</td></tr>";

  echo "</table>";
 }

mysql_free_result($result);
mysql_close($conn);

include 'files/footer.php';
?>
