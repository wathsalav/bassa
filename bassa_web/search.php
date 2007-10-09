<?php
include 'lib/config.php';
include 'lib/opendb.php';
require_once 'lib/functions.php';
include 'files/header.php';
$search=$_POST['search'];
$sort=$_POST['sort'];
include 'files/navigation.php';

echo "<h2>Search Results</h2>";

// Search function 
if ($search!=''){
searchbox($search,$sort);
}
else {
searchbox("",$sort);
}

mysql_close($conn);
include 'files/footer.php';
?>

