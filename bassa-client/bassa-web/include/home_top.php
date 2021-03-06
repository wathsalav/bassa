<?php
require_once 'class.FrontCache.php';
require_once 'class.Functions.php';
require_once 'class.Cache.php';
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
                        <th scope="col">Download</th>
                        <th scope="col"></th>
                    </tr>
                </thead>
                <tfoot>
                    <tr>
                        <td colspan="4">Go to Most Popular to see all downloads</td>
                    </tr>
                </tfoot>
                <tbody>
                    <?php
                         $cache->printPopularDownloads(0);
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
                        <th scope="col">Size</th>
                        <th scope="col">Status</th>
                        <th scope="col">Date</th>
                        <th scope="col">Download</th>
                        <th scope="col"></th>
                    </tr>
                </thead>
                <tfoot>
                    <tr>
                        <td colspan="4">Go to Latest Downloads to see all downloads</td>
                    </tr>
                </tfoot>
                <tbody>
                    <?php
		      $cache->printAllDownloads($_GET['offset'], "#tabs-2");
                    ?>
                </tbody>
            </table>          
        </div>
    </div>
</div>
