<?php require_once '../include/class.Functions.php'; ?>
<table width="450">
    <tr>
        <td height="35"><b>Category</b></td>
        <td >
            <b>:</b>
            <select name="category" id="category">
                <option value="0">None</option>
                <?php $functions->viewTags(); ?>
            </select>
        </td>
        <td><input type="submit" value="change"/></td>
    </tr>
</table>
