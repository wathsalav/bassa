<?php require_once '../include/class.Functions.php'; ?>
 <link rel="stylesheet" type="text/css" href="../stylesheets/table.css"  />
<table id="gradient-style">
    <thead>
        <tr>
            <th scope="col" ></th>
            <th scope="col" width="425">File</th>
            <th scope="col" >File Size</th>
            <th scope="col" >States</th>
            <th scope="col" width="50">Downloads</th>
            <th scope="col" width="75">Date</th>
            <th scope="col" ></th>
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
                 $functions->print_Search_Result($search_item);
         ?>
    </tbody>
</table>