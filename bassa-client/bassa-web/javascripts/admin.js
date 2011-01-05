$(document).ready(function() {
    $('a.delete').click(function(e) {
        e.preventDefault();
        var parent = $(this).parent();
        $.ajax({
            type: 'get',
            url: 'index.php',
            data: 'ajax=1&delete=' + parent.attr('id').replace('record-',''),
            beforeSend: function() {
                parent.animate({
                    'backgroundColor':'#fb6c6c'
                },300);
            },
            success: function() {
                parent.slideUp(300,function() {
                    parent.remove();
                });
            }
        });
    });
});

$(document).ready(function()
{
    $('table#gradient-style td a.deleteDownload').click(function()
    {
        if (confirm("Are you sure you want to delete this Download Item?"))
        {
            var id = $(this).parent().parent().attr('id');
            var data = 'id=' + id ;
            var parent = $(this).parent().parent();

            $.ajax(
            {
                type: "POST",
                url: "delete_download.php",
                data: data,
                cache: false,

                success: function()
                {
                    parent.fadeOut('slow', function() {
                        $(this).remove();
                    });
                }
            });
        }
    });

    $('table#gradient-style td a.delete').click(function()
    {
        if (confirm("Are you sure you want to delete this User?"))
        {
            var id = $(this).parent().parent().attr('id');
            var data = 'id=' + id ;
            var parent = $(this).parent().parent();

            $.ajax(
            {
                type: "POST",
                url: "delete_row.php",
                data: data,
                cache: false,

                success: function()
                {
                    parent.fadeOut('slow', function() {
                        $(this).remove();
                    });
                }
            });
        }
    });

    $('table#gradient-style tr:odd').css('background',' #aaaa33');
});
