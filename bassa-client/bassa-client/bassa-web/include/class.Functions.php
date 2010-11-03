<?php

require_once 'class.Session.php';
require_once 'class.FrontCache.php';

class Functions {

    function Functions() {
        $front = new FrontCache();
    }

    function printHomeDownloads($result) {
        $front = new FrontCache();
        if (mysql_num_rows($result) > 0) {
            while ($row = mysql_fetch_array($result)) {
                $tagImage = $front->selectTagImage($row['tag']);
                $fname = substr($row['file_name'], 1);
                $hits = $row['hits'];

                echo '<tr>';
                echo '<td><img src="images/category/'. $tagImage. '" border="0"/></td>';
                echo '<td>' . $fname . '</td>';
                echo '<td align="right">' . $hits . '</td>';
                echo '<td align="center"><a href="http://10.16.90.98/data/' . $fname . '"><img src="images/Clear Green Button.png" border="0"/></a></td>';
                echo '</tr>';
            }
        } else {
            echo "No Downloads found";
        }
    }

    function printTagDownloads($tag) {
        $front = new FrontCache();
        $result = $front->selectMostPopularFromTag($tag);
        if (mysql_num_rows($result) > 0) {
            while ($row = mysql_fetch_array($result)) {
            	$fname = substr($row['file_name'], 1);
                echo '<tr>';                
                echo '<td><a href="http://10.16.90.98/data/' . $fname . '">' . $fname . '</a></td>';
                echo '<td align="right">' . $row['hits'] . '</td>';
                echo '</tr>';
            }
        } else {
            echo "No Downloads found";
        }
    }

    function print_All_Downloads(){
        $front = new FrontCache();
        $result = $front->selectAllMostPopular();
        if (mysql_num_rows($result) > 0) {
            while ($row = mysql_fetch_array($result)) {
                $tagImage = $front->selectTagImage($row['tag']);
                $fname = substr($row['file_name'], 1);
                $size = $this->calSize($row['size']);
                $status = $this->detStatus($row['status']);
                $hits = $row['hits'];
                $date = $this->detDate($row['date_time']);

                echo '<tr>';
                echo '<td><img src="../images/category/'. $tagImage. '" border="0"/></td>';
                echo '<td>' . $fname . '</td>';
                echo '<td>' . $size . '</td>';
                echo '<td><img src="../images/' . $status . '" border="0"/></td>';
                echo '<td align="right">' . $hits . '</td>';
                echo '<td align="right">' . $date . '</td>';
                echo '<td align="center"><a href="http://10.16.90.98/data/' . $fname . '"><img src="../images/Play Green Button.png" border="0"/></a></td>';
                echo '</tr>';
            }
        } else {
            echo "No Downloads found";
        }
    }

    function print_MP_Downloads() {
        $front = new FrontCache();
        $result = $front->selectAllMostPopular();
        if (mysql_num_rows($result) > 0) {
            while ($row = mysql_fetch_array($result)) {
                $tagImage = $front->selectTagImage($row['tag']);
                $fname = substr($row['file_name'], 1);
                $size = $this->calSize($row['size']);
                $status = $this->detStatus($row['status']);
                $hits = $row['hits'];
                
                echo '<tr>';
                echo '<td><img src="../images/category/'. $tagImage. '" border="0"/></td>';
                echo '<td>' . $fname . '</td>';
                echo '<td>' . $size . '</td>';
                echo '<td><img src="../images/' . $status . '" border="0"/></td>';
                echo '<td align="right">' . $hits . '</td>';
                echo '<td align="center"><a href="http://10.16.90.98/data/' . $fname . '"><img src="../images/Add Green Button.png" border="0"/></a></td>';
                echo '</tr>';
            }
        } else {
            echo "No Downloads found";
        }
    }

