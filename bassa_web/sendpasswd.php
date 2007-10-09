<?php
include 'lib/config.php';
include 'lib/opendb.php';
require_once 'lib/functions.php';
include 'files/header.php';
include 'files/navigation.php';
echo "<h2>Password Reminder</h2>";

echo "<form name='reminder' action='sendpasswd.php' method='post' onSubmit='return checkEmail(this)'>
	You Email Address : <input type='text' name='remail'>
	<input type='submit' value='Send Password' class='submit'>
	</form>";

$email= $_POST['remail'] ;
send_mail($email);

mysql_close($conn);
include 'files/footer.php';
?>
