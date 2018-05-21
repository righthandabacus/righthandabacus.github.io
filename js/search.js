$(document).ready(function(){
    var search_key = document.location.href.split('/').pop().replace(/\.[A-Za-z0-9]+$/g,'').replace(/[^A-Za-z0-9]/g,' ');
    var numcols = $('#fordatatable th').length;
    $('#fordatatable').DataTable({
        order: [[numcols-1,'desc']],
        pageLength: 10,
        scrollX: true,
        stateSave: false,
        search: {
            search: search_key
        }
    });
});
