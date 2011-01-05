<div id="message" align="center"style="color:#FF0000;"></div>
<link rel="stylesheet" type="text/css" href="../stylesheets/signup.css" />
<script type="text/javascript" src="../javascripts/signup.js"></script>
<form name="signup" action="" method="post">
    <fieldset class="formContainer">
        <h1>Register! in Bassa.</h1><br/><br/>
        <div class="rowContainer">
            <label for="txtFirstname">Username</label>
            <input id="name" type="text"  name="name"/>
            <div class="tooltipContainer info">Minimum 4 characters, maximum 15 characters.</div>
            <div class="tooltipContainer error">Username must be between 4 and 15 characters.</div>
        </div>
        <div class="rowContainer">
            <label for="txtLastname">Email Address</label>
            <input id="email" type="text"  name="email"/>
            <div class="tooltipContainer info">Please use a real email address as we need to email you to confirm your account.</div>
            <div class="tooltipContainer error">The email provided does not appear to be valid</div>
        </div>
        <div class="rowContainer">
            <label for="txtAddress">Verify Email </label>
            <input id="verifyEmail" type="text"  name="verifyEmail" value=""/>
            <div class="tooltipContainer info">Type in your email again for verification purposes.</div>

            <div class="tooltipContainer error">Email verification is failed!.</div>
        </div>
        <div class="rowContainer">
            <label for="txtEmail">Choose Password</label>
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
            <label for="txtEmail"></label>
            <script type="text/javascript">sjcap();</script>
        </div>
        <div class="rowContainer">
            <label for="txtEmail">CAPTCHA</label>
            <input type="text" id="uword" name="uword" class="" size="20"/>
            <div class="tooltipContainer info">Please type text as it is shown.</div>
            <div class="tooltipContainer error">Incorrect text as it is shown!</div>
        </div>
        <input id="signup"name="signup" type="hidden" value="1"/>
        <input type="button" id="btnSubmit" value="Signup" onclick="validateForm()" />
    </fieldset>
</form>