function getData(){wait=(wait+1)%10,$.ajax({url:"/get_data",type:"get",async:!1,success:function(t){temps.length<10?temps.push(t.temp):(temps.shift(),temps.push(t.temp)),(2<temps[9]-temps[0]||t.temp>=t.temp_limit&&0==wait)&&(swal("Резкое повышение температуры!","Проверьте, всё ли хорошо с Вашим домом. Вполне вероятно, что он горит.","warning"),$(".swal-button")[0].style.display="block",$(".swal-button")[0].style.width="4rem",$(".swal-button")[0].style.height="2rem",$(".swal-modal")[0].style.background="#2E2E36",$(".swal-title")[0].style.color="#CCCCCC",$(".swal-text")[0].style.color="#CCCCCC",$(".swal-footer")[0].style.color="#CCCCCC")},error:function(t){}})}var temps=[],wait=0;$(document).ready(function(){getData(),setInterval("getData()",1e3)});