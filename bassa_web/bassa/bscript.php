<?php
include '../lib/config.php';
include '../lib/opendb.php';

//create new xml parser 
$xmlparser = xml_parser_create();

// Get XML DATA from bassa
$xmldata=$_POST['bassa_form'];
// put xml data into an array
xml_parse_into_struct($xmlparser,$xmldata,$data);
//free xml parser
xml_parser_free($xmlparser);
//Print xml array
//print_r($data);
//count number of elements in array
$num_array=count($data);

//Extract elements from array
for($i=1;$i<$num_array;$i++){
  $tag=$data[$i]['tag'];
  //check xml tag
  switch ($tag){
  case "FILE_NAME":
    $file_name=$data[$i]['value'];
    break; 
  case "STATUS":
    $file_status=$data[$i]['value'];
    break;
  case "CACHE_URL":
    $cache_url=$data[$i]['value'];
    break;
  case "ORIGIN_URL":
    $origin_url=$data[$i]['value'];
    break;
  case "SERVER_RESPONSE":
    $server_response=$data[$i]['value'];
    break;
  case "SIZE":
    $size=$data[$i]['value'];
    break;
  }
 }

//Echo collected data
echo "File: $file_name,  Status: $file_status,  Cache URL: $cache_url, Origin URL: $origin_url, Size: $size<br/>";

//verify whether the file exits
$result=mysql_query("SELECT * from downloads where file='$file_name'");
$num=mysql_num_rows($result);
if($num!=0){

  //Update database
  $update_result=mysql_query("UPDATE downloads SET status='$file_status' WHERE file='$file_name'");
  if($update_result){
    echo "Database successfully updated.<br/>";
  }
  else {
    echo "Unable to update the database.<br/>";
  }
 }
 else {
   echo "There is no such file($file_name) in the database.<br/>";
 }
mysql_close($conn);
?>
