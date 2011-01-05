<?php
require_once 'class.FrontCache.php';
require_once 'class.Functions.php';
?>
<div id="home_top">
    <div id="tabs">
        <ul>
            <li style="width:425px;"><a href="#tabs-1">Most Popular Downloads</a></li>
            <li style="width:425px;"><a href="#tabs-2">Latest Downloads</a></li>
        </ul>
        <div id="tabs-1">
            <table id="gradient-style">
                <thead>
                    <tr>
                        <th scope="col"></th>
                        <th scope="col" width="700">File</th>
                        <th scope="col">Downloads</th>
                        <th scope="col"></th>
                    </tr>
                </thead>
                <tfoot>
                    <tr>
                        <td colspan="4">Go Most Popular to see all downloads</td>
                    </tr>
                </tfoot>
                <tbody>
                    <?php
                    $select_MP_downloads = $front->selectMostPopular(7);
                    $functions->printHomeDownloads($select_MP_downloads);
                    ?>
                </tbody>
            </table>           
        </div>
        <div id="tabs-2">
            <table id="gradient-style">
                <thead>
                    <tr>
                        <th scope="col"></th>
                        <th scope="col" width="700">File</th>
                        <th scope="col">Downloads</th>
                        <th scope="col"></th>
                    </tr>
                </thead>
                <tfoot>
                    <tr>
                        <td colspan="4">Go Latest Downloads to see all downloads</td>
                    </tr>
                </tfoot>
                <tbody>
                    <?php
                    $select_L_downloads = $front->selectLatest(7);
                    $functions->printHomeDownloads($select_L_downloads);
                    ?>
                </tbody>
            </table>          
        </div>
    </div>
</div>