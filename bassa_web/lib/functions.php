<?php
#################################
### File  : functions.php     ###
### Author: Gowri Ananthan    ###
### Email : gna@ucsc.cmb.ac.lk###
### Licence: BSD              ###
#################################

############### Extract file name from URL##################
function file_name($url){
  $parts = explode("/", "$url");
  $file= ($parts[count($parts)-1] == "") ? $parts[count($parts)-2] : $parts[count($parts)-1];
  $b_file= str_replace('%20', ' ', $file);
  $filename_a = array_shift(explode('?', basename($b_file)));
  $filename_b=explode('&',$filename_a);
  $filename=basename($filename_b[0]);
  return $filename;
  }

############### Search a file in database ######################
function search_file($url,$size){
  include 'lib/config.php';
  $file=file_name($url);

  //check is it local URL
  if(strstr($url,'$ftp_host')){
    echo "Please exclude $bhost in your proxy setting.";
    $url='';
    $_SESSION['url']='';
  }
  else{

    $sqldowncheck=mysql_query("SELECT * FROM downloads where file='".$file."' ");

    $numdown=mysql_numrows($sqldowncheck);
    // If file exits
    if ($numdown!=0){
      $my_size=mysql_result($sqldowncheck,0,'size');
      $my_url=mysql_result($sqldowncheck,0,'url');
      $my_status=mysql_result($sqldowncheck,0,'status');
      $my_count=mysql_result($sqldowncheck,0,'hits');

      $parts = explode(" ", "$my_status");
      $my_status=$parts[0];

      if($my_size==$size || $my_url==$url || $size>2000000){
	$my_count+=1;
	mysql_query("UPDATE downloads SET hits='$my_count' where file='$file'");

	// check status
	if ($my_status==completed){
	  // If status completed automatically downlaod.
	  echo "<font>File($file) Found </font><br/>";
	  echo "<head><meta http-equiv='refresh' content='0;URL=$ftp_host/$file'></head>";
	  $_SESSION['url']='';
	}
	else if($my_status==(queue||pending)){ 
	  echo "<font>File on Queue. </font><br/>";
	  echo "$file will be downloaded soon.<br/>";
	}
	else if($my_status==failed){ 
	  echo "File($file) download failed!";
	}
      }
      else {
	$down=list_downloads($sqldowncheck);
	echo $down;
	echo "If the file is not listed above,<br/>";
	echo "Please Login to get the requested file($file). <br/>";
      }
    }

    // If the file not in the database
    else {
      echo "<font>File Not Found!</font><br/>";
      echo "Please Login to get the requested file($file). <br/>";
    }
  }      
}

########### Search BOX ##########################################################
function searchbox($file,$sort){
  if ($sort=='file'){ $order='';}
  else {$order='DESC';}

  $checksimilar=mysql_query("SELECT * FROM downloads where file LIKE '%$file%' ORDER BY '$sort' $order ");
  $numsimilar=mysql_numrows($checksimilar);
  if($numsimilar!=0){
    echo "<h3>There are $numsimilar file(s) Listed here.</h3><br/>";
    echo "<table>";
    echo '<th>File Name</th>';
    echo "<th>Size</th><th>Hits</th><th>Download</th><th>Info</th>";
    if(isset($_SESSION['email']) && !empty($_SESSION['email'])){
      if ($_SESSION['email']==$admin){
	echo '<form name="del"  method="post" action="action.php?action=Delete">';
	echo "<th>User</th><th><input name='del'  type='submit' value='Del'></th></tr>";
      }
      else{
	echo '<form name="add"  method="post" action="action.php?action=Add">';
	echo "<th><input name='add'  type='submit' value='Add'></th></tr>";
      }
    }
    $status=list_downloads($checksimilar);
    echo $status;
    echo "</table>";
    echo "</form>";
  }
}

