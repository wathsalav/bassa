<?php
include 'lib/config.php';
include 'lib/opendb.php';
require_once 'lib/functions.php';
include 'files/header.php';
include 'files/navigation.php';

$query=("select * from btv_items where item_id ='".$_GET['id']."'");
$result=mysql_query($query);
while($row = mysql_fetch_array($result)) {
  $title=$row['title'];
  $link=$row['link'];
  $date=$row['pub_date'];
  $description=$row['description'];
  $author=$row['author'];
  $enc_url=$row['enc_url'];
 }
$query="select * from downloads where url='$enc_url'";
$result=mysql_query($query);
$row = mysql_fetch_array($result);
$cache_url= $row['local_url'];
$id = $row['id'];

$query="select * from meta_info where origin_url='$enc_url' and mime_type='video/flv'";
$result=mysql_query($query);
while($row = mysql_fetch_array($result)){
  $meta_url=$row['meta_url'];  
 }

if (strstr($cache_url,'.flv')||strstr($cache_url,'.swf')){
  $burl = $cache_url;
 }
else if (strstr($meta_url, '.flv')){
  $burl = $meta_url;
 }

echo "<table>";
echo "<tr><td colspan='2' align='center'>";
?>
<?php
if ($burl)
echo '<object type="application/x-shockwave-flash" data="FlowPlayer.swf" width="400" height="330" id="FlowPlayer">
        <param name="allowScriptAccess" value="always" />
        <param name="movie" value="FlowPlayer.swf" />
        <param name="quality" value="high" />
        <param name="scaleMode" value="showAll" />
        <param name="allowfullscreen" value="true" />
        <param name="wmode" value="transparent" />
        <param name="allowNetworking" value="all" />
        <param name="flashvars" value="config={ videoFile: \''.$burl.'\'}" />
</object>';
echo "</td></tr>";	
echo "<tr><th colspan=2>File Details</th></tr>";
echo "<tr><td valign='top' bgcolor='#dfe1e7'><small><em><b>Title</b></em></small></td><td valign='top'>".$title."</td></tr>";
echo "<tr><td valign='top' bgcolor='#dfe1e7'><small><em><b>Author</b></em></small></td><td valign='top'>".$author."</td></tr>";
echo "<tr><td valign='top' bgcolor='#dfe1e7'><small><em><b>Description</b></em></small></td><td valign='top'>".$description."</td></tr>";
echo "<tr><td valign='top' bgcolor='#dfe1e7'><small><em><b>Published On</b></em></small></td><td valign='top'> $pub_date</td></tr>";
echo "<tr><td valign='top' bgcolor='#dfe1e7'><a href='".$link."'><small><em><b>More...<small><em><b></a></td></tr>";
echo "</table>";

if ($cache_url)
	echo "<a href='".$cache_url."'><img src='images/download_bar.png'/></a>";
else
	echo "<strong><em>This FIle Will Be Available Shortly...</em></strong>";

include 'commentbox.php';

echo "<h3>Comments</h3>";
$query="Select * from comments where file_id='$row[id]'";
$result=mysql_query($query);

        while($row = mysql_fetch_array($result)) {
	$email=$row['user_id'];
	$subject=$row['subject'];
	$comments=$row['comments'];
	$date=$row['date'];
	$result1=mysql_query("Select name from users where email='$email'");
	$name=mysql_result($result1,0,name);
	
	echo "$date<br/>";
	echo "User : $name<br/>";
	echo "Subject : $subject<br/>";
	echo "$comments<br/>";
	echo "<hr>";
	}

mysql_free_result($result);

include 'files/footer.php';
?>

