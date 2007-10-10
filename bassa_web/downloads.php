<?php
include 'lib/config.php';
include 'lib/opendb.php';
include 'lib/functions.php';
include 'files/header.php';
include 'files/navigation.php';

echo "<h2>Downloads</h2>";
echo "<div class='right'> Page  $page</div>";
echo "<img src='images/download.gif' alt='Download'>Completed Downloads  ";
//    echo "<img src='images/downarrow1.png'> Download in Progress (1=10%) <br/>";
echo "<img src='images/iconerror.gif'> Download Failed <br/> ";
echo "If you unable to download a file, please do Right-click and Save Link As then Save file.<br/>";

// how many rows to show per page
$rowsPerPage = 20;

// by default we show first page
$pageNum = 1;

// if $_GET['page'] defined, use it as page number
if(isset($_GET['page']))
  {
    $pageNum = $_GET['page'];
  }

// counting the offset
$offset = ($pageNum - 1) * $rowsPerPage;

$query = " SELECT * FROM downloads ORDER BY status,hits DESC" .
  " LIMIT $offset, $rowsPerPage";
$result = mysql_query($query) or die('Error, query failed');

// print the random numbers
echo "<table><tr><th>File Name</th><th>Size (MB)</th><th>Hits</th><th>Download</th><th>Info</th>";
if(isset($_SESSION['email']) && !empty($_SESSION['email']) ){
  if ($_SESSION['email']!=$admin){
    echo '<form name="add"  method="post" action="action.php?action=Add">';
    echo "<th><input name='add'  type='submit' value='Add' /></th>";
  }
  if($_SESSION['email']==$admin){
    echo '<form  name="del" method="post" action="action.php?action=Delete">';
    echo "<th><input name='delete'  type='submit' value='Delete' /></th>";
  }
 }

$downloads=list_downloads($result);
echo $downloads;
//echo "</form>";
echo "</table>";

// how many rows we have in database
$query   = "SELECT COUNT(file) AS numrows FROM downloads";
$result  = mysql_query($query) or die('Error, query failed');
$row     = mysql_fetch_array($result, MYSQL_ASSOC);
$numrows = $row['numrows'];

// how many pages we have when using paging?
$maxPage = ceil($numrows/$rowsPerPage);

// print the link to access each page
$self = $_SERVER['PHP_SELF'];
$nav  = '';

//for($page = 1; $page <= $maxPage; $page++)
for($page = $pageNum-5; $page <= $pageNum+5; $page++)
  {
    //if ($page == $pageNum)
    if ($page>$maxPage || $page<=0)
      {
	$nav .= " * "; // no need to create a link to nonexisting page
      }
    else
      {
	$nav .= " <a href=\"$self?page=$page\">$page</a> ";
      }
  }

// creating previous and next link
// plus the link to go straight to
// the first and last page

if ($pageNum > 1)
  {
    $page  = $pageNum - 1;
    $prev  = " <a href=\"$self?page=$page\">[Prev]</a> ";

    $first = " <a href=\"$self?page=1\">[First Page]</a> ";
  }
 else
   {
     $prev  = '&nbsp;'; // we're on page one, don't print previous link
     $first = '&nbsp;'; // nor the first page link
   }

if ($pageNum < $maxPage)
  {
    $page = $pageNum + 1;
    $next = " <a href=\"$self?page=$page\">[Next]</a> ";

    $last = " <a href=\"$self?page=$maxPage\">[Last Page]</a> ";
  }
 else
   {
     $next = '&nbsp;'; // we're on the last page, don't print next link
     $last = '&nbsp;'; // nor the last page link
   }
// print the navigation link
echo $first . $prev . $nav . $next . $last;

// and close the database connection
include 'lib/closedb.php';
include 'files/footer.php';

?>

