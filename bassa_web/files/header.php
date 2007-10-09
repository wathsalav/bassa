<?php
session_start();
echo "<html>
<head>
<title> UCSC - GAD (Globally Accessible Disk)</title>
<meta name='description' content='GAD-Bassa frontend' />
<meta name='author' content='Gowri Ananthan' />
<meta name='email' content='gna@ucsc.cmb.ac.lk' />
<style type='text/css' media='all'>@import 'css/default.css';</style>

<SCRIPT language='JavaScript'>
<!--email validation-->
function checkEmail(loginform) {

	if (loginform.name.value == '') {
    	alert( 'Please enter your Full Name.' );
    	loginform.name.focus();
    	return false ;
  	}

	if (/^\w+([\.-]?\w+)*@\w+([\.-]?\w+)*(\.\w{2,3})+$/.test(loginform.email.value)){
	return true;
	}
	alert('Invalid E-mail Address! Please re-enter.');
    	loginform.email.focus();
	return false;

}

<!--AJAX-->
        function show_the_overlay() {
                document.getElementById('display_overlay_holder').style.display = 'block';
        }
        function hide_the_overlay() {
                document.getElementById('display_overlay_holder').style.display = 'none';
        }
</SCRIPT>
</head>
<body>";
echo date('l dS \of F Y');
echo "<br/>";
$ip=$_SERVER['REMOTE_ADDR'];
echo "Your IP is <i>$ip</i> <br/>";
echo ' <div class="right">';
echo "<img src='images/gadisk.png'>";
echo "<img src='images/bassa.png'>";

echo '<form action="search.php" method="post">
                        <input type="text" name="search" class="text" />
                        <input type="submit" value="Search Downloads" class="submit"/><br>
			Sort by:
			<input type=radio name=sort value="file">File Name
			<input type=radio name=sort value="size">Size
			<input type=radio name=sort value="hits">URL
			<input type=radio name=sort value="down_date">Date
			<input type=radio name=sort value="hits">Hits<br/>
                </form>';

if(isset($_SESSION['email']) && isset($_SESSION['passwd']) && !empty($_SESSION['passwd'])){
  echo '<a href="action.php?action=Logout" ><font>Logout</font></a>  ';
 }
echo "</div>";
echo "<hr>";
?>
