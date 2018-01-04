$(document).ready(function(){
    var numcols = $('#fordatatable th').length;
    $('#fordatatable').DataTable({
        order: [[numcols-1,'desc']],
        pageLength: 10,
        scrollX: true,
        stateSave: true
    });
});
