<?php require_once '../include/class.Cache.php'; ?>

<table id="gradient-style">
    <thead>
        <tr>
            <th scope="col" ></th>
            <th scope="col" width="490">File</th>
            <th scope="col" >File Size</th>
            <th scope="col" >Status</th>
            <th scope="col" width="50">Download</th>
            <th scope="col" ></th>
        </tr>
    </thead>
    <tfoot>
        <tr>
            <td colspan="6"></td>
        </tr>
    </tfoot>
    <tbody>
        <?php $cache->printPopularDownloads(0); ?>
    </tbody>
</table>
