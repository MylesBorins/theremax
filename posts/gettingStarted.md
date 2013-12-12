{
  title: "Getting Started",
  date:  "2013-9-16",
  description: "cmake | homebrew"
}

<br>
![Take This](/images/zelda-take-this.png)
<br>

---

>###It's Dangerous to Go Alone!
>There are a handful of dependencies you will need installed in order to compile Theremax.  While the application has been built using only cross platform tools, it has not yet been tested on Linux or PC, so proceed with caution and feel free to send me a pull request if some code is broken on your system.  

>The following getting stated guide will assume that you are using osx.

---

##Step 0: List of dependencies
* cmake
* openGL
* GLUT
* openCV
* jack (linux only)

##Step 1: Install a package manager

Assuming you are starting from a brand new laptop the first thing you are going to want to do is install a package manager.  I highly suggest [homebrew](http://brew.sh) if you want to know why feel free to ask me some time.  Installing it couldn't be simpler, just run the following command and enter in you password when prompted.

```bash
ruby -e "$(curl -fsSL https://raw.github.com/mxcl/homebrew/go/install)"
```

Once homebrew is install run the command
```
brew doctor
```
To get information about what you need to fix on your system before you can start compiling stuff.  This may include things such as installing XCode / command line utilities, changing permissions on /usr/local, and cleaning up hand rolled packages living in /usr/local.

One homebrew is *rarin to brew* you can start installing the dependencies