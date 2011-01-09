<?php require_once '../include/class.Cache.php'; ?>

 <link rel="stylesheet" type="text/css" href="../stylesheets/table.css"  />
<table id="gradient-style">
    <thead>
        <tr>
            <th scope="col" ></th>
            <th scope="col" width="425">File</th>
            <th scope="col" >File Size</th>
            <th scope="col" >Status</th>
            <th scope="col" width="75">Date</th>
            <th scope="col" width="50">Download</th>
            <!--<th scope="col" ></th>-->
        </tr>
    </thead>
    <tfoot>
        <tr>
            <td colspan="7"></td>
        </tr>
    </tfoot>
    <tbody>
         <?php
                 $search_item = $_GET['search_item'];
                 $cache->printSearchResults($search_item, $_GET['offset']);
         ?>
    </tbody>
</table>
