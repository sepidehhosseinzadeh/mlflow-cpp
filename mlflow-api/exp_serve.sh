#curl -d '[{"x": 1}, {"x": -1}]' -H 'Content-Type: application/json' -X POST localhost:1234/invocations
#curl -X POST -H "Content-Type:application/json" --data '[{"fixed  acidity": 6.2, "volatile acidity": 0.66, "citric acid": 0.48, "residual sugar": 1.2, "chlorides": 0.029, "free sulfur dioxide": 29, "total sulfur dioxide": 75, "density": 0.98, "pH": 3.33, "sulphates":0.39, "alcohol": 12.8}]' http://127.0.0.1:1234/invocations

curl -d "$1" -H 'Content-Type: application/json' -X POST localhost:$2/invocations
