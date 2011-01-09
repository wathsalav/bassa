<?php

require_once 'constants.php';
require_once 'class.Session.php';

//require 'class.Cache.php';

class FrontCache {

    var $connection;         //The MySQL database connection

    function FrontCache() {
        $this->connection = mysql_connect(DB_SERVER, DB_USER, DB_PASS) or die(mysql_error());
        mysql_select_db(DB_NAME, $this->connection) or die(mysql_error());
    }

    function addTag($new_tag) {
        $query = "INSERT INTO tag (tag_name)VALUES (" . $new_tag . ")";
        mysql_query($query, $this->connection) or die(mysql_error());
    }

    function addFrontCache($ulr, $category, $user_id) {
        $query = "INSERT INTO front_cache (file_name, tag, client_uuid) VALUES ('" . $ulr . "', '" . $category . "', '" . $user_id . "')";
        mysql_query($query, $this->connection) or die(mysql_error());
    }

    function selectTags() {
        $query = "SELECT * FROM tag";
        $result = mysql_query($query, $this->connection) or die(mysql_error());
        return $result;
    }

    function selectTag($tagId) {
        $query = "SELECT tag_name FROM tag WHERE id='. $tagId.'";
        $result = mysql_query($query, $this->connection) or die(mysql_error());
        $tag_name = mysql_fetch_row($result);
        return $tag_name;
    }

    function selectTagImage($tagId) {
        $query = "SELECT image FROM tag WHERE id ='" . $tagId . "'";
        $result = mysql_query($query, $this->connection) or die(mysql_error());
        $image = mysql_fetch_row($result);
        return $image[0];
    }

    function selectPopulerTag() {
        $query = "SELECT tag_name FROM tag
            WHERE id IN (SELECT tag FROM front_cache
            GROUP BY tag ORDER BY COUNT( tag ) DESC)LIMIT 5";
        $result = mysql_query($query, $this->connection) or die(mysql_error());
        return $result;
    }

    function selectFaculty($faculty_id) {
        $query = "SELECT faculty_name FROM faculty WHERE faculty_id=" + $faculty_id;
        $result = mysql_query($query, $this->connection) or die(mysql_error());
        $faculty_name = mysql_fetch_row($result);
        return $faculty_name;
    }

    function selectAllFaculty() {
        $query = "SELECT * FROM faculty";
        $result = mysql_query($query, $this->connection) or die(mysql_error());
        return $result;
    }

    function selectMostPopular($limit) {
        $query = "SELECT * FROM front_cache WHERE status='C' ORDER BY hits DESC LIMIT " . $limit;
        $result = mysql_query($query, $this->connection) or die(mysql_error());
        return $result;
    }

    function selectAllMostPopular() {
        $query = "SELECT * FROM front_cache ORDER BY hits DESC ";
        $result = mysql_query($query, $this->connection) or die(mysql_error());
        return $result;
    }

    function selectLatest($limit) {
        $query = "SELECT * FROM front_cache WHERE status='C' ORDER BY date_time DESC LIMIT " . $limit;
        $result = mysql_query($query, $this->connection) or die(mysql_error());
        return $result;
    }

    function selectAllLatest() {
        $query = "SELECT * FROM front_cache ORDER BY date_time DESC";
        $result = mysql_query($query, $this->connection) or die(mysql_error());
        return $result;
    }

    function selectSearchItems($search_item) {
        $query = "SELECT * FROM front_cache WHERE file_name LIKE  '%" . $search_item . "%'ORDER BY hits DESC ";
        $result = mysql_query($query, $this->connection) or die(mysql_error());
        return $result;
    }

