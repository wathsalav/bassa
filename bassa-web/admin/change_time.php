<div id="chang_time">    
    <table width="450">
        <tr>
            <td width="75"><b>Start time :</b></td>
            <td width="75"><select name="start_time">
                    <?php
                    for ($i = 0; $i < 24; $i++) {
                        echo '<option value="' . $i . ':00">  ' . $i . ':00</option>';
                    }
                    ?>
                </select></td>
            <td width="75"><b>End time:</b></td>
            <td width="75"><select name="end_time">
                    <?php
                    for ($i = 0; $i < 24; $i++) {
                        echo '<option value="' . $i . ':00">  ' . $i . ':00</option>';
                    }
                    ?>
                </select></td>
                <td><input type="submit" value="change"/></td>
        </tr>
    </table>

</div>