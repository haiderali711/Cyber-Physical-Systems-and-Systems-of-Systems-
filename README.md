## Content
* [Steps to clone Repository](#steps-to-clone-Repository)
* [Building the Program](#building-the-program)



### Steps to clone Repository

1.  Installing **Git** in the local Machine

2.  Copy the *Copy with SSH* or *Copy with HTTPS* link for this Repository

3.  Open the Terminal on the local Machine in any prefered location

4.  Run the command `git clone <link from Step 1>` in the Terminal
    *  **_In case of the "SSH link", an SSH Key should have been created locally and added in the Gitlabs Account_**
    *  [Generate new SSH KEY](https://git.chalmers.se/help/ssh/README#generating-a-new-ssh-key-pair)

5.  For a more interactive experience **Github Desktop** can also be used as the tool for working with the Git repository

### Building the Program
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

4.  Run the following command to Build the project
    >   `docker build -t <TagName for your build>:latest -f Dockerfile .`

5.  Run the following command to Run the build created
    >   `docker run --rm -ti <TagName for your build>:latest <Arguments if required>`
