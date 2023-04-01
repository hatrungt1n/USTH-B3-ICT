#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/csma-module.h"
#include "ns3/internet-module.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/ssid.h"

// Default Network Topology
//
//  Wifi 10.1.2.0
//              AP
//    *    *    *
//    |    |    |  
//    n2   n3   n0   n1  
//              |    |    
//              ======
//           LAN 10.1.1.0

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("Lab4");

int 
main (int argc, char *argv[])
{
  bool verbose = true;
  uint32_t nCsma = 2;
  uint32_t nWifi = 2;
  bool tracing = true;

  CommandLine cmd (__FILE__);
  cmd.AddValue ("nCsma", "Number of \"extra\" CSMA nodes/devices", nCsma);
  cmd.AddValue ("nWifi", "Number of wifi STA devices", nWifi);
  cmd.AddValue ("verbose", "Tell echo applications to log if true", verbose);
  cmd.AddValue ("tracing", "Enable pcap tracing", tracing);

  cmd.Parse (argc,argv);

  if (verbose)
    {
      LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
      LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);
    }
  //create 2 csma nodes 
  NodeContainer csmaNodes;
  csmaNodes.Create (nCsma);

  //Set a helper which creates and connects CSMA devices and channels
  CsmaHelper csma;
  csma.SetChannelAttribute ("DataRate", StringValue ("100Mbps"));
  csma.SetChannelAttribute ("Delay", TimeValue (NanoSeconds (6560)));

  //create a container to hold the devices created and install the devices into the nodes
  NetDeviceContainer csmaDevices;
  csmaDevices = csma.Install (csmaNodes);

  //Create 2 wifi nodes (2 station nodes: STA1, STA2)
  NodeContainer wifiStaNodes;
  wifiStaNodes.Create (nWifi);

  //Create the AP node, which is also the first csma node
  NodeContainer wifiApNode = csmaNodes.Get (0);
 
  //constructs the wifi devices and the interconnection channel between these wifi nodes
  YansWifiChannelHelper channel = YansWifiChannelHelper::Default ();
  YansWifiPhyHelper phy;
  phy.SetChannel (channel.Create ());

  WifiHelper wifi;
  wifi.SetRemoteStationManager ("ns3::AarfWifiManager");

  WifiMacHelper mac;
  Ssid ssid = Ssid ("ns-3-ssid");
  mac.SetType ("ns3::StaWifiMac",
               "Ssid", SsidValue (ssid),
               "ActiveProbing", BooleanValue (false));

  NetDeviceContainer staDevices;
  staDevices = wifi.Install (phy, mac, wifiStaNodes);

  mac.SetType ("ns3::ApWifiMac",
               "Ssid", SsidValue (ssid));

  NetDeviceContainer apDevices;
  apDevices = wifi.Install (phy, mac, wifiApNode);
  
  //Adding mobility model
  MobilityHelper mobility;

  mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                 "MinX", DoubleValue (0.0),
                                 "MinY", DoubleValue (0.0),
                                 "DeltaX", DoubleValue (5.0),
                                 "DeltaY", DoubleValue (10.0),
                                 "GridWidth", UintegerValue (3),
                                 "LayoutType", StringValue ("RowFirst"));

  mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
  mobility.Install (wifiStaNodes);
  mobility.Install (wifiApNode);

  // mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
  //                            "Bounds", RectangleValue (Rectangle (-50, 50, -50, 50)));

  //install protocol stacks to csma nodes and wifi nodes
  InternetStackHelper stack;
  stack.Install (csmaNodes);
  stack.Install (wifiStaNodes);

  //set base and assign IP addresses to device interfaces
  Ipv4AddressHelper address;

  address.SetBase ("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer csmaInterfaces;
  csmaInterfaces = address.Assign (csmaDevices);

  address.SetBase ("10.1.2.0", "255.255.255.0");
  address.Assign (staDevices);
  address.Assign (apDevices);

  //set up an echo server
  UdpEchoServerHelper echoServer (9);

  //set applications into server, server is the second csma node
  ApplicationContainer serverApps = echoServer.Install (csmaNodes.Get (nCsma-1));
  serverApps.Start (Seconds (1.0));
  serverApps.Stop (Seconds (11.0));

  //set up an echo client and get the address of the server
  UdpEchoClientHelper echoClient (csmaInterfaces.GetAddress (nCsma-1), 9);
  echoClient.SetAttribute ("MaxPackets", UintegerValue (100));
  echoClient.SetAttribute ("Interval", TimeValue (Seconds (0.1)));
  echoClient.SetAttribute ("PacketSize", UintegerValue (1024));

  //set applications into client, client is the AP node
  ApplicationContainer clientApps = echoClient.Install (wifiApNode.Get (0));
  clientApps.Start (Seconds (1.0));
  clientApps.Stop (Seconds (11.0));

  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  Simulator::Stop (Seconds (11.0));

  //capture pcap trace from all nodes
  if (tracing)
    {
      phy.EnablePcap ("apTrace", apDevices.Get (0));
      phy.EnablePcap ("sta1Trace", staDevices.Get (0));
      phy.EnablePcap ("sta2Trace", staDevices.Get (1));
      csma.EnablePcap ("Strace", csmaDevices.Get (0), true);
    }

  Simulator::Run ();
  Simulator::Destroy ();
  return 0;
}
