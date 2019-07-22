# Сервер для выдачи ключей  

Вайпалка поддерживает три сервиса решения каптчи: [x-captcha.ru](http://x-captcha.ru), [anticaptcha.com](https://anticaptcha.com) и [captcha.guru](https://captcha.guru)  
Авторизация происходит через basic auth, в директории обращения (в примере ниже это webroot/captcha/) должны лежать соответственно 3 файла: `xcaptcha`, `anticaptcha` и `gurocaptcha`  
Можете сменить на свои названия в /src/engine/setting.py в методе `get_key`, это не принципиально  
Так же вместо тупого обращения к файлам можно написать на пыхе скрипт в пару строк и цепляться к нему. Мне пыху было ставить лень, поэтому обращение идет к файлам.  

Файл должен содержать только одну строку с ключом 32 символа. Любой лишний символ при получении провалит проверку и вайпалка забракует ключ.  
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

Не забудьте прописать свой домен в /src/engine/setting.py в методе `get_key` и заменить содержимое заглушки в `if key == "0"` (метод `set_key`):  
```
key, keyreq = self.get_key(solver, username, password)
```
