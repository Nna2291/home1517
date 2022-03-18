from time import sleep

from flask import Flask, jsonify, redirect, render_template
from flask import request

app = Flask(__name__)
queue = []
temp_history = []
status = {'vent': 1, 'heat': 0, 'auto_light1': 0, 'auto_light2': 0, 'temp': -1.0, 'hum': -1.0, 'temp_limit': 25, 'lights1': 0, 'lights2': 0,
          'red1': 0, 'green1': 0, 'blue1': 0, 'red2': 0, 'green2': 0, 'blue2': 0, 'light_val': 0, 'light_status': 0, 'door': 0}


@app.route('/')
def index():
    return render_template('home.html')


@app.route('/cool', methods=["POST"])
def cool():
    if request.json['cool']:
        status['vent'] = 1
    elif not request.json['cool']:
        status['vent'] = 0
    return 'ok'


@app.route('/heat', methods=["POST"])
def heat():
    if request.json['heat']:
        status['heat'] = 1
    elif not request.json['heat']:
        status['heat'] = 0
    return 'ok'


@app.route('/first_floor')
def first_floor():
    return jsonify(status)


@app.route('/door', methods=["POST", "GET"])
def door():
    if request.method == "POST":
        if status["door"] == 1:
            status["door"] = 0
        else:
            status["door"] = 1
        return jsonify({'door_stat': status["door"]})
    return render_template('door.html')


@app.route('/lights')
def light():
    return render_template('lights.html')


@app.route('/other')
def other():
    return render_template('other.html')


@app.route('/get_data')
def data():
    return jsonify(status)


@app.route('/change_color1', methods=["POST"])
def colch1():
    tt = request.json
    status['red1'] = tt['r']
    status['blue1'] = tt['b']
    status['green1'] = tt['g']
    return 'oka'

@app.route('/change_color2', methods=["POST"])
def colch2():
    tt = request.json
    status['red2'] = tt['r']
    status['blue2'] = tt['b']
    status['green2'] = tt['g']
    return 'oka'


@app.route('/change_light1', methods=["POST"])
def change_light1():
    tt = request.json
    if tt['light']:
        status['lights1'] = 1
    else:
        status['lights1'] = 0
    return 'oka'

@app.route('/change_light2', methods=["POST"])
def change_light2():
    tt = request.json
    if tt['light']:
        status['lights2'] = 1
    else:
        status['lights2'] = 0
    return 'oka'


@app.route('/change_auto1', methods=["POST"])
def change_auto1():
    tt = request.json
    if tt['auto']:
        status['auto_light1'] = 1
    else:
        status['auto_light1'] = 0
    return 'oka'

@app.route('/change_auto2', methods=["POST"])
def change_auto2():
    tt = request.json
    if tt['auto']:
        status['auto_light2'] = 1
    else:
        status['auto_light2'] = 0
    return 'oka'


@app.route('/second_floor')
def second_floor():
    jio = request.args.get('light_val')
    status['light_val'] = float(jio)
    hum = request.args.get('hum')
    temp = request.args.get('temp')
    light_status = request.args.get('ls')
    try:
        status['hum'] = float(hum)
        status['temp'] = float(temp)
        status['light_status'] = int(light_status)
    except ValueError:
        status['hum'] = 0
        status['temp'] = 0
        status['light_status'] = 0
    return jsonify(status)


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


@app.route('/temperature_and_humidity', methods=['GET', 'POST'])
def temp_and_hum():
    global temp_history, status
    if request.method == 'POST':
        temp, hum = float(request.form['temp']), float(request.form['hum'])
        if len(temp_history) >= 4:
            temp_history = [temp] + temp_history[:3]
            if temp_history[-1] - temp > 5 and temp > status['temp_limit']:
                pass
        status['temp'] = temp
        status['hum'] = hum
    return redirect('/')


if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0')