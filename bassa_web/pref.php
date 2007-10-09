<?php
include 'lib/config.php';
include 'lib/opendb.php';
include 'files/header.php';
include 'files/navigation.php';
include 'lib/functions.php';

if (isset($_SESSION['email']) && isset($_SESSION['passwd']) && $_SESSION['email']!='' && $_SESSION['passwd']!=''){
include 'changepasswd.php';
}

else {
echo "please login";
include 'loginbox.php';
}


include 'files/footer.php';
?>