########### Insert file in to database & send Bassa download request ############
function insert_file($email,$file,$url,$ip,$size){
  include 'lib/config.php';
  /*Open socket*/
  /*File check*/
  if ($file!=''){
    if ($url!=''){
      $date=date('d-m-Y h:s');

      $b_url= str_replace('%20', ' ', $url);
      $sqldowncheck=mysql_query("SELECT file FROM downloads where file='".$file."' && size='".$size."'");
      $numdown=mysql_numrows($sqldowncheck);
      if ($numdown==0){
	$fp=fsockopen($bhost,$bport);
	if($fp) {
	  $xml_details="<bassa><offline_download_query><url>$b_url</url><size>$size</size><name>$guser</name><email>$gemail</email></offline_download_query></bassa>";
	  fputs($fp,$xml_details);
	  echo "Your request had been send to BASSA for download.<br/>";
	  mysql_query("INSERT INTO downloads (file,url,email,size,down_date,ip) VALUES('$file','$b_url','$email','$size','$date','$ip')") or die(mysql_error());
	}
	else {
	  mysql_query("INSERT INTO downloads (file,url,email,size,status) VALUES('$file','$b_url','$email','$size','pending')") or die(mysql_error());
	  echo "The Download request pending.<br/>";
	}
	$_SESSION['size']='';
      }
    }
    /*add file in to user account*/
    $sqluserdowncheck=mysql_query("SELECT user_id FROM userdown where down_id='".$file."' and user_id='".$email."'");
    $numuserdown=mysql_numrows($sqluserdowncheck);
    if($numuserdown==0){
      mysql_query("INSERT INTO userdown (user_id,down_id) VALUES('$email','$file')") or die (mysql_error());
      echo "$file had been added into your account<br/>";
    }
    else{
      echo "File already exist in your account.<br>";
    }
    $_SESSION['url']='';
  }
}
###################### List Files ########################
function list_downloads($result){
  include 'lib/config.php';

  $num=mysql_numrows($result);
  $i=0;
  while ($i<$num) { 
    $data.="<tr>";
    $my_id=mysql_result($result,$i,id);
    $my_file=mysql_result($result,$i,file);
    $my_url=mysql_result($result,$i,url);
    $my_status=mysql_result($result,$i,status);
    $my_hits=mysql_result($result,$i,hits);
    $my_size=mysql_result($result,$i,size);
    $my_email=mysql_result($result,$i,email);

    //size in Meg 
    $size=round(($my_size/1048576),2);
    //file name formating
    $file= str_replace('%20', ' ', $my_file);
    $file= str_replace('?', '', $file);

    //seperating status and code
    $parts = explode(" ", "$my_status");
    $my_status=$parts[0];
    $per=$parts[1];
    //Displaying data
    $data.="<td>$file</td><td>$size</td><td>$my_hits</td>";

    if ($my_status==completed){
      $data.="<td><a href='$ftp_host/$my_file'><img src='images/download.gif'></a></td>";	
    }
    if($my_status==queue){
      $data.="<td>On Queue</td>";
    }
    if($my_status==pending){
      $data.="<td>Pending</td>";	
    }
    if($my_status==failed){
      $data.="<td><img src='images/iconerror.gif' title='Error Code : $per'></td>";	
    }
    if($my_status==''){
      $data.="<td>File Not Found</td>";	
    }

    //list more information
    $data.="<td><a href='http://cache.cmb.ac.lk/gadisk/info.php?id=$my_id'><img src='images/info.png'></a></td>";	

    //Displaying checkbox
    if(isset($_SESSION['email']) && !empty($_SESSION['email'])){
      if($_SESSION['email']==$admin){
      }
      $data.="<td><input name='check[]' type='checkbox'  value='$file' /></td>";
    }

    $data.="</tr>";
    $i++;                        
  }
  return $data;
}

############## Mail password to user #####################################
function send_mail($email){
  include 'lib/config.php';
  $from="From : $gemail";
  $subject="GAD Notification";
  $mailresult=mysql_query("SELECT password FROM users where email='".$email."'");
  $mailnum=mysql_numrows($mailresult);

  if ($email!=''){
    if ($mailnum!=0){
      $pass=generate_password();
      $upmail=mysql_query("UPDATE users set password='".$pass."' where email='".$email."'");
	
      $body="Dear User!,";
      $body.="Your password is : $pass .";
      $body.="Please change Password after Login.";
      $body.="Thank you for using GAD-Bassa";
      mail( $email, $subject ,$body, $from );
      echo "Your password has been sent to $email<br/>";
      echo "Thank you for using <font>GADisk!</font><br/>";
      mysql_free_result($mailresult);
    }
    else {
      echo "E-mail not exist in our system!";
    }
  }
  else{
    echo "Please enter your email address";
  }
}

