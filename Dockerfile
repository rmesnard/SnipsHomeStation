FROM raspbian/stretch

RUN set -x && \
	apt-get update && apt-get dist-upgrade -y
 
RUN set -x && \	
	apt-get install -y dirmngr apt-transport-https apt-utils

RUN set -x && \
	bash -c  'echo "deb https://raspbian.snips.ai/stretch stable main" > /etc/apt/sources.list.d/snips.list'

RUN set -x && \
	apt-key adv --keyserver gpg.mozilla.org --recv-keys D4F50CDCA10A2849

RUN set -x && \
	apt-get update	
	
#since recommended packets are NOT installed by default, we have to install them explicit
RUN set -x && \
	apt-get install -y snips-makers-tts alsa-utils snips-audio-server curl unzip snips-template python3-pip python3-setuptools python3-wheel libasound2-dev libasound2 libasound2-data git espeak

RUN set -x && \
	apt-get install -y libjpeg-dev zlib1g-dev libfreetype6-dev liblcms1-dev libopenjp2-7 libtiff5
	
#python libraries

RUN cd /opt && pip3 install configparser pyalsaaudio protobuf RPi.GPIO smbus2 hermes-python paho-mqtt pillow pyserial
	
COPY ./config/ /config/ 
COPY ./skills/ /skills/ 

COPY start-snips.sh start-snips.sh

EXPOSE 1833/tcp

CMD ["bash","/start-snips.sh"]
