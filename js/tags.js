$(document).ready(function(){
    var url = document.location.toString();
    if (url.match('#')) {
        $('.nav-tabs a[href="#' + url.split('#')[1] + '"]').tab('show');
        setTimeout(function(){window.scrollTo(0,0)}, 500);
    };
    $('.nav-tabs a').on('shown.bs.tab', function(e){
        window.location.hash = e.target.hash;
        window.scrollTo(0,0);
    });
});
