#!/usr/bin/env bash

gnome-terminal --title="opendlv-vehicle-view-multi" --command="bash -c 'cd recordings;docker run --rm --init --net=host --name=opendlv-vehicle-view -v $PWD:/opt/vehicle-view/recordings -v /var/run/docker.sock:/var/run/docker.sock -p 8081:8081 chalmersrevere/opendlv-vehicle-view-amd64:v0.0.60;exec $SHELL'"

gnome-terminal --title="h264decoder" --command="bash -c 'docker run --rm -ti --net=host --ipc=host -e DISPLAY=$DISPLAY -v /tmp:/tmp h264decoder:v0.0.3 --cid=253 --name=img;exec $SHELL'"

gnome-terminal --title="Python Visualiser" --command="bash -c 'docker run --rm -ti --net=host --ipc=host -e DISPLAY=$DISPLAY -v /tmp:/tmp pyhtongraph --cid=253;exec $SHELL'"

gnome-terminal --title="Auto-Steering" --command="bash -c 'docker run --rm -ti --net=host --ipc=host -e DISPLAY=$DISPLAY -v /tmp:/tmp auto-steering:latest --cid=253 --name=img --width=640 --height=480 --verbose;exec $SHELL'"

