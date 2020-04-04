##################################################
# Section 1: Build the application
##################################################
FROM alpine:3.5 as builder
MAINTAINER Haider Ali haiali@student.chalmers.se

##################################################
#-We use the alpine Linux as the base image for our docker file
#-Usually each docker image occupies the space of its base image
#- instead of using the APT packages we use APK which is the similar tool 
#as APT but for Alpine linux
#- we follow the same steps as before for the linux for building the #program
##################################################

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
FROM alpine:3.5
MAINTAINER Haider Ali haiali@student.chalmers.se

RUN apk update && \
	apk --update add libstdc++ && \
	rm -rf /var/cache/apk/* 

WORKDIR /opt
COPY --from=builder /tmp/helloworld .
ENTRYPOINT ["/opt/helloworld"]
