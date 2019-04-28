# SnipsDoorbell Home Station Docker Image


A Docker Image for a Doorbell with voice interaction using SNIPS   (HOME STATION)


Project Here  :

https://www.hackster.io/remy-mesnard/doorbell-intercom-with-snips-voice-assistant-68e77a

GitHub Project :
https://github.com/users/rmesnard/projects/1

Wiki :


#build

install git : 

sudo apt-get install git

Build with docker :

sudo docker build -t lijah/snips-homestation github.com/rmesnard/SnipsHomeStation


#install

create volume :

sudo docker volume create snips_config
sudo docker volume create snips_log

#run 

sudo docker run -d --name snips-homestation \
	-v snips_log:/var/log \
	-v snips_config:/usr/share/snips \
	--privileged \
	--device=/dev/snd:/dev/snd \
	--device=/dev/mem:/dev/mem \
	lijah/snips-homestation


#share config 


docker run -d -p 192.168.2.106:445:445 \
-v  snips_config:/share/data \
-v  snips_log:/share/log \
--name samba_snips trnape/rpi-samba \
-u "admin:<YOUR PASSWORD>" \
-s "snips_config:/share/data:rw:admin" \
-s "snips_log:/share/log:rw:admin" 

#console

docker exec -it snips-homestation bash

cd /usr/share/snips
