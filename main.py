from flask import Flask, jsonify
from flask import request

app = Flask(__name__)


@app.route('/')
def op():
    return 'higyui'


@app.route('/postjson', methods=['POST'])
def postJsonHandler():
    content = request.get_json()
    print(content)
    return 'JSON posted'


if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0')
