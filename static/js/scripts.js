const colorButton = document.querySelectorAll("button.choose_color")

document.addEventListener('change', changeColor)

colorButton[0].style.background = document.querySelectorAll("input[type='color']")[0].value
colorButton[1].style.background = document.querySelectorAll("input[type='color']")[1].value

function printColor(color) {
    const r = parseInt(color.substr(1, 2), 16)
    const g = parseInt(color.substr(3, 2), 16)
    const b = parseInt(color.substr(5, 2), 16)
    return {r, g, b}
}


function changeColor(event) {
    if (event.target.closest("input[type='color']")) {
        colorButton[+event.target.id - 1].style.background = event.target.value
        $.ajax({
            url: "/change_color",
            type: "POST",
            data: JSON.stringify(printColor(event.target.value)),
            contentType: "application/json; charset=utf-8",
            dataType: "json",
        });
    }
}

