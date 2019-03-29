<p align="center">Многофункциональная вайпалка харкача, с приятным GUI и няшной аниме-тяночкой в комплекте.  
<p>
<img src="https://i.imgur.com/o2zZLfX.png" width="100%" alt="Сейчас загрузится..."/>
</p>

## Как поставить
1. Скачайте установщик [питона](https://python.org) для вашей ОС. Установите, прожав **все галочки** в инсталлере!  
2. Скачайте архив с вайпалкой со страницы [релизов](https://github.com/tsunamaru/2ch-wiper/releases), распакуйте.  
3. Закиньте свои проксички в файл proxies.cfg  
4. Опционально наполните папки images и videos контентом для вайпа, а так же копипасты (файл texts.txt).  
5. Виндузятники жмякают wiper_win32.exe, линуксоиды запускают wiper_linux64 **из консоли**, это важно.  
6. ?????  
7. Ну охуеть теперь.  

## А что тыкать-то?
[Видеогайд](https://lolifox.org/rus/src/1553199120164-0.webm) по использованию и [огромная пикча](https://lolifox.org/rus/src/1553199199927-1.jpg) с описанием каждой кнопки.  

## Как собрать 
Есть два стула: QT Creator и qmake-qt5 && make.  

В первом случае вам нужно скачать этот самый [QT Creator](https://www.qt.io/), по пути ~вставив себе анальный зонд~ зарегавшись на их сайте, выбрав при установке QT >5.11 и конпелятор MinGW (либо GCC) в зависимости от вашей ОС. Далее склонируйте репу, откройте wipe.pro через криэйтор, тыкните нужный конпелятор и жмякните Build All. Не забудьте перед запуском скопировать папки engine и gui в директорию с вашим билдом.  
Если у вас шинда, то скорее всего вам потребуется выполнить windeployqt, чтобы вайполка не ругалась на отсутсвующие библиотеки.   

Во втором случае (только линукс) установите из вашего пакмана gcc, g++, qt5 и qmake-qt5. Склонируйте репу, перейдите в папку, выполните `qmake-qt5 && make`. На выходе получите готовый эльф, можно запустить сразу из сосноли командой ./wiper  

## Тупые вопросы, неочевидные моменты, возможные проблемы и способы их решения
**Q:** Частые connection refused или posting failed.  
**A:** Проблемы ваших интернетов и/или дохлые прокси.  

**Q:** Exception in thread <...> при использовании socks5.  
**A:** Это проблема библиотеки requests. Используйте https/socks4 прокси.  

**Q:** Где брать прокси?  
**A:** Из свободных источников. Лучше всего смотреть страны СНГ и це еуропки. Не берите Латинскую Америку и Азию, они давно в пермачах.  

**Q:** Как вайпать без прокси?  
**A:** Пока -- никак. Возможно этот функционал будет реализован в будущем. А может и не будет.  

**Q:** .exe не стартует или сыпет ошибками.  
**A:** Проверьте отсутсвие русских символов до пути расположения.  

**Q:** Failed to embed / download file.  
**A:** Проверьте отсутствие русских символов в именах файлов.  

**Q:** Во время вайпа вылезают ошибки empty range for randrange или проёбываются потоки.  
**A:** Увеличьте минимальный порог постов в тредах для шрапнели.  

**Q:** Статистика не учитывает посты или пишет абсолютно рандомную хуйню.  
**A:** It's a bug. Когда-нибудь пофикшу.  

**Q:** Когда релиз под ведро?  
**A:** КТТС.  

**Q:** Хуи сосёшь?  
**A:** Только бочки.  

## Вопросы?
Telegram: [@tsunamaru](https://t.me/tsunamaru)  
Почта: tsunamaru@airmail.cc  
