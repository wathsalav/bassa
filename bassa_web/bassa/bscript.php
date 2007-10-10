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
  if ($tag == "CACHE_NOTIFICATION"){
    $doc_type = $tag;
  }
  else if ($tag == "META_INFO"){
    $doc_type = $tag;
  }
  if ($doc_type == "CACHE_NOTIFICATION"){
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
  else if ($doc_type == "META_INFO"){
    switch ($tag){
    case "FILE_NAME":
      $file_name = $data[$i]['value'];
      break;
    case "ORIGIN_URL":
      $origin_url = $data[$i]['value'];
      break;
    case "META_URL":
      $meta_url = $data[$i]['value'];
      break;
    case "MIME":
      $mime_type = $data[$i]['value'];
      break;
    }
  }
 }
if ($doc_type == "CACHE_NOTIFICATION"){
  //Echo collected data
  echo "File: $file_name,  Status: $file_status,  Cache URL: $cache_url, Origin URL: $origin_url, Size: $size<br/>";
  //Update database
  $update_result=mysql_query("UPDATE downloads SET local_url='$cache_url', status='$file_status' WHERE url='$origin_url'");
  if($update_result){
    echo "Database successfully updated.<br/>";
  }
  else {
    echo "Unable to update the database.<br/>";
  }
 }
 else if ($doc_type == "META_INFO"){
   //Echo collected data
   echo "File: $file_name, Meta URL: $meta_url, Origin URL: $origin_url, Mime-Type: $mime_type<br/>";
   $insert_meta = mysql_query("INSERT INTO meta_info (file_name, origin_url, meta_url, mime_type) VALUES('$file_name','$origin_url','$meta_url','$mime_type')");
   if ($insert_meta){
     echo "Database updated.<br/>";
   }
   else{
     echo "\n".mysql_error()."\n";
     echo "Unable to updatebase.<br/>";
   }
 }
mysql_close($conn);
?>