    function print_L_Downloads() {
        $front = new FrontCache();
        $result = $front->selectAllLatest();
        if (mysql_num_rows($result) > 0) {
            while ($row = mysql_fetch_array($result)) {
                $tagImage = $front->selectTagImage($row['tag']);
                $fname = substr($row['file_name'], 1);
                $size = $this->calSize($row['size']);
                $status = $this->detStatus($row['status']);
                $date = $this->detDate($row['date_time']);

                echo '<tr>';
              	echo '<td><img src="../images/category/'. $tagImage. '" border="0"/></td>';
                echo '<td>' . $fname . '</td>';               
                echo '<td>' . $size . '</td>';
                echo '<td><img src="../images/' . $status . '" border="0"/></td>';
                echo '<td align="right">' . $date . '</td>';
                echo '<td align="center"><a href="http://10.16.90.98/data/' . $fname . '"><img src="../images/Play Green Button.png" border="0"/></a></td>';
                echo '</tr>';
            }
        } else {
            echo "No Downloads found";
        }
    }

    function print_User_Downloads() {
        $front = new FrontCache();
        $result = $front->selectUserDownloads();
        if (mysql_num_rows($result) > 0) {
            while ($row = mysql_fetch_array($result)) {
                $tagImage = $front->selectTagImage($row['tag']);
                $fname = substr($row['file_name'], 1);
                $size = $this->calSize($row['size']);
                $status = $this->detStatus($row['status']);
                $hits = $row['hits'];
                $date = $this->detDate($row['date_time']);

                echo '<tr>';
                echo '<td><img src="../images/category/'. $tagImage. '" border="0"/></td>';
                echo '<td>' . $fname . '</td>';
                echo '<td>' . $size . '</td>';
                echo '<td><img src="../images/' . $status . '" border="0"/></td>';
                echo '<td align="right">' . $hits . '</td>';
                echo '<td align="right">' . $date . '</td>';
                echo '<td align="center"><a href="http://10.16.90.98/data/' . $fname . '"><img src="../images/Play Green Button.png" border="0"/></a></td>';
                echo '</tr>';
            }
        } else {
            //echo "No User Downloads found";
        }
    }

    function print_Search_Result($search_item){
        $front = new FrontCache();
        $result = $front->selectSearchItems($search_item);
        if (mysql_num_rows($result) > 0) {
            while ($row = mysql_fetch_array($result)) {
                $tagImage = $front->selectTagImage($row['tag']);
                $fname = substr($row['file_name'], 1);
                $size = $this->calSize($row['size']);
                $status = $this->detStatus($row['status']);
                $hits = $row['hits'];
                $date = $this->detDate($row['date_time']);

                echo '<tr>';
                echo '<td><img src="../images/category/'. $tagImage. '" border="0"/></td>';
                echo '<td>' . $fname . '</td>';
                echo '<td>' . $size . '</td>';
                echo '<td><img src="../images/' . $status . '" border="0"/></td>';
                echo '<td align="right">' . $hits . '</td>';
                echo '<td align="right">' . $date . '</td>';
                echo '<td align="center"><a href="http://10.16.90.98/data/' . $fname . '"><img src="../images/Play Green Button.png" border="0"/></a></td>';
                echo '</tr>';
            }
        } else {
            echo "No Downloads found";
        }
    }

    function print_All_Users(){
        $front = new FrontCache();
        $result = $front->selectAllUsers();
        if (mysql_num_rows($result) > 0) {
            while ($row = mysql_fetch_array($result)) {
                $user_name = $row['username'];
                $user_lavel = $this->detUserLevel($row['userlevel']);
                $email = $row['email'];
                $sex = $row['sex'];
                $faculty = $this-> getFacultyShortNameFromFacultyID($row['faculty']);

                echo '<tr>';
                echo '<td>' . $user_name .'</td>';
                echo '<td>' . $user_lavel . '</td>';
                echo '<td>' . $email . '</td>';
                echo '<td>' . $sex . '</td>';
                echo '<td>' . $faculty . '</td>';
                echo '</tr>';
            }
        } else {
            echo "No Downloads found";
        }
    }

    function showMessage($type, $message){
        if($type == 'error'){
             echo '<div class="ui-state-error ui-corner-all" style="margin-top:2px; padding: 1em;"> ';
             echo '<span class="ui-icon ui-icon-info" style="float: left; margin-right: .3em;"></span>';
             echo $message;
             echo '</div>';
        }
        elseif($type == 'complete'){
            echo '<div class="ui-state-highlight ui-corner-all" style="margin-top:2px; padding: 1em;">';
            echo '<span class="ui-icon ui-icon-info" style="float:left; margin-right: .3em;"></span>';
            echo $message;
            echo '</div>';
        }
    }

