<link rel="stylesheet"  href="stylesheets/button.css"  />
<script type="text/javascript" src="javascripts/button.js"></script>
<form action="include/class.Process.php" method="post">
    <table width="300">
        <?php
        if ($session->isAdmin()) {
            echo 'Welcome Administrator <b>' . $session->username . '</b>, you are logged in.<br/><br/>';
            echo '<a class="button_lo" href="include/class.Process.php"><img src="images/Eject Blue Button.png"/>Logout</a>';
            echo '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;';
            echo '<a class="button_su" href="signup"><img src="images/User.png"/>Create Admin</a>';
        } else if ($session->logged_in) {
            echo 'Welcome <b>' . $session->username . '</b>, you are logged in.<br/><br/>';
            echo '<a class="button_lo" href="include/class.Process.php"><img src="images/Eject Blue Button.png"/>Logout</a>';
            echo '<br/><br/>';
        } else {
        ?>
            <tr >
                <td width="50">Username</td>
                <td width="100"><input name="user" maxlength="30" type="text" value="<?php echo $form->value("user");
            $form->error("user"); ?>" size="10"/></td>
            <td width="50">Password</td>
            <td width="100"><input name="pass" maxlength="30" type="password" value="<?php echo $form->value("pass");
                                   $form->error("pass"); ?>"size="10"/></td>
        </tr>
    </table>
    <table width="300">
        <tr>
        <input type="hidden" name="sublogin" value="1">
        <td width="50"><input name="Login" type="Submit" value="Login"/></td>
        <td width="100"><a href="">[Forgot Password?]</a></td>
        <td width="75"><a class="button_su" href="signup"><img src="images/User.png"/>Sing Up</a></td>
        <?php
               if ($form->num_errors > 0) {
                   echo "<td width=\"80\"><font size=\"2\" color=\"#ff0000\">Cannot Login</font></td>";
               }
           }
        ?>
        </tr>
    </table>
</form>
