<?php
include 'lib/config.php';
include 'lib/opendb.php';
require_once 'lib/functions.php';
include 'files/header.php';
include 'files/navigation.php';

if(isset($_POST['email']) && isset($_POST['passwd']) && !empty($_POST['passwd']) ){
  $_SESSION['email']=$_POST['email'];
  $_SESSION['passwd']=$_POST['passwd'];
 }


if ($_SESSION['email']==$admin && $_SESSION['passwd']==$pass){
  echo "<head><meta http-equiv='refresh' content='0;URL=admin.php'></head>";
 }

 else{
   $query="SELECT email,password,name FROM users where email='".$_SESSION['email']."' AND password='".$_SESSION['passwd']."'";
   $result=mysql_query($query);
   $num=mysql_numrows($result);
   if ($num!=0){
     $email=$_SESSION['email'];
     $passwd=$_SESSION['passwd'];
     $_SESSION['name']=mysql_result($result,0,name);

     echo "<div class='right'>";
     echo "<p><h3><a href='pref.php'>User Preferences</a></h3>";
     echo "</div>";
     echo "<h2>Your Download page</h2>";

     if (isset($_SESSION['url']) && $_SESSION['url']!='' ){
       $url=$_SESSION['url'];
       $ip=$_SERVER['REMOTE_ADDR'];
       $file=file_name($url);
       $email=$_SESSION['email'];
       $size=$_SESSION['size'];
       insert_file($email,$file,$url,$ip,$size);
       echo "<head><meta http-equiv='refresh' content='3;URL=login.php'></head>";
     }
     else {
       echo "Welcome $_SESSION[name] !<br/>";
     }

     echo "<p>";

     /* Display downloads*/
     $userdownid="SELECT down_id FROM userdown where user_id='".$_SESSION['email']."'";
     $resultid = mysql_query($userdownid);
     echo '<form name="list"  method="post" action="action.php?action=Delete">';
     echo "<input type='hidden' name='num' value='$numid'>";
     echo "<table><tr><th>File Name</th><th>Size (MB)</th><th>Hits</th><th>Downloads</th><th>Info</th>";
     echo "<th><input name='delete' id='delete' type='submit' value='Delete'></th>";
     echo "</tr>";
     while ($row=mysql_fetch_row($resultid)) {
       $down_id=$row[0];
       $query="SELECT * FROM downloads where id=$down_id ORDER BY status DESC";
       $result = mysql_query($query) or die('Query failed: ' . mysql_error());
       // print data in to HTML table
       $downloads=list_downloads($result);
       echo $downloads;	
     }
     echo "</table>";
     echo "</form>";
     echo "<font>Please Logout when you leave!</font>";
     //login end
   }
   else {
     $_SESSION['email']='';
     $_SESSION['passwd']='';
     echo "<font>Incorrect E-mail / password</font><br/>";        
     echo "Please try Login again<br/>";
     include 'loginbox.php';
     echo "If you don't have an account please <a href='regform.php'>Signup!</a>";
   }      
 }

mysql_close($conn);
include 'files/footer.php';
?>

