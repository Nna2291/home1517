document.addEventListener('change', changeHeat);
document.addEventListener('change', changeCool);
document.addEventListener('ValueChange', changeAutoHeat)
document.addEventListener('change', changeAutoHeat)

function changeHeat(event) {
    if (event.target.closest("#heat")) {
        $.ajax({
            url: "/heat",
            type: "POST",
            data: JSON.stringify({heat: $('#heat').prop('checked')}),
            contentType: "application/json; charset=utf-8",
            dataType: "json",
        });
    }
}

function changeCool(event) {
    if (event.target.closest("#cool")) {
        $.ajax({
            url: "/cool",
            type: "POST",
            data: JSON.stringify({heat: $('#cool').prop('checked')}),
            contentType: "application/json; charset=utf-8",
            dataType: "json",
        });
    }
}

function changeAutoHeat(event) {
    if (event.target.closest("input[name=auto_heat]")) {
        $.ajax({
            url: "/auto_heat",
            type: "POST",
            data: JSON.stringify({temp_limit: $('input[name=auto_heat]')[1].value, auto_heat: $('input[name=auto_heat]')[0].checked}),
            contentType: "application/json; charset=utf-8",
            dataType: "json",
        });
    }
}

function gett() {
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
            $('input[name=auto_heat]')[0].checked = response['auto_heat'];
            $('input[name=auto_heat]')[1].value = response['temp_limit'];
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