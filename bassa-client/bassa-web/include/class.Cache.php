<?php
//require_once 'class.FrontCache.php';
require_once $_SERVER['DOCUMENT_ROOT'].'/bassa/php/bassalib.php';

class Cache {

  var $fname;
  var $size;
  var $time;
  var $goAhead;
  var $userId;

  function addCache($url, $user_id) {
    add($url, "", "", $user_id);
  }

  function printAllDownloads($offset, $anchor) {
    $results = getAllDownloads($offset, 1);
    $this->printDownloads($results, $offset, $anchor);
  }
  
  function printUserDownloads($offset) {
    $front = new FrontCache();
    $results = getUserDownloads($front->getUserId(), $offset, 1);
    $this->printDownloads($results, $offset);
  }

  function printSearchResults($key_word, $offset) {
    $results = search($key_word, $offset);
    $this->printDownloads($results, $offset, '&search_item='.$key_word);
  }

  function printDownloads($results, $offset, $anchor=NULL)
  {
    if ($offset == NULL)
      $offset = 0;
    for ($i=0; $i<$results['total']; $i++)
    {
      echo "<tr>";
      echo "<td></td>";
      //echo '<td><img src="http://'.$_SERVER['SERVER_NAME'].':'.$_SERVER['SERVER_PORT'].'/bassa/images/category/'. $tagImage. '" border="0"/></td>';
      echo "<td>".$results['file'][$i]."</td>";
      echo "<td>".$this->calSize($results['content-length'][$i])."</td>";
      echo '<td><img src="http://'.$_SERVER['SERVER_NAME'].':'.$_SERVER['SERVER_PORT'].'/bassa/images/'.$this->detStatus($results['status'][$i]).'" border="0"/></td>';
      echo '<td align="right">' . $this->detDate($results['start-time'][$i]) . '</td>';
      echo '<td align="center"><a href="/bassa/getfile.php?object_url='.$results['local-url'][$i].'&origin_url='.$results['url'][$i].'"><img src="http://'.$_SERVER['SERVER_NAME'].':'.$_SERVER['SERVER_PORT'].'/bassa/images/download_now.png" border="0"/></a></td>';
      echo "</tr>";
    }
    if ($results['total'] == $results['object-limit'])
      $next_offset = $offset + $results['total'];
    else
      $next_offset = $offset;
    $prev_offset = $offset - $results['object-limit'];
    if ($prev_offset < 0)
      $prev_offset = 0;
    echo "<tr>";
    if ($anchor == NULL)
    {
      echo '<td align="center"><a href="'.$_SERVER['PHP_SELF'].'?offset='.$prev_offset.'">Prev</a></td>';
      echo "<td></td><td></td>";
      echo "<td></td><td></td>";
      echo '<td align="center"><a href="'.$_SERVER['PHP_SELF'].'?offset='.$next_offset.'">Next</a></td>';
    } 
    else
    {
      echo '<td align="center"><a href="'.$_SERVER['PHP_SELF'].'?offset='.$prev_offset.$anchor.'">Prev</a></td>';   
      echo "<td></td><td></td>";
      echo "<td></td><td></td>";
      echo '<td align="center"><a href="'.$_SERVER['PHP_SELF'].'?offset='.$next_offset.$anchor.'">Next</a></td>';   
    }
    echo "</tr>";

  }

  function queueStatus($url){
    $result = search($url);
    if ($result != NULL) {
      return $result;
    }
    else
      return NULL;
  }

  function calSize($size) {
    $newSize = $size;
    for ($i=0; $i<4 && $newSize>=1024; $i++)
    {
      $newSize = $newSize/1024;  
    }
    $newSize = round($newSize*100)/100;
    switch ($i)
    {
      case 0:
        $newSize = $newSize."B";
        break;
      case 1:
        $newSize = $newSize."KB";
        break;
      case 2:
        $newSize = $newSize."MB";
        break;
      case 3:
        $newSize = $newSize."GB";
        break;
    }
    return $newSize;
  }

  function detStatus($status) {
    $image;
    if ($status == 'C')
      $image = 'ok.png';
    else if($status == 'F')
      $image='dialog_error.png';
    else
      $image='sleep.png';
    return $image;
  }
  
  function detDate($date_time) {
    if ($date_time == '1970-01-01T00:00:00Z')
      return "Download in Queue";
    $dateTime = split(' ', $date_time);
    $date;
    if ($dateTime[0] == date("Y-m-d"))
      $date = "Today";
    else
      $date = $dateTime[0];
    return $date;
  }

}

$cache = new Cache();
?>
