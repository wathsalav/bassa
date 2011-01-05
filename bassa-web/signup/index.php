<?php
include("../include/class.Session.php");
if ($session->isAdmin() || !$session->logged_in){
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
        <title>Bassa</title>
        <link rel="stylesheet" type="text/css" href="../stylesheets/default.css"  />
        <link rel="stylesheet" type="text/css" href="../stylesheets/topmenu.css"  />
        <link rel="stylesheet" type="text/css" href="../stylesheets/table.css"  />        
		<link rel="stylesheet"  href="../stylesheets/button.css"  />
		
        <script type='text/javascript' src='../javascripts/jquery.js'></script>
        <script type='text/javascript' src='../javascripts/jquery.kwicks.js'></script>
        <script type='text/javascript' src='../javascripts/top_menu.js'></script>
        <script type="text/javascript" src="../javascripts/home_contents.js" ></script>        

        <link type="text/css" href="../themes/custom/ui.all.css" rel="stylesheet" />
        <script type="text/javascript" src="../javascripts/ui/ui.core.js"></script>
        <script type="text/javascript" src="../javascripts/ui/ui.tabs.js"></script>
        <script type="text/javascript">
            $(function() {
                $("#tabs").tabs();
            });
        </script>
    </head>

    <body>
        <div id="wrapper">
            <div id="header">
                <table>
                    <tr>
                        <th><div id="logo" align="left"><img src="../images/logo.png" height="125" width="125"/></div></th>
                        <td><div id="search">
                                <?php include ("../include/search.php"); ?>
                            </div>
                        </td>
                        <td><div id="login">
                               <a class="button_lo" href="../include/class.Process.php"><img src="../images/Play Blue Button.png"/>Login</a>
                            </div>
                        </td>
                    </tr>
                </table>
            </div>
            <div><?php include("../include/topmenu.php"); ?></div>
            <div id="content">
               <?php include ("signup.php");?>
            </div>
        </div>
            <div id="footer" align="center">
 <?php include '../include/footer.php'; ?>
            </div>        
    </body>
</html>

<?php
}  else {
    echo 'Please Log Out to create new account';
}
?>

