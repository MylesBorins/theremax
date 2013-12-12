{
  title: "iterations",
  date:  "2012-2-13",
  description: "How did I get here?"
}

<br>
![Hack the planet](/images/hackers-desktop.gif)
<br>

---

>###Network all the things
>initially I was really interested in exploring a network based application where users could have synchronized music generating drones.  While this idea was eventually scrapped for the theremax I developed quite a bit of the underlying framework for networked applications that I think are worth documenting

---

At first I spent a lot of time researching bonjour, and its lower level counter part dsn_sd in an attempt to make a cross platform zeroconf communication framework.  The goal being to allow clients to find each other on a local network without the need for a dns server or manual configuration.

I slowly found that all the higher level bonjour functionality is implemented in objective-c, specifically designed to run on OSX only.  I decided to explore the lower level library dns_sd as an alternative, this is the library that the higher level bonjour function are built on top of.

You can find this first iteration on [github](https://github.com/TheAlphaNerd/libmdns).  This includes an example of creating a server and client using dns_sd.  Unfortunately I found the library a bit frustrating to work with.   Not only was it fairly verbose, it required shims to run on linux with avahi, and there were inconsistent results with creating servers.  Rather than writing different implementation of the framework for different platforms I opted to explore something else.

This brought me to [liblo](http://liblo.sourceforge.net/), a library for doing open sound control.  With a bit of research I found that there is a [Multicast-IP](https://www.iana.org/assignments/multicast-addresses/multicast-addresses.xhtml#multicast-addresses-1) that will forward messages to all systems on a subnet 224.0.0.1

I created the [beacon](https://github.com/TheAlphaNerd/some-drone-shit), an application that broadcasts a ping every second and maintains all unique systems that are currently broadcasting.  This simple solution allows you to not only maintain a list of all systems within your local network, it de-centralizes the exchange of data, allowing you to have a constantly updating global model in its own thread that can be used to modify other global parameters such as pitch or tempo.

```c++
* make address for multicast ip
* pick a port number for you by passing NULL as the last argument */

    lo_address t = lo_address_new("224.0.0.1", "7770");
    // lo_server multi = lo_server_new_multicast("drone", "7771", error);
    /* start a new server on port 7770 */
    lo_server_thread st = lo_server_thread_new_multicast("224.0.0.1", "7770", error);

    /* add method that will match the path /foo/bar, with two numbers, coerced
* to float and int */
    lo_server_thread_add_method(st, "/ping", "is", ping_handler, NULL);

    /* add method that will match the path /quit with no args */
    lo_server_thread_add_method(st, "/quit", "", quit_handler, NULL);

    lo_server_thread_start(st);
    
    
    while (!done) {
#ifdef WIN32
        Sleep(1);
#else
        usleep(1000000);
#endif
        char hostname[128] = "";

        gethostname(hostname, sizeof(hostname));
        
        int pid = getpid();
        lo_send(t, "/ping", "is", pid, hostname);
        memberCleanup(&members);
    }
```

---

One of the interesting side effects of switching  projects three times over the course of just as many weeks was the ability to explore with different methods of compiling my application.  The original application used a fairly straight forward MakeFile, with all src files in the root directory.

When I decided to scrap dns_sd and focus on liblo I decide to spend a bit of time cleaning up my make file and create a project that had a seperate directory for src files, include files, and compiled binaries.  While this method of compilation did simplify things, one has to accept that MakeFiles are evil.  They easily spiral out of control and become extremely nasty to maintain as one attempts to make multi-platform applications.

When I began to work with openCV I found a [tutorial](http://docs.opencv.org/doc/tutorials/introduction/linux_gcc_CMake/linux_gcc_CMake.html#linux-gcc-usage) on the openCV site that went step by step guide showing how to create an opencv project using CMake. While CMake has a number of nice features, one in particular in really nice; CMake 