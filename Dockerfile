FROM debian:jessie
MAINTAINER Celty Project ""
EXPOSE 6881
# Create Celty user
RUN useradd -s /sbin/nologin celty
# Install dependencies
RUN apt-get update
RUN apt-get install -y libev4 libstdc++6
# Copy over the files
ADD ./etc/celty.service /lib/systemd/system/celty.service
ADD ./bin/celty /usr/local/bin/
ADD ./bin/*.moe /usr/local/lib/celty/
ADD ./etc/celty.cfg /usr/local/etc/celty/
ADD ./etc/client.def /usr/local/etc/celty/
USER celty
RUN celty -m /usr/local/lib/celty -c /usr/local/etc/celty/celty.cfg
