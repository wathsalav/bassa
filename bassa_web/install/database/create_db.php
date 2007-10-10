<?php
require_once '../../lib/config.php';
$conn = mysql_connect($dbhost, $dbuser, $dbpass) or die (mysql_error("Unable to connect Database!"));
$sql=<<<EOS
CREATE DATABASE IF NOT EXISTS $dbname
EOS;
$result=mysql_query($sql) or die (mysql_error());
$sql=<<<EOS
USE $dbname
EOS;
$result=mysql_query($sql) or die (mysql_error());

/*
 $sql=<<<EOS
 CREATE TABLE IF NOT EXISTS department (
 dept_id INT NOT NULL PRIMARY KEY AUTO_INCREMENT,
 name varchar(100) UNIQUE NOT NULL
 )
 EOS;
 $result=mysql_query($sql) or die (mysql_error());
*/

$sql=<<<EOS
CREATE TABLE IF NOT EXISTS users (
				    email varchar(100) PRIMARY KEY NOT NULL,
				    name varchar(500) NOT NULL,
				    password varchar(100) NOT NULL,
				    dept_id INT,
				    INDEX (dept_id),
				    FOREIGN KEY (dept_id) REFERENCES department(dept_id)
				    )
EOS;
$result=mysql_query($sql) or die (mysql_error());


$sql=<<<EOS
CREATE TABLE IF NOT EXISTS downloads (
					id INT AUTO_INCREMENT PRIMARY KEY,
					file varchar(200) NOT NULL,
					file_type varchar(50),
					url varchar(1024) NOT NUll,
					size BIGINT,
					local_url varchar(1024) NOT NUll,
					status varchar(100) NOT NUll DEFAULT 'queue',
					hits INT DEFAULT '1',
					date TIMESTAMP,
					down_date varchar(50) NOT NULL,
					email varchar(100),
					ip varchar(100)
					)
EOS;
$result=mysql_query($sql) or die (mysql_error());

$sql=<<<EOS
CREATE TABLE IF NOT EXISTS userdown(
				      user_id varchar(100),
				      file_id varchar(50),
				      down_id INT,
				      FOREIGN KEY (user_id) REFERENCES users(email),
				      FOREIGN KEY (down_id) REFERENCES downloads(id)
				      )
EOS;
$result=mysql_query($sql) or die (mysql_error());

$sql=<<<EOS
CREATE TABLE IF NOT EXISTS developers(
					Name varchar(100),
					Designation varchar(100),
					Projects varchar(100),
					About varchar(1000)
					)
EOS;
$result=mysql_query($sql) or die (mysql_error());

$sql=<<<EOS
CREATE TABLE IF NOT EXISTS comments(
				      user_id varchar(100),
				      file_id varchar(50),
				      FOREIGN KEY (user_id) REFERENCES users(email),
				      FOREIGN KEY (file_id) REFERENCES downloads(id),
				      subject varchar(50),
				      comments varchar(500),
				      date TIMESTAMP
				      )
EOS;
$result=mysql_query($sql) or die (mysql_error());

$sql=<<<EOS
CREATE TABLE IF NOT EXISTS meta_info(
				       down_id INT AUTO_INCREMENT,
				       file_name varchar(500),
				       meta_url varchar(1024),
				       origin_url varchar(500),
				       mime_type varchar (100),
				       PRIMARY KEY (down_id),
				       FOREIGN KEY (origin_url) REFERENCES downloads (url)
				       )
EOS;

$result=mysql_query($sql) or die (mysql_error());

$sql=<<<EOS
CREATE TABLE IF NOT EXISTS btv_channels(
					  title varchar(500),
					  description varchar(2000),
					  link varchar(512),
					  rating varchar(100),
					  pub_date timestamp,
					  image_title varchar(400),
					  image_url varchar(1024),
					  PRIMARY KEY (link)
					  )
EOS;

$result=mysql_query($sql) or die (mysql_error());

$sql=<<<EOS
CREATE TABLE IF NOT EXISTS btv_items(
				       item_id INT AUTO_INCREMENT,
				       title varchar(500) UNIQUE,
				       link varchar(512),
				       author varchar(150),
				       category varchar(100),
				       description varchar(2000),
				       comments varchar(2000),
				       enc_type varchar(50),
				       enc_url varchar(1024),
				       guid varchar(1000),
				       pub_date timestamp,
				       source varchar(1024),
				       ichannel varchar(512), 
				       PRIMARY KEY (item_id),
				       FOREIGN KEY (enc_url) REFERENCES downloads(url) ON UPDATE CASCADE,
				       FOREIGN KEY (ichannel) REFERENCES btv_channels(link) ON UPDATE CASCADE
				       )
EOS;



$result=mysql_query($sql) or die (mysql_error());
// Load department list into database
//$sql=<<<EOS
//LOAD DATA INFILE 'department' INTO TABLE department
//  FIELDS TERMINATED BY ','
//EOS;
//$result=mysql_query($sql) or die (mysql_error());
?>
