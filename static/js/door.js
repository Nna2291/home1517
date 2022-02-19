$(document).ready(function () {
    $("#open").click(function (e) {
        e.preventDefault();
        link = "/door";
        $.ajax({
            type: "POST", url: link,
            success: function (data) {
                if (data["door_stat"] == 1)
                {
                    $("#status").text("Дверь открыта")
                }else
                {
                    $("#status").text("Дверь закрыта")
                }
            },
        });
    });
});