    function viewTags() {
        $front = new FrontCache();
        $result = $front->selectTags();
        if (mysql_num_rows($result) > 0) {
            while ($row = mysql_fetch_array($result)) {
                echo '<option value="' . $row['id'] . '">' . $row['tag_name'] . '</option>';
            }
        } else {
            echo "No Tags found";
        }
    }    

    function viewFaculty() {
        $front = new FrontCache();
        $result = $front->selectAllFaculty();
        if (mysql_num_rows($result) > 0) {
            while ($row = mysql_fetch_array($result)) {
                echo '<option value="' . $row['faculty_id'] . '">' . $row['faculty_name'] . '</option>';
            }
        } else {
            echo "No Faculty found";
        }
    }

    function checkGender() {
        $front = new FrontCache();
        $result = $front->getUserDetails();
        $sex = array('male' => '0', 'female' => '0');
        if (mysql_num_rows($result) > 0) {
            $user = mysql_fetch_array($result);
            if ($user['sex'] == 'male')
                $sex['male'] = 'checked';
            elseif ($user['sex'] == 'female')
                $sex['female'] = 'checked';
            else
                $sex['male'] = 'che';
        }
        return $sex;
    }

    function detDate($date_time) {
        $dateTime = split(' ', $date_time);
        $date;
        if ($dateTime[0] == date("Y-m-d"))
            $date = "Today";
        else
            $date = $dateTime[0];
        return $date;
    }

    function detUserLevel($user_level_id){
        $user_level;
        if($user_level_id == '1'){
            $user_level = 'normal';
        }
        elseif ($user_level_id == '9') {
            $user_level = 'Admin';
        }
        return $user_level;
    }

    function selectProfilePic() {
        $session =new Session();
        $url = "../images/user/" . $session->username;
        $handle = @fopen($url, 'r');
        if ($handle !== false)
            $url = "../images/user/" . $session->username;
        else
            $url = "../images/user/default.png";

        return $url;
    }

    function selectEmail() {
        $front = new FrontCache();
        $result = $front->getUserDetails();
        if (mysql_num_rows($result) > 0) {
            $row = mysql_fetch_array($result);
            $email = $row['email'];
        }
        else
            $email='';
        return $email;
    }

    function selectPassword(){
        $front = new FrontCache();
        $result = $front->getUserDetails();
        if (mysql_num_rows($result) > 0) {
            $row = mysql_fetch_array($result);
            $password = $row['password'];
        }
        else
            $password='';
        return $password ;
    }

    function selectFaculty() {
        $front = new FrontCache();
        $result = $front->getFacultyOfUser();
        if (mysql_num_rows($result) > 0) {
            $row = mysql_fetch_array($result);
            $faculty = $row['faculty_name'];
        } else
            $faculty = '';
        return $faculty;
    }

    function selectGender(){
        $front = new FrontCache();
        $result = $front->getUserDetails();
        if (mysql_num_rows($result) > 0) {
            $row = mysql_fetch_array($result);
            $sex = $row['sex'];
        } else
            $sex = '';
        return  $sex;
    }

    function calSize($size) {
        $newSize = 0;
        if ($size < 1048576) {
            $new = round(($size / 1024), 2);
            $newSize = $new . "KB";
        } elseif ($size > 1048576) {
            $new = round(($size / 1048576), 2);
            $newSize = $new . "MB";
        } else {
            $newSize = $size . "b";
        }
        return $newSize;
    }

    function detStatus($status) {
        $image;
        if ($status == 'C')
            $image = 'Play.png';
        else
            $image='Remove.png';
        return $image;
    }

    function getFacultyShortNameFromFacultyID($faculty_id){
        $front = new FrontCache();
        $faculty_short_name = $front->getFacultyShortNameFromFacaltyID($faculty_id);
        return $faculty_short_name;
    }

    function getFacultyShortNameFromUserID() {
        $front = new FrontCache();
        $faculty = $front->getFacultyShortNameFromUserID();
        return $faculty;
    }

}

$functions = new Functions();
?>
