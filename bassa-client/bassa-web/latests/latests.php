<?php 
	require_once  '../include/class.Functions.php'; 
        require_once  '../include/class.Cache.php';
?>

<table id="gradient-style">
    <thead>
        <tr>
            <th scope="col" ></th>
            <th scope="col" width="490">File</th>
            <th scope="col">File Size</th>
            <th scope="col">Status</th>
            <th scope="col" width="80">Date</th>
            <th scope="col" width="80">Download</th>
            <!--<th scope="col"></th>-->
        </tr>
    </thead>
    <tfoot>
        <tr>
            <td colspan="6"></td>
        </tr>
    </tfoot>
    <tbody>
        <?php
                $cache->printAllDownloads($_GET['offset'], NULL);
        ?>
    </tbody>
</table>
