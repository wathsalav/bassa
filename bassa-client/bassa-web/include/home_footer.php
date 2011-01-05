<?php
require_once 'class.FrontCache.php';
require_once 'class.Functions.php';
?>

<div id="home_footer">
    <ul class="home_contents horizontal" >
        <?php
        $result = $front->selectPopulerTag();
        if (mysql_num_rows($result) > 0) {
            while ($row = mysql_fetch_array($result)) {
                $i = 1;
                echo '<li id="home_content_' . $i++ . '">';
        ?>
                <div id="title">
                    <table width="150">
                        <tr>
                            <td><b><?php echo $row['tag_name']; ?></b></td>
                        </tr>
                    </table>
                    <table id="gradient-style" width="400">
                        <tbody>
                    <?php
                    //$result = $front->selectMostPopularFromTag($row['tag_name']);
                    ?>
                </tbody>
            </table>
        </div>
        <?php
                    echo '</li>';
                }
            } else {
                echo "No Tags found";
            }
        ?>
    </ul>
</div>