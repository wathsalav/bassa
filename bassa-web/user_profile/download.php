<?php require_once '../include/class.User1.php'; ?>
<div id="download">   
    <form method="post" action="">
        <table>
            <tr>
                <td height="35"><b>Download URL</b></td>
                <td >
                    <b>:</b>
                    <input type="text" size="45" name="url" value="<?php echo $_SESSION["url_bassa"];?>"/>
                </td>
            </tr>
            <tr>
                <td height="35"><b>Category</b></td>
                <td >
                    <b>:</b>
                    <select name="category" id="category">
                        <option value="0">None</option>
                        <?php $functions->viewTags(); ?>
                    </select>
                </td>
            </tr>
            <tr>
                <td height="35"><input type ="hidden" name="download" value="1"/></td>
                <td><input type="submit" value="Download"/></td>
            </tr>
        </table>
    </form>
</div>