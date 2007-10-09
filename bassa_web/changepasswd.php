<?php
echo "
<form name='changepasswd' action='action.php?action=ChangePassword' method='post'>
<table>
    <tr>
        <th colspan=2> Change Password Form</th> 
    </tr>
    <tr>
      <td>Old Password</td>
      <td><INPUT type='password' name='passwd' size='15'  class='text'><br></td>
    </tr>
    <tr>
      <td>New Password</td>
      <td><INPUT type='password' name='newpasswd' size='15' class='text'><br></td>
    </tr>
    <tr>
      <td colspan='2'><INPUT type='submit' value='Change Password'  ></td>
    </tr>
</table>
</form>
 ";

?>
