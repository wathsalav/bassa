<?php
echo "
<form name='loginform' action='login.php' method='post' onSubmit='return checkEmail(this)'>
<table>
    <tr>
        <th colspan=2> Login Form</th> 
    </tr>
    <tr>
      <td>E-Mail address</td>
      <td><INPUT type='text' name='email' size='15'  class='text'><br></td>
    </tr>
    <tr>
      <td>Password</td>
      <td><INPUT type='password' name='passwd' size='15' class='text'><br></td>
    </tr>
    <tr>
      <td><INPUT type='submit' name='submit' value='Login' ></td>
	<td><a href='sendpasswd.php'> Forget password ? </a></td>
    </tr>
</table>
</form>";
?>
