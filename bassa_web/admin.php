<?php
#################################
### File  : admin.php         ###
### Author: Gowri Ananthan    ###
### Email : gna@ucsc.cmb.ac.lk###
### Licence: BSD              ###
#################################
include 'files/header.php';
include 'files/navigation.php';
include 'lib/config.php';
include 'lib/opendb.php';
include 'lib/functions.php';

if($_SESSION['email']==$admin && $_SESSION['passwd']==$pass){
  echo "<h2>Welcome to Admin page</h2>";
  echo "<p>";
  echo "<a href='users.php'>Display Users</a><br/>";
  echo '<a href="action.php?action=ResendBassa">Resend Queued Requests</a><br/>';
  echo '<a href="action.php?action=Bfile">Check Bassa Files status</a><br/>';

  echo '<form  name="query" method="post" action="action.php?action=querydb">';
  echo "<input name='querybox'  type='text' size='50' value='select * from downloads where '/>";
  echo "<input name='query'  type='submit' value='Query DB' />";
  echo "</form>";


 }
 else {
   echo "<font>Access Denied!</font><br/>";
   echo "Only administrator is allowed to view this page.";
 }


include 'files/footer.php';

?>
