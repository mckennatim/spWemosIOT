var env={	
 	"portforward": {
 		"appid": "tauth_pf",
 		"cbPath": "#registered",			
		"url": {
			"soauth": "http://71.192.254.240:7080",
			"api": "http://71.192.254.240:3332/api",
			"spa": "http://71.192.254.240/spas/admind/dist/#",
			"base": "http://71.192.254.240/spas"
		}
 	},
 	"local": {
 		"appid": "pahoRawLo",			
 		"cbPath": "",			
		"url": {
			"mqtt_server": "10.0.1.102",
			"mqtt_port": 3333,
			"soauth": "http://10.0.1.102:7080",
			"api": "http://10.0.1.102:3332/api",
			"spa": "http://10.0.1.102/spWemosIOT/auth/aclient/pahoRaw.html",
			"base": "http://10.0.1.102/spWemosIOT/auth/aclient/pahoRaw.html"
		}
 	},
 	"http": {
 		"appid": "tauth_ht",			
 		"cbPath": "#registered",			
		"url": {
			"soauth": "http://162.217.250.109:7080",
			"api": "http://71.192.254.240:3332/api",
			"spa": "http://71.192.254.240/spas/admind/dist/#",
			"base": "http://71.192.254.240/spas"
		}
 	},
 	"https": {
 		"appid": "tauth_hs",			
 		"cbPath": "#registered",			
		"url": {
			"soauth": "https://services.sitebuilt.net/soauth",
			"api": "http://71.192.254.240:3332/api",
			"spa": "http://71.192.254.240/spas/admind/dist/#",
			"base": "http://71.192.254.240/spas"
		}
 	},
 	"production": {
 		"appid": "tauth",			
 		"cbPath": "#registered",			
		"url": {
			"soauth": "https://services.sitebuilt.net/soauth",
			"api": "https://services.sitebuilt.net/geniot/api2",
			"spa": "http://71.192.254.240/spas/admind/dist/#",
			"base": "http://71.192.254.240/spas"
		}
 	}
}
