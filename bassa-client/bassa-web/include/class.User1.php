<?php

require_once 'class.Session.php';
require_once 'class.FrontCache.php';
require_once 'class.Functions.php';
require_once 'class.Cache.php';

class User1 {

    var $massage;

    function User1() {      
        if (isset($_POST['signup'])) {            
            $this->signup();
        }
        if (isset($_POST['update'])) {
            $this->update();
        }
        if (isset($_POST['profimage'])) {
            $this->profimage();
        }
        if (isset($_POST['download'])) {
            $this->addDownload();
        }
    }

    function signup() {
        $username = $_POST["name"];
        $password = $_POST["txtRetypePassword"];
        $email = $_POST["email"];
        $encryptedPassword = md5($password);

        $front = new FrontCache();
        $session = new Session();
        $functions = new Functions();

        if (!get_magic_quotes_gpc()) {
            $username = addslashes($username);
        }
       
        if (!$front->usernameTaken($username)) {
            if ($session->isAdmin()) {
                $front->addAdmin($username, $encryptedPassword, $email);
                $functions->showMessage('complete', 'You are signed up WITH ADMIN PRIVILAGES');
            } else {
                $front->addUser($username, $encryptedPassword, $email);
                $functions->showMessage('complete', 'You are signed up');
            }           
        } else {
             $functions->showMessage('error', 'UserName ' . $username . ' exists. Try again.');
        }

    }

    function update() {
        $email = $_POST["verifyEmail"];
        $password = $_POST["password"];
        $sex = $_POST["sex"];
        $faculty = $_POST["faculty"];
        $check = 0;

        $front = new FrontCache();
        $functions = new Functions();

        if ($email != '') {
            $currentEmail = $functions->selectEmail();
            if ($email != $currentEmail) {
                $front->updateEmail($email);
                $functions->showMessage('complete', 'Email is change');
            }
        }
        if ($password != '') {
            $encryptedPassword = md5($password);
            $currentPassword = $functions->selectPassword();          
            if ($encryptedPassword != $currentPassword) {
                $front->updatePassword($encryptedPassword);
                $functions->showMessage('complete', 'Password is change');
            }
        }
        if (is_numeric($faculty) && $faculty > 0) {
            $currentFaculty = $functions->selectFaculty();
            if ($faculty != $currentFaculty) {
                $front->updateUserFaculty($faculty);
		$functions->showMessage('complete', 'Faculty is change');
            }
        }
        if ($sex != '') {
            $currentSex = $functions->selectGender();          
            if ($sex != $currentSex) {
                $front->updateSex($sex);
		$functions->showMessage('complete', 'Gender is change');
            }
        }
    }

    function profimage() {
        $session = new Session();
        if ((($_FILES["file"]["type"] == "image/gif")
                || ($_FILES["file"]["type"] == "image/jpeg")
                || ($_FILES["file"]["type"] == "image/pjpeg")
                || ($_FILES["file"]["type"] == "image/png")
                || ($_FILES["file"]["type"] == "image/bmp"))
                || ($_FILES["file"]["size"] < 20000)) {
            if ($_FILES["file"]["error"] > 0) {
                // echo "Re/turn Code: " . $_FILES["file"]["error"] . "<br />";
            } else {
                move_uploaded_file($_FILES["file"]["tmp_name"], "../images/user/" . $session->username);
		$functions->showMessage('complete', 'Profile Picthure is change');
            }
        } else {
            echo "Invalid file";
        }
    }

    function addDownload(){
        $front = new FrontCache();
        $cache = new Cache();
        $functions = new Functions();
        
        $url = $_POST['url'];
        $category = $_POST['category'];
	$user_id = $front->getUserId();     
	$file_name = basename($url);
	$file_name_with_shash = '/';
	$file_name_with_shash .= $file_name;

	$res = $cache->queueStatus($_POST['url']);
        if ($res == NULL)
        { 
	  $front->addFrontCache($file_name_with_shash, $category, $user_id);
	  $cache->addCache($url, $user_id);
	  $functions->showMessage('complete', "Download Added to Queue.");
        }
        else if($res['status'][0] == 'P' || $res['status'][0]=='Q')
        {
	  $front->addFrontCache($file_name_with_shash, $category, $user_id);
	  $functions->showMessage('complete', "Download Already in Queue. Content Will be Available Soon!");
        }
        else if($res['status'][0] == 'F')       
        {
	  $front->addFrontCache($file_name_with_shash, $category, $user_id);
	  $cache->addCache($url, $user_id);
	  $functions->showMessage('error', "Previous Attempt on Downloading this File had Failed. Retrying...");
        }
        else
        {
	  $functions->showMessage('complete', '<b><a href="'.$res['local-url'][0].'">Download Now</a></b>');
          echo '<script type="text/javascript">';
          echo 'window.location = "'.$res['local-url'][0].'"';
          echo '</script>';
        }
    }    

}

$user1 = new User1();
?>
