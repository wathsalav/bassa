<script type="text/javascript">
	$(function() {
		$("#tabs").tabs();
	});
	</script>
<div id="tabs">
	<ul>
		<li style="width:300px;"><a href="#tabs-1">Downloads</a></li>
		<li style="width:300px;"><a href="#tabs-2">Users</a></li>
	</ul>
    <div id="tabs-1"> <?php include 'allDownloads.php';?></div>
    <div id="tabs-2"> <?php include 'allUsers.php';?></div>
</div>