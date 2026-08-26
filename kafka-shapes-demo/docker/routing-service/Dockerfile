FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# libprotobuf23 is required by librtiprotobuftransf.so
RUN apt-get update && apt-get install -y --no-install-recommends \
    libprotobuf23 \
    && rm -rf /var/lib/apt/lists/*

# RTI Connext DDS and gateway plugin shared libraries (staged by build.sh)
COPY libs/ /usr/local/lib/
RUN ldconfig

# RTI Routing Service application binary (staged by build.sh)
COPY bin/rtiroutingserviceapp /usr/local/bin/rtiroutingserviceapp
RUN chmod +x /usr/local/bin/rtiroutingserviceapp

# Demo configuration files: pbdesc committed; XML staged by build.sh from kafka-shapes-demo/config/
COPY config/ /config/
WORKDIR /config

COPY entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh

ENTRYPOINT ["/entrypoint.sh"]
