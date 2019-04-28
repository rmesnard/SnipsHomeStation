#!/bin/bash
set -e


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

echo "Start homestation skill"
cd /usr/share/snips/skills
nohup python3 -u listener.py 2> /var/log/listener.log &
snips_listener_pid=$!


echo "running ok"

wait "$snips_audio_server_pid"

