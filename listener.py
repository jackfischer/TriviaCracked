from flask import Flask
from flask import request
import json

app = Flask(__name__)

@app.route("/", methods=['GET', 'POST'])
def hello():
  print request.data
  return request.data

if __name__ == "__main__":
  app.run()

