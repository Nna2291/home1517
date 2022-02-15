const colorButton = document.querySelectorAll("button.choose_color")

document.addEventListener('change', changeColor)

colorButton[0].style.background = document.querySelectorAll("input[type='color']")[0].value
colorButton[1].style.background = document.querySelectorAll("input[type='color']")[1].value

function changeColor(event) {
    if (event.target.closest("input[type='color']")) {
        colorButton[+event.target.id - 1].style.background = event.target.value
    }
}