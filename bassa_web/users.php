<?php
include 'files/header.php';
include 'files/navigation.php';
include 'lib/config.php';
include 'lib/opendb.php';
include 'lib/functions.php';

if($_SESSION['email']==$admin && $_SESSION['passwd']==$pass){
  $query=("SELECT * from users");
  $result=mysql_query($query) or die (mysql_error());
  $num=mysql_numrows($result);
  echo "We have $num users!";
  echo "<table>";
  echo "<th>Name</th><th>E-Mail</th><th>Department</th>";
  echo "<th><input name='delete'  type='submit' value='Delete'></th>";
  while ($row=mysql_fetch_array($result)){
    extract($row);
    echo "<tr>";
    echo "<td>$name</td>";
    echo '<form  method="post" action="action.php?action=UserList">';
    echo "<td><input type='submit' name='email' value='$email' class='submit'></td>";
    echo "</form>";
    echo "<td>$dept_id</td>";
    echo '<form  method="post" action="action.php?action=UserDelete">';
    echo "<td><input name='radio' type='radio'  value='$email'></td>";
    echo "</form>";
    echo "</tr>";
  }
  echo "</table>";
 }
 else {
   echo "<font>Access Denied!</font><br/>";
   echo "Only administrator is allowed to view this page.";
 }

include 'files/footer.php';

?>
