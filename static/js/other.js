function gett() {
    $.ajax({
        url: "/cool",
        type: "POST",
        data: JSON.stringify({cool: $('#cool').prop('checked')}),
        contentType: "application/json; charset=utf-8",
        dataType: "json",
    });
    $.ajax({
        url: "/heat",
        type: "POST",
        data: JSON.stringify({heat: $('#heat').prop('checked')}),
        contentType: "application/json; charset=utf-8",
        dataType: "json",
    });
    $.ajax({
        url: "/get_data",
        type: "get",

        success: function (response) {
            $('#hum').text(response['hum']);
            $('#temp').text(response['temp']);
        },
        error: function (xhr) {
            //Do Something to handle error
        }
    });
}

function setCoolHeat() {
    $.ajax({
        url: "/get_data",
        type: "get",

        success: function (response) {
            $("#heat")[0].checked = response['heat'];
            $("#cool")[0].checked = response['vent'];
        },
        error: function (xhr) {
            //Do Something to handle error
        }
    });
}

setCoolHeat();

$(document).ready(function () {
    gett();
    setInterval('gett()', 500);
});