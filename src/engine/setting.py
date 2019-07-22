# -*- coding: utf-8 -*-

import tools
import argparse
import os
import requests
import urllib3
import random
from requests.auth import HTTPBasicAuth

from scheme import *
from tools import *

urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)


# ====== logging ======
def activate_debug(logMode):
    import logging
    print("\n*** DEBUG MODE ACTIVATED ***")
    if logMode == 1:
        logging.basicConfig(filename='LOG.txt', level=logging.DEBUG)
    elif logMode == 2:
        logging.basicConfig(level=logging.DEBUG)


# ====== config ======
class Setup:

    def __init__(self, args):
        parser = argparse.ArgumentParser()
        parser.add_argument("-u", "--username", dest="username")  # login for getting key from server
        parser.add_argument("--password", dest="password")  # pass for auth on server
        parser.add_argument("-b", "--board", dest="board")  # board index
        parser.add_argument("-t", "--thread", dest="thread") # thread number or "0" if need to create threads
        parser.add_argument("-c", "--chaos", dest="chaos")  # chaos flag and thread for posting (-1 if without chaos, 0 if shrapnel active)
        parser.add_argument("-p", "--potocks", dest="potocksCount")  # computing thread count (or 0 for 1 post in 5 minutes)
        parser.add_argument("-d", "--debug", dest="debug")  # logging (0 if without logging)
        parser.add_argument("-s", "--solver", dest="solver")  # anticaptcha solver
        parser.add_argument("-k", "--key", dest="key")  # key (0 if using server key)
        parser.add_argument("-r", "--repeats", dest="proxyRepeatsCount")  # proxy loops
        parser.add_argument("-m", "--mode", dest="mode")  # wiper mode
        parser.add_argument("--minBan", dest="minBan")  # [deprecated] min ban number (-1 if not 8 mode)
        parser.add_argument("--maxBan", dest="maxBan")  # [deprecated] max ban number (-1 if not 8 mode)
        parser.add_argument("-cb", "--complainBoard", dest="complainBoard")  # [deprecated] board for reporting (-1 if not 1 mode)
        parser.add_argument("-l", "--links", dest="linksCount")  # [deprecated] max links count in reports (-1 if not 1 mode)
        parser.add_argument("-w", "--withPosts", dest="withPosts")  # [deprecated] write post links in report messages
        parser.add_argument("-T", "--trigger", dest="triggerForm")  # trigger mode (0 if without trigger and/or creating threads)
        parser.add_argument("-sh", "--shrapnel", dest="shrapnelCharge")  # shrapnel threads count (0 if not needed)
        parser.add_argument("-mp", "--min", dest="minPostsCount")  # min posts count in threads for shrapnel mode (-1 if shrapnel not needed)
        parser.add_argument("-M", "--media", dest="mediaKind")  # attachmets type (0 if not needed or wiping /d/)
        parser.add_argument("-g", "--group", dest="mediaGroup")  # subfolder (or just "." if attachments in root directory)
        parser.add_argument("-mc", "--medias", dest="mediasCount")  # attachments count, up to 4 (-1 for take attachments from posts)
        parser.add_argument("-S", "--sage", dest="sageMode")  # SAGE mode
        parser.add_argument("-rn", dest="randMediaName")  # randomize file name or not
        parser.add_argument("-SH", "--shakal", dest="shakalPower")  # distort level for images (from 0 to 100)
        parser.add_argument("-C", "--color", dest="shakalColor")  # colorize attached images
        parser.add_argument("-a", "--affine", dest="shakalAffine")  # affine transformation for images
        parser.add_argument("-P", "--png", dest="toPNG")  # convert to PNG before posting
        args = parser.parse_args(args)

        self.username = args.username
        self.password = args.password
        if int(args.debug) != 0:
            activate_debug(int(args.debug))
        self.cpFile, self.bansFile, self.fullFile = self.set_encoding()  # file with copypastes

        self.board = args.board  # board index
        self.thread = args.thread  # thread
        self.chaos = args.chaos  # chaos / thread for posting
        self.potocksCount = int(args.potocksCount)  # computing threads count
        self.TIMEOUT, self.PAUSE = self.set_consts(self.potocksCount)  # timeout and pause

        self.solver, self.key, self.keyreq = self.set_key(int(args.solver), args.key, args.username, args.password)  # anticaptcha solver, API key and status
        self.proxyRepeatsCount = int(args.proxyRepeatsCount)  # proxy loops
        self.mode, self.pastes, self.bigPaste = self.set_mode(int(args.mode))  # wiper mode, copypastes

        if self.mode == 8:
            self.minBan = int(args.minBan) # min ban ID
            self.maxBan = int(args.maxBan)  # max ban ID
        elif self.mode == 1:
            self.complainBoard = args.complainBoard
            self.linksCount = int(args.linksCount)
            self.complainCatalog = Catalog(self.complainBoard)

        self.catalog = 0  # ¯\_(ツ)_/¯
        self.threads = []

        if self.thread != "0":
            self.triggerForm, self.shrapnelCharge, self.targetThread = self.set_trigger(int(args.triggerForm), int(args.shrapnelCharge), int(args.minPostsCount), args)  # trigger mode, shrapnel threads count
        else:
            self.triggerForm = 0
            self.shrapnelCharge = 0

        self.mediaKind, self.mediaPaths, self.mediasCount = self.set_media(int(args.mediaKind), args.mediaGroup, int(args.mediasCount))  # attachments type, subfolder, attachments count
        
        self.sageMode = int(args.sageMode) # SAGE mode

        if args.randMediaName == "1": self.randMediaName = True  # randomize attachments name
        else: self.randMediaName = False
        
        self.shakalPower = int(args.shakalPower)  # distortion level
        if args.shakalColor == "1": self.shakalColor = True  # colorization
        else: self.shakalColor = False
        if args.shakalAffine == "1": self.shakalAffine = True  # affine transform
        else: self.shakalAffine = False
        if args.toPNG == "1": self.toPNG = True  # convert to PNG
        else: self.toPNG = False

    # === earlier here was OS detection and codepage setting, now there just filenames ===
    def set_encoding(self):
        self.complainFile = "complaints.txt" # deprecated
        self.cpFile = "texts.txt"
        self.bansFile = "bans.txt" # deprecated
        self.fullFile = "parasha.txt" # deprecated
        return self.cpFile, self.bansFile, self.fullFile

    # === setting pause between posts and timeout ===
    def set_consts(self, potocksCount):
        if potocksCount == 0:
            TIMEOUT = 60
            PAUSE = random.randint(15, 20)
            self.potocksCount = 4
        else:
            TIMEOUT = 60
            PAUSE = random.randint(15, 20)
        return TIMEOUT, PAUSE

    # === getting key from server ===
    def get_key(self, solver, username, password):
        if solver == 0:
            solverStr = "xcaptcha"
            print("Trying to get key for x-captcha.ru service...")
        elif solver == 1:
            solverStr = "gurocaptcha"
            print("Trying to get key for captcha.guru service...")
        elif solver == 2:
            solverStr = "anticaptcha"
            print("Trying to get key for anti-captcha.com service...")

        keyreq = requests.get('https://example.com/captcha/' + solverStr, auth=(username, password))
        if keyreq.status_code == 200 and len(keyreq.text) == 32:
            print("Key loaded!")
            key = keyreq.text
        elif keyreq.status_code == 404 or len(keyreq.text) == 0:
            print("Authorization was successful, but key not found!")
            print("Please contact @tsunamaru for more details.")
            crash_quit("Authorization was successful, but key not found! Please contact @tsunamaru for more details.")
        elif keyreq.status_code == 401:
            print("Incorrect login or password!")
            crash_quit("Incorrect login or password!")
        else:
            print("Unexpected server response:", keyreq, keyreq.text)
            print("Please contact @tsunamaru for more details.")
            crash_quit("Unexpected server response. Please contact @tsunamaru for more details.")
        self.set_key(solver, key, username, password)
        return key, keyreq

    # === key validation ===
    def set_key(self, solver, key, username, password):
        if key == "0":
            print("Sorry, keyserver was shutted down 15/07/2019")
            print("You may consider to setup self-hosted solution, if you really want...")
            print("For additional details, see /docs/keyserver.md")
            crash_quit("Keyserver unavailable")
        elif len(key) == 32:
            print("Waiting for key verify...")
            if solver == 0:
                keyStatus = requests.get("http://x-captcha2.ru/res.php?key=" + key + "&action=getbalance")
                if keyStatus.status_code == 200:
                    if keyStatus.text == "ERROR_KEY_USER":
                        print("Incorrect key!")
                        crash_quit("Incorrect key!")
                    elif keyStatus.text == "ERROR_PAUSE_SERVICE":
                        print("Server on maintenance, please switch to another solver or wait few minutes and retry.")
                        crash_quit("Server on maintenance, please switch to another solver or wait few minutes and retry.")
                    keyxc = keyStatus.text
                    keyxc = keyxc.split("|")
                    print("Key confirmed, your balance:", keyxc[1])
                elif keyStatus.status_code == 500:
                    print("X-captcha blocked your IP, restart router or change VPN!")
                    crash_quit("X-captcha blocked your IP, restart router or change VPN!")

            elif solver == 1 or solver == 2:
                if solver == 1:
                    keyStatus = requests.post("http://api.captcha.guru/getBalance", json={"clientKey": key}, verify = False).json()
                else:
                    keyStatus = requests.post("http://api.anti-captcha.com/getBalance", json={"clientKey": key}, verify = False).json()
                if (keyStatus["errorId"] == 0):
                    print("Key confirmed, your balance:", (keyStatus["balance"]))
                elif (keyStatus["errorId"] == 1):
                    if (keyStatus["errorDescription"] == "ERROR_KEY_DOES_NOT_EXIST"):
                        print("Incorrect key!")
                        crash_quit("Incorrect key!")
                    else:
                        print(keyStatus["errorDescription"])
                        crash_quit(keyStatus["errorDescription"])
            keyreq = 0
        else:
            print("Entered incorrect key!")
            crash_quit("Entered incorrect key!")
        return solver, key, keyreq

    # === set wiper mode ===
    def set_mode(self, mode):
        if mode == 1:
            with open(self.complainFile, 'r', encoding='utf-8') as file:
                pastes = file.read()
                pastes = pastes.split("\n\n")
                bigPaste = 0
        elif mode == 4:
            try:
                with open(self.cpFile, 'r', encoding='utf-8') as file:
                    pastes = file.read()
                    pastes = pastes.split("\n\n")
                    bigPaste = 0
            except:
                with open(self.cpFile, 'r') as file:
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

    # === set trigger mode ===
    def set_trigger(self, form, shrapnelCharge, minPostsCount, args):
        if shrapnelCharge == 0: # and self.thread > 1
            try:
                self.threads.append(Thread(self.board, self.thread, self.mode, form))
            except Exception:
                print("Thread doesn't exist!")
                crash_quit("Thread doesn't exist!")
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
                if (shrapnelCharge == 0):
                    print("There is no threads with the specified parameters!")
                    crash_quit("There is no threads with the specified parameters!")

        if self.chaos != "-1" and self.chaos != "0":
            targetThread = Thread(self.board, self.chaos, self.mode, form)
        else:
            targetThread = self.threads[0]

        return form, shrapnelCharge, targetThread

    # === set attachments ===
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
                    if os.path.exists(mediaDir) == False:
                        print("Directory " + mediaDir + " doesn't exist!")
                        crash_quit("Directory " + mediaDir + " not exist!")
                for media in os.listdir("./" + mediaDir):
                    if media.endswith(".jpg") or media.endswith(".png") or media.endswith(".gif") or media.endswith(".bmp") or media.endswith(".mp4") or media.endswith(".webm"):
                        mediaPaths.append("./" + mediaDir + "/" + media)
                    else:
                        pass
                if len(mediaPaths) == 0:
                    print("Not found any files in " + mediaDir + "!")
                    crash_quit("Not found any files in " + mediaDir + "!")

            elif self.shrapnelCharge == 0:
                for post in self.threads[0].posts:
                    for media in post.medias:
                        print("Downloading ", media.name, "(" + str(post.num) + "/" + str(self.threads[0].postsCount) + " post)")
                        media.download()
            else:
                self.TIMEOUT += 60
        else:
            mediasCount = 0
        return mediaKind, mediaPaths, mediasCount
