function getData(){
    $.ajax({
        url: "/get_data",
        type: "get",
        async: false,
    
        success: function (response) {
            if (temps.length < 5){
                temps.push(response['temp']);
            }
            else {
                temps.shift()
                temps.push(response['temp']);
                if (temps[4] - temps[0] > 2) {
                    swal("Резкое повышение температуры!", "Проверьте, всё ли хорошо с Вашим домом. Вполне вероятно, что он горит.", "warning");
                    $('.swal-button')[0].style.display = 'block';
                    $('.swal-button')[0].style.width = '4rem';
                    $('.swal-button')[0].style.height = '2rem';
                    $('.swal-modal')[0].style.background = '#2E2E36'
                    $('.swal-title')[0].style.color = '#CCCCCC'
                    $('.swal-text')[0].style.color = '#CCCCCC'
                    $('.swal-footer')[0].style.color = '#CCCCCC'
                }
            }
        },
        error: function (xhr) {
            //Do Something to handle error
        }
    });
};

var temps = [];

$(document).ready(function () {
    getData();
    setInterval('getData()', 1000);
});