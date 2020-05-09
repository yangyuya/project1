/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/ipv4-global-routing-helper.h"
#include "ns3/tcp-l4-protocol.h"
#include "ns3/applications-module.h"
#include "ns3/csma-module.h"



using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("all-to-all");

int 
main (int argc, char *argv[])
{
  bool verbose = true;
  bool tracing = true;

  CommandLine cmd;
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
  cmd.AddValue ("tracing", "Enable PCAP tracing", tracing);
  
  cmd.Parse (argc,argv);

  if (verbose)
    {
      LogComponentEnable ("PacketSink", LOG_LEVEL_INFO);
      LogComponentEnable ("OnOffApplication", LOG_LEVEL_INFO);
    }


//创建网络结点

//分连接情况创建结点
  NodeContainer node1;
  node1.Create (2);
  NodeContainer node2;
  node2.Add (node1.Get(0));
  node2.Create (1);
  NodeContainer node3;
  node3.Create (2);
  NodeContainer node4;
  node4.Add (node3.Get(0));
  node4.Create (1);


  NodeContainer csmaNodes_1;
  csmaNodes_1.Add (node1.Get(1));
  csmaNodes_1.Create (2);

  NodeContainer csmaNodes_2;
  csmaNodes_2.Add (node3.Get(1));
  csmaNodes_2.Add (csmaNodes_1.Get(1));
  csmaNodes_2.Add (csmaNodes_1.Get(2));

  NodeContainer csmaNodes_3;
  csmaNodes_3.Add (node2.Get(1));
  csmaNodes_3.Create (2);

  NodeContainer csmaNodes_4;
  csmaNodes_4.Add (node4.Get(1));
  csmaNodes_4.Add (csmaNodes_3.Get(1));
  csmaNodes_4.Add (csmaNodes_3.Get(2));  

  NodeContainer csmaNodes_5;
  csmaNodes_5.Add (csmaNodes_1.Get(1));
  csmaNodes_5.Create (2);

  NodeContainer csmaNodes_6;
  csmaNodes_6.Add (csmaNodes_1.Get(2));
  csmaNodes_6.Create (2);

  NodeContainer csmaNodes_7;
  csmaNodes_7.Add (csmaNodes_3.Get(1));
  csmaNodes_7.Create (2);

  NodeContainer csmaNodes_8;
  csmaNodes_8.Add (csmaNodes_3.Get(2));
  csmaNodes_8.Create (2);

//第一层与第二层之间是pointtopoint，第二层到第三层之间是ethernet。第三层与第四层之间也是ethernet
  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("1.5Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (500)));

  NetDeviceContainer p2pDevices_1;
  p2pDevices_1 = pointToPoint.Install (node1);
  NetDeviceContainer p2pDevices_2;
  p2pDevices_2 = pointToPoint.Install (node2);
  NetDeviceContainer p2pDevices_3;
  p2pDevices_3 = pointToPoint.Install (node3);
  NetDeviceContainer p2pDevices_4;
  p2pDevices_4 = pointToPoint.Install (node4);

  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue ("1Mbps"));
  csma.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (500)));

  NetDeviceContainer csmaDevices_1;
  csmaDevices_1 = csma.Install (csmaNodes_1);
  NetDeviceContainer csmaDevices_2;
  csmaDevices_2 = csma.Install (csmaNodes_2);
  NetDeviceContainer csmaDevices_3;
  csmaDevices_3 = csma.Install (csmaNodes_3);
  NetDeviceContainer csmaDevices_4;
  csmaDevices_4 = csma.Install (csmaNodes_4);
  NetDeviceContainer csmaDevices_5;
  csmaDevices_5 = csma.Install (csmaNodes_5);
  NetDeviceContainer csmaDevices_6;
  csmaDevices_6 = csma.Install (csmaNodes_6);
  NetDeviceContainer csmaDevices_7;
  csmaDevices_7 = csma.Install (csmaNodes_7);
  NetDeviceContainer csmaDevices_8;
  csmaDevices_8 = csma.Install (csmaNodes_8);

 
