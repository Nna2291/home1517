from time import sleep

from flask import Flask, jsonify, redirect, render_template
from flask import request

app = Flask(__name__)
queue = []
temp_history = []
status = {'vent': 0, 'heat': -1, 'auto_light': 1, 'temp': -1.0, 'hum': -1.0, 'temp_limit': 30, 'lights': 0,
          'red': 0, 'green': 0, 'blue': 255, 'light_val': 0, 'light_status': 0, 'door': 0, 'z': 0}


@app.route('/')
def index():
    return render_template('home.html')


@app.route('/dick2')
def dick2():
    hum = request.args.get('hum')
    temp = request.args.get('temp')
    try:
        status['hum'] = float(hum)
        status['temp'] = float(temp)
    except ValueError:
        status['hum'] = 0
        status['temp'] = 0
    return jsonify(status)


@app.route('/door')
def door():
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


@app.route('/dick')
def dick():
    jio = request.args.get('light_val')
    status['light_val'] = float(jio)
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
