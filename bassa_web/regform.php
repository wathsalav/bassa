<?php
include 'lib/config.php';
include 'lib/opendb.php';
include 'lib/functions.php';
$passwd=generate_password();
include 'files/header.php';
include 'files/navigation.php';
?>

<h2 align="center">If you wish to use our offline downloader Please fill the form below</h2>
  <form  name="regform" action="regformsubmit.php" method="post" onSubmit="return checkEmail(this)">
  <table align="center">
  <tr>
  <th colspan=2>User Registration Form</th>
  </tr>
  <tr>
  <td>E-Mail address <font>*</font></td>
  <td><INPUT type="text" name="email" size="30"  ><br>(user name)</td>
  </tr>
  <tr>
  <td>Full Name <font>*</font></td>
  <td><INPUT type="text" name="name" size="30"><br></td>
  </tr>
  <tr>
  <?php
    <tr>
      <td><INPUT type='submit' name='submit' value='Submit Form' onclick='verify()'></td>
      <td><INPUT type='reset' name='reset'></td>
    </tr>";

echo '<INPUT type="hidden" name="passwd" size="15" value="<? echo $passwd?>">';
echo "</table>";
echo "</form>";
echo "<font>*</font> Your password will be sent to the E-mail address you provide. So please give correct E-mail ID.<br/>";
include 'files/footer.php';
?>


