FROM ubuntu:latest
MAINTAINER Celty Project ""
EXPOSE 6881
EXPOSE 6881/udp
# Create Celty user
RUN useradd -s /sbin/nologin celty
# Install dependencies
RUN apt-get update
RUN apt-get install -y software-properties-common
RUN add-apt-repository ppa:ubuntu-toolchain-r/test
RUN apt-get update
RUN apt-get install -y gcc-5 libev4 libpqxx-4.0 libmemcached10
# Copy over the files
ADD ./etc/celty.service /lib/systemd/system/celty.service
ADD ./bin/celty /usr/local/bin/
ADD ./bin/*.moe /usr/local/lib/celty/
ADD ./etc/celty.cfg /usr/local/etc/celty/
ADD ./etc/client.def /usr/local/etc/celty/
RUN chown celty -R /usr/local/etc/celty/
USER celty
RUN celty -m /usr/local/lib/celty -c /usr/local/etc/celty/celty.cfg
# RUN systemctl enable celty
# RUN systemctl start celty
