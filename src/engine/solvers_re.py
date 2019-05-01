# -*- coding: utf-8 -*-

import tools
import os
import time
import requests
import urllib3

from tools import *

urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)


# ====== x-captcha.ru ======
class CaptchaSolver_XCaptcha:

    def __init__(self, key, keyreq):
        self.api = "http://x-captcha2.ru/in.php"
        self.key = key
        try:
            if keyreq.status_code == 200:
                print("Solver 'X-Captcha' initialized!")
        except Exception:
            print("Solver 'X-Captcha' initialized with key: " + self.key)

    def solve(self, image, badproxies, forbiddenproxy, deadproxy, postsCounter):

        while True:
            task = (('key', self.key), ('method', 'userrecaptcha'), ('googlekey', '6LeQYz4UAAAAAL8JCk35wHSv6cuEV5PyLhI6IxsM'), ('pageurl', 'https://2ch.hk/b/'))
            data = requests.post(self.api, data = task, verify = False)

            response = data.text
            response = response.split("|")

            if (response[0] == "OK"):

                while True:
                    solveData = requests.get("http://x-captcha2.ru/res.php?key=" + self.key + "&id=" + response[1])
                    solveResponse = solveData.text 
                    solveResponse = solveResponse.split("|")

                    if (solveResponse[0] == "OK"):
                        Stats.incCaptchas()
                        return solveResponse[1]
                    
                    time.sleep(3)
            elif data.text == "ERROR_KEY_USER":
                print("\nKey error, exiting...")
                crash_quit("Key error!", badproxies, forbiddenproxy, deadproxy, postsCounter)
            elif data.text == "ERROR_NOT_SLOT_ZERO":
                print("\nВсе ушли бухать, ждём 30 сек...")
                time.sleep(27)
            elif data.text == "ERROR_NOT_SLOT_BUSY":
                print("\nИндуссы просят пощады, ждём 30 сек...")
                time.sleep(27)
            time.sleep(3)


# ====== captcha.guru ======
class CaptchaSolver_captchaguru:

    def __init__(self, key, keyreq):
        self.api = "http://api.captcha.guru/"
        self.key = key
        try:
            if keyreq.status_code == 200:
                print("Solver 'captcha.guru' initialized!")
        except Exception:
            print("Solver 'captcha.guru' initialized with key: " + self.key)

    def solve(self, image, badproxies, forbiddenproxy, deadproxy, postsCounter):
        task = {}
        task["type"] = "NoCaptchaTask"
        task["websiteURL"] = "https://2ch.hk/b/"
        task["websiteKey"] = "6LeQYz4UAAAAAL8JCk35wHSv6cuEV5PyLhI6IxsM"
        data = requests.post(self.api + "createTask", json = {"clientKey": self.key, "task": task}, verify = False).json()
        if (data["errorId"] == 0):
            while True:
                response = requests.post(self.api + "getTaskResult", json = {"clientKey" : self.key, "taskId" : str(data["taskId"])}, verify = False).json()
                if (response["status"] == "ready"):
                    Stats.incCaptchas()
                    return response["solution"]["gRecaptchaResponse"]
                time.sleep(3)
        elif (data["errorId"] == 1):
            if (data["errorDescription"] == "ERROR_KEY_DOES_NOT_EXIST"):
                print("\nKey revoked, exiting...")
                crash_quit("Key revoked!", badproxies, forbiddenproxy, deadproxy, postsCounter)
            elif (data["errorDescription"] == "ERROR_ZERO_BALANCE"):
                print("\nNot enough money to continue...")
                crash_quit("Not enough money to continue...", badproxies, forbiddenproxy, deadproxy, postsCounter)
            elif (data["errorDescription"] == "ERROR_NO_SLOT_AVAILABLE"):
                print("\nNot have any available workers, 10 sec timeout...")
                time.sleep(7)
            else:
                print("\nSomething fucked up, sorry. Server responce:", (data["errorDescription"]))
                crash_quit(data["errorDescription"], badproxies, forbiddenproxy, deadproxy, postsCounter)
        time.sleep(3)


# ====== anti-captcha.com ======
class CaptchaSolver_anticaptcha:

    def __init__(self, key, keyreq):
        self.api = "http://api.anti-captcha.com/"
        self.key = key
        try:
            if keyreq.status_code == 200:
                print("Solver 'anti-captcha' initialized!")
        except Exception:
            print("Solver 'anti-captcha' initialized with key: " + self.key)

    def solve(self, image, badproxies, forbiddenproxy, deadproxy, postsCounter):
        task = {}
        task["type"] = "NoCaptchaTaskProxyless"
        # пока так, позже прикручу передачу наших проксичек
        task["websiteURL"] = "https://2ch.hk/b/"
        task["websiteKey"] = "6LeQYz4UAAAAAL8JCk35wHSv6cuEV5PyLhI6IxsM"
        data = requests.post(self.api + "createTask", json = {"clientKey": self.key, "task": task}, verify = False).json()
        if (data["errorId"] == 0):
            while True:
                response = requests.post(self.api + "getTaskResult", json = {"clientKey" : self.key, "taskId" : str(data["taskId"])}, verify = False).json()
                if (response["status"] == "ready"):
                    Stats.incCaptchas()
                    return response["solution"]["gRecaptchaResponse"]
                time.sleep(3)
        elif (data["errorId"] == 1):
            if (data["errorDescription"] == "ERROR_KEY_DOES_NOT_EXIST"):
                print("\nKey revoked, exiting...")
                crash_quit("Key revoked!", badproxies, forbiddenproxy, deadproxy, postsCounter)
            elif (data["errorDescription"] == "ERROR_ZERO_BALANCE"):
                print("\nNot enough money to continue...")
                crash_quit("Not enough money to continue...", badproxies, forbiddenproxy, deadproxy, postsCounter)
            else:
                print("\nSomething fucked up, sorry. Server responce:", (data["errorDescription"]))
                crash_quit(data["errorDescription"], badproxies, forbiddenproxy, deadproxy, postsCounter)
        time.sleep(3)
