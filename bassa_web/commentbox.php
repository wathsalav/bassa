<?php
echo "
<form name='commentbox' action='action.php?action=addcomment&id=$id' method='post''>
<table>
    <tr>
        <th colspan=2> COMMENT BOX</th> 
    </tr>
    <tr>
      <td>Subject</td>
      <td><INPUT type='text' name='subject' size='50'  ></td>
    </tr>
    <tr>
      <td>Comment</td>
      <td><textarea name='comment' cols='38' rows='5' wrap='on'></textarea><br/>
	500 characters Max.</td>
    </tr>
    <tr>
      <td><INPUT type='submit' name='submit' value='Send' ></td>
      <td><INPUT type='reset' name='reset' value='Reset' ></td>
    </tr>
</table>
</form>";
?>
