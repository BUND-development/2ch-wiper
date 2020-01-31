# -*- coding: utf-8 -*-

import tools
import json
import os
import requests
import urllib3
from bs4 import BeautifulSoup
urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)



headers = {}
headers["Host"] = "2ch.hk"
headers["User-Agent"] = "Mozilla/5.0 (Windows NT 10.0; rv:68.0) Gecko/20100101 Firefox/68.0"
headers["Accept"] = "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8"
headers["Accept-Language"] = "ru-RU,ru;q=0.8,en-US;q=0.5,en;q=0.3"
headers["Accept-Encoding"] = "gzip, deflate, br"
headers["Connection"] = "close"
headers["UPGRADE-INSECURE-REQUESTS"] = "1"
headers["DNT"] = "1"



# ====== board scheme ======
class Catalog:

    def __init__(self, board):
        self.board = board  # board index
        print("\nDownloading board /" + self.board + "/")
        self.schema = requests.get(''.join(["https://2ch.hk/", board, "/catalog.json"]), headers=headers, verify = False).json() # board DOM
        self.threadsCount = len(self.schema["threads"])  # active threads count


# ====== attachments to post ======
class Media:

    def __init__(self, name, path):
        self.name = name  # attach name
        self.path = path  # attach path on server
        self.cached = False  # caching attachments locally on PC

    # === downloading attach from server ===
    def download(self):
        if self.cached == False:
            self.file = requests.get("https://2ch.hk" + self.path, headers=headers, verify = False).content  # attachment itself
            self.cached = True


# ====== existing post ======
class Post:

    def __init__(self, schema, mode, triggerForm):
        self.ID = str(schema["num"])  # post number on board
        self.comment = self.set_comment(schema["comment"], mode, triggerForm)  # post text
        self.sage = self.set_sage(schema)  # sage mode
        self.num = schema["number"]  # post number in thread (starting from 1)
        self.medias = []  # attachments
        for media in schema["files"]:
            self.medias.append(Media(media["name"], media["path"]))
        print("Triggered to >>" + self.ID)

    # === formatting post text ===
    def set_comment(self, text, mode, triggerForm):
        # === replace <br> to \n ===
        text = text.replace("<br>", "\n")
        # === bold ===
        text = text.replace("<strong>", "[b]")
        text = text.replace("</strong>", "[/b]")
        # === italic ===
        text = text.replace("<em>", "[i]")
        text = text.replace("</em>", "[/i]")
        # === supscript ===
        text = text.replace("<sup>", "[sup]")
        text = text.replace("</sup>", "[/sup]")
        # === subscript ===
        text = text.replace("<sub>", "[sub]")
        text = text.replace("</sub>", "[/sub]")
        # === code ===
        text = text.replace("<code>", "[code]")
        text = text.replace("</code>", "[/code]")

        # === removing answers to other posts and/or (OP) mark ===
        if mode == 7 and triggerForm == 0:
            text = text.replace(" (OP)", "")
            soup = BeautifulSoup(text, features="html.parser")
        else:
            soup = BeautifulSoup(text, features="html.parser")
            for a in soup.find_all("a", {"class": "post-reply-link"}):
                a.decompose()

        # === underline ===
        for u in soup.find_all("span", {"class": "u"}):
            u.replace_with("[u]" + u.get_text() + "[/u]")
        # === overline ===
        for o in soup.find_all("span", {"class": "o"}):
            o.replace_with("[o]" + o.get_text() + "[/o]")
        # === spoilers ===
        for spoiler in soup.find_all("span", {"class": "spoiler"}):
            spoiler.replace_with("[spoiler]" + spoiler.get_text() + "[/spoiler]")
        # === crossed text ===
        for s in soup.find_all("span", {"class": "s"}):
            s.replace_with("[s]" + s.get_text() + "[/s]")

        # === saving ===
        return str(soup.get_text()).lstrip('\n').rstrip('\n')

    # === get SAGE ===
    def set_sage(self, schema):
        return False if schema["email"].find("mailto:sage") == -1 else True


# ====== thread scheme ======
class Thread:

    def __init__(self, board, ID, mode, triggerForm):
        self.board = board
        self.ID = ID  # thread number on board
        if (int(ID) != 0):
            print("\nDownloading thread", self.ID)
            self.schema = requests.get(''.join(["https://2ch.hk/", board, "/res/", ID, ".json"]), headers=headers, verify = False).json()  # DOM
            self.postsCount = self.schema["posts_count"] + 1  # posts count in thread
            self.lastID = str(self.schema["max_num"])  # last post number
            self.posts = self.download_posts(mode, triggerForm)  # posts
            self.loaf = ""  # "multipost"
            for postNum in range(min(len(self.posts), 30)):
                self.loaf += (">>" + self.posts[postNum].ID + " ")

    # === downloading DOM ===
    def download_posts(self, mode, triggerForm):
        posts = []
        for post in self.schema["threads"][0]["posts"]:
            posts.append(Post(post, mode, triggerForm))
        return posts
