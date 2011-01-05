<?php require_once '../include/class.Functions.php'; ?>
<link rel="stylesheet" type="text/css" href="../stylesheets/table.css"  />
<table id="gradient-style" width="800">
    <thead>
        <tr>
            <th scope="col" width="100">User Name</th>
            <th scope="col" width="100">User Level</th>
            <th scope="col" width="200">Email</th>
            <th scope="col" width="50">Gender</th>
            <th scope="col" >Faculty</th>
        </tr>
    </thead>
    <tfoot>
        <tr>
            <td colspan="7"></td>
        </tr>
    </tfoot>
    <tbody>
         <?php $functions->print_All_Users(); ?>
    </tbody>
</table>