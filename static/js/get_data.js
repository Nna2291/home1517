function getData() {
    $.ajax({
        url: "/get_data",
        type: "get",

        success: function (response) {
            if (response['temp'] - temp > 3) {
                swal("Резкое повышение температуры!", "Проверьте, всё ли хорошо с Вашим домом. Вполне вероятно, что он горит.", "warning");
                $('.swal-button')[0].style.display = 'block';
                $('.swal-button')[0].style.width = '4rem';
                $('.swal-button')[0].style.height = '2rem';
                $('.swal-modal')[0].style.background = '#2E2E36'
                $('.swal-title')[0].style.color = '#CCCCCC'
                $('.swal-text')[0].style.color = '#CCCCCC'
                $('.swal-footer')[0].style.color = '#CCCCCC'
                
            }
            temp = response['temp']
        },
        error: function (xhr) {
            //Do Something to handle error
        }
    });
}

function getFirst(){
    var temp = 0;
    $.ajax({
        url: "/get_data",
        type: "get",
        async: false,
    
        success: function (response) {
            temp = response['temp'];
        },
        error: function (xhr) {
            //Do Something to handle error
        }
    });
    return temp;
};

var temp = getFirst();

$(document).ready(function () {
    getData();
    setInterval('getData()', 10000);
});