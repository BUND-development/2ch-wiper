## -*- coding: utf-8 -*-

import tools
import time
import sys
import threading
import io
import random
import string
import os
import signal
try:
	import requests
except:
	print("\nНе установлен модуль \"requests\". Попытаемся установить!\n")
	os.system('pip install --user requests pysocks' if os.name == 'nt' else 'pip3 install --user requests pysocks')
	try:
		import requests
		print("\nОтлично, модуль установлен! Нажми любую клавишу для продолжения...")
		input()
	except:
		tools.crash_quit("Не удалось поставить модуль \"requests\". Аварийное завершение...")
try:
	import PIL.Image
except:
	print("\nНе установлен модуль \"pillow\". Попытаемся установить!\n")
	os.system('pip install --user pillow' if os.name == 'nt' else 'pip3 install --user pillow')
	try:
		import pillow
		print("\nОтлично, модуль установлен! Нажми любую клавишу для продолжения...")
		input()
	except:
		tools.crash_quit("Не удалось поставить модуль \"pillow\". Аварийное завершение...")
import urllib3
urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)

from scheme import Catalog, Thread
from setting import Setup
import solvers_2ch
import solvers_re
from tools import *

# ====== Макросы макросики ======
CHARS = string.ascii_uppercase + string.digits
TEXT_CHARS = string.ascii_uppercase + string.digits + string.ascii_lowercase + (string.whitespace * 10) + string.punctuation
NAME_SIZE = 14
badproxies = []
forbiddenproxy = []
postsCounter = 0


# ====== Шапка ======
def show_logo():
	os.system('cls' if os.name == 'nt' else 'clear')
	print("\n***************************************** v2.6 **")
	print("*    2CH.HK WIPE MACHINE - ReCaptcha edition    *")
	print("*        Оригинальный проект: glow_stick        *")
	print("*      Быдлокод: owodelta, kobato, arelive      *")
	print("*            cryptostimor, tsunamaru            *")
	print("*************************************************")


# ====== API капчи сосача ======
class Captcha:

	def __init__(self, proxy, agent, board, thread, solver, TIMEOUT, captchaType):
		if (captchaType) == "2ch":
			self.api = "https://2ch.hk/api/captcha/2chaptcha/"
		else:
			self.api = "https://2ch.hk/api/captcha/recaptcha/id"
		self.proxy = proxy
		self.board = board
		self.thread = thread
		self.solver = solver
		self.TIMEOUT = TIMEOUT
		self.set_headers(agent)

#		os.environ["https_proxy"] = self.proxy["https"]
#		os.environ["http_proxy"] = self.proxy["http"]

		captcha = requests.get(self.api + "id?board=" + self.board + "&thread=" + self.thread, proxies=self.proxy, headers=self.headers, timeout=self.TIMEOUT, verify=False).json()
		self.id = captcha["id"]
		self.image = requests.get(self.api + "image/" + self.id, proxies=self.proxy, headers=self.headers, timeout=self.TIMEOUT, verify=False).content

	def set_headers(self, agent):
		self.headers = {}
		self.headers["Host"] = "2ch.hk"
		self.headers.update(agent)
		self.headers["Accept"] = "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
		self.headers["Accept-Language"] = "ru-RU,ru;q=0.8,en-US;q=0.5,en;q=0.3"
		self.headers["Accept-Encoding"] = "gzip, deflate, br"
		self.headers["Connection"] = "keep-alive"
		self.headers["Upgrade-Insecure-Requests"] = "1"

	def solve(self):
		global postsCounter
		print(self.proxy["http"], "solving captcha")
		self.value = self.solver.solve(self.image, badproxies, forbiddenproxy, postsCounter)
		return (None, self.id), (None, self.value)

	def verify(self):
		return requests.get(self.api + "check/" + self.id + '?value=' + self.value, proxies=self.proxy, headers=self.headers, verify=False, timeout=self.TIMEOUT).json()["result"] == 1


