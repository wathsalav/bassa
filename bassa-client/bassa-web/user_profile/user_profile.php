<?php
require_once '../include/class.Functions.php';
require_once '../include/class.FrontCache.php';
?>
<table>
    <tr>
        <td><?php include '../include/prof_picture.php'; ?></td>
        <td><?php include '../include/prof_details.php'; ?></td>
        <td valign="top"><?php include 'download.php';  ?></td>
    </tr>
</table>
<div id="download_item" align="center">
    <h1>My Downloads</h1>
    <?php include 'user_downloads.php'; ?>
</div>

