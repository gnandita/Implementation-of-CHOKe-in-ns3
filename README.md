# Implementation-of-CHOKe-in-ns3

## Overview:

CHOKe (CHOose and Keep for responsive flows / CHOose and Kill for unresponsive flows) is an Active Queue Management (AQM) algorithm that punishes flows which send more packets than their fair share to a router [1]. Linux Kernel code for CHOKe is available at [2]. This repository contains an implementation of CHOKe in ns-3 [3].

### References:

[1] Rong Pan, B. Prabhakar, K. Psounis, CHOKe - A Stateless Active Queue Management scheme for approximating fair bandwidth allocation, INFOCOM 2000. Nineteenth Annual Joint Conference of the IEEE Computer and Communications Societies. Proceedings. IEEE

[2] https://lwn.net/Articles/422481/

[3] http://www.nsnam.org/
