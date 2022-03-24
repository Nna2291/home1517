import os

from dotenv import load_dotenv
from flask import Flask, jsonify, render_template, redirect, url_for
from flask import request
from flask_bcrypt import check_password_hash
from flask_login import LoginManager, login_user, logout_user, login_required

import forms
import models

load_dotenv('.env')
app = Flask(__name__)
app.secret_key = os.getenv('KEY')

login_manager = LoginManager()
login_manager.init_app(app)
login_manager.login_view = 'login'
queue = []
temp_history = []
status = {
    'vent': 0, 'heat': 0, 'auto_light1': 0, 'auto_light2': 0, 'temp': -1.0,
    'hum': -1.0, 'temp_limit': 30, 'temp_auto': 25, 'auto_heat': 0, 'red1': 0,
    'green1': 0, 'blue1': 0, 'red2': 0, 'green2': 0, 'blue2': 0,
    'light_val': 0, 'light_status1': 0, 'light_status2': 0, 'door': 0
}


@login_manager.user_loader
def load_user(userid):
    try:
        return models.User.get(models.User.id == userid)
    except models.DoesNotExist:
        return None


@app.route('/login', methods=('GET', 'POST'))
def login():
    form = forms.LoginForm()
    if form.validate_on_submit():
        try:
            user = models.User.get(models.User.login == form.login.data)
        except models.DoesNotExist:
            return jsonify({'message': 'error'})
        else:
            if check_password_hash(user.password, form.password.data):
                login_user(user)
                """
                Создаем сессию в браузере клиента
                """
                return redirect(url_for('index'))
            else:
                print("Ваш пароль и логин не совпадают", "error")
    return render_template('login.html', form=form)


@app.route('/')
@login_required
def index():
    return render_template('home.html')


@app.route('/logout')
@login_required
def log_out():
    logout_user()
    return redirect('/')


@app.route('/cool', methods=["POST"])
@login_required
def cool():
    if request.json['cool']:
        status['vent'] = 1
    elif not request.json['cool']:
        status['vent'] = 0
    return 'ok'


@app.route('/heat', methods=["POST"])
@login_required
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
@login_required
def door():
    if request.method == "POST":
        if status["door"] == 1:
            status["door"] = 0
        else:
            status["door"] = 1
        return jsonify({'door_stat': status["door"]})
    return render_template('door.html')


@app.route('/lights')
@login_required
def light():
    return render_template('lights.html')


@app.route('/other')
@login_required
def other():
    return render_template('other.html')


@app.route('/auto_heat', methods=["POST"])
@login_required
def auto_heat():
    tt = request.json
    if tt['auto_heat']:
        status['auto_heat'] = 1
    else:
        status['auto_heat'] = 0
    status['temp_auto'] = tt['temp_auto']
    return 'oka'


@app.route('/temp_limit', methods=["POST"])
@login_required
def temp_limit():
    tt = request.json
    status['temp_limit'] = tt['temp_limit']
    return 'oka'


@app.route('/get_data')
@login_required
def data():
    return jsonify(status)


@app.route('/change_color1', methods=["POST"])
@login_required
def colch1():
    tt = request.json
    status['red1'] = tt['r']
    status['blue1'] = tt['b']
    status['green1'] = tt['g']
    print(status)
    return 'oka'


@app.route('/change_color2', methods=["POST"])
@login_required
def colch2():
    tt = request.json
    status['red2'] = tt['r']
    status['blue2'] = tt['b']
    status['green2'] = tt['g']
    return 'oka'


@app.route('/change_light1', methods=["POST"])
@login_required
def change_light1():
    tt = request.json
    if tt['light']:
        status['light_status1'] = 1
    else:
        status['light_status1'] = 0
    return 'oka'


@app.route('/change_light2', methods=["POST"])
@login_required
def change_light2():
    tt = request.json
    if tt['light']:
        status['light_status2'] = 1
    else:
        status['light_status2'] = 0
    return 'oka'


@app.route('/change_auto1', methods=["POST"])
@login_required
def change_auto1():
    tt = request.json
    if tt['auto']:
        status['auto_light1'] = 1
    else:
        status['auto_light1'] = 0
    return 'oka'


@app.route('/change_auto2', methods=["POST"])
@login_required
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
    try:
        status['hum'] = float(hum)
        status['temp'] = float(temp)
    except:
        status['hum'] = 0
        status['temp'] = 0
    return jsonify(status)


if __name__ == '__main__':
    models.initialize()
    try:
        models.User.create_user('Integrals', '1517')
    except ValueError:
        pass
    app.run(debug=True, host='0.0.0.0')
