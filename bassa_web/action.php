<?php
#################################
### File  : action.php        ###
### Author: Gowri Ananthan    ###
### Email : gna@ucsc.cmb.ac.lk###
### Licence: BSD              ###
#################################

include 'lib/config.php';
include 'lib/opendb.php';
include 'lib/functions.php';
include 'files/header.php';
include 'files/navigation.php';

if(isset ($_REQUEST['action'])){
  switch ($_REQUEST['action']){
	
  case 'Logout':
    $_SESSION['email']='';
    $_SESSION['passwd']='';
    session_destroy();
    echo "You Successfully Logout!<br/>";
    echo "Please visit to http://cache.cmb.ac.lk/gadisk/ to check downloads!<br/>";
    echo "Thank you for using GAD-BASSA!";
    echo "<head><meta http-equiv='refresh' content='3;URL=index.php'></head>";
    break;

  case 'ChangePassword':
    $email=$_SESSION['email'];
    $passwd=$_POST['passwd'];
    $newpasswd=$_POST['newpasswd'];
    $query="SELECT email,password FROM users where email='".$email."' AND password='".$passwd."'";
    $result=mysql_query($query);
    $num=mysql_numrows($result);
    if ($num!=0){
      mysql_query("UPDATE users SET password='$newpasswd' WHERE email='$email'");
      $_SESSION['passwd']=$newpasswd;
      echo "Your password has been changed Successfully!";
      echo "<head><meta http-equiv='refresh' content='2;URL=login.php'></head>";
    }
    else {
      echo "Password Incorrect!";
      echo "<head><meta http-equiv='refresh' content='3;URL=login.php'></head>";
    }
    break;		
	
  case 'Delete':
    $email=$_SESSION['email'];
    $del=$_POST['check'];
    echo 'del is'.$del;
    $c=count($del);
    $i=0;
    while($i<$c){
      $del_id=$del[$i];
      if ($email==$admin){
	echo "Admin Deleting..... file $del_id from Downloads.<br>";
	$sql = ("DELETE FROM downloads WHERE file='$del_id'") or die (mysql_error());
	$sql1 = ("DELETE FROM userdown WHERE down_id='$del_id'") or die (mysql_error());
      }
      if($email!=$admin){
	echo "Deleting..... file $del_id from your account<br>";
	$sql = ("DELETE FROM userdown WHERE down_id='".$del_id."' && user_id='".$email."'") or die (mysql_error());
      }
      $result=mysql_query($sql);
      $result1=mysql_query($sql1);
      if($result){
	echo "$del_id had been deleted from downloads<br/>";
      }
      if($result1){
	echo "$del_id had been deleted users.<br/>";
      }
      $i++;
    }
    echo "<meta http-equiv=\"refresh\" content=\"1;URL=login.php\">";

    break;
	
  case 'Add':
    $email=$_SESSION['email'];
    $add=$_POST['check'];
    if (isset($add) && $add!=''){
      $c=count($add);
      $i=0;
      while($i<$c){
	$add_id=$add[$i];
	echo "Adding..... $add_id to your account.<br>";
	insert_file($email,$add_id);
	$i++;
      }
      echo "<meta http-equiv=\"refresh\" content=\"2;URL=login.php\">";
    }
    else {
      echo "Nothing to add";
    }

    break;

  case 'UserDelete':
			
    $email=$_SESSION['email'];
    $del_id=$_POST['radio'];
    if ($email==$admin){
      $sql = ("DELETE FROM users WHERE email='$del_id'") or die (mysql_error());
      $result = mysql_query($sql) or die (mysql_error());
    }
    else{
      echo "Access Denied!";
    }
    if($result){
      echo "User $email had been deleted ";
      echo "<meta http-equiv=\"refresh\" content=\"2;URL=admin.php\">";
    }
    break;
	
  case 'UserList':
    $email=$_SESSION['email'];
    if ($email==$admin){
      $uemail=$_POST['email'];
      $result=mysql_query("Select * from downloads where email='$uemail'");
      $downloads=list_downloads($result);
      echo "<table>";
      echo "<table><tr><th>File Name</th><th>Size (MB)</th><th>Hits</th><th>Last Accessed</th><th>Downloads</th>";
      //echo '<form name="add"  method="post" action="action.php?action=Delete">';
      //echo "<th><input name='delete'  type='submit' value='Delete'></th>";
      echo $downloads;
      //echo "</form>";
      echo "</table>";
    }
    else{
      echo "Access Denied!";
    }

    break;
	
  case 'ResendBassa':       
    $email=$_SESSION['email'];
    if (isset($email)){
      if ($email==$admin){
	$result=mysql_query("select url,size,file from downloads where status='queue' or status='pending'");   
	$num=mysql_numrows($result);
	if($num!=0){
	  $i=0; 	
	  while ($i<$num){  
	    $size=mysql_result($result,$i,size);
	    $url=mysql_result($result,$i,url);
	    update_status($url,$size,$email);
	    $i++;
	  }	
	  echo "$num Request had been sent to bassa";
	}
			
	else {echo "There is nothing to send";}
      }
      else{
	$size=$_GET['size'];
	$url=$_GET['url'];
	update_status($url,$size,$email);
      }
    }
    break;
                

  case 'Bfile':
    $email=$_SESSION['email'];
    if ($email==$admin){
      // For Bassa testing
      $query=("select size,file from downloads where status='queue'");
      // For GADisk testing
      //$query=("select size,file from downloads");
      $result=mysql_query($query);
      $num=mysql_numrows($result);
      if($num!=0){
	$i=0;
	while ($i<$num){
	  $size=mysql_result($result,$i,size);
	  $file=mysql_result($result,$i,file);		
	  download_search($file,$size);
	  $i++;
	}
	echo "$num files had beed send.";
      }
      else {echo "There is nothing to send";}
    }
    else{
      echo "Access Denied!";
    }
    break;

  case 'querydb':
    if (isset($_GET['query']) && $_GET['query']=file){
      $query="select * from downloads order by file";
    }
    if (isset($_POST['querybox'])){
      $query=$_POST['querybox'];
    }
    $result = mysql_query($query) or die('Error, query failed');
    $num=mysql_numrows($result);
    if ($num!=0){
      echo "<table><tr><th>File Name</th><th>Size (MB)</th><th>Hits</th><th>Last Accessed</th><th>Download</th>";
      if($_SESSION['email']==$admin && $_SESSION['passwd']==$pass){
	echo '<form  name="del" method="post" action="action.php?action=Delete">';
	echo "<th>User</th><th><input name='delete'  type='submit' value='Delete' /></th>";
      }
      else {
	echo "Search results!";
      }

      $downloads=list_downloads($result);
      echo $downloads;
      echo "</table>";
    }
    else{
      echo "Nothing to list.";
    }
    break;
	
  case 'addcomment':
    $email=$_SESSION['email'];
    $subject=$_POST['subject'];
    $comment=$_POST['comment'];
    $file_id=$_GET['id'];
    echo $email."<br/>";
    if(!empty($subject) && !empty($comment)){
      $result=mysql_query("INSERT INTO comments (user_id,file_id,subject,comments) VALUES('$email','$file_id','$subject','$comment')") or die(mysql_error());
      if($result){echo "Your comment updated successfully";}
      else {echo "There is some problem occuered.";}
      echo "<meta http-equiv=\"refresh\" content=\"1;URL=info.php?id=$file_id\">";
    }
    else {
      echo "Subject & comment field should not be empty.";
    }
  Default:
    echo "Please wait ....."; 
    echo "<meta http-equiv=\"refresh\" content=\"3;URL=login.php\">";
    $_SESSION['url']='';
    //switch end
	
  }		

  include 'files/footer.php';
 }
?>
