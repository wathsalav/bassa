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

$sql=<<<EOS
CREATE TABLE IF NOT EXISTS users (
	email varchar(100) PRIMARY KEY NOT NULL,
	name varchar(500) NOT NULL,
	password varchar(100) NOT NULL
)
EOS;
$result=mysql_query($sql) or die (mysql_error());


$sql=<<<EOS
CREATE TABLE IF NOT EXISTS downloads (
	id INT AUTO_INCREMENT PRIMARY KEY,
	file varchar(200) NOT NULL,
	file_type varchar(50),
	url varchar(500) NOT NUll,
	size INT,
	local_url varchar(500) NOT NUll,
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
	comments varchar(500)
	date TIMESTAMP,
)
EOS;
$result=mysql_query($sql) or die (mysql_error());

// Load department list into database

$sql=<<<EOS
LOAD DATA INFILE 'department' INTO TABLE department
  FIELDS TERMINATED BY ','
EOS;
$result=mysql_query($sql) or die (mysql_error());

?>