    function selectUserDownloads() {
        $session = new Session();
        $query = "SELECT * FROM front_cache WHERE client_uuid IN (
                SELECT id FROM users WHERE username = '" . $session->username . "' )
                ORDER BY date_time DESC ";
        $result = mysql_query($query, $this->connection) or die(mysql_error());
        return $result;
    }

    function selectMostPopularFromTag($tag) {
        $query = "SELECT file_name,hits FROM front_cache WHERE tag IN (
                SELECT id FROM tag WHERE tag_name =  '" . $tag . "') ORDER BY hits DESC LIMIT 5";
        $result = mysql_query($query, $this->connection) or die(mysql_error());
        return $result;
    }

    function selectAllUsers() {
        $query = "SELECT * FROM users";
        $result = mysql_query($query, $this->connection) or die(mysql_error());
        return $result;
    }

    function getFronCache() {
        $query = "SELECT * FROM front_cache";
        $result = mysql_query($query, $this->connection) or die(mysql_error());
        return $result;
    }

    function getUserDetails() {
        $session = new Session();
        $query = "SELECT * FROM users WHERE username = '$session->username'";
        $result = mysql_query($query, $this->connection) or die(mysql_error());
        return $result;
    }

    function getUserId() {
        $session = new Session();
        $query = "SELECT id FROM users WHERE username = '$session->username'";
        $result = mysql_query($query, $this->connection) or die(mysql_error());
        $user_id = mysql_fetch_row($result);
        return $user_id[0];
    }

    function getFacultyOfUser() {
        $session = new Session();
        $query = "SELECT faculty_name FROM faculty WHERE faculty_id IN(
                SELECT faculty FROM users WHERE username= '$session->username') ";
        $result = mysql_query($query, $this->connection) or die(mysql_error());
        return $result;
    }

    function getFacultyShortNameFromFacaltyID($faculty_id) {
        $query = "SELECT faculty_short_name FROM faculty WHERE faculty_id = '" . $faculty_id . "'";
        $result = mysql_query($query, $this->connection) or die(mysql_error());
        $faculty_short_name = mysql_fetch_row($result);
        return $faculty_short_name[0];
    }

    function getFacultyShortNameFromUserID() {
        $session = new Session();
        $query = "SELECT faculty_short_name FROM faculty WHERE faculty_id IN(
                SELECT faculty FROM users WHERE username= '$session->username')";
        $result = mysql_query($query, $this->connection) or die(mysql_error());
        $faculty_short_name = mysql_fetch_row($result);
        return $faculty_short_name[0];
    }

    function usernameTaken($username) {
        $query = "SELECT username FROM " . TBL_USERS . " WHERE username = '" . $username . "'";
        $result = mysql_query($query, $this->connection) or die(mysql_error());
        return (mysql_num_rows($result) > 0);
    }

    function addAdmin($username, $password, $email) {
        $query = "INSERT INTO `users` (`username`, `password`, `userlevel`, `email`)
                VALUES ('$username', '$password','9','$email')";
        $result = mysql_query($query, $this->connection) or die(mysql_error());
    }

    function addUser($username, $password, $email) {
        $query = "INSERT INTO `users` (`username`, `password`, `userlevel`, `email`)
                VALUES ('$username', '$password','1','$email')";
        $result = mysql_query($query, $this->connection) or die(mysql_error());
    }

    function updateEmail($email) {
        $session = new Session();
        $query = "UPDATE users SET email = '$email' WHERE username = '$session->username'";
        $result = mysql_query($query, $this->connection) or die(mysql_error());
    }

    function updatePassword($password) {
        $session = new Session();
        $query = "UPDATE users SET password = '$password' WHERE username = '$session->username'";
        $result = mysql_query($query, $this->connection) or die(mysql_error());
    }

    function updateUserFaculty($faculty) {
        $session = new Session();
        $query = "UPDATE users SET faculty = '$faculty' WHERE username = '$session->username'";
        $result = mysql_query($query, $this->connection) or die(mysql_error());
    }

    function updateSex($sex) {
        $session = new Session();
        $query = "UPDATE users SET sex = '$sex' WHERE username = '$session->username'";
        $result = mysql_query($query, $this->connection) or die(mysql_error());
    }

}

$front = new FrontCache();

?>

