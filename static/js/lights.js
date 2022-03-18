const colorButton = document.querySelectorAll("button.choose_color");

document.addEventListener('change', changeColor);
document.addEventListener('change', changeLightOn);
document.addEventListener('change', changeAuto);

colorButton[0].style.background = document.querySelectorAll("input[type='color']")[0].value;
colorButton[1].style.background = document.querySelectorAll("input[type='color']")[1].value;

function printColor(color) {
    const r = parseInt(color.substr(1, 2), 16);
    const g = parseInt(color.substr(3, 2), 16);
    const b = parseInt(color.substr(5, 2), 16);
    return {r, g, b}
}

function changeColor(event) {
    if (event.target.closest("input[type='color']")) {
        colorButton[+event.target.id - 1].style.background = event.target.value;
        $.ajax({
            url: "/change_color" + event.target.id,
            type: "POST",
            data: JSON.stringify(printColor(event.target.value)),
            contentType: "application/json; charset=utf-8",
            dataType: "json",
        });
    }
}

function changeLightOn(event) {
    if (event.target.closest("input[name='light']")) {
        $.ajax({
            url: "/change_light" + event.target.id,
            type: "POST",
            data: JSON.stringify({light: event.target.checked}),
            contentType: "application/json; charset=utf-8",
            dataType: "json",
        });
    }
}

function changeAuto(event) {
    if (event.target.closest("input[name='auto']")) {
        $.ajax({
            url: "/change_auto" + event.target.id,
            type: "POST",
            data: JSON.stringify({auto: event.target.checked}),
            contentType: "application/json; charset=utf-8",
            dataType: "json",
        });
    }
}



function componentToHex(c) {
    var hex = c.toString(16);
    return hex.length == 1 ? "0" + hex : hex;
  }

function rgbToHex(r, g, b) {
return "#" + componentToHex(r) + componentToHex(g) + componentToHex(b);
}

function gett() {
    $.ajax({
        url: "/get_data",
        type: "get",

        success: function (response) {
            $('input[type=color]')[0].value = rgbToHex(response['red1'], response['green1'], response['blue1']);
            $('input[type=color]')[1].value = rgbToHex(response['red2'], response['green2'], response['blue2']);
            colorButton[0].style.background = rgbToHex(response['red1'], response['green1'], response['blue1']);
            colorButton[1].style.background = rgbToHex(response['red2'], response['green2'], response['blue2']);
            $("input[name='light']")[0].checked = response['lights1'];
            $("input[name='light']")[1].checked = response['lights2'];
            $("input[name='auto']")[0].checked = response['auto_light1'];
            $("input[name='auto']")[1].checked = response['auto_light2'];
        },
        error: function (xhr) {
            //Do Something to handle error
        }
    });
}

gett();
