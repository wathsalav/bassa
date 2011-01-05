<?php
require '../include/init_db.php';
init_db();
require '../include/class.Session.php';
if ($session->isAdmin()) {
?>
    <!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
    <html xmlns="http://www.w3.org/1999/xhtml">
        <head>
            <meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
            <title>Bassa</title>

            <link rel="stylesheet" type="text/css" href="../stylesheets/default.css"  />
            <link rel="stylesheet" type="text/css" href="../stylesheets/topmenu.css"  />
            <link rel="stylesheet" type="text/css" href="../themes/custom/ui.all.css"  />

            <script type="text/javascript" src="../javascripts/jquery.js"></script>
            <script type="text/javascript" src="../javascripts/jquery.kwicks.js"></script>
            <script type="text/javascript" src="../javascripts/ui/ui.core.js"></script>
            <script type="text/javascript" src="../javascripts/ui/ui.tabs.js"></script>
            <script type="text/javascript" src="../javascripts/top_menu.js"></script>

        </head>
        <body>
            <div id="wrapper">
                <div id="header">
                    <table>
                        <tr>
                            <th><div id="logo" align="left"><img src="../images/logo.png" height="125" width="125"/></div></th>
                            <td><div id="search"><?php include ("../include/search.php"); ?></div></td>
                            <td><div id="sing"><?php include("../include/sign.php"); ?></div></td>
                        </tr>
                    </table>
                </div>
                <div><?php include("../include/topmenu.php"); ?></div>
                <div id="content">
                    <?php include 'admin.php'; ?>
                </div>
            </div>          
            <div id="footer" align="center">
                <?php include '../include/footer.php'; ?>
            </div>
            </body>
        </html>
<?php
    } else {
        echo 'You have not administrotor privlage to see this page. Please Loging';
    }
?>
