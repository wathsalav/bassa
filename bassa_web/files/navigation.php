
<?php
echo "<div class='navigation'>
                        [ <a href='index.php'>HOME</a> ] !i! [
                        <a href='downloads.php'>DOWNLOADS</a> ] !i! [
			<a href='btv.php'>BTV</a> ] !i! [
                        <a href='doc.php'>DOCUMENTATION</a> ] !i! [
                        <a href='contact.php'>CONTACT</a> ] ";       
if($_SESSION['email'] && $_SESSION['passwd']){
  echo "!i! [ <a href='login.php'>MY ACCOUNT</a>($_SESSION[email]) <img src='images/presence_online.png'> ]";        
 }
echo "
</div>        
<div class='center'>";
?>