############## Generate Password ######################
function  generate_password($length = 6)
{
  // start with a blank password
  $password = "";
  // define possible characters
  $possible = "0123456789bcdfghjkmnpqrstvwxyz"; 
  // set up a counter
  $i = 0; 
  // add random characters to $password until $length is reached
  while ($i < $length) { 
    // pick a random character from the possible ones
    $char = substr($possible, mt_rand(0, strlen($possible)-1), 1);
    // we don't want this character if it's already in the password
    if (!strstr($password, $char)) { 
      $password .= $char;
      $i++;
    }
  }
  return $password;
}
        
################## GET File size #########################
function remotefsize($url) {
  $sch = parse_url($url, PHP_URL_SCHEME);
  if (($sch != "http") && ($sch != "https") && ($sch != "ftp") && ($sch != "ftps")) {
    return false;
  }
  if (($sch == "http") || ($sch == "https")) {
    $headers = get_headers($url, 1);
    if ((!array_key_exists("Content-Length", $headers))) { return false; }
    return $headers["Content-Length"];
  }
  if (($sch == "ftp") || ($sch == "ftps")) {
    $server = parse_url($url, PHP_URL_HOST);
    $port = parse_url($url, PHP_URL_PORT);
    $path = parse_url($url, PHP_URL_PATH);
    $user = parse_url($url, PHP_URL_USER);
    $pass = parse_url($url, PHP_URL_PASS);
    if ((!$server) || (!$path)) { return false; }
    if (!$port) { $port = 21; }
    if (!$user) { $user = "anonymous"; }
    if (!$pass) { $pass = "anon@localhost"; }
    switch ($sch) {
    case "ftp":
      $ftpid = ftp_connect($server,$port);
      break;
    case "ftps":
      $ftpid = ftp_ssl_connect($server, $port);
      break;
    }
    if (!$ftpid) { return false; echo "unable to connect"; }
    $login = ftp_login($ftpid, $user, $pass);
    if (!$login) { return false; }
    $ftpsize = ftp_size($ftpid, $path);
    ftp_close($ftpid);
    if ($ftpsize == -1) { return false; }
    return $ftpsize;
  }
}
################## Multiple Bassa file search ################################
function download_search($file,$size){
  include 'lib/config.php';
  $b_file= str_replace(' ', '%20', $file);
  $burl="$ftp_host/$b_file";
  $bsize=remotefsize($burl);
  //	$bfile=file_name($burl);
  if ($size=$bsize || (($size='0' || $size='2147483648') && $bsize>='5242880')){
    mysql_query("UPDATE downloads set status='completed' where file='$file'");
    echo "$file staus updated <br/>";
  }
  else {
    mysql_query("UPDATE downloads set status='queue' where file='$file'");
    echo "$file not found <br/>";
  }
}

################## Update file status & Resend the file#####################################

function update_status($url,$size,$email){
  $file=file_name($url);
  include 'lib/config.php';
  if ($email==$admin){
    $result=mysql_query("UPDATE downloads SET status='queue' where file='$file' && size='$size'");
  }
  else{
    $result=mysql_query("UPDATE downloads SET status='queue' where file='$file' && size='$size' && email='$email'");
  }
  if ($result){
    $fp=fsockopen($bhost,$bport);
    if($fp){
      $xml_details="<bassa><offline_download_query><url>$url</url><size>$size</size><name>$guser</name><email>$gemail</email></offline_download_query></bassa>";
      fputs($fp,$xml_details);
      echo "$file resent to bassa.<br/>";
      echo "Status successfully updated.<br/>";
      fclose($fp);
    }       
    else{
      echo "Unable to send the file to Bassa.";
    }

    echo "<head><meta http-equiv='refresh' content='2;URL=login.php'></head>";
  }
  else {
    echo "Unable to update the database.<br/>";
  }


}

############################## END ##############################
?>
