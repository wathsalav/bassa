<?php
include 'lib/config.php';
include 'lib/opendb.php';
require_once 'lib/functions.php';
include 'files/header.php';
include 'files/navigation.php';

$email=$_SESSION['email'];
$id=$_GET['id'];
$query=("select * from downloads where id='$id'");
$result=mysql_query($query);
while($row = mysql_fetch_array($result)) {
  $size=$row['size'];
  $user=$row['email'];
  $status=$row['status'];
  $url=$row['url'];
  $file=$row['file'];
  $date=$row['date'];
  $cache_url=$row['local_url'];
  $parts = explode(" ", "$status");
  $status_1=$parts[0];
  $status_2=$parts[1];
 }
$query="select * from meta_info where origin_url='$url'";
$result=mysql_query($query);
while($row = mysql_fetch_array($result)){
  $meta_url=$row['meta_url'];
  $mime_type=$row['mime_type']; 
 }
if (strstr($cache_url,'.flv')||strstr($cache_url,'.swf')){
  $burl = $cache_url;
 }
 else if (strstr($meta_url, '.flv')){
   $burl = $meta_url;
 }
echo "<table>";
echo "<tr><td colspan='2' align='center'>";
?>
<?php
if ($burl)
  echo '<object type="application/x-shockwave-flash" data="FlowPlayer.swf" width="400" height="330" id="FlowPlayer">
	<param name="allowScriptAccess" value="always" />
        <param name="movie" value="FlowPlayer.swf" />
        <param name="quality" value="high" />
        <param name="scaleMode" value="showAll" />
        <param name="allowfullscreen" value="true" />
        <param name="wmode" value="transparent" />
        <param name="allowNetworking" value="all" />
        <param name="flashvars" value="config={ videoFile: \''.$burl.'\'}" />
</object>';
?>
<?php

echo "</td></tr>";	
echo '<tr><th colspan=2>File Details</th></tr>';
echo '<tr><td>File Name </td><td> '.$file. '</td></tr>';
echo '<tr><td>Origin URL </td><td> '.$url.'</td></tr>';
echo '<tr><td>Last Accessed Date </td><td> '.$date.'</td></tr>';
echo '<tr><td>Size </td><td> '.$size . ' Bytes </td></tr>';
echo "<tr><td>Status </td><td> $status_1</td></tr>";
		
if ($email==$admin){
  echo '<tr><td>User</td><td> '.$user.'</td></tr>';
 }

if ($status_2!=''){
  echo '<tr><td>Status Code</td><td> '.$status_2 . '</td></tr>';
  echo "<tr><td>Reason </td><td> ";
  if ($status_2==200){echo "OK";}
  if ($status_2==206){echo "Partial Content";}
  if ($status_2==401){echo "Unauthorized";}
  if ($status_2==403){echo "Forbidden";}
  if ($status_2==404){echo "Not Found";}
  if ($status_2==503){echo "Service Unavailable";}
  else {echo "<a href='http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html'> more ...</a>";}
  echo "<td></tr>";
 }

if ($status_1==completed){ echo " <tr><td>Click here to Download </td><td><a href='$cache_url'>$file</a></td></tr/>";}
if ($status_1==failed && ($email==$user ||$email==$admin)){
  echo "<tr><td>If you want to retry this download, please click here </td> ";
  $server_name = $_SERVER['SERVER_NAME'];
  echo "<td><a href='http://$server_name/gadisk/action.php?action=ResendBassa&url=$url&size=$size'><img src='images/retry.jpg'></a></td></tr>";
 }

echo "</table>";

if (isset($email) && !empty($email)){
  include 'commentbox.php';
 }


echo "<h3>Comments</h3>";
$query="Select * from comments where file_id='$id'";
$result=mysql_query($query);

while($row = mysql_fetch_array($result)) {
  $email=$row['user_id'];
  $subject=$row['subject'];
  $comments=$row['comments'];
  $date=$row['date'];
  $result1=mysql_query("Select name from users where email='$email'");
  $name=mysql_result($result1,0,name);
	
  echo "$date<br/>";
  echo "User : $name<br/>";
  echo "Subject : $subject<br/>";
  echo "$comments<br/>";
  echo "<hr>";
 }

mysql_free_result($result);

include 'files/footer.php';
?>