# ====== Постинг ======
class Post:

	def __init__(self, proxy, agent, board, thread, solver, captchaType):
		self.proxy = {"http": proxy, "https": proxy}
		self.agent = {"User-Agent": agent}
		self.board = board
		self.thread = thread
		self.solver = solver
		self.captchaType = captchaType
		self.buffer = {}
		self.params = []
		self.params.append(("task", (None, "post")))
		self.params.append(("board", (None, self.board)))
		self.params.append(("thread", (None, self.thread)))
		self.params.append(("usercode", (None, "")))
		self.params.append(("code", (None, "")))
		if self.captchaType == "2ch":
			self.params.append(("captcha_type", (None, "2chaptcha")))
		else:
			self.params.append(("captcha_type", (None, "recaptcha")))
		self.params.append(("oekaki_image", (None, "")))
		self.params.append(("oekaki_metadata", (None, "")))
#		self.params["g-recaptcha-response"] = (None, "")
#		self.params["formimages"] = []

		self.set_headers()
#		self.set_subject("Ответ")

	def set_headers(self):
		self.headers = {}
		self.headers["Host"] = "2ch.hk"
		self.headers.update(self.agent)
		self.headers["Accept"] = "application/json, text/javasctipt, */*; q=0.01"
		self.headers["Accept-Language"] = "ru-RU,ru;q=0.8,en-US;q=0.5,en;q=0.3"
		self.headers["Accept-Encoding"] = "gzip, deflate, br"
		self.headers["Referer"] = str("https://2ch.hk/" + self.board + "/res/" + self.thread + ".html")
		self.headers["X-Requested-With"] = "XMLHttpRequest"
		self.headers["Content-Length"] = str(15000000 + random.randint(0, 100))
		self.headers["Cookie"] = ""
		self.headers["Connection"] = "keep-alive"

	def prepare(self, TIMEOUT):
		try:
			self.buffer["chaptcha_id"], self.buffer["chaptcha_value"] = Captcha(self.proxy, self.agent, self.board, self.thread, self.solver, TIMEOUT, self.captchaType).solve()
			if self.captchaType == "2ch":
				self.params.append(("2chaptcha_id", self.buffer["chaptcha_id"]))
				self.params.append(("2chaptcha_value", self.buffer["chaptcha_value"]))
			else:
				self.params.append(("2chaptcha_id", self.buffer["chaptcha_id"]))
				self.params.append(("g-recaptcha-response", self.buffer["chaptcha_value"]))
			print(self.proxy["http"], "solved")
			return True
		except Exception as e:
			#print(e)
			print("У проксички "+self.proxy["http"]+" вышел таймаут.")
			return False

	def set_subject(self, text):
		self.params.append(("subject", (None, text)))

	def set_text(self, text):
		self.params.append(("comment", (None, text)))
		
	def set_sage(self):
		self.params.append(("email", (None, "sage")))
		self.params.append(("sage", (None, "on")))

	def set_image(self, file_name, randName, shakalPower=0, shakalColor=False, shakalAffine=False, toPNG=False):
		image = self.shakal(file_name, shakalPower, shakalColor, shakalAffine, toPNG)
		if randName:
			file_name_displayed = str(''.join(str(random.randint(0, 9)) for _ in range(NAME_SIZE-1)) + "0")
			if toPNG: file_name_displayed += ".png"
			else: file_name_displayed += ".jpg"
		else:
			file_name_displayed = file_name[file_name.rfind('/')+1:]
		self.params.append(("formimages[]", (file_name_displayed, image, "image/jpeg")))

	def set_video(self, file_name, randName):
		if file_name.find(".mp4") != -1: ext = "mp4"
		elif file_name.find(".webm") != -1: ext = "webm"

		video = open(file_name, "rb")
		video_bytes = video.read()
		video.close()

		if randName:
			file_name_displayed = str(''.join(str(random.randint(0, 9)) for _ in range(NAME_SIZE-1)) + "0")
			file_name_displayed += str("." + ext)
		else:
			file_name_displayed = file_name[file_name.rfind('/')+1:]
		self.params.append(("formimages[]", (file_name_displayed, video_bytes, str("video/" + ext))))

	def set_media(self, mediaName, media, randName, shakalPower=0, shakalColor=False, shakalAffine=False, toPNG=False):
		if randName:
			file_name_displayed = str(''.join(str(random.randint(0, 9)) for _ in range(NAME_SIZE-1)) + "0")
		else:
			file_name_displayed = mediaName

		if mediaName.find(".jpg") != -1 or mediaName.find(".png") != -1 or mediaName.find(".gif") != -1 or mediaName.find(".bmp") != -1:
			media = self.shakal(io.BytesIO(media), shakalPower, shakalColor, shakalAffine, toPNG)

			if toPNG:
				mediaType = "image/png"
				file_name_displayed += ".png"
			else:
				mediaType = "image/jpeg"
				file_name_displayed += ".jpg"

		elif mediaName.find(".mp4") != -1:
			mediaType = "video/mp4"
			file_name_displayed += ".mp4"
		elif mediaName.find(".webm") != -1:
			mediaType = "video/webm"
			file_name_displayed += ".webm"
		else:
			mediaType = "image"
			file_name_displayed += ".jpg"

		self.params.append(("formimages[]", (file_name_displayed, media, mediaType)))

	def shakal(self, image, power, color=False, affine=False, toPNG=False):
		image = PIL.Image.open(image).convert("RGBA")
		width, height = image.size
		for x in range(power): image.putpixel((random.randint(0, width-1), random.randint(0, height-1)), (random.randint(0, 255), random.randint(0, 255), random.randint(0, 255)))

		if color:
			red = random.randint(0, 255)
			dif = abs(red - 127)
			green = random.randint(0, dif)
			if random.randint(0, 1): green = 255 - green
			blue = random.randint(max(0, 255-red-green), min(255, 510-red-green))
			if random.randint(0, 1): red, green = green, red
			if random.randint(0, 1): red, blue = blue, red
			if random.randint(0, 1): blue, green = green, blue
			layer = PIL.Image.new("RGBA", image.size, (red, green, blue, 65))
			image.paste(layer, (0, 0), layer)
		if affine:
			shakal = (1 + random.randint(0, 100)/1000 - 0.05, 0 + random.randint(0, 100)/1000 - 0.05, 1 + random.randint(0, 100)/1000 - 0.05, 0 + random.randint(0, 100)/1000 - 0.05, 1 + random.randint(0, 100)/1000 - 0.05, 0 + random.randint(0, 100)/1000 - 0.05)
			image = image.transform(image.size, PIL.Image.AFFINE, shakal, resample=PIL.Image.BICUBIC)

		image = image.crop((random.randint(0, power), random.randint(0, power), width-1 - random.randint(0, power), height-1 - random.randint(0, power)))
		if toPNG:
			image_bytes = io.BytesIO()
			image.save(image_bytes, "PNG", quality=20 + random.randint(10, 30), optimize=bool(random.getrandbits(1)), progressive=bool(random.getrandbits(1)))
		else:
			image = image.convert("RGB")
			image_bytes = io.BytesIO()
			image.save(image_bytes, "JPEG", quality=60 + random.randint(10, 30), optimize=bool(random.getrandbits(1)), progressive=bool(random.getrandbits(1)))
		image.close()
		return image_bytes.getvalue()

	def send(self, TIMEOUT, PAUSE):
		response = {}
		try:
			print(self.proxy["http"], "posting")
			wait_time = random.randint(6, 15)
			time.sleep(PAUSE)
			response = requests.post("https://2ch.hk/makaba/posting.fcgi?json=1", files=self.params, proxies=self.proxy, headers=self.headers, timeout=TIMEOUT, verify=False).json()
			Stats.incPosts()
			Stats.printStats(badproxies, forbiddenproxy)
			return response['Status'] == 'OK', response
		except Exception as e:
			#print(e)
			return False, response


