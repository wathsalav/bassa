<script language='javascript'>
function goto_items($channel)
{
        location.href=$channel;
}
</script>

<?php
include 'lib/opendb.php';
include 'lib/functions.php';
include 'files/header.php';
include 'files/navigation.php'; 

$rows_per_page = 5;
$cname = $_GET['cname'];
$qtitle = $_GET['title'];
$page_cur = 1;
if (isset($_GET['page']))
   $page_cur = $_GET['page'];

$offset = ($page_cur - 1)*$rows_per_page;

$item_query="SELECT * FROM btv_items WHERE btv_items.ichannel='".base64_decode($cname)."' ORDER BY title DESC LIMIT ".$offset.",".$rows_per_page;

$item_stuff=mysql_query($item_query);
if (!item_stuff)
echo "Failed\n";
echo "<table>";
if ($qtitle)
echo "<tr><th colspan='4'>".strtoupper(base64_decode($qtitle))." [Videos]</th></tr>";
else
echo "<tr colspan='4'><th>[Videos]</th></tr>";
echo "<tr><th>Thumb</th><th>Title</th><th>Author</th><th>Category</th></tr>";
while ($row=mysql_fetch_assoc($item_stuff))
{
echo "<tr onclick='goto_items(\"btv_watch.php?id=".$row['item_id']."\")';>";
$image_query="SELECT * FROM meta_info WHERE mime_type LIKE 'image%' AND origin_url='".$row['enc_url']."'";
$is=mysql_query($image_query);
$r=mysql_fetch_assoc($is);
$thumb_img="";
$fstat_query="SELECT * FROM downloads WHERE url='".$row['enc_url']."'";
$fstat_result = mysql_query ($fstat_query);
$row2=mysql_fetch_assoc($fstat_result);
$stat=$row2['status'];
if($r['meta_url'])
	echo "<td width='80' height='60'><img width='80' height='60' align='top' src='".$r['meta_url']."'/></td>";
else
	if ($stat == 'queue')
		echo "<td class='nthumb' valign='center' width='80' height='60'><font class='nthumb'>Coming Soon...</font></td>";
	else if ($stat == 'failed')
		echo "<td class='fthumb' valign='center' width='80' height='60'><font class='nthumb'>Failed.</font></td>";
	else
		echo "<td class='xthumb' valign='center' width='80' height='60'><font class='nthumb'>No Link.</font></td>";
if ($row['title'])
	echo "<td valign='top' width='50%'><em>".$row['title']."</em></td>";
else
	echo "<td valign='top' width='50%'><em>NA</em></td>";
if ($row['author'])
	echo "<td valign='top' width='15%' bgcolor='#dfe1e7'><em>".$row['author']."</em></td>";
else
	echo "<td valign='top' width='15%' bgcolor='#dfe1e7'><em>NA</em></td>";
if ($row['category'])
	echo "<td valign='top' width='25%'><em>".$row['category']."</em></td>";
else
	echo "<td valign='top' width='25%'><em>NA</em></td>";
echo "</tr>";   
}
echo "</table>";

// how many rows we have in database
$navi_query   = "SELECT COUNT(title) AS numrows FROM btv_items WHERE btv_items.ichannel='".base64_decode($_GET['cname'])."'";
$navi_result  = mysql_query($navi_query) or die('Error, query failed');
$navi_row     = mysql_fetch_array($navi_result, MYSQL_ASSOC);
$numrows = $navi_row['numrows'];

// how many pages we have when using paging?
$max_page = ceil($numrows/$rows_per_page);

// print the link to access each page
$self = $_SERVER['PHP_SELF'];
$nav  = '';

//for($page = 1; $page <= $maxPage; $page++)
for($page = $page_cur-5; $page <= $page_cur+5; $page++)
{
   //if ($page == $pageNum)
   if ($page>$max_page || $page<=0)
   {
      $nav .= " * "; // no need to create a link to nonexisting page
   }
   else
   {
      $nav .= " <a href=\"$self?page=$page&cname=$cname&title=$qtitle\">$page</a> ";
   }
}


//for($page = 1; $page <
// creating previous and next link
// plus the link to go straight to
// the first and last page

if ($page_cur > 1)
{
   $page  = $page_cur - 1;
   $prev  = " <a href=\"$self?page=$page&cname=$cname&title=$qtitle\">[Prev]</a> ";

   $first = " <a href=\"$self?page=1&cname=$cname&title=$qtitle\">[First Page]</a> ";
}
else
{
   $prev  = '&nbsp;'; // we're on page one, don't print previous link
   $first = '&nbsp;'; // nor the first page link
}

if ($page_cur < $max_page)
{
   $page = $page_cur + 1;
   $next = " <a href=\"$self?page=$page&cname=$cname&title=$qtitle\">[Next]</a> ";
   $last = " <a href=\"$self?page=$max_page&cname=$cname&title=$qtitle\">[Last Page]</a> ";
}
else
{
   $next = '&nbsp;'; // we're on the last page, don't print next link
   $last = '&nbsp;'; // nor the last page link
}
// print the navigation link
echo $first . $prev . $nav . $next . $last;

include "lib/closedb.php";
include "files/footer.php";
?>

