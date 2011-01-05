<?php require_once '../include/class.Functions.php'; ?>
<table width="450">
    <tr>
        <td><b>Faculty :</b></td>
    </tr>
    <tr>        
        <td >            
            <select name="faculty" id="faculty">
                <option value="0">None</option>
                <?php $functions->viewFaculty(); ?>
            </select>
        </td>
    </tr>
    <tr>
        <td><input type="submit" value="change"/></td>
    </tr>
</table>