//安装协议栈（结点不能重复），分配IP地址
  InternetStackHelper stack;
  stack.Install (node1);
  stack.Install (node3);
  stack.Install (node2.Get(1));
  stack.Install (node4.Get(1));
  stack.Install (csmaNodes_5);
  stack.Install (csmaNodes_6);
  stack.Install (csmaNodes_7);
  stack.Install (csmaNodes_8);

  Ipv4AddressHelper address;
  Ipv4InterfaceContainer p2pInterfaces_1;
  address.SetBase ("192.168.1.0", "255.255.255.0");
  p2pInterfaces_1 = address.Assign (p2pDevices_1);
  Ipv4InterfaceContainer p2pInterfaces_2;
  address.SetBase ("192.168.2.0", "255.255.255.0");
  p2pInterfaces_2 = address.Assign (p2pDevices_2);
  Ipv4InterfaceContainer p2pInterfaces_3; 
  address.SetBase ("192.168.3.0", "255.255.255.0");
  p2pInterfaces_3 = address.Assign (p2pDevices_3); 
  Ipv4InterfaceContainer p2pInterfaces_4;
  address.SetBase ("192.168.4.0", "255.255.255.0");
  p2pInterfaces_4 = address.Assign (p2pDevices_4);  

  
  Ipv4InterfaceContainer csmaInterfaces_1;
  address.SetBase ("10.1.1.0", "255.255.255.0");
  csmaInterfaces_1 = address.Assign (csmaDevices_1);
  Ipv4InterfaceContainer csmaInterfaces_2;
  address.SetBase ("10.2.1.0", "255.255.255.0");
  csmaInterfaces_2 = address.Assign (csmaDevices_2);
  Ipv4InterfaceContainer csmaInterfaces_3;
  address.SetBase ("10.3.1.0", "255.255.255.0");
  csmaInterfaces_3 = address.Assign (csmaDevices_3);
  Ipv4InterfaceContainer csmaInterfaces_4;
  address.SetBase ("10.4.1.0", "255.255.255.0");
  csmaInterfaces_4 = address.Assign (csmaDevices_4);
  Ipv4InterfaceContainer csmaInterfaces_5;
  address.SetBase ("10.0.1.0", "255.255.255.0");
  csmaInterfaces_5 = address.Assign (csmaDevices_5);
  Ipv4InterfaceContainer csmaInterfaces_6;
  address.SetBase ("10.0.2.0", "255.255.255.0");
  csmaInterfaces_6 = address.Assign (csmaDevices_6);
  Ipv4InterfaceContainer csmaInterfaces_7;
  address.SetBase ("10.0.3.0", "255.255.255.0");
  csmaInterfaces_7 = address.Assign (csmaDevices_7);
  Ipv4InterfaceContainer csmaInterfaces_8;
  address.SetBase ("10.0.4.0", "255.255.255.0");
  csmaInterfaces_8 = address.Assign (csmaDevices_8);


  Config::SetDefault("ns3::Ipv4GlobalRouting::RandomEcmpRouting" , BooleanValue(true));
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

//1,2,3,4,5,6,7-----8

  int16_t sinkPort = 24;
