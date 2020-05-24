#!/usr/bin/env bash

#gnome-terminal --title="opendlv-vehicle-view-multi" --command="bash -c 'docker pull chalmersrevere/opendlv-vehicle-view-amd64:v0.0.60';exec $SHELL"

#gnome-terminal --title="h264decoder" --command="bash -c 'docker build https://github.com/chalmers-revere/opendlv-video-h264-decoder.git#v0.0.3 -f Dockerfile.amd64 -t h264decoder:v0.0.3';exec $SHELL"

#gnome-terminal --title="Python Visualiser" --command="bash -c 'docker build -t pyhtongraph -f GraphDockerfile .';exec $SHELL"

#gnome-terminal --title="Auto-Steering" --command="bash -c 'docker build -t auto-steering:latest -f Dockerfile .';exec $SHELL"

gnome-terminal --title="Builds" -e "bash -c 'docker build -t auto-steering:latest -f Dockerfile .;docker build -t pyhtongraph -f GraphDockerfile .;docker build https://github.com/chalmers-revere/opendlv-video-h264-decoder.git#v0.0.3 -f Dockerfile.amd64 -t h264decoder:v0.0.3;docker pull chalmersrevere/opendlv-vehicle-view-amd64:v0.0.60;docker system prune;exec $SHELL'"
