import requests

def call_api(city_name):
	url = "http://api.openweathermap.org/data/2.5/weather"
	api_key = "d14f9d7cc8a0c8af189d902e396458ea"
	params = {"appid" : api_key, "q" : city_name, "units" : "metric"}
	response = requests.get(url, params = params)
	response_in_json = response.json()
	weather_result = format_response(response_in_json)
	misc_result = misc_info(response_in_json)

	return weather_result, misc_result

def format_response(response_json):
	try:
		City_name_0 = response_json['name']
		City_Latitude = str(response_json['coord']['lat'])
		City_Longitude = str(response_json['coord']['lon'])
		Country = response_json['sys']['country']
		City_overview = City_name_0 + ' -(Latitude: ' + City_Latitude + ', Longitude: ' + City_Longitude + ') ' + Country
		Clouds = str(response_json['weather'][0]['description']).capitalize()
		Temperature = str(response_json['main']['temp']) + '°C'
		final_str = '%s\nClouds: %s\nTemperature: %s' % (City_overview,Clouds,Temperature)

	except:
		final_str = "Name isn't Valid"

	return final_str

def misc_info(response_json):
	try:
		temp_min = str(response_json['main']['temp_min']) + '°C'
		temp_max = str(response_json['main']['temp_max']) + '°C'
		pressure = str(response_json['main']['pressure']) + ' hPa'
		humidity = str(response_json['main']['humidity']) + ' %'
		wind_speed = str(response_json['wind']['speed']) + ' m/s'
		time_zone = str(response_json['timezone']) + ' UTC'
		miscellaneous_str = "Min Temp: %s\nMax Temp: %s\nPressure: %s\nHumidity: %s\nWind Speed: %s\nTimezone: %s" % (temp_min,temp_max,pressure,humidity,wind_speed,time_zone)

	except:
		miscellaneous_str = "Name isn't Valid"

	return miscellaneous_str


city = input("Enter City Name: ")
call_weather_result, call_misc_result = call_api(city)
print(call_weather_result)
if call_weather_result != "Name isn't Valid":
	further_info = input("Display Misc Information?\n[Y/N]\n")
	if further_info == "N":
		print("You're all set!")
	elif further_info == "Y":
		print(call_misc_result)
