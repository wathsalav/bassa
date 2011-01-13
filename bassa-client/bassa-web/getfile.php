<?php
require_once 'include/class.Cache.php';

$object_url=$_GET['object_url'];
$origin_url=$_GET['origin_url'];
$res=$cache->queueStatus($origin_url);

if($res == NULL)
{
  echo "<h1>File Not Available</h1>";
}
else
{
  if ($cache->updateHits($origin_url))
  {
    header("Content-Length: ".$res['content-length'][0]);
    header("Content-Disposition: attachment; filename=".$res['file'][0].";\r\n");
    $fd = fopen($object_url, 'r');
    while($line = fgets($fd, 1024))
    {
      echo $line;
    }
  }
}
?>