# ====== Вайпалка ======
class Wiper:

	def __init__(self, setup, catalog, threads):
		print("\nЗА БУНД И САГУ, ЗА ЧЕСТЬ И ОТВАГУ!")
		self.proxies = [proxy[:-1] for proxy in open("proxies.cfg", "r").readlines()]
		if len(self.proxies) == 0:
			crash_quit("Нет проксей в файле proxy!")
		random.shuffle(self.proxies)
		Stats.setProxies(len(self.proxies))
		self.agents = [agent[:-1] for agent in open("engine/useragents.cfg").readlines()]
		self.board = setup.board
		self.thread = setup.thread
		self.setup = setup
		self.catalog = catalog
		self.threads = []
		if (self.thread == "0"):
			self.threads.append(Thread(self.setup.board, "0", self.setup.mode, 0))
		else:
			self.threads = threads

#		self.set_solver(setup.solver)
		self.captchaType = "re"
		if setup.solver == 0:
			self.solver = solvers_re.CaptchaSolver_XCaptcha(self.setup.key, self.setup.keyreq)
		elif setup.solver == 1:
			self.solver = solvers_re.CaptchaSolver_captchaguru(self.setup.key, self.setup.keyreq)
		elif setup.solver == 2:
			self.solver = solvers_re.CaptchaSolver_anticaptcha(self.setup.key, self.setup.keyreq)

	def set_solver(self, solver):
		captcha = requests.get("https://2ch.hk/api/captcha/2chaptcha/id?board=b&thread=0", headers={"User-Agent": self.agents[0]}, timeout=self.setup.TIMEOUT, verify=False).json()
		captchaID = captcha["id"]
		image = requests.get("https://2ch.hk/api/captcha/2chaptcha/image/" + captchaID, headers={"User-Agent": self.agents[0]}, timeout=self.setup.TIMEOUT, verify=False).content
		error = open("engine/error.gif","rb")

		if image == error.read():
			self.captchaType = "re"
			if solver == 0:
				self.solver = solvers_re.CaptchaSolver_XCaptcha(self.setup.key, self.setup.keyreq)
			elif solver == 1:
				self.solver = solvers_re.CaptchaSolver_captchaguru(self.setup.key, self.setup.keyreq)
			elif solver == 2:
				self.solver = solvers_re.CaptchaSolver_anticaptcha(self.setup.key, self.setup.keyreq)
		else:
			self.captchaType = "2ch"
			if solver == 0:
				self.solver = solvers_2ch.CaptchaSolver_XCaptcha(self.setup.key, self.setup.keyreq)
			elif solver == 1:
				self.solver = solvers_2ch.CaptchaSolver_captchaguru(self.setup.key, self.setup.keyreq)
			elif solver == 2:
				self.solver = solvers_2ch.CaptchaSolver_anticaptcha(self.setup.key, self.setup.keyreq)

	def trap_replace(self, text):
		if bool(random.getrandbits(1)):
			text = text.replace("А", "A").replace("а", "a")
		if bool(random.getrandbits(1)):
			text = text.replace("Е", "E").replace("е", "e")
		if bool(random.getrandbits(1)):
			text = text.replace("О", "O").replace("о", "o")
		if bool(random.getrandbits(1)):
			text = text.replace("Р", "P").replace("р", "p")
		if bool(random.getrandbits(1)):
			text = text.replace("С", "C").replace("с", "c")
		if bool(random.getrandbits(1)):
			text = text.replace("Х", "X").replace("х", "x")
		if bool(random.getrandbits(1)):
			text = text.replace("В", "B")
		if bool(random.getrandbits(1)):
			text = text.replace("М", "M")
		if bool(random.getrandbits(1)):
			text = text.replace("Н", "H")
		if bool(random.getrandbits(1)):
			text = text.replace("Т", "T")
		if bool(random.getrandbits(1)):
			text = text.replace("у", "y")
		return text

	def send_post(self):
		global postsCounter
		if (len(self.proxies) == 0): return False
		proxy = self.proxies.pop(0)
		agent = random.choice(self.agents)
		response = {"Error": "proxy"}

		counter = 0

		try:
			#will quit the thread if exception is here. Fixed now.
			while counter < self.setup.proxyRepeatsCount:
				if self.setup.shrapnelCharge == 0:
					threadNum = 0
					if (self.setup.chaos == "-1"):
						thread = self.threads[0]
					else:
						thread = self.setup.targetThread
				else:
					threadNum = random.randint(0, self.setup.shrapnelCharge-1)
					if (self.setup.chaos == "-1"):
						thread = self.threads[threadNum]
					elif (self.setup.chaos == "0"):
						thread = self.threads[random.randint(0, len(self.threads)-1)]
					else:
						thread = self.setup.targetThread

				post = Post(proxy, agent, self.board, thread.ID, self.solver, self.captchaType)

				if (post.prepare(self.setup.TIMEOUT)):
					charnum = random.randint(1, 100)
					if self.thread != "0":
						black_anus = random.randint(1, len(thread.posts)-1)  # номер поста для триггера
						white_anus = random.randint(0, len(self.threads[threadNum].posts)-1)  # номер поста для дублирования
						if self.setup.mode == 7 and self.setup.mediaKind == 0 and len(self.threads[threadNum].posts[white_anus].comment) == 0:
							for i in range(10):
								white_anus = random.randint(0, len(self.threads[threadNum].posts)-1)
								if len(self.threads[threadNum].posts[white_anus].comment) != 0: break  # это КОСТЬIЛЬ

					# === берём сначала триггер ===
					if self.setup.triggerForm == 1:
						trigger = ">>" + thread.lastID + '\n'
					elif self.setup.triggerForm == 2:
						trigger = ">>" + thread.posts[black_anus].ID + '\n'
					elif self.setup.triggerForm == 3:
						trigger = thread.loaf + '\n'
					elif self.setup.triggerForm == 4:
						trigger = ">>" + thread.posts[0].ID + '\n'
					elif self.setup.triggerForm == 0:
						trigger = ""

					# === потом текст поста ===
					if self.setup.mode == 1:
						text = self.setup.pastes[random.randint(0, len(self.setup.pastes)-1)] + '\n'
						for i in range(random.randint(1, self.setup.linksCount)):
							text += ("https://2ch.hk/" + self.setup.complainBoard + "/res/" + self.setup.complainCatalog.schema["threads"][random.randint(0, len(self.setup.complainCatalog.schema["threads"]))]["num"] + '\n')
						post.set_text(trigger + text)
					elif self.setup.mode == 2:
						post.set_text(trigger)
					elif self.setup.mode == 0:
						post.set_text(trigger + ''.join(random.choice(TEXT_CHARS) for _ in range(charnum)))
					elif self.setup.mode == 3:
						post.set_text(trigger + ("> " + thread.posts[black_anus].comment).replace("\n", "\n> ").replace("\n> \n", "\n\n"))
					elif self.setup.mode == 4:
						post.set_text(trigger + self.trap_replace(self.setup.pastes[random.randint(0, len(self.setup.pastes)-1)]))
					elif self.setup.mode == 5:
						post.set_text(trigger + "https://2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J2ch.hk/b/res/MZx7W05J")
					elif self.setup.mode == 6:
						post.set_text(trigger + self.trap_replace(self.setup.bigPaste))
					elif self.setup.mode == 7:
						post.set_text(trigger + self.trap_replace(self.threads[threadNum].posts[white_anus].comment))
					elif self.setup.mode == 8:
						post.set_text(trigger + self.setup.pastes[random.randint(0, len(self.setup.pastes)-1)].replace("BANNUMBER", str(random.randint(self.setup.minBan, self.setup.maxBan))))

					# === до 4 прикреплений ===
					if self.setup.mediaKind > 0:
						try:
							if self.setup.mediaKind == 3:
								if self.setup.shrapnelCharge != 0:
									mediasCount = min(len(self.threads[threadNum].posts[white_anus].medias), 4)
									for media in self.threads[threadNum].posts[white_anus].medias:
										print("Скачиваю", media.name, "("+str(white_anus+1)+" пост "+self.threads[threadNum].ID+" треда)")
										media.download()
								else:
									mediasCount = min(len(self.threads[threadNum].posts[white_anus].medias), 4)
							else:
								mediasCount = self.setup.mediasCount

							for mediaNum in range(mediasCount):
								if self.setup.mediaKind != 3:
									blue_anus = random.randint(0, len(self.setup.mediaPaths)-1)  # номер пикчи или видео с диска
								if self.setup.mediaKind == 1:
									post.set_image(self.setup.mediaPaths[blue_anus], self.setup.randMediaName, self.setup.shakalPower, self.setup.shakalColor, self.setup.shakalAffine, self.setup.toPNG)
								elif self.setup.mediaKind == 2:
									post.set_video(self.setup.mediaPaths[blue_anus], self.setup.randMediaName)
								elif self.setup.mediaKind == 3:
									post.set_media(self.threads[threadNum].posts[white_anus].medias[mediaNum].name, self.threads[threadNum].posts[white_anus].medias[mediaNum].file, self.setup.randMediaName, self.setup.shakalPower, self.setup.shakalColor, self.setup.shakalAffine, self.setup.toPNG)
						except Exception as e:
							print(e)
							print("Не могу скачать / прикрепить файл.")

					# === и сажу туды ===
					if self.setup.sageMode == 2:
						if self.threads[threadNum].posts[white_anus].sage == True:
							post.set_sage()
						else:
							post.params.append(("email", (None, "")))
					elif self.setup.sageMode == 1:
						post.set_sage()
					elif self.setup.sageMode == 0:
						post.params.append(("email", (None, "")))

					success, response = post.send(self.setup.TIMEOUT, self.setup.PAUSE)
					if success:
						Stats.incPosts()
						Stats.addGoodProxy(proxy)
						post_id = 0
						postsCounter += 1
						try:
							post_id = response["Target"]
						except Exception:
							post_id = response["Num"]
						if self.setup.shrapnelCharge == 0:
							print(proxy+" - success. Post id: "+str(post_id))
						else:
							print(proxy+" - success. Post id: "+str(post_id)+" ("+thread.ID+" thread)")
						if self.setup.thread != "0":
							self.threads[threadNum].lastID = str(post_id)

						print(str(self.setup.proxyRepeatsCount-counter)+" LOOPS LEFT")
						counter += 1
					else:
						print(proxy, "posting failed -", response)

						try:
							if response["Error"] == -6:
								print("Забанили, суки... "+proxy+" в плохой лист")
								badproxies.append(proxy)
								if len(self.proxies) == 0:
									print("Закончились проксички!")
									counter = self.setup.proxyRepeatsCount
								else:
									proxy = self.proxies.pop(0)
									counter = 0
							elif response["Error"] == -4:
								print("Заносим "+proxy+" в forbidden.txt")
								badproxies.append(proxy)
								forbiddenproxy.append(proxy)
								if len(self.proxies) == 0:
									print("Закончились проксички!")
									counter = self.setup.proxyRepeatsCount
								else:
									proxy = self.proxies.pop(0)
									counter = 0
							elif response["Error"] == -7:
								if self.setup.shrapnelCharge == 0:
									print("Моча вычищает тред. КОНЧАЮ.")
									safe_quit(badproxies, forbiddenproxy, postsCounter)
								else:
									print("Тред "+self.threads[threadNum].ID+" закрылся.")
									del self.threads[threadNum]
									if len(self.threads) == 0:
										print("Все треды закрыты. Це перемога.")
										safe_quit(badproxies, forbiddenproxy, postsCounter)
							elif not response:
								print("Ошибка сети, пробуем ещё раз...")

						except Exception:
							if len(response) > 0:
								print("Таймаут... "+proxy+" в плохой лист")
								badproxies.append(proxy)
								if len(self.proxies) == 0:
									print("Закончились проксички!")
									counter = self.setup.proxyRepeatsCount
								else:
									proxy = self.proxies.pop(0)
									counter = 0
			if not proxy in badproxies:
				self.proxies.append(proxy)

		except Exception as e:
			print(e)
#			badproxies.append(proxy)

		return True

	def wipe(self, thread_count):
		global postsCounter
		Stats.setnumOfThreads(thread_count)

		class WiperThread(threading.Thread):

			def __init__(self, wiper):
				threading.Thread.__init__(self)
				self.wiper = wiper

			def run(self):
				self.wiper.send_post()

		class InputThread(threading.Thread):

			def __init__(self):
				threading.Thread.__init__(self)

			def run(self):
				Stats.printStats(badproxies, forbiddenproxy)
				eternal_input(badproxies, forbiddenproxy, postsCounter)

		threads = []
		inthr = InputThread()
		inthr.start()
		for i in range(thread_count):
			threads.append(WiperThread(self))
			threads[-1].start()

		for i in range(len(threads)):
			threads[i].join()

show_logo()

try:
	setup = Setup(sys.argv[1:])
	WiperObj = Wiper(setup, setup.catalog, setup.threads)
	signal.signal(signal.SIGINT, safe_quit)
	WiperObj.wipe(setup.potocksCount)

	safe_quit(badproxies, forbiddenproxy, postsCounter)

except Exception as e:
	crash_quit("Потеряна связь с интернетами, проверь соединение. А может, погроммисты обосрались, тогда дрочи в присядку.")

