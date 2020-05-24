## Content

[![pipeline status](https://git.chalmers.se/courses/dit638/students/group_01/badges/master/pipeline.svg)](https://git.chalmers.se/courses/dit638/students/group_01/-/commits/master)

[![coverage report](https://git.chalmers.se/courses/dit638/students/group_01/badges/master/coverage.svg)](https://git.chalmers.se/courses/dit638/students/group_01/-/commits/master)

*   [Steps to clone Repository](#steps-to-clone-repository)
*   [Building the Necessary tools](#building-the-necessary-tools)
*   [Running the built tools](#running-the-built-tools)
*   [Addition of new Features](#addition-of-new-features)
*   [Commit message structure](#commit-message-structure)



### Steps to clone Repository

1.  Installing **Git** in the local Machine

2.  Copy the *Copy with SSH* or *Copy with HTTPS* link for this Repository

3.  Open the Terminal on the local Machine in any preferred location

4.  Run the command `git clone <link from Step 1>` in the Terminal
    *  **_In case of the "SSH link", an SSH Key should have been created locally and added in the Gitlab's Account_**
    *   [Generate new SSH KEY](https://git.chalmers.se/help/ssh/README#generating-a-new-ssh-key-pair)

5.  For a more interactive experience with Git, **Github Desktop** can also be used as the tool for working with the Git repository

### Building the Necessary tools

1.  Install **Docker** on your Machine.
    *   If Working on **Linux**
        >   Follow the Instructions --> [HERE](https://docs.docker.com/install/linux/docker-ce/ubuntu/) to install Docker        
    *   If working on **Mac OSX / Windows**
        >   Install a stable version of Ubutntu in the [Virtual Box](https://tecadmin.net/install-ubuntu-on-virtualbox/)
        OR
        >   Install [Docker Desktop](https://www.docker.com/get-started)  
        
2.  Open the Terminal and run the followiong command to access the repository folder
    >   `cd <Repository Path>`

3.  Run the following command to test if docker is successfully installed
    >    `docker --version`

4.  Run the following commands to Build the project (All the following Micro-services should be built in order to run the Project)
    ```bash
    #   OPTION 1 -> RUN THE FOLLOWING COMMANDS ONE BY ONE
    
    -   docker pull chalmersrevere/opendlv-vehicle-view-amd64:v0.0.60

    -   docker build https://github.com/chalmers-revere/opendlv-video-h264-decoder.git#v0.0.3 -f Dockerfile.amd64 -t h264decoder:v0.0.3

    -   docker build -t auto-steering:latest -f Dockerfile .

    -   docker build -t pyhtongraph:latest -f GraphDockerfile .
    ```

    ```bash
    #   OPTION 2 -> RUN THE SCRIPT 

    -   cd <Repository Directory>

    -   chmod +x ./BuildScript.sh

    -   ./BuildScript.sh

    ```


### Running the built tools

1.  Run the following command to Run the build created
    >   `cd <Repository Directory>`
    ```bash
    # OPTION 1 --> RUN EACH COMMAND IN SEPARATE TERMINAL
    
    # FIRST TERMINAL
    -   cd <folder containing the video recordings>

    -   docker run --rm --init --net=host --name=opendlv-vehicle-view -v $PWD:/opt/vehicle-view/recordings -v /var/run/docker.sock:/var/run/docker.sock -p 8081:8081 chalmersrevere/opendlv-vehicle-view-amd64:v0.0.60

    # SECOND TERMINAL
    -   docker run --rm -ti --net=host --ipc=host -e DISPLAY=$DISPLAY -v /tmp:/tmp h264decoder:v0.0.3 --cid=253 --name=img

    # THIRD TERMINAL
    -   docker run --rm -ti --net=host --ipc=host -e DISPLAY=$DISPLAY -v /tmp:/tmp pyhtongraph --cid=253

    # FOURTH TERMINAL
    -   xhost +
	
	docker run --rm -ti --net=host --ipc=host -v /tmp:/tmp auto-steering:latest --cid=253 --name=img --width=640 --height=480

    ```

    ```bash
    #   OPTION 2 -> RUN THE SCRIPT 

    -   chmod +x ./RunContainer.sh

    -   ./RunContainer.sh

    #	ONE OF COMMANDS IN THE TERMINAL WONT WORK SINCE THERE IS NO SHARED MEMORY INTIALIAZED UNTIL THE FIRST VIDEO HAS STARTED, SO AFTER STARTING THE FIRST VIDEO RUN THIS COMMAND

    -	xhost +
	
	    docker run --rm -ti --net=host --ipc=host -v /tmp:/tmp auto-steering:latest --cid=253 --name=img --width=640 --height=480

    ```

2.  Once each micro-service is running in each 

    >   Run http://localhost:8081/ in any default browser

    >   Play any video from the interface and see the Outcome! 

### New Features

1.  **Master branch** is _protected_, so direct push to Master are not allowed

2.  Each *Feature* will have its' own branch

3.  Commits from all the group members about a feature, will be pushed only on the Branch which has been dedicated for the same feature.

4.  A feature will be added to the **Master** branch through a Merge Request.

5.  Each **Merge Request** must have been Peer Reviewed by an other Team Member before the Feature Branch is merged with Master Branch.

6.  If fixing bugs or unexpected behaviours in an existing Feature, the Commit Messages will contain `PATCH : <Commit Message about what has been fixed>`


### Commit message structure

>   git commit -m "Subject line of the commit Message      
>   
>   Body of the Message will be written here"

1.  Separate subject from body with a blank line as shown in the example above

2.  Subject line should be maximum 50 characters

3.  Start the Subject line with a Capital letter

4.  Do not end the subject line with a period (.)

5.  Use the imperative mood in the subject line

6.  Wrap the body at 72 characters

7.  Use the body to explain what and why vs. how






