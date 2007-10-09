<?php
include 'lib/config.php';
include 'lib/opendb.php';
require_once 'lib/functions.php';
include 'files/header.php';
include 'files/navigation.php';

### variable declaration ###
$url= $_SESSION['url'];
$ip=$_SERVER['REMOTE_ADDR'];
$passwd= $_POST['passwd'];
$dept= $_POST['dept'];
$email= $_POST['email'];
$name= $_POST['name'];

/* Check user name*/
if (isset($email) && !empty($email) && !empty($passwd)){
$sqlusercheck="SELECT email FROM users where email='".$email."'";
$result=mysql_query($sqlusercheck);
$num=mysql_numrows($result);
if ($num!=0){
echo "Your Email address already exit!<br/>";
}
else{
mysql_query("INSERT INTO users (email,name,password,dept_id) VALUES('$email','$name','$passwd','$dept') ") or die(mysql_error());
/*Function call insert_file*/
$size=remotefsize($url);
insert_file($email,$file,$url,$ip,$size);
/*Function call send_mail*/
send_mail($email);
echo "<h2>Thank you!</h2>";
echo "<p>Please Login using your email address as user name and password.<br/>";
echo "Click here for ";
include 'loginbox.php';
}
}
else {
echo "Plese provide email address!";
}

mysql_close($conn);

include 'files/footer.php';
?>
