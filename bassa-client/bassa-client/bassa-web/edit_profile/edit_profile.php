<?php
require_once '../include/class.Functions.php';

?>
<div> <?php require_once '../include/class.User1.php'; ?> </div>

<form action="#" enctype="multipart/form-data" method="post">
    <fieldset class="formContainer">
        <h1>Edit Your Profile</h1>
        <br/><br/>
        <div class="rowContainer">
            <label for="txtFirstname">Email Address</label>
            <input name="email" type="text" id="email" value=<?php echo $functions->selectEmail(); ?> />
            <div class="tooltipContainer info" align="justify">Please use a real email address as we need to email you to confirm your account.</div>
            <div class="tooltipContainer error" align="left">The email provided does not appear to be valid</div>
        </div>
        <div class="rowContainer">
            <label for="txtAddress">Verify Email </label>
            <input  name="verifyEmail" type="text" id="verifyEmail" value=""/>
            <div class="tooltipContainer info">Type in your email again for verification purposes.</div>
            <div class="tooltipContainer error">Email verification is failed!.</div>
        </div>
        <div class="rowContainer">
            <label for="txtEmail">New Password</label>
            <input id="password" type="password"  name="password" value=""/>
            <div class="tooltipContainer info">Must be at least 6 characters long.</div>
            <div class="tooltipContainer error">Passwords must be at least 6 characters and can only contain letters and numbers.</div>
        </div>
        <div class="rowContainer">
            <label for="txtEmail">Re-type Password</label>
            <input id="txtRetypePassword" type="password" name="txtRetypePassword" />
            <div class="tooltipContainer info">Type in your password again for verification purposes.</div>
            <div class="tooltipContainer error">Password verification is failed!</div>
        </div>
        <div class="rowContainer">
            <label for="txtLastname">Faculty</label>
            <label>
                <select name="faculty" id="faculty" >
                    <option value="0">None</option>
                    <?php $functions->viewFaculty(); ?>
                </select>
            </label>
        </div>
        <div class="rowContainer">
            <label for="txtEmail">Sex</label>            
            <table>
                <tr><?php $sex=$functions->checkGender(); ?>
                    <td><input name="sex" type="radio" id="sex_m" value="male" <?php echo $sex['male']; ?> />
                        <input name="uName" type="hidden" value="<?php echo $username; ?>"/>
                    </td>
                    <td>Male</td>
                </tr>
                <tr>
                    <td><input name="sex" type="radio" id="sex_f" value="femail" <?php echo $sex['female'];?> />
                    </td>
                    <td>Female</td>
                </tr>
            </table><br/>
        </div><br/>
        <div class="rowContainer">
            <label for="txtImage">Image</label>
            <input type="file" name="file"/>
            <input name="profimage" type="hidden" value="Upload image"/>
        </div>
        <input name="update" type="hidden" value="Upload image"/>
        <input name="edit" type="submit" id="btnSubmit" value="Edit Profile"  align="left" />
    </fieldset>
</form>
