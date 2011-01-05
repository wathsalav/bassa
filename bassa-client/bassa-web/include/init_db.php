<?php
  include ("constants.php");
  function init_db()
  {
    $mconn = mysql_connect(DB_SERVER, DB_USER, DB_PASS) or die(mysql_error());
    $cdb_qry = "CREATE DATABASE ".DB_NAME;
    mysql_query($cdb_qry, $mconn);
    mysql_select_db(DB_NAME, $mconn);
    $actg_qry = "CREATE TABLE IF NOT EXISTS active_guests(
		 ip varchar(15) NOT NULL,
		 timestamp int(11) unsigned NOT NULL,
		 PRIMARY KEY (ip))";
    if (!mysql_query($actg_qry, $mconn))
      echo mysql_error();
    $acug_qry = "CREATE TABLE IF NOT EXISTS active_users(
		 username varchar(30) NOT NULL,
		 timestamp int(11) unsigned NOT NULL,
		 PRIMARY KEY (username))";
    mysql_query($acug_qry, $mconn);
    $bndu_query = "CREATE TABLE IF NOT EXISTS banned_users(
		   username varchar(30) NOT NULL,
		   timestamp int(11) unsigned NOT NULL,
		   PRIMARY KEY (username))";
    mysql_query($bndu_query, $mconn);
    $ci_qry = "CREATE TABLE IF NOT EXISTS cache_index (
	       id bigint(20) NOT NULL AUTO_INCREMENT,
	       origin_url varchar(1000) NOT NULL,
	       file_name varchar(512) DEFAULT NULL,
	       object_url varchar(1024) DEFAULT NULL,
	       object_path varchar(1024) DEFAULT NULL,
	       status varchar(1) DEFAULT NULL,
	       date_time timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
	       content_length bigint(20) DEFAULT NULL,
	       hits bigint(20) DEFAULT NULL,
	       proto_bf int(11) DEFAULT NULL,
	       client_uuid varchar(256) NOT NULL,
	       start_time bigint(20) NOT NULL,
	       end_time bigint(20) NOT NULL,
	       PRIMARY KEY (id),
	       UNIQUE KEY origin_url (origin_url))";
    mysql_query($ci_qry, $mconn);
    $fc_qry = "CREATE TABLE IF NOT EXISTS faculty (
	       faculty_id int(11) NOT NULL AUTO_INCREMENT,
	       faculty_name varchar(100) NOT NULL,
	       faculty_short_name varchar(50) NOT NULL,
	       PRIMARY KEY (faculty_id),
	       UNIQUE KEY faculty (faculty_name),
	       KEY faculty_id (faculty_id))";
    mysql_query($fc_qry, $mconn);
    $frnct_qry = "CREATE TABLE IF NOT EXISTS front_cache(
		  id int(11) NOT NULL AUTO_INCREMENT,
		  file_name varchar(1000) NOT NULL,
		  size int(11) NOT NULL DEFAULT 0,
		  tag varchar(20) NOT NULL DEFAULT 0,
		  status varchar(1) NOT NULL DEFAULT 'Q',
		  date_time timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
		  hits bigint(20) NOT NULL DEFAULT 0,
		  client_uuid varchar(256) NOT NULL,
		  PRIMARY KEY (id))";
    mysql_query($frnct_qry, $mconn);
    $tag_qry = "CREATE TABLE IF NOT EXISTS tag (
		id int(11) NOT NULL AUTO_INCREMENT,
		tag_name varchar(20) NOT NULL,
		image varchar(20) NOT NULL,
		PRIMARY KEY (id))";
    mysql_query($tag_qry, $mconn);
    $user_qry = "CREATE TABLE IF NOT EXISTS users (
		 username varchar(30) NOT NULL,
		 password varchar(32) DEFAULT NULL,
		 userid varchar(32) DEFAULT NULL,
		 userlevel tinyint(1) unsigned NOT NULL,
		 email varchar(50) DEFAULT NULL,
		 timestamp int(11) unsigned NOT NULL,
		 sex varchar(10) DEFAULT NULL,
		 faculty varchar(1000) DEFAULT NULL,
		 id int(11) NOT NULL AUTO_INCREMENT,
		 PRIMARY KEY (id),
		 UNIQUE KEY username (username))";
    mysql_query($user_qry, $mconn);
    $add_admin = "INSERT INTO users (username, password, userid,
		  userlevel, email, timestamp, sex, faculty, id) VALUES(
		  'admin',PASSWORD('admin'), 'xyz', 9, 'admin@bassa-cache.org', 0, 'male', 14, 1)";
    if (!mysql_query($add_admin, $mconn))
      echo mysql_error();
  }
?>

