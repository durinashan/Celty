FROM debian:stretch
MAINTAINER Vijay A. Van Ness <vijay@lethalbit.net>
EXPOSE 6881
EXPOSE 6881/udp
EXPOSE 2600
EXPOSE 2600/udp
# Create Celty user
RUN useradd -s /sbin/nologin celty
# Install dependencies
RUN apt-get update
RUN apt-get install -y gcc-5 libev4 libpqxx-4.0v5 libmemcached11 libstdc++6
# Copy over the files
ADD ./etc/celty.service /lib/systemd/system/celty.service
ADD ./bin/celty /usr/local/bin/
ADD ./bin/*.moe /usr/local/lib/celty/
ADD ./etc/celty.cfg /usr/local/etc/celty/
ADD ./etc/client.def /usr/local/etc/celty/
RUN chown celty -R /usr/local/etc/celty/
# USER celty
# RUN celty -m /usr/local/lib/celty -c /usr/local/etc/celty/celty.cfg --keep-head
# Fix SystemD
# VOLUME [ “/sys/fs/cgroup” ]
# RUN (cd /lib/systemd/system/sysinit.target.wants/; for i in *; do [ $i == systemd-tmpfiles-setup.service ] || rm -f $i; done); \
# rm -f /lib/systemd/system/multi-user.target.wants/*;\
# rm -f /etc/systemd/system/*.wants/*;\
# rm -f /lib/systemd/system/local-fs.target.wants/*; \
# rm -f /lib/systemd/system/sockets.target.wants/*udev*; \
# rm -f /lib/systemd/system/sockets.target.wants/*initctl*; \
# rm -f /lib/systemd/system/basic.target.wants/*;\
# rm -f /lib/systemd/system/anaconda.target.wants/*;
# Start SystemD
# RUN systemctl enable celty
# RUN systemctl start celty
