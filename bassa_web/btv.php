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
$query="SELECT * FROM btv_channels";
$qresult=mysql_query($query);
echo "<br/>";
echo "<h3>FEATURED CHANNELS</h3>";
echo "<table>";
echo "<tr><th>Title</th><th>Description</th><th>Rating</th></tr>";
while ($row = mysql_fetch_assoc($qresult))
  {
    echo "<tr onclick='goto_items(\"btv_prog.php?cname=".base64_encode($row['link'])."&title=".base64_encode($row['title'])."\")';>";
    $title = "<br/><b><em><small>".strtoupper($row['title'])."<small></em></b>";
    if ($row['image_url'])
      echo "<td valign='top' width='20%' bgcolor='#dfe1e7'><img src='".$row['image_url']."'/>".$title."</td>";
    else
      echo "<td valign='top' width='20%' bgcolor='#dfe1e7'><img src='images/tv.png'/>".$title."</td>";
    echo "<td valign='top'>".$row['description']."</td>";
    if ($row['rating'])
      $rating = "<small><b>".strtoupper($row['rating'])."</b></small>";
    else
      $rating = "<small><b>UNSPECIFIED</b></small>";
    echo "<td valign='top' width='10%' bgcolor='#dfe1e7'><b>".$rating."</b></td>";
    echo "</tr>";
  }
echo "</table>";
include "files/footer.php";
include "lib/closedb.php";
?>

