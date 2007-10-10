<?php
include '../lib/config.php';
include '../lib/opendb.php';
include '../lib/functions.php';

//Declare and initialize global vars
$email = "btv@somewhat.org";
$ip = $_SERVER['REMOTE_ADDR'];
//create new xml parser 
$xmlparser = xml_parser_create();

// Get XML DATA from bassa
$xmldata=$_POST['bassa_form'];

//echo $xmldata;

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
  if ($tag == "NOTIFICATION"){
    $doc_type = $tag;
  }
  if ($tag == "CHANNEL")
    $subdoc_type = $tag;
  if ($tag == "ITEM")
    $subdoc_type = $tag;
  //check xml tag
  if ($subdoc_type == "CHANNEL"){
    switch ($tag){
    case "TITLE":
      $chan_title=$data[$i]['value'];
      break; 
    case "DESCRIPTION":
      $chan_desc=$data[$i]['value'];
      break;
    case "LINK":
      $chan_link=$data[$i]['value'];
      break;
    case "RATING":
      $rating=$data[$i]['value'];
      break;
    case "PUB_DATE":
      $cpubdate=$data[$i]['value'];
      break;
    case "IMAGE_TITLE":
      $image_title=$data[$i]['value'];
      break;
    case "IMAGE_URL":
      $image_url=$data[$i]['value'];
    }
  }
  else if ($subdoc_type == "ITEM"){
    //echo ">>>>>>>>".$tag."<<<<<<<<\n";
    //echo ">>>>>>>>".$data[$i]['value']."<<<<<<<<\n";
    switch ($tag){
    case "TITLE":
      $ititle = $data[$i]['value'];
      break;
    case "LINK":
      $ilink = $data[$i]['value'];
      break;
    case "AUTHOR":
      $author = $data[$i]['value'];
      break;
    case "DESCRIPTION":
      $idesc = $data[$i]['value'];
      break;
    case "CATEGORY":
      $category = $data[$i]['value'];
      break;
    case "COMMENTS":
      $comment = $data[$i]['value'];
      break;
    case "ENCLOSURE_TYPE":
      $enc_type = $data[$i]['value'];
      break;
    case "ENCLOSURE_URL":
      $enc_url = $data[$i]['value'];
      break;
    case "ENCLOSURE_LENGTH":
      $enc_len = $data[$i]['value'];
      break;
    case "GUID":
      $guid = $data[$i]['value'];
      break;
    case "PUB_DATE":
      $ipubdate = $data[$i]['value'];
      break;
    case "SOURCE":
      $source = $data[$i]['value'];
      break;
    }
  }
 }
/*
 echo "***********************************************\n";
 echo ">>>>>>>>>>>Channel<<<<<<<<<<\n";
 echo "Chan Title: ".$chan_title."\n";
 echo "Chan Desc: ".$chan_desc."\n";
 echo "Chan Link: ".$chan_link."\n";
 echo "Chan Rating: ".$rating."\n";
 echo "Image Title: ".$image_title."\n";
 echo "Image URL: ".$image_url."\n";
 echo ">>>>>>>>>>>Item<<<<<<<<<<\n";
 echo "Item Title: ".$ititle."\n";
 echo "Item Link: ".$ilink."\n";
 echo "Item Author: ".$author."\n";
 echo "Item Description: ".$idesc."\n";
 echo "Item Category: ".$category."\n";
 echo "Item Comments: ".$comments."\n";
 echo "Item EncTyp: ".$enc_type."\n";
 echo "Item EncURL: ".$enc_url."\n";
 echo "Item EncLen: ".$enc_len."\n";
 echo "Item GUID: ".$guid."\n";
 echo "Item PubData: ".$ipubdate."\n";
 echo "Item Source: ".$source."\n";
*/

if ($doc_type == "NOTIFICATION"){
  $cdesc_escaped = mysql_real_escape_string ($chan_desc);
  $insert_channel = "INSERT INTO btv_channels (title, description, link, rating, pub_date, image_title, image_url) VALUES (\"$chan_title\", \"$cdesc_escaped\", \"$chan_link\", \"$rating\", \"$cpubdate\",\"$image_title\", \"$image_url\")";
  $update_channel = "UPDATE btv_channels SET title=\"$chan_title\", description=\"$cdesc_escaped\", rating=\"$rating\", pub_date=\"$cpubdate\", image_title=\"$image_title\", image_url=\"$image_url\" WHERE link=\"$link\"";
  $chan_stat = mysql_query($insert_channel);
  if ($chan_stat)
    echo "Channel Added\n";
  else
    $chan_stat = mysql_query($update_channel);
  if ($chan_stat)
    echo "Channel Updated\n";
  $idesc_escaped = mysql_real_escape_string ($idesc);
  $icomment_escaped = mysql_real_escape_string ($comments);
  $insert_item = "INSERT INTO btv_items (title, link, author, description, category, comments, enc_type, enc_url, guid, pub_date, source, ichannel) VALUES (\"$ititle\", \"$ilink\", \"$author\", \"$idesc_escaped\", \"$category\", \"$icomment_escaped\", \"$enc_type\", \"$enc_url\", \"$guid\", \"$ipubdate\", \"$source\", \"$chan_link\")";
  $item_stat = mysql_query($insert_item);
  if ($item_stat)
    echo "Item Added\n";
  else
    echo "Failed to Add the Item".mysql_error()."\n";
  //Insert download to downloads table
  $filename = file_name($enc_url);
  //echo $filename."\n";
  //echo $email."\n";
  insert_file($email,$filename,$enc_url,"192.248.16.123",$enc_len);	
 }
mysql_close($conn);
?>
