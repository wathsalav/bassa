<?php require_once '../include/class.Functions.php'; ?>

<table id="gradient-style">
    <thead>
        <tr>
            <th></th>
            <th scope="col" width="400">File</th>
            <th scope="col" width="75">File Size</th>
            <th scope="col" >States</th>
            <th scope="col" width="50">Downloads</th>
            <th scope="col" width="80">Date</th>
            <th scope="col" ></th>
        </tr>
    </thead>
    <tfoot>
        <tr>
            <td colspan="7"></td>
        </tr>
    </tfoot>
    <tbody>
        <?php $functions->print_User_Downloads(); ?>
    </tbody>
</table>
