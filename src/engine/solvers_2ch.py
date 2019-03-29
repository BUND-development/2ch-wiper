## -*- coding: utf-8 -*-

import tools
import base64
import os
import time
import requests
import urllib3

from tools import *

urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)

# ====== captcha.guru ======
class CaptchaSolver_captchaguru:

    def __init__(self, key, keyreq):
        self.api = "https://api.captcha.guru/"
        self.key = key
        try:
            if keyreq.status_code == 200:
                print("Solver 'captcha.guru' initialized!")
        except Exception:
            print("Solver 'captcha.guru' initialized with key: " + self.key)

    def solve(self, image, badproxies, forbiddenproxy, deadproxy, postsCounter):
        task = {}
        task["type"] = "ImageToTextTask"
        task["body"] = base64.b64encode(image).decode("utf-8")
        task["phrase"] = False
        task["case"] = False
        task["numeric"] = 1
        task["math"] = False
        task["minLength"] = 6
        task["maxLength"] = 6
        data = requests.post(self.api + "createTask", json = {"clientKey": self.key, "task": task}, verify = False).json()
        if (data["errorId"] == 0):
            while True:
                response = requests.post(self.api + "getTaskResult", json = {"clientKey" : self.key, "taskId" : str(data["taskId"])}, verify = False).json()
                if (response["status"] == "ready"):
                    Stats.incCaptchas()
                    return response["solution"]["text"]
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
        task["type"] = "ImageToTextTask"
        task["body"] = base64.b64encode(image).decode("utf-8")
        task["phrase"] = False
        task["case"] = False
        task["numeric"] = 1
        task["math"] = False
        task["minLength"] = 6
        task["maxLength"] = 6
        data = requests.post(self.api + "createTask", json = {"clientKey": self.key, "task": task}, verify = False).json()
        if (data["errorId"] == 0):
            while True:
                response = requests.post(self.api + "getTaskResult", json = {"clientKey" : self.key, "taskId" : str(data["taskId"])}, verify = False).json()
                if (response["status"] == "ready"):
                    Stats.incCaptchas()
                    return response["solution"]["text"]
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
