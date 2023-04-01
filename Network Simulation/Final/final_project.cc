#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/applications-module.h"
#include "ns3/mobility-module.h"
#include "ns3/internet-module.h"
#include "ns3/yans-wifi-helper.h"
#include "ns3/command-line.h"
#include "ns3/flow-monitor.h"
#include "ns3/flow-monitor-helper.h"
#include <cstdio>

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("WifiAdhoc");

void main_function(uint32_t nNodes, uint32_t packetSize, bool verbose, bool pcap, uint32_t maxPackets, uint32_t interval, uint32_t serverNode)
{ 
  std::cout << "Running simulation with " << nNodes << " nodes..." << std::endl;

  if (verbose)
  {
    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);
  }

  // Disable RTS/CTS by setting the RTS/CTS threshold
  UintegerValue threshold = 1000;
  Config::SetDefault("ns3::WifiRemoteStationManager::RtsCtsThreshold", threshold);

  nNodes = (nNodes < 2) ? 2 : nNodes;

  // Create the nodes that compose the network
  NodeContainer nodes;
  nodes.Create(nNodes);

  // Configure the PHY and channel helpers
  YansWifiChannelHelper channel = YansWifiChannelHelper::Default();
  YansWifiPhyHelper wifiPhy;

  wifiPhy.SetChannel(channel.Create());

  WifiMacHelper mac;

  WifiHelper wifi;

  mac.SetType("ns3::AdhocWifiMac");

  NetDeviceContainer devices;
  devices = wifi.Install(wifiPhy, mac, nodes);

  //Adding mobility model
  MobilityHelper mobility;
  mobility.SetPositionAllocator("ns3::GridPositionAllocator", 
                                "MinX", DoubleValue(0.0), 
                                "MinY", DoubleValue(0.0), 
                                "DeltaX", DoubleValue(5.0), 
                                "DeltaY", DoubleValue(10.0), 
                                "GridWidth", UintegerValue(3), 
                                "LayoutType", StringValue("RowFirst"));

  // Set the mobility model to be ns3::ConstantPositionMoblityModel to fixed the position of the devices
  mobility.SetMobilityModel("ns3::ConstantPositionMobilityModel");
  mobility.Install(nodes);

  // Use InternetStackHelper to install protocol stacks
  InternetStackHelper stack;
  stack.Install(nodes);

  Ipv4AddressHelper address;
  address.SetBase("10.1.1.0", "255.255.255.0");
  Ipv4InterfaceContainer nodeInterfaces;
  nodeInterfaces = address.Assign(devices);

  //set up an echo server
  UdpEchoServerHelper echoServer(9);

  ApplicationContainer serverApps = echoServer.Install(nodes.Get(serverNode));
  serverApps.Start(Seconds(2.0));
  serverApps.Stop(Seconds(15));

  UdpEchoClientHelper echoClient(nodeInterfaces.GetAddress(serverNode), 9);
  echoClient.SetAttribute("MaxPackets", UintegerValue(maxPackets)); 
  echoClient.SetAttribute("Interval", TimeValue(Seconds(interval)));
  echoClient.SetAttribute("PacketSize", UintegerValue(packetSize)); 

  for (uint32_t i = 0; i < nNodes; i++)
  {
    if (i == serverNode) continue;
    ApplicationContainer clientApp = echoClient.Install(nodes.Get(i));
    clientApp.Start(Seconds(2.0));
    clientApp.Stop(Seconds(15));
  }

  // Enable pcap
  if (pcap)
  {
    wifiPhy.EnablePcap("wifi-adhoc", devices.Get(2), true);
  }

    // Install flowMonitor to collect data
  Ptr<FlowMonitor> flowMonitor;
  FlowMonitorHelper flowHelper;
  flowMonitor = flowHelper.InstallAll();

  Simulator::Stop(Seconds(15));
  Simulator::Run();
  Simulator::Destroy();

  flowMonitor->SerializeToXmlFile("collectedData.xml", true, true);
}

int main(int argc, char *argv[])
{
  uint32_t packetSize = 512;
  uint32_t maxPackets = 10; 
  uint32_t interval = 1;   
  bool verbose = false;    
  bool pcap = false;  
  uint32_t serverNode = 0;

  // Change the parameters via command line argument
  CommandLine cmd;
  cmd.AddValue("packetSize", "", packetSize);
  cmd.AddValue("maxPackets", "Max packets to send", maxPackets);
  cmd.AddValue("interval", "Interval between packets", interval);
  cmd.AddValue("verbose", "Enable logging", verbose);
  cmd.AddValue("pcap", "Enable pcap", pcap);
  cmd.Parse(argc, argv);

  for (uint32_t i = 2; i <= 30; i++)
    {
      main_function(i, packetSize, verbose, pcap, maxPackets, interval, serverNode);
    }
}