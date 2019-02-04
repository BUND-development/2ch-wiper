## -*- coding: utf-8 -*-

import time
import requests
import urllib3
urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)

from tools import *

# ====== X-капча ======
class CaptchaSolver_XCaptcha:

    def __init__(self, key, keyreq):
        self.api = "http://x-captcha2.ru/in.php"
        self.key = key
        try:
            if keyreq.status_code == 200:
                print("Solver 'X-Captcha' initialized with key: [ДАННЫЕ УДАЛЕНЫ]")
        except Exception:
            print("Solver 'X-Captcha' initialized with key: " + self.key)

    def solve(self, image, badproxies, forbiddenproxy, postsCounter):

        while True:
            task = (('key', self.key), ('method', 'userrecaptcha'), ('googlekey', '6LeQYz4UAAAAAL8JCk35wHSv6cuEV5PyLhI6IxsM'), ('pageurl', 'https://2ch.hk/b/'))
            data = requests.post(self.api, data=task, verify=False)

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
                print("\nОшибка ключа, не могу продолжать работу...")
                crash_quit("Ошибка ключа!", badproxies, forbiddenproxy, postsCounter)
            time.sleep(3)


# ====== Гуру-капча ======
class CaptchaSolver_captchaguru:

    def __init__(self, key, keyreq):
        self.api = "https://api.captcha.guru/"
        self.key = key
        try:
            if keyreq.status_code == 200:
                print("Solver 'captcha.guru' initialized with key: [ДАННЫЕ УДАЛЕНЫ]")
        except Exception:
            print("Solver 'captcha.guru' initialized with key: " + self.key)

    def solve(self, image, badproxies, forbiddenproxy, postsCounter):
        task = {}
        task["type"] = "NoCaptchaTask"
        task["websiteURL"] = "https://2ch.hk/b/"
        task["websiteKey"] = "6LeQYz4UAAAAAL8JCk35wHSv6cuEV5PyLhI6IxsM"
        data = requests.post(self.api + "createTask", json={"clientKey": self.key, "task": task}, verify=False).json()
        if (data["errorId"] == 0):
            while True:
                response = requests.post(self.api + "getTaskResult", json={"clientKey" : self.key, "taskId" : str(data["taskId"])}, verify=False).json()
                if (response["status"] == "ready"):
                    return response["solution"]["gRecaptchaResponse"]
                time.sleep(3)
        elif (data["errorId"] == 1):
            if (data["errorDescription"] == "ERROR_KEY_DOES_NOT_EXIST"):
                print("\nКлюч отозван, не могу продолжать работу...")
                crash_quit("Ключ отозван!", badproxies, forbiddenproxy, postsCounter)
            elif (data["errorDescription"] == "ERROR_ZERO_BALANCE"):
                print("\nЗакончились деньги на капче, не могу продолжать работу...")
                crash_quit("Закончились деньги!", badproxies, forbiddenproxy, postsCounter)
            elif (data["errorDescription"] == "ERROR_NO_SLOT_AVAILABLE"):
                print("\nНет свободных индуссов на сервере, таймаут 10 секунд...")
                time.sleep(7)
            else:
                print("\nПроизошла неведомая ебаная хуйня, сорян. Вот ответ от сервера:", (data["errorDescription"]))
                crash_quit(data["errorDescription"], badproxies, forbiddenproxy, postsCounter)
        time.sleep(3)


# ====== Антикапча ======
class CaptchaSolver_anticaptcha:

    def __init__(self, key, keyreq):
        self.api = "http://api.anti-captcha.com/"
        self.key = key
        try:
            if keyreq.status_code == 200:
                print("Solver 'anti-captcha' initialized with key: [ДАННЫЕ УДАЛЕНЫ]")
        except Exception:
            print("Solver 'anti-captcha' initialized with key: " + self.key)

    def solve(self, image, badproxies, forbiddenproxy, postsCounter):
        task = {}
        task["type"] = "NoCaptchaTaskProxyless"
        # пока так, позже прикручу передачу наших проксичек
        task["websiteURL"] = "https://2ch.hk/b/"
        task["websiteKey"] = "6LeQYz4UAAAAAL8JCk35wHSv6cuEV5PyLhI6IxsM"
        data = requests.post(self.api + "createTask", json={"clientKey": self.key, "task": task}, verify=False).json()
        if (data["errorId"] == 0):
            while True:
                response = requests.post(self.api + "getTaskResult", json={"clientKey" : self.key, "taskId" : str(data["taskId"])}, verify=False).json()
                if (response["status"] == "ready"):
                    return response["solution"]["gRecaptchaResponse"]
                time.sleep(3)
        elif (data["errorId"] == 1):
            if (data["errorDescription"] == "ERROR_KEY_DOES_NOT_EXIST"):
                print("\nКлюч отозван, не могу продолжать работу...")
                crash_quit("Ключ отозван!", badproxies, forbiddenproxy, postsCounter)
            elif (data["errorDescription"] == "ERROR_ZERO_BALANCE"):
                print("\nЗакончились деньги на капче, не могу продолжать работу...")
                crash_quit("Закончились деньги!", badproxies, forbiddenproxy, postsCounter)
            else:
                print("\nПроизошла неведомая ебаная хуйня, сорян. Вот ответ от сервера:", (data["errorDescription"]))
                crash_quit(data["errorDescription"], badproxies, forbiddenproxy, postsCounter)
        time.sleep(3)
