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

This brought me to [liblo](http://liblo.sourceforge.net/)