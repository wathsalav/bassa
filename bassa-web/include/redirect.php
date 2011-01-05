<?php
$size = 0;
$url = "";

if (isset($_GET['url'])) {
    $size = $_GET['size'];
    $url = $_GET['url'];
    $_SESSION['link'] = $url;
    $_SESSION['size'] = $size;
    echo"<h1>Login to add your download.</h1>";
} else {

}
?>