//  unsigned int nFlows = 7;


  PacketSinkHelper packetSinkHelper_1("ns3::TcpSocketFactory",InetSocketAddress(csmaInterfaces_7.GetAddress(1),sinkPort));
  ApplicationContainer sinkApp_1 = packetSinkHelper_1.Install(csmaNodes_7.Get(1));
  sinkApp_1.Start(Seconds(0.));
  sinkApp_1.Stop(Seconds(10.));

  OnOffHelper client_1("ns3::TcpSocketFactory", InetSocketAddress(csmaInterfaces_7.GetAddress (1), sinkPort));
  client_1.SetAttribute ("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=50]"));
  client_1.SetAttribute ("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
  client_1.SetAttribute ("DataRate", DataRateValue (DataRate ("1Mbps")));
  client_1.SetAttribute ("PacketSize", UintegerValue (2000));
  ApplicationContainer clientApp_1 = client_1.Install (csmaNodes_5.Get (1));
  clientApp_1.Start(Seconds (1.0 ));
  clientApp_1.Stop (Seconds (11.0));

  PacketSinkHelper packetSinkHelper_2("ns3::TcpSocketFactory",InetSocketAddress(csmaInterfaces_5.GetAddress(2),sinkPort));
  ApplicationContainer sinkApp_2 = packetSinkHelper_2.Install(csmaNodes_5.Get(2));
  sinkApp_2.Start(Seconds(0.));
  sinkApp_2.Stop(Seconds(10.));

  OnOffHelper client_2("ns3::TcpSocketFactory", InetSocketAddress(csmaInterfaces_5.GetAddress (2), sinkPort));
  client_2.SetAttribute ("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=50]"));
  client_2.SetAttribute ("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
  client_2.SetAttribute ("DataRate", DataRateValue (DataRate ("1Mbps")));
  client_2.SetAttribute ("PacketSize", UintegerValue (2000));
  ApplicationContainer clientApp_2 = client_2.Install (csmaNodes_7.Get (2));
  clientApp_2.Start(Seconds (1.0 ));
  clientApp_2.Stop (Seconds (11.0));

  PacketSinkHelper packetSinkHelper_3("ns3::TcpSocketFactory",InetSocketAddress(csmaInterfaces_8.GetAddress(1),sinkPort));
  ApplicationContainer sinkApp_3 = packetSinkHelper_3.Install(csmaNodes_8.Get(1));
  sinkApp_3.Start(Seconds(0.));
  sinkApp_3.Stop(Seconds(10.));

  OnOffHelper client_3("ns3::TcpSocketFactory", InetSocketAddress(csmaInterfaces_8.GetAddress (1), sinkPort));
  client_3.SetAttribute ("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=50]"));
  client_3.SetAttribute ("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
  client_3.SetAttribute ("DataRate", DataRateValue (DataRate ("1Mbps")));
  client_3.SetAttribute ("PacketSize", UintegerValue (2000));
  ApplicationContainer clientApp_3 = client_3.Install (csmaNodes_6.Get (1));
  clientApp_3.Start(Seconds (1.0 ));
  clientApp_3.Stop (Seconds (11.0));

  PacketSinkHelper packetSinkHelper_4("ns3::TcpSocketFactory",InetSocketAddress(csmaInterfaces_6.GetAddress(2),sinkPort));
  ApplicationContainer sinkApp_4 = packetSinkHelper_4.Install(csmaNodes_6.Get(2));
  sinkApp_4.Start(Seconds(0.));
  sinkApp_4.Stop(Seconds(10.));

  OnOffHelper client_4("ns3::TcpSocketFactory", InetSocketAddress(csmaInterfaces_6.GetAddress (2), sinkPort));
  client_4.SetAttribute ("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=50]"));
  client_4.SetAttribute ("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
  client_4.SetAttribute ("DataRate", DataRateValue (DataRate ("1Mbps")));
  client_4.SetAttribute ("PacketSize", UintegerValue (2000));
  ApplicationContainer clientApp_4 = client_4.Install (csmaNodes_8.Get (2));
  clientApp_4.Start(Seconds (1.0 ));
  clientApp_4.Stop (Seconds (11.0));

/*
  ApplicationContainer sinkApp[nFlows];   
  for(unsigned int i = 0;i < nFlows; i++)
  {
    PacketSinkHelper packetSinkHelper ("ns3::TcpSocketFactory", InetSocketAddress(csmaInterfaces_8.GetAddress(2),sinkPort + i));
    sinkApp[i] = packetSinkHelper.Install (csmaNodes_8.Get(2));
    sinkApp[i].Start(Seconds (0.0));
    sinkApp[i].Stop(Seconds (10.0));
  }

  
  OnOffHelper client_1("ns3::TcpSocketFactory", InetSocketAddress(csmaInterfaces_8.GetAddress (2), sinkPort));
  client_1.SetAttribute ("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=50]"));
  client_1.SetAttribute ("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
  client_1.SetAttribute ("DataRate", DataRateValue (DataRate ("1Mbps")));
  client_1.SetAttribute ("PacketSize", UintegerValue (2000));
  ApplicationContainer clientApp_1 = client_1.Install (csmaNodes_5.Get (1));
  clientApp_1.Start(Seconds (1.0 ));
  clientApp_1.Stop (Seconds (11.0));

  OnOffHelper client_2("ns3::TcpSocketFactory", InetSocketAddress(csmaInterfaces_8.GetAddress (2), sinkPort+1));
  client_2.SetAttribute ("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=50]"));
  client_2.SetAttribute ("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
  client_2.SetAttribute ("DataRate", DataRateValue (DataRate ("1Mbps")));
  client_2.SetAttribute ("PacketSize", UintegerValue (2000));
  ApplicationContainer clientApp_2 = client_2.Install (csmaNodes_5.Get (2));
  clientApp_2.Start(Seconds (1.1 ));
  clientApp_2.Stop (Seconds (11.0));

  OnOffHelper client_3("ns3::TcpSocketFactory", InetSocketAddress(csmaInterfaces_8.GetAddress (2), sinkPort+2));
  client_3.SetAttribute ("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=50]"));
  client_3.SetAttribute ("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
  client_3.SetAttribute ("DataRate", DataRateValue (DataRate ("1Mbps")));
  client_3.SetAttribute ("PacketSize", UintegerValue (2000));
  ApplicationContainer clientApp_3 = client_3.Install (csmaNodes_6.Get (1));
  clientApp_3.Start(Seconds (1.2 ));
  clientApp_3.Stop (Seconds (11.0));

  OnOffHelper client_4("ns3::TcpSocketFactory", InetSocketAddress(csmaInterfaces_8.GetAddress (2), sinkPort+3));
  client_4.SetAttribute ("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=50]"));
  client_4.SetAttribute ("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
  client_4.SetAttribute ("DataRate", DataRateValue (DataRate ("1Mbps")));
  client_4.SetAttribute ("PacketSize", UintegerValue (2000));
  ApplicationContainer clientApp_4 = client_4.Install (csmaNodes_6.Get (2));
  clientApp_4.Start(Seconds (1.3 ));
  clientApp_4.Stop (Seconds (11.0));

  OnOffHelper client_5("ns3::TcpSocketFactory", InetSocketAddress(csmaInterfaces_8.GetAddress (2), sinkPort+4));
  client_5.SetAttribute ("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=50]"));
  client_5.SetAttribute ("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
  client_5.SetAttribute ("DataRate", DataRateValue (DataRate ("1Mbps")));
  client_5.SetAttribute ("PacketSize", UintegerValue (2000));
  ApplicationContainer clientApp_5 = client_5.Install (csmaNodes_7.Get (1));
  clientApp_5.Start(Seconds (1.4 ));
  clientApp_5.Stop (Seconds (11.0));

  OnOffHelper client_6("ns3::TcpSocketFactory", InetSocketAddress(csmaInterfaces_8.GetAddress (2), sinkPort+5));
  client_6.SetAttribute ("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=50]"));
  client_6.SetAttribute ("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
  client_6.SetAttribute ("DataRate", DataRateValue (DataRate ("1Mbps")));
  client_6.SetAttribute ("PacketSize", UintegerValue (2000));
  ApplicationContainer clientApp_6 = client_6.Install (csmaNodes_7.Get (2));
  clientApp_6.Start(Seconds (1.5 ));
  clientApp_6.Stop (Seconds (11.0));

  OnOffHelper client_7("ns3::TcpSocketFactory", InetSocketAddress(csmaInterfaces_8.GetAddress (2), sinkPort+6));
  client_7.SetAttribute ("OnTime", StringValue("ns3::ConstantRandomVariable[Constant=50]"));
  client_7.SetAttribute ("OffTime", StringValue("ns3::ConstantRandomVariable[Constant=0]"));
  client_7.SetAttribute ("DataRate", DataRateValue (DataRate ("1Mbps")));
  client_7.SetAttribute ("PacketSize", UintegerValue (2000));
  ApplicationContainer clientApp_7 = client_7.Install (csmaNodes_8.Get (1));
  clientApp_7.Start(Seconds (1.6 ));
  clientApp_7.Stop (Seconds (11.0));

*/

/*
//抓包在端口192.168.4.1(c2)，10.1.4.1(a4)，10.0.8.1(t4) 
  pointToPoint.EnablePcap ("improve", p2pDevices_3.Get (0), true); 
  csma.EnablePcap ("improve", csmaDevices_4.Get (0), true);
  csma.EnablePcap ("improve", csmaDevices_8.Get (0), true);
*/

//抓包在端口192.168.4.1(c2) 192.168. 
  pointToPoint.EnablePcap ("improve1", p2pDevices_1.Get (0), true); 
  pointToPoint.EnablePcap ("improve1", p2pDevices_2.Get (0), true); 
  pointToPoint.EnablePcap ("improve1", p2pDevices_3.Get (0), true); 
  pointToPoint.EnablePcap ("improve1", p2pDevices_4.Get (0), true); 


  
  Simulator::Stop (Seconds(10));
  Simulator::Run ();
  Simulator::Destroy ();

  return 0;
}
