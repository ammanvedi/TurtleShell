# TurtleShell
---

turtle shell is basic shell prgram written in c


Initialisation:

The program will read the file .profile from the current working directory to determine the system PATH and HOME, 

example bash profile:

`export PATH=$PATH:/Users/ammanvedi/.rvm/bin:/usr/local/bin`         
`export HOME=/Users/ammanvedi/`

the shell will begin in the HOME directory specified in .profile

Programs within your $PATH can be run as normal

Assigning $PATH and $HOME:

To change the session PATH and HOME use the Following syntax

`/User/Home/ > $PATH=:/<PATH1>:/<PATH1>:/<PATH1>`                                           
`/User/Home/ > $HOME=/<PATH>`

Changing Directory:

`/User/Home/ > cd Documents/`

or full path

`/User/Home/ > cd /User/Home/Documents/`

