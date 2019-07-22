# -*- coding: utf-8 -*-

import os
import threading
import time
import urllib3
urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)

# ====== shutting down ======
def safe_quit(badproxies, forbiddenproxy, deadproxy, postsCounter, sig = 0, frame = 0):
    print("\n\nWaiting for proxy list update...\n")

    try:
        f = open("proxies.cfg", "r+")
        d = f.readlines()
        f.seek(0)

        for i in d:
            if i.rstrip() not in badproxies:
                f.write(i)

        f.truncate()
        f.close()
    except:
        pass

    d = open("forbidden.txt", "a")
    for proxy in forbiddenproxy:
        d.write(proxy + '\n')
    d.close()

    print(str((len(badproxies) - len(forbiddenproxy) - len(deadproxy))), "banned proxies cleaned!")
    print(str(len(forbiddenproxy)), "'access denied' proxies cleaned!")
    print(str(len(deadproxy)), "dead proxies cleaned!")

    data = {}
    data["posts"] = str(postsCounter)
    data["bans"] = str(len(badproxies) - len(forbiddenproxy) - len(deadproxy))
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


    print("\nSee You Space Cowboy...")
    os._exit(0)

def crash_quit (reason, badproxies = [], forbiddenproxy = [], deadproxy = [], postsCounter = 0):
    with open(".response", "w", encoding="utf-8") as file:
        file.write("crash " + reason + "\n")
    safe_quit(badproxies, forbiddenproxy, deadproxy, postsCounter)

# ====== input processing ======
def eternal_input(self, badproxies, forbiddenproxy, deadproxy, postsCounter):
    while True:
        print("Choose your option")
        choice = input("[S]tatistics, [Q]uit\n")
        choice = choice.rstrip()
        try:
            if choice.lower() == "s" or choice.lower() == "ы":
                Stats.printStats(badproxies, forbiddenproxy, deadproxy)
            elif choice.lower() == "q" or choice.lower() == "й":
                safe_quit(badproxies, forbiddenproxy, deadproxy, postsCounter)
                badproxies.clear()
                forbiddenproxy.clear()
                deadproxy.clear()
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

    def printStats(badproxies, forbiddenproxy, deadproxy):
        print("=====================================")
        print("Current threads:\t", str(threading.active_count() - 2))
        print("Proxies left:\t\t", str(Stats.numOfProxies - len(badproxies)))
        print("Captchas solved:\t", str(Stats.captchasSolved))
        print("Posts sent:\t\t", str(Stats.postsSent))
        print("Banned proxies:\t\t", str((len(badproxies) - len(forbiddenproxy) - len(deadproxy))))
        print("Denied proxies:\t\t", str(len(forbiddenproxy)))
        print("Dead proxies:\t\t", str(len(deadproxy)))
        print("=====================================\n")
