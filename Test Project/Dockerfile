##################################################
# Section 1: Build the application
##################################################
FROM alpine:latest as builder
MAINTAINER Haider Ali haiali@student.chalmers.se

RUN apk update \
	&& apk upgrade \ 
	&& apk --update add \
        build-base \
        cmake \
	&& rm -rf /var/cache/apk/* 
	
ADD . /opt/sources
WORKDIR /opt/sources

RUN cd /opt/sources && mkdir build && \
    cd build && \
    cmake -D CMAKE_BUILD_TYPE=Release .. && \
    make && make test && cp helloworld /tmp

##################################################
# Section 2: Bundle the application.
FROM alpine:latest
MAINTAINER Haider Ali haiali@student.chalmers.se

RUN apk update && \
	apk --update add libstdc++ && \
	rm -rf /var/cache/apk/* 

WORKDIR /opt
COPY --from=builder /tmp/helloworld .
ENTRYPOINT ["/opt/helloworld"]
