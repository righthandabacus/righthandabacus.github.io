$(document).ready(function(){
    var numcols = $('#fordatatable thead tr').length;
    $('#fordatatable').DataTable({
        order: [[numcols-1,'desc']],
        pageLength: 10,
        scrollX: true,
        stateSave: true
    });
});
