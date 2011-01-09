<?php
if ($session->isAdmin()) {
?>
    <div id="menu">
        <ul class="kwicks">
            <li id="kwick1"><a href="../">Home</a></li>
            <li id="kwick2"><a href="../most_popular">Popular Downloads</a></li>
            <li id="kwick3"><a href="../latests">Latest Downloads</a></li>
            <!--<li id="kwick4"><a href="../list_all">All Downloads</a></li>-->
            <li id="kwick5"><a href="../admin">Administrator</a></li>
            <li id="kwick6"><a href="../help">Help</a></li>
        </ul>
    </div>
<?php
} else if ($session->logged_in) {
?>
    <div id="menu">
        <ul class="kwicks">
            <li id="kwick1"><a href="../">Home</a></li>
            <li id="kwick2"><a href="../most_popular">Popular Downloads</a></li>
            <li id="kwick3"><a href="../latests">Latest Downloads</a></li>
            <!--<li id="kwick4"><a href="../list_all">All Downloads</a></li>-->
            <li id="kwick4"><a href="../user_profile">Profile</a></li>
            <li id="kwick5"><a href="../help">Help</a></li>
        </ul>
    </div>
<?php
} else {
?>
    <div id="index_menu">
        <ul class="kwicks">
            <li id="kwick1"><a href="../">Home</a></li>
            <li id="kwick2"><a href="../most_popular">Popular Downloads</a></li>
            <li id="kwick3"><a href="../latests">Latest Downloads</a></li>
            <!--<li id="kwick4"><a href="../list_all">All Downloads</a></li>-->
            <li id="kwick5"><a href="../help">Help</a></li>
        </ul>    
    </div>
<?php
}
?>
