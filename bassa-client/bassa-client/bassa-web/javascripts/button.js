$(document).ready(function(){
    $(".button_ep").hover(function(){
        $(".button_ep img")
        .animate({
            top:"-10px"
        }, 200).animate({
            top:"-4px"
        }, 200) // first jump
        .animate({
            top:"-7px"
        }, 100).animate({
            top:"-4px"
        }, 100) // second jump
        .animate({
            top:"-6px"
        }, 100).animate({
            top:"-4px"
        }, 100); // the last jump
    });
     $(".button_lo").hover(function(){
        $(".button_lo img")
        .animate({
            top:"-10px"
        }, 200).animate({
            top:"-4px"
        }, 200) // first jump
        .animate({
            top:"-7px"
        }, 100).animate({
            top:"-4px"
        }, 100) // second jump
        .animate({
            top:"-6px"
        }, 100).animate({
            top:"-4px"
        }, 100); // the last jump
    });
    $(".button_su").hover(function(){
        $(".button_su img")
        .animate({
            top:"-10px"
        }, 200).animate({
            top:"-4px"
        }, 200) // first jump
        .animate({
            top:"-7px"
        }, 100).animate({
            top:"-4px"
        }, 100) // second jump
        .animate({
            top:"-6px"
        }, 100).animate({
            top:"-4px"
        }, 100); // the last jump
    });
}); 
