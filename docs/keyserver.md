# Сервер для выдачи ключей  

Вайпалка поддерживает три сервиса решения каптчи -- x-captcha.ru, anticaptcha.com и captcha.guru  
Авторизация происходит с помощью basic auth, в директории обращения должны лежать соответственно 3 файла: `xcaptcha, anticaptcha, gurocaptcha`  
Можете сменить на свои названия в /src/engine/setting.py, метод `get_key`, это не принципиально  
Вместо обращения к файлам, можете написать в пару строк скрипт на пыхе и цепляться к нему. Мне пыху было ставить лень, поэтому обращение идет к файлам.  

Файл должен содержать только одну строку с ключом 32 символа. Любой лишний символ при получении провалит проверку и вайпалку забракует ключ.  
Логины и пароли для авторизации можно создать через `htpasswd` (входит в состав `apache2-utils`)  

Конфиг для nginx:  
```
server {

	listen 80;
	listen [::]:80;
	listen 443 ssl;
	listen [::]:443 ssl;
	ssl_certificate <...>;
	ssl_certificate_key <...>;
	root <...>;
	server_name <...>;
	autoindex off;


	if ($request_method !~ ^(GET)$ ) {
		return	444;
	}

	location ~ /\.ht {
		deny	all;
	}

	location /captcha/ {
		access_log <...>;
		try_files $uri $uri/ =404;
		auth_basic "Wanna key, senpai?~";
		auth_basic_user_file <...>;

		if ($http_user_agent !~* (python-requests) ) {
			return	403;
		}
	}
}
```
