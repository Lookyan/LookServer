# LookServer

##Dependencies
libevent with pthreads

## Benchmarks

LookServer with -n 2 parameter vs nginx on the same file:

<b>LookServer: </b>
```
$ ab -n 50000 -c 100 -r http://localhost:5555/
Server Software:        LookServer/0.1
Server Hostname:        localhost
Server Port:            5555

Document Path:          /
Document Length:        15 bytes

Concurrency Level:      100
Time taken for tests:   1.637 seconds
Complete requests:      50000
Failed requests:        0
Total transferred:      7950000 bytes
HTML transferred:       750000 bytes
Requests per second:    30548.45 [#/sec] (mean)
Time per request:       3.273 [ms] (mean)
Time per request:       0.033 [ms] (mean, across all concurrent requests)
Transfer rate:          4743.36 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    2  38.4      0    1000
Processing:     0    1   1.6      1     203
Waiting:        0    1   1.6      1     203
Total:          1    3  38.5      1    1003

Percentage of the requests served within a certain time (ms)
  50%      1
  66%      1
  75%      1
  80%      2
  90%      2
  95%      2
  98%      3
  99%      3
 100%   1003 (longest request)

```

<b>nginx:</b>
```
$ ab -n 50000 -c 100 -r http://localhost:5555/
Server Software:        nginx/1.4.6
Server Hostname:        localhost
Server Port:            80

Document Path:          /
Document Length:        15 bytes

Concurrency Level:      100
Time taken for tests:   5.121 seconds
Complete requests:      50000
Failed requests:        0
Total transferred:      8250000 bytes
HTML transferred:       750000 bytes
Requests per second:    9763.52 [#/sec] (mean)
Time per request:       10.242 [ms] (mean)
Time per request:       0.102 [ms] (mean, across all concurrent requests)
Transfer rate:          1573.22 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    0   0.3      0       8
Processing:     0    8  72.7      2    1814
Waiting:        0    8  72.7      2    1814
Total:          0    9  72.7      2    1814

Percentage of the requests served within a certain time (ms)
  50%      2
  66%      4
  75%      5
  80%      6
  90%      8
  95%     11
  98%     11
  99%     13
 100%   1814 (longest request)

```
