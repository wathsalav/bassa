<link rel="stylesheet"  href="../stylesheets/button.css"  />
<script type="text/javascript" src="../javascripts/button.js"></script>

<div id="prof_details">
    <table>
        <tr>
            <td width="50" height="35"><b>Name</b></td>
            <td ><b>:</b> <?php echo $session->username ?></td>
        </tr>
        <tr>
            <td width="50" height="35"><b>Email</b></td>
            <td ><b>:</b> <?php echo $functions->selectEmail(); ?></td>
        </tr>
        <tr>
            <td width="50" height="35"><b>Faculty</b></td>
            <td width="150"><b>:</b> <?php echo $functions->getFacultyShortNameFromUserID(); ?></td>
        </tr>        
    </table>
    <a class="button_ep" href="../edit_profile/index.php"><img src="../images/Write Document.png" alt="" />Edit Profile</a>
</div>