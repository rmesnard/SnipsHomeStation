#!/bin/bash
set -e

ENABLE_INTERCOM=${ENABLE_INTERCOM:-yes}

echo "Install config."
if [ ! -d "/usr/share/snips/config" ]; then
  echo "Install default config."
  mkdir /usr/share/snips/config
  cp -R -f /config /usr/share/snips
fi
chmod -R 777 /usr/share/snips/config

rm -f /etc/asound.conf
cp -f /usr/share/snips/config/asound.conf /etc/asound.conf


echo "Install skills."
if [ ! -d "/usr/share/snips/skills" ]; then
  echo "Install default skills."
  mkdir /usr/share/snips/skills
  cp -R -f /skills /usr/share/snips
fi
chmod -R 777 /usr/share/snips/skills

if [ -d /usr/share/snips/config ]; then
	echo "use shared config."
	rm -f /etc/snips.toml
	cp -f /usr/share/snips/config/snips.toml /etc/snips.toml
	chmod -R 777 /etc/snips.toml
fi

#go back to root directory
cd /

echo "Start snips services"

chmod -R 777 /var/log

#start the snips audio server 
snips-audio-server --hijack localhost:64321 2> /var/log/snips-audio-server.log &
snips_audio_server_pid=$!

echo "snips services started.. check logs"

if [ $ENABLE_INTERCOM == yes ]; then
	echo "Start intercom"
	cd /usr/share/snips/skills
	nohup python3 -u intercom.py 2> /var/log/doorbell.log &
	snips_listener_pid=$!
fi

#start homestation skill
cd /usr/share/snips/skills
nohup python3 -u homestation.py 2> /var/log/homestation.log &
snips_homestation_pid=$!


echo "running ok"

wait "$snips_audio_server_pid"

