## -*- coding: utf-8 -*-

import os
import requests
import argparse
import urllib3
from requests.auth import HTTPBasicAuth
urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)

from scheme import *
from tools import *

# ====== Запись логов ======
def activate_debug(logMode):
	import logging
	print("\n*** DEBUG MODE ACTIVATED ***")
	if logMode == 1:
		logging.basicConfig(filename='LOG.txt', level=logging.DEBUG)
	elif logMode == 2:
		logging.basicConfig(level=logging.DEBUG)


# ====== Конфигурация ======
class Setup:

	def __init__(self, args):
		parser = argparse.ArgumentParser()
		parser.add_argument("-u", "--username", dest="username")  # логин
		parser.add_argument("--password", dest="password")  # пароль
		parser.add_argument("-b", "--board", dest="board")  # доска
		parser.add_argument("-t", "--thread", dest="thread") # тред (или "0", если доску)
		parser.add_argument("-c", "--chaos", dest="chaos")  # флаг хаоса и тред для постинга (-1, если без хаоса, 0, если шрапнельный хаос)
		parser.add_argument("-p", "--potocks", dest="potocksCount")  # число потоков (или 0, если 1 пост в 5 минут)
		parser.add_argument("-d", "--debug", dest="debug")  # номер вывода логов (или 0, если без них)
		parser.add_argument("-s", "--solver", dest="solver")  # номер решателя
		parser.add_argument("-k", "--key", dest="key")  # ключ (или "0" для казенного)
		parser.add_argument("-r", "--repeats", dest="proxyRepeatsCount")  # число повторов прокси
		parser.add_argument("-m", "--mode", dest="mode")  # режим вайпалки
		parser.add_argument("--minBan", dest="minBan")  # минимальный номер разбана (или -1, если не 8 режим)
		parser.add_argument("--maxBan", dest="maxBan")  # максимальный номер разбана (или -1, если не 8 режим)
		parser.add_argument("-cb", "--complainBoard", dest="complainBoard")  # доска для жалоб (или -1, если не 1 режим)
		parser.add_argument("-l", "--links", dest="linksCount")  # максимальное число ссылок в жалобах (или -1, если не 1 режим)
		parser.add_argument("-w", "--withPosts", dest="withPosts")  # флаг приписки постов к ссылкам в жалобах
		parser.add_argument("-T", "--trigger", dest="triggerForm")  # номер режима триггера (или 0, если доску или просто без него)
		parser.add_argument("-sh", "--shrapnel", dest="shrapnelCharge")  # число тредов для шрапнели (или 0, если без неё)
		parser.add_argument("-mp", "--min", dest="minPostsCount")  # минимальное число постов в тредах для шрапнели (или -1, если без неё или с указанием тредов)
		parser.add_argument("-M", "--media", dest="mediaKind")  # номер вида прикреплений (или 0, если дэ или просто без них)
		parser.add_argument("-g", "--group", dest="mediaGroup")  # подкаталог прикреплений (или ".", если из корня)
		parser.add_argument("-mc", "--medias", dest="mediasCount")  # число прикреплений (или -1, если из постов)
		parser.add_argument("-S", "--sage", dest="sageMode")  # номер режима сажи
		parser.add_argument("-SH", "--shakal", dest="shakalPower")  # уровень шакала от 0 до 100
		parser.add_argument("-C", "--color", dest="shakalColor")  # флаг цветного шакала
		parser.add_argument("-a", "--affine", dest="shakalAffine")  # флаг аффинного шакала
		parser.add_argument("-P", "--png", dest="toPNG")  # флаг конвертации в PNG с альфа-каналом
		args = parser.parse_args(args)

		self.username = args.username
		self.password = args.password
		if int(args.debug) != 0:
			activate_debug(int(args.debug))
		self.cpFile, self.bansFile, self.fullFile = self.set_encoding()  # файлы с пастами

		self.board = args.board  # доска
		self.thread = args.thread  # тред
		self.chaos = args.chaos  # хаос / тред для постинга
		self.potocksCount = int(args.potocksCount)  # число потоков
		self.TIMEOUT, self.PAUSE = self.set_consts(self.potocksCount)  # таймаут, пауза

		self.solver, self.key, self.keyreq = self.set_key(int(args.solver), args.key, args.username, args.password)  # солвер, ключ, статус ключа
		self.proxyRepeatsCount = int(args.proxyRepeatsCount)  # число повторов прокси
		self.mode, self.pastes, self.bigPaste = self.set_mode(int(args.mode))  # режим вайпалки, пасты

		if self.mode == 8:
			self.minBan = int(args.minBan) # минимальный ID бана
			self.maxBan = int(args.maxBan)  # максимальный ID бана
		elif self.mode == 1:
			self.complainBoard = args.complainBoard
			self.linksCount = int(args.linksCount)
			self.complainCatalog = Catalog(self.complainBoard)

		self.catalog = 0  # ¯\_(ツ)_/¯
		self.threads = []

		if self.thread != "0":
			self.triggerForm, self.shrapnelCharge, self.targetThread = self.set_trigger(int(args.triggerForm), int(args.shrapnelCharge), int(args.minPostsCount), args)  # режим триггера, число тредов шрапнели
		else:
			self.triggerForm = 0
			self.shrapnelCharge = 0

		self.mediaKind, self.mediaPaths, self.mediasCount = self.set_media(int(args.mediaKind), args.mediaGroup, int(args.mediasCount))  # тип прикреплений, подкаталог, число прикреплений к треду
		
		self.sageMode = int(args.sageMode) # режим сажи
		
		self.shakalPower = int(args.shakalPower)  # уровень шакала
		if args.shakalColor == "1": self.shakalColor = True  # флаг цветного шакала
		else: self.shakalColor = False
		if args.shakalAffine == "1": self.shakalAffine = True  # флаг аффинного шакала
		else: self.shakalAffine = False
		if args.toPNG == "1": self.toPNG = True  # флаг конвертации в PNG
		else: self.toPNG = False

	# === определение ОС и кодировки ===
	def set_encoding(self):
		self.complainFile = "complaints.txt"
		self.cpFile = "texts.txt"
		self.bansFile = "bans.txt"
		self.fullFile = "parasha.txt"
		return self.cpFile, self.bansFile, self.fullFile

	# === установка паузы между постами и таймаута ===
	def set_consts(self, potocksCount):
		if potocksCount == 0:
			TIMEOUT = 60
			PAUSE = 60
			self.potocksCount = 4
		else:
			TIMEOUT = 10
			PAUSE = 20
		return TIMEOUT, PAUSE

	# === получение казённого ключа ===
	def get_key(self, solver, username, password):
		if solver == 0:
			solverStr = "xcaptcha"
			print("Пытаюсь получить казеный ключ для икскаптчи...")
		elif solver == 1:
			solverStr = "gurocaptcha"
			print("Пытаюсь получить казеный ключ для гурокаптчи...")
		elif solver == 2:
			solverStr = "anticaptcha"
			print("Пытаюсь получить казеный ключ для антикапчи...")

		keyreq = requests.get('https://2ch-ri.ga/captcha/'+solverStr, auth=(username, password))
		if keyreq.status_code == 200 and len(keyreq.text) == 32:
			print("Ключ загружен!")
			key = keyreq.text
		elif keyreq.status_code == 404 or len(keyreq.text) == 0:
			print("Ключ недоступен!")
			crash_quit("Ключ недоступен!")
		elif keyreq.status_code == 401:
			print("Неверный логин/пароль!")
			crash_quit("Неверный логин/пароль!")
		else:
			print("Получен неожиданный ответ от сервера:", keyreq, keyreq.text)
			crash_quit("Получен неожиданный ответ от сервера с ключом!")
		self.set_key(solver, key, username, password)
		return key, keyreq

	# === валидация ключа ===
	def set_key(self, solver, key, username, password):
		if key == "0":
			key, keyreq = self.get_key(solver, username, password)
		elif len(key) == 32:
			print("Верифицируем ключ...")
			if solver == 0:
				keyStatus = requests.get("http://x-captcha2.ru/res.php?key=" + key + "&action=getbalance")
				if keyStatus.status_code == 200:
					if keyStatus.text == "ERROR_KEY_USER":
						print("Ключ не существует!")
						crash_quit("Ключ не существует!")
					elif keyStatus.text == "ERROR_PAUSE_SERVICE":
						print("Сервер на профилактике, используй другой солвер.")
						crash_quit("Сервер на профилактике, используй другой решатель!")
					keyxc = keyStatus.text
					keyxc = keyxc.split("|")
					print("Ключ подтверждён, ваш баланс:", keyxc[1])
				elif keyStatus.status_code == 500:
					print("Икскапча заблокировала твой IP, перезагрузи роутер!")
					crash_quit("Икскапча заблокировала твой IP, перезагрузи роутер!")

			elif solver == 1 or solver == 2:
				if solver == 1:
					keyStatus = requests.post("https://api.captcha.guru/getBalance", json={"clientKey": key}, verify=False).json()
				else:
					keyStatus = requests.post("https://api.anti-captcha.com/getBalance", json={"clientKey": key}, verify=False).json()
				if (keyStatus["errorId"] == 0):
					print("Ключ подтверждён, ваш баланс:", (keyStatus["balance"]))
				elif (keyStatus["errorId"] == 1):
					if (keyStatus["errorDescription"] == "ERROR_KEY_DOES_NOT_EXIST"):
						print("Ключ не существует!")
						crash_quit("Ключ не существует!")
					else:
						print(keyStatus["errorDescription"])
						crash_quit(keyStatus["errorDescription"])
			keyreq = 0
		else:
			print("Неправильно введен ключ!")
			crash_quit("Неправильно введен ключ!")
		return solver, key, keyreq

	# === установка режима вайпалки ===
	def set_mode(self, mode):
		if mode == 1:
			with open(self.complainFile, 'r', encoding='utf-8') as file:
				pastes = file.read()
				pastes = pastes.split("\n\n")
				bigPaste = 0
		elif mode == 4:
			with open(self.cpFile, 'r', encoding='utf-8') as file:
				pastes = file.read()
				pastes = pastes.split("\n\n")
				bigPaste = 0
		elif mode == 8:
			with open(self.bansFile, 'r', encoding='utf-8') as file:
				pastes = file.read()
				pastes = pastes.split("\n\n")
				bigPaste = 0
		elif mode == 6:
			bigPaste = ""
			with open(self.fullFile, 'r', encoding='utf-8') as file:
				govno = [row.strip() for row in file]
			bigPaste = '\xa0'.join(govno)
			bigPaste += '\xa0'
			pastes = 0
		else:
			pastes = 0
			bigPaste = 0
		return mode, pastes, bigPaste

	# === установка триггера ===
	def set_trigger(self, form, shrapnelCharge, minPostsCount, args):
		if shrapnelCharge == 0: # and self.thread > 1
			try:
				self.threads.append(Thread(self.board, self.thread, self.mode, form))
			except Exception:
				crash_quit("Тред не существует!")
		elif shrapnelCharge > 0: # and self.thread > 0
			self.catalog = Catalog(self.board)
			if minPostsCount == -1:
				for i in range(shrapnelCharge):
					self.threads.append(Thread(self.board, args[23+i], self.mode, form))
			else:
				i = 0
				for thread in self.catalog.schema["threads"]:
					if int(thread["posts_count"]) >= minPostsCount:
						self.threads.append(Thread(self.board, str(thread["num"]), self.mode, form))
						i += 1
						if i == shrapnelCharge:
							break
				shrapnelCharge = i

		if self.chaos != "-1" and self.chaos != "0":
			targetThread = Thread(self.board, self.chaos, self.mode, form)
		else:
			targetThread = self.threads[0]

		return form, shrapnelCharge, targetThread

	# === установка прикреплений ===
	def set_media(self, mediaKind, mediaGroup, mediasCount):
		mediaPaths = []
		if mediaKind != 0:
			if mediaKind > 1:
				self.TIMEOUT += 30
			if mediaKind < 3:
				if mediaKind == 1:
					mediaDir = "images"
				elif mediaKind == 2:
					mediaDir = "videos"
				if len(mediaGroup) > 0 and mediaGroup != ".":
					mediaDir += "/"
					mediaDir += mediaGroup
				for media in os.listdir("./"+mediaDir):
					if media.endswith(".jpg") or media.endswith(".png") or media.endswith(".gif") or media.endswith(".bmp") or media.endswith(".mp4") or media.endswith(".webm"):
						mediaPaths.append("./"+mediaDir+"/"+media)
				if mediasCount == 0:
					mediaKind = 0

			elif self.shrapnelCharge == 0:
				for post in self.threads[0].posts:
					for media in post.medias:
						print("Скачиваю ", media.name, "("+str(post.num)+"/"+str(self.threads[0].postsCount)+" пост)")
						media.download()
			else:
				self.TIMEOUT += 60
		else:
			mediasCount = 0
		return mediaKind, mediaPaths, mediasCount
