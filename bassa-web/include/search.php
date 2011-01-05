<script type="text/javascript" language="JavaScript">
    function FromActionURLchange(){
        if(document.fm1.rd1[0].checked==true){
            document.fm1.action="../search";
            return true;
        }
        if(document.fm1.rd1[1].checked==true){
            document.fm1.action="http://www.google.com/search";
            return true;
        }
    }
</script>
<form  action="" method="get" name="fm1">
    <table>
        <tr>
            <td width="250"><input type="text" name="search_item" size="27"/></td>
            <td><input type="submit" value="Search" onClick="return FromActionURLchange();"/></td>
        </tr>
    </table>
    <table>
        <tr>
            <td width="80"> Bassa Search</td><td width="10"> <input type="radio"  name="rd1"  checked="" /></td>
            <td width="85"> Google Search</td><td width="10"><input type="radio"  name="rd1" /></td>
        </tr>
    </table>
</form>

