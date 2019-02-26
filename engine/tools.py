## -*- coding: utf-8 -*-

import os
import threading

try:
	import urllib3
except:
	print("\nModule \"requests\" not found, performing installation...\n")
	os.system('pip install --user requests pysocks' if os.name == 'nt' else 'pip3 install --user requests pysocks')
	try:
		import urllib3
		print("\nSuccess!")
	except:
		print("Failed to install \"requests\" module. Emergency exit...")
		input()
		os._exit()

urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)

# ====== shutting down ======
def safe_quit(badproxies, forbiddenproxy, postsCounter, sig = 0, frame = 0):
	print("\n\nWaiting for proxy list update...")

	f = open("proxies.cfg", "r+")
	d = f.readlines()
	f.seek(0)

	for i in d:
		if i.rstrip() not in badproxies:
			f.write(i)

	f.truncate()
	f.close()

	d = open("forbidden.txt", "a")
	for proxy in forbiddenproxy:
		d.write(proxy + '\n')
	d.close()

	print(str((len(badproxies) - len(forbiddenproxy))), "banned proxies cleaned!")
	print(str(len(forbiddenproxy)), "'access denied' proxies cleaned!")

	data = {}
	data["posts"] = str(postsCounter)
	data["bans"] = str(len(badproxies) - len(forbiddenproxy))
	with open(".response", "a", encoding="utf-8") as file:
		for key in data:
			file.write(key + " " + data[key] + "\n")
	
	goodProxies = Stats.goodProxies
	try:
		with open("goodproxies.txt", "r") as file:
			proxies = file.readlines()
		for proxy in proxies:
			goodProxies.append(proxy[:-1])
		goodProxies = list(set(goodProxies))
		with open("goodproxies.txt", "w") as file:
			for proxy in goodProxies:
				file.write(proxy + "\n")
	except:
		with open("goodproxies.txt", "w") as file:
			for proxy in goodProxies:
				file.write(proxy + "\n")


	print("Exiting...")
	os._exit(0)

def crash_quit (reason, badproxies = [], forbiddenproxy = [], postsCounter = 0):
	with open(".response", "w", encoding="utf-8") as file:
		file.write("crash " + reason + "\n")
	safe_quit(badproxies, forbiddenproxy, postsCounter)

# ====== input processing ======
def eternal_input(badproxies, forbiddenproxy, postsCounter):
	while True:
		print("Choose your option")
		choice = input("[S]tatistics, [Q]uit\n")
		choice = choice.rstrip()
		try:
			if choice.lower() == "s" or choice.lower() == "ы":
				Stats.printStats(badproxies, forbiddenproxy)
			elif choice.lower() == "q" or choice.lower() == "й":
				safe_quit(badproxies, forbiddenproxy, postsCounter)
				badproxies.clear()
				forbiddenproxy.clear()
			else:
				print("Undefined option!")
		except Exception as e:
			print(e)


# ====== statistics ======
class Stats:

	# should be fixed now?
	# //tsunamaru

	numOfProxies = 0
	numOfThreads = 0
	postsSent = 0
	captchasSolved = 0
	goodProxies = []

	def setProxies(amount):
		Stats.numOfProxies = amount

	def setnumOfThreads(amount):
		Stats.numOfThreads = amount

	def incCaptchas():
		Stats.captchasSolved += 1

	def incPosts():
		Stats.postsSent += 1

	def addGoodProxy(proxy):
		Stats.goodProxies.append(proxy)

	def printStats(badproxies, forbiddenproxy):
		print("=====================================")
		print("Current threads:\t", str(threading.active_count()))
		print("Proxies left:\t\t", str(Stats.numOfProxies - len(badproxies)))
		print("Captchas solved:\t", str(Stats.captchasSolved))
		print("Posts sent:\t\t", str(Stats.postsSent))
		print("Banned proxies:\t\t", str((len(badproxies) - len(forbiddenproxy))))
		print("Access denied:\t\t", str(len(forbiddenproxy)))
		print("=====================================\n")
