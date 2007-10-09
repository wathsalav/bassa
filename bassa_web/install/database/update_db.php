<?php
require_once '../../lib/config.php';
$conn = mysql_connect($dbhost, $dbuser, $dbpass) or die (mysql_error("Unable to connect Database!"));
$sql=<<<EOS
USE $dbname
EOS;
$result=mysql_query($sql) or die (mysql_error());


$sql=<<<EOS
CREATE TABLE IF NOT EXISTS department (
        dept_id INT NOT NULL PRIMARY KEY AUTO_INCREMENT,
        name varchar(100) UNIQUE NOT NULL
)
EOS;
$result=mysql_query($sql) or die (mysql_error());

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
	file varchar(100) PRIMARY KEY NOT NULL,
	url varchar(500) NOT NUll,
	size INT,
	status varchar(100) NOT NUll DEFAULT 'queue',
	hits INT DEFAULT '1',
	date TIMESTAMP,
	email varchar(100)
)
EOS;
$result=mysql_query($sql) or die (mysql_error());

$sql=<<<EOS
CREATE TABLE IF NOT EXISTS userdown(
	user_id varchar(100),
	down_id varchar(100),
	FOREIGN KEY (user_id) REFERENCES users(email),
	FOREIGN KEY (down_id) REFERENCES downloads(file),
	ip varchar(100)
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


$numdep=system('cat department|wc -l');
$i=1;
while ($i<=$numdep){
$name=system("head -'$i' department|tail -1");
$sql=<<<EOS
INSERT INTO department (name) VALUES('$name')
EOS;
$result=mysql_query($sql) or die (mysql_error());
$i++;
}
?>
