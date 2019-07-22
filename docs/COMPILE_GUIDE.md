# Простой способ через Docker
Если на вашей машине установлен Docker CE/EE и Docker Compose, то всё становится гораздо проще.  

```
git clone https://github.com/tsunamaru/2ch-wiper.git
cd 2ch-wiper
docker-compose up --build -d
docker cp qtbuilder:/tmp/release ./src
```

Первая сборка длится 1-2 часа в зависимости от вашего процессора. Все последующие разы (при условии сохранения контейнера) будут занимать не больше минуты.  
Собранные **статические** бинарники будут лежать в /src/release  

# Обычный способ
## Shared build
Есть два стула: QT Creator (работает везде) или qmake-qt5 && make (работает только в прыщах, но это не точно).  

1. Скачайте [QT Creator](https://www.qt.io/), выберите при установке QT >5.9 и конпелятор MinGW (если у вас Windows) либо GCC (если *nix).  
Cклонируйте репу, откройте wipe.pro через QT Creator, тыкните нужный конпелятор и жмякните Build All. 
Не забудьте перед запуском скопировать папки engine и gui в директорию с вашим билдом.  
Если у вас шинда, то скорее всего потребуется выполнить windeployqt, чтобы вайполка не ругалась на отсутствующие библиотеки.   
Если никсы -- установите самостоятельно недостающие либы из вашего пакмана, если вы ещё этого не сделали (обычно хватает `qt5-qtbase` и `qt5-qtmultimedia`).  

2. Установите из вашего пакмана `gcc g++ qt5-qtbase-devel qt5-qtmultimedia-devel`.  
Склонируйте репу, перейдите в папку, выполните `qmake-qt5 && make`. На выходе получите готовый эльф, можно запустить сразу из сосноли командой `./wiper`  

## Static build
Нижеследующая инструкция только для прыщей, под шинду ебитесь сами.  

### Сборка билда под Windows:
```
cd
git clone https://github.com/mxe/mxe.git
cd mxe
make qtbase qtmultimedia
make install
cd ..
export PATH=~/mxe/usr/bin:$PATH

git clone https://github.com/tsunamaru/2ch-wiper.git
cd 2ch-wiper/src
i686-w64-mingw32.static-qmake-qt5
make
```
Готовый .exe будет в /release  
Ставить MXE в хомяк не обязательно, просто укажите свой собственный путь в PATH.  
Не собирается MXE? Проверьте [зависимости](https://mxe.cc/#requirements).  

### Сборка под *nix
https://wohlsoft.ru/pgewiki/Building_static_Qt_5