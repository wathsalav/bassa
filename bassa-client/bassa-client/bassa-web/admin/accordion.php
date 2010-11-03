<script type="text/javascript" src="../javascripts/ui/jquery.ui.widget.js"></script>
<script type="text/javascript" src="../javascripts/ui/jquery.ui.accordion.js"></script>
<script type="text/javascript">
    $(function() {
        $("#accordion").accordion();
    });
</script>

<div id="accordion">
    <h3><a href="#">Change Time</a></h3>    
    <div>
     <?php    include 'change_time.php'; ?>
    </div>
    <h3><a href="#">Change Category</a></h3>
    <div>
     <?php    include 'change_category.php'; ?>
    </div>
    <h3><a href="#">Change Faculty</a></h3>
    <div>        
     <?php    include 'change_faculty.php'; ?>
    </div>    
</div>