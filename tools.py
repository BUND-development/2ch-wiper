## -*- coding: utf-8 -*-

import threading
import os
import urllib3
urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)

# ====== Отключение ======
def safe_quit(badproxies, forbiddenproxy, postsCounter, sig=0, frame=0):
	print("\n\nЖду, пока обновится лист с проксичками...")

	f = open("proxies", "r+")
	d = f.readlines()
	f.seek(0)

	for i in d:
		if i.rstrip() not in badproxies:
			f.write(i)

	f.truncate()
	f.close()

	d = open("forbidden.csv", "a")
	for proxy in forbiddenproxy:
		d.write(proxy + '\n')
	d.close()

	print(str((len(badproxies) - len(forbiddenproxy))), "забаненых проксичек почищено!")
	print(str(len(forbiddenproxy)), "запрещенных проксичек почищено!")

	data = {}
	data["posts"] = str(postsCounter)
	data["bans"] = str(len(badproxies) - len(forbiddenproxy))
	with open(".responce", "a") as file:
		for key in data:
			file.write(key+" "+data[key]+"\n")

	print("Выключаюсь...")
	os._exit(0)

def crash_quit (reason, badproxies=[], forbiddenproxy=[], postsCounter=0):
	with open(".responce", "w") as file:
		file.write("crash "+reason+"\n")
	safe_quit(badproxies, forbiddenproxy, postsCounter)

# ====== Обработка клавиш ======
def eternal_input(badproxies, forbiddenproxy, postsCounter):
	while True:
		print("Choose your option")
		choice = input("[S]tatistics, [Q]uit, [C]lear parasha\n")
		choice = choice.rstrip()
		try:
			if choice.lower() == "s" or choice.lower() == "ы":
				Stats.printStats(badproxies, forbiddenproxy)
			elif choice.lower() == "q" or choice.lower() == "й":
				safe_quit(badproxies, forbiddenproxy, postsCounter)
				badproxies.clear()
				forbiddenproxy.clear()
			elif choice.lower() == "c" or choice.lower() == "с":
				badproxies.clear()
				print("Параша почищена")
			else:
				print("Ты пишешь хуйню")
		except Exception as e:
			print(e)


# ====== Стата ======
class Stats:

	numOfProxies = 0
	numOfThreads = 0
	postsSent = 0
	captchasSolved = 0

	def setProxies(amount):
		Stats.numOfProxies = amount

	def setnumOfThreads(amount):
		Stats.numOfThreads = amount

	def incCaptchas():
		Stats.captchasSolved += 1

	def incPosts():
		Stats.postsSent += 1

	def printStats(badproxies, forbiddenproxy):
		print("=====================================")
		print("Проксичек осталось:\t", str(Stats.numOfProxies - len(badproxies)))
		print("Начальные потоки:\t", str(Stats.numOfThreads))
		print("Каптч решено:\t\t", str(Stats.captchasSolved))
		print("Забаненые проксички:\t", str((len(badproxies) - len(forbiddenproxy))))
		print("Доступ запрещен:\t", str(len(forbiddenproxy)))
		print("Текущие потоки:\t\t", str(threading.active_count()))
		#print("Создано тредов/постов: ", posti, "\n")
		if threading.active_count() <= 2:
			print("ALL THREADS FINISHED, PRESS \"Q\"")
		print("=====================================\n")

