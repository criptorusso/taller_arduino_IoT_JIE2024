#!/usr/bin/python3

from flask import Flask
from flask_restful import reqparse, abort, Api, Resource
from flask_cors import CORS

app = Flask(__name__)

CORS(app)
api = Api(app)

class Alarma(Resource):
	def get(self):
		return '', 400

	def post(self):
		try:
			parser = reqparse.RequestParser()
			parser.add_argument('temp', type=str, location="args")
			args = parser.parse_args()
			temperatura = args['temp']
			print(temperatura)
			if temperatura.rstrip() != '':
				f = open('evento', 'w')
				f.write(temperatura)
				f.close()
		except e:
			pass
		finally:
			return "dato recibido", 200

## Setup de las rutas del API
##
api.add_resource(Alarma, '/alarma', endpoint='alarma')


if __name__ == '__main__':
	app.run(host= '0.0.0.0', debug=True)
