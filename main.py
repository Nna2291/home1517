from flask import Flask
from flask import request

app = Flask(__name__)
queue = []
temp_history = []
status = {'vent': -1, 'heat': -1, 'auto_light': 1, 'temp': -1, 'hum': -1, 'temp_limit': 30, 'lights': 0}


@app.route('/postjson', methods=['POST'])
def postJsonHandler():
    content = request.get_json()
    print(content)
    return 'JSON posted'


# свет
# данные о темп и влажности
# уведомление об открытии двери
# сигнализация при повышении температуры выше заданной границы
# вентиляция
# обогрев


@app.route('/')
def index():
    return f"t = {status['temp']}, h = {status['hum']}"
    #return render_template('home.html')


@app.route('/queue')
def queue():
    global queue
    if queue:
        q = jsonify(queue)
        queue = []
        return q
    else:
        sleep(5)
        return 0


@app.route('door_opened')
def alert_door_opened():
    pass


@app.route('/temperature_and_humidity', methods=['GET', 'POST'])
def temp_and_hum():
    global temp_history, status
    if request.metod == 'POST':
        temp, hum = float(request.form['temp'], request.form['hum'])
        if len(temp_history) >= 4:
            temp_history = [temp] + temp_history[:3]
            if temp_history[-1] - temp > 5 and temp > status['temp_limit']:
                alert_temp()
        status['temp'] = temp
        status['hum'] = hum
    return redirect('/')


def alert_temp():
    pass


@app.route('/heating')
def heating():
    queue.append('heating')
    return redirect('/')


@app.route('/vent')
def vent():
    queue.append('vent')
    return redirect('/')


@app.route('/temperature_limit/<float:value>', methods=['GET', 'POST'])
def temperature_limit(value):
    global status
    status['temp_limit'] = value
    return redirect('/')


@app.route('/lights/<int:light>')
def lights(light):
    status['lights'] = light
    return rediect('/')


if __name__ == '__main__':
    app.run(debug=True)
