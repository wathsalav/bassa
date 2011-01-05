<?php
require_once 'class.FrontCache.php';
require_once 'class.Functions.php';
?>
<div id="home_footer">
    <ul class="home_contents horizontal" >
        <li id="home_content_1">
            <div id="title">
                <table width="150">
                    <tr>
                        <td><b>Video Tutorials</b></td>
                    </tr>
                </table>
                <table id="gradient-style" width="400" h>
                    <tbody>
                        <?php $functions->printTagDownloads('Video Tutorials'); ?>
                    </tbody>
                </table>
            </div>
        </li>
        <li id="home_content_2">
            <div id="title">
                <table>
                    <tr>
                        <td><b>E-Books</b></td>
                    </tr>
                </table>
                <table id="gradient-style" width="400">
                    <tbody>
                       <?php $functions->printTagDownloads('E-Books'); ?>
                    </tbody>
                </table>
            </div>
        </li>
        <li id="home_content_2">
            <div id="title">
                <table>
                    <tr>
                        <td><b>Softwares</b></td>
                    </tr>
                </table>
                <table id="gradient-style" width="400">
                    <tbody>
                       <?php $functions->printTagDownloads('Softwares'); ?>
                    </tbody>
                </table>
            </div>
        </li>
        <li id="home_content_3">
            <div id="title">
                <table>
                    <tr>
                        <td><b>Movies</b></td>
                    </tr>
                </table>
                <table id="gradient-style" width="400">
                    <tbody>
                       <?php $functions->printTagDownloads('Movies'); ?>
                    </tbody>
                </table>
            </div>
        </li>
        <li id="home_content_4">
            <div id="title">
                <table>
                    <tr>
                        <td><b>Songs</b></td>
                    </tr>
                </table>
                <table id="gradient-style" width="400">
                    <tbody>
                        <?php $functions->printTagDownloads('Songs'); ?>
                    </tbody>
                </table>
            </div>
        </li>
    </ul>
</div>