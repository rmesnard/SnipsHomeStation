# Snips Home Station Docker Image


A Docker Image to build a SNIPS Satellite to work with Snips Server and Snips Doorbell   (HOME STATION)


Initial Project Here  :
https://www.hackster.io/remy-mesnard/doorbell-intercom-with-snips-voice-assistant-68e77a

GitHub Project :
https://github.com/users/rmesnard/projects/1

Wiki :


Issues are group on SnipsDoorbell Project :  https://github.com/rmesnard/SnipsDoorbell

#build

install git : 

sudo apt-get install git

Build with docker :

sudo docker build -t lijah/snips-homestation github.com/rmesnard/SnipsHomeStation


#install


create volume :

sudo docker volume create snips_config
sudo docker volume create snips_log

sudo docker volume inspect snips_config

/var/lib/docker/volumes/snips_config/_data

sudo cp /var/lib/docker/volumes/snips_config/_data/ledpanel/test.py test.py

#run 

sudo docker run -d --name snips-homestation \
-v snips_log:/var/log \
-v snips_config:/usr/share/snips \
--privileged \
--device=/dev/snd:/dev/snd \
--device=/dev/ttyS1 \
lijah/snips-homestation


--device=/dev/ttyUSB0 \
#share config 


docker run -d -p 192.168.2.106:445:445 \
-v  snips_config:/share/data \
-v  snips_log:/share/log \
--name samba_snips trnape/rpi-samba \
-u "admin:<YOUR PASSWORD>" \
-s "snips_config:/share/data:rw:admin" \
-s "snips_log:/share/log:rw:admin" 

#console

sudo docker exec -it snips-homestation bash

cd /usr/share/snips
