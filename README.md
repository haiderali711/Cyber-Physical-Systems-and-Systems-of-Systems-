## Content
*   [Steps to clone Repository](#steps-to-clone-repository)
*   [Building the Program](#building-the-program)
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

    This will run all the necessary commands required to create the build contained in the Dockerfile

5.  Run the following command to Run the build created
    >   `docker run --rm -ti <TagName for your build>:latest <Arguments if required>`


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
