# project1

inter-to-cluster.cc是实现pattern1的代码，运行代码方式如下：
将inter-to-cluster.cc复制粘贴到 /ns3安装目录/ns-allinone-3.27/ns-3.27/scratch
随后进入该目录下：cd /ns3安装目录/ns-allinone-3.27/ns-3.27/scratch
运行代码：./waf --run /scratch/inter-to-cluster
随后能在/scratch目录下看到三个抓包文件：inter-0-0.pcap，inter-2-1.pcap，inter-6-1.pcap

many-to-one.cc是实现pattern1的代码，运行代码方式如下：
将many-to-one.cc复制粘贴到 /ns3安装目录/ns-allinone-3.27/ns-3.27/scratch
随后进入该目录下：cd /ns3安装目录/ns-allinone-3.27/ns-3.27/scratch
运行代码：./waf --run /scratch/many-to-one
随后能在/scratch目录下看到三个抓包文件：many-0-0.pcap，many-2-1.pcap，many-6-1.pcap

improve.cc是实现pattern1的代码，运行代码方式如下：
将improve.cc复制粘贴到 /ns3安装目录/ns-allinone-3.27/ns-3.27/scratch
随后进入该目录下：cd /ns3安装目录/ns-allinone-3.27/ns-3.27
这个代码运行可能会出现bug，需要注释掉/ns3安装目录/ns-allinone-3.27/ns-3.27/src/internet/model/global-route-manager-impl.cc文件的316行
运行代码：./waf --run /scratch/improve
随后能在/scratch目录下看到三个抓包文件：improve-0-0.pcap，improve-2-1.pcap，improve-6-1.pcap
