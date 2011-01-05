<?php require_once  '../include/class.Functions.php'; ?>

<table id="gradient-style">
    <thead>
        <tr>
            <th scope="col" ></th>
            <th scope="col" width="490">File</th>
            <th scope="col">File Size</th>
            <th scope="col">States</th>
            <th scope="col" width="80">Date</th>
            <th scope="col"></th>
        </tr>
    </thead>
    <tfoot>
        <tr>
            <td colspan="6"></td>
        </tr>
    </tfoot>
    <tbody>
        <?php $functions->print_L_Downloads();?>
    </tbody>
</table>