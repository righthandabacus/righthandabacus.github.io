---
layout: post
title: "Call dependency graph in NS-3"
date: 2011-08-02 00:10:38 -0400
tags: code
---

To understand NS-3 better, or to know how something is being done in NS-3, or just to know where to modify, run the following [code](/img/verysimple.cc):

{% raw %}
```c++
/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright 2011 (c) Adrian Sai-wah Tam
 *
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
 *
 * Author: Adrian Sai-wah Tam <adrian.sw.tam@gmail.com>
 *
 */

#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/internet-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/applications-module.h"
#include "ns3/point-to-point-layout-module.h"

// Network topology: n1--- n0---n2

using namespace ns3;

NS_LOG_COMPONENT_DEFINE ("VerySimple");

int 
main (int argc, char *argv[])
{
  // To turn on individual log, use:
  //    LogComponentEnable("VerySimple", LOG_LEVEL_ALL);
  // To print the names of all log-capable componenets, use:
  //    LogComponentPrintList();
  LogComponentEnableAll(LOG_LEVEL_ALL);
  LogComponentEnableAll(LOG_PREFIX_TIME);
  LogComponentEnableAll(LOG_PREFIX_FUNC);

  // Create topology and install IPv4
  NS_LOG_INFO ("Build topology.");
  PointToPointHelper pointToPoint;
  pointToPoint.SetDeviceAttribute ("DataRate", StringValue ("1000Mbps"));
  pointToPoint.SetChannelAttribute ("Delay", StringValue ("100us"));
  pointToPoint.SetQueue("ns3::DropTailQueue", "Mode", EnumValue(DropTailQueue::BYTES), "MaxBytes", UintegerValue(32768));
  PointToPointStarHelper star (2, pointToPoint);

  NS_LOG_INFO ("Install internet stack on all nodes.");
  InternetStackHelper internet;
  star.InstallStack (internet);
  star.AssignIpv4Addresses (Ipv4AddressHelper ("10.1.1.0", "255.255.255.0"));

  // Turn on global static routing
  NS_LOG_INFO ("Enable static global routing.");
  Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

  // Create a packet sink on spoke 0 to receive packets.
  NS_LOG_INFO ("Create applications.");
  uint16_t port = 50000;
  Address hubLocalAddress (InetSocketAddress (Ipv4Address::GetAny (), port));
  PacketSinkHelper packetSinkHelper ("ns3::UdpSocketFactory", hubLocalAddress);
  ApplicationContainer hubApp = packetSinkHelper.Install (star.GetSpokeNode (0));
  hubApp.Start (Seconds (1.0));
  hubApp.Stop (Seconds (10.0));

  // Create an applications to send TCP from spoke 1 to the spoke 0
  OnOffHelper onOffHelper ("ns3::UdpSocketFactory", Address ());
  onOffHelper.SetAttribute ("OnTime", RandomVariableValue (ConstantVariable (1)));
  onOffHelper.SetAttribute ("OffTime", RandomVariableValue (ConstantVariable (0)));
  onOffHelper.SetAttribute ("MaxBytes", UintegerValue (1));
  ApplicationContainer spokeApps;
  AddressValue remoteAddress (InetSocketAddress (star.GetSpokeIpv4Address (0), port));
  onOffHelper.SetAttribute ("Remote", remoteAddress);
  spokeApps.Add (onOffHelper.Install (star.GetSpokeNode (1)));
  spokeApps.Start (Seconds (1.0));
  spokeApps.Stop (Seconds (10.0));

  // Turn on trace
  pointToPoint.EnableAsciiAll (Create<OutputStreamWrapper>(&std::cout));

  // Run
  NS_LOG_INFO ("Run Simulation.");
  Simulator::Run ();
  Simulator::Destroy ();
  NS_LOG_INFO ("Done.");

  return 0;
}
```
{% endraw %}

This code try to dump every single debug message to the console, as well as the packet trace. From there, we can see what is being run in NS-3. The [output](/img/verysimple.output.txt) is as follows:

## Topology creation

(line 1-198)
```
VerySimple:main(): Build topology.
NodeList:NodeListPriv()
ObjectBase:ConstructSelf(): construct tid=ns3::NodeListPriv, params=1
ObjectBase:ConstructSelf(): try to construct "ns3::NodeListPriv::NodeList"
ObjectBase:ConstructSelf(): construct "ns3::NodeListPriv::NodeList" from initial value.
ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
Simulator:ScheduleDestroy(1)
ObjectBase:ConstructSelf(): construct tid=ns3::DefaultSimulatorImpl, params=0
ObjectBase:ConstructSelf(): construct tid=ns3::SimulatorImpl, params=0
ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
ObjectBase:ConstructSelf(): construct tid=ns3::MapScheduler, params=0
ObjectBase:ConstructSelf(): construct tid=ns3::Scheduler, params=0
ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s DefaultSimulatorImpl:ScheduleWithContext(0x7fcc12c479b0, 0, 0, 0, 0x7fcc12c47a60)
0s MapScheduler:Insert(0x7fcc12c47a10, 0x7fcc12c47a60, 0, 5)
0s ObjectBase:ConstructSelf(): construct tid=ns3::Node, params=3
0s ObjectBase:ConstructSelf(): try to construct "ns3::Node::DeviceList"
0s ObjectBase:ConstructSelf(): construct "ns3::Node::DeviceList" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::Node::ApplicationList"
0s ObjectBase:ConstructSelf(): construct "ns3::Node::ApplicationList" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::Node::Id"
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s DefaultSimulatorImpl:ScheduleWithContext(0x7fcc12c479b0, 1, 0, 0, 0x7fcc12c47bd0)
0s MapScheduler:Insert(0x7fcc12c47a10, 0x7fcc12c47bd0, 0, 6)
0s ObjectBase:ConstructSelf(): construct tid=ns3::Node, params=3
0s ObjectBase:ConstructSelf(): try to construct "ns3::Node::DeviceList"
0s ObjectBase:ConstructSelf(): construct "ns3::Node::DeviceList" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::Node::ApplicationList"
0s ObjectBase:ConstructSelf(): construct "ns3::Node::ApplicationList" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::Node::Id"
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s DefaultSimulatorImpl:ScheduleWithContext(0x7fcc12c479b0, 2, 0, 0, 0x7fcc12c47d30)
0s MapScheduler:Insert(0x7fcc12c47a10, 0x7fcc12c47d30, 0, 7)
0s ObjectBase:ConstructSelf(): construct tid=ns3::Node, params=3
0s ObjectBase:ConstructSelf(): try to construct "ns3::Node::DeviceList"
0s ObjectBase:ConstructSelf(): construct "ns3::Node::DeviceList" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::Node::ApplicationList"
0s ObjectBase:ConstructSelf(): construct "ns3::Node::ApplicationList" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::Node::Id"
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s PointToPointNetDevice:PointToPointNetDevice(0x7fcc12c47da0)
0s ObjectBase:ConstructSelf(): construct tid=ns3::PointToPointNetDevice, params=6
0s ObjectBase:ConstructSelf(): try to construct "ns3::PointToPointNetDevice::Mtu"
0s PointToPointNetDevice:SetMtu(0x7fcc12c47da0, 1500)
0s ObjectBase:ConstructSelf(): construct "ns3::PointToPointNetDevice::Mtu" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::PointToPointNetDevice::Address"
0s ObjectBase:ConstructSelf(): construct "ns3::PointToPointNetDevice::Address" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::PointToPointNetDevice::DataRate"
0s ObjectBase:ConstructSelf(): construct "ns3::PointToPointNetDevice::DataRate"
0s ObjectBase:ConstructSelf(): try to construct "ns3::PointToPointNetDevice::ReceiveErrorModel"
0s ObjectBase:ConstructSelf(): construct "ns3::PointToPointNetDevice::ReceiveErrorModel" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::PointToPointNetDevice::InterframeGap"
0s ObjectBase:ConstructSelf(): construct "ns3::PointToPointNetDevice::InterframeGap" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::PointToPointNetDevice::TxQueue"
0s ObjectBase:ConstructSelf(): construct "ns3::PointToPointNetDevice::TxQueue" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::NetDevice, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s DefaultSimulatorImpl:ScheduleWithContext(0x7fcc12c479b0, 0, 0, 0, 0x7fcc12c47f30)
0s MapScheduler:Insert(0x7fcc12c47a10, 0x7fcc12c47f30, 0, 8)
0s Queue:Queue()
0s DropTailQueue:DropTailQueue()
0s ObjectBase:ConstructSelf(): construct tid=ns3::DropTailQueue, params=3
0s ObjectBase:ConstructSelf(): try to construct "ns3::DropTailQueue::Mode"
0s DropTailQueue:SetMode(2)
0s ObjectBase:ConstructSelf(): construct "ns3::DropTailQueue::Mode"
0s ObjectBase:ConstructSelf(): try to construct "ns3::DropTailQueue::MaxPackets"
0s ObjectBase:ConstructSelf(): construct "ns3::DropTailQueue::MaxPackets" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::DropTailQueue::MaxBytes"
0s ObjectBase:ConstructSelf(): construct "ns3::DropTailQueue::MaxBytes"
0s ObjectBase:ConstructSelf(): construct tid=ns3::Queue, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s PointToPointNetDevice:SetQueue(0x7fcc12c47da0, 0x7fcc12c47fa0)
0s PointToPointNetDevice:PointToPointNetDevice(0x7fcc12c480b0)
0s ObjectBase:ConstructSelf(): construct tid=ns3::PointToPointNetDevice, params=6
0s ObjectBase:ConstructSelf(): try to construct "ns3::PointToPointNetDevice::Mtu"
0s PointToPointNetDevice:SetMtu(0x7fcc12c480b0, 1500)
0s ObjectBase:ConstructSelf(): construct "ns3::PointToPointNetDevice::Mtu" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::PointToPointNetDevice::Address"
0s ObjectBase:ConstructSelf(): construct "ns3::PointToPointNetDevice::Address" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::PointToPointNetDevice::DataRate"
0s ObjectBase:ConstructSelf(): construct "ns3::PointToPointNetDevice::DataRate"
0s ObjectBase:ConstructSelf(): try to construct "ns3::PointToPointNetDevice::ReceiveErrorModel"
0s ObjectBase:ConstructSelf(): construct "ns3::PointToPointNetDevice::ReceiveErrorModel" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::PointToPointNetDevice::InterframeGap"
0s ObjectBase:ConstructSelf(): construct "ns3::PointToPointNetDevice::InterframeGap" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::PointToPointNetDevice::TxQueue"
0s ObjectBase:ConstructSelf(): construct "ns3::PointToPointNetDevice::TxQueue" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::NetDevice, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s DefaultSimulatorImpl:ScheduleWithContext(0x7fcc12c479b0, 1, 0, 0, 0x7fcc12c48240)
0s MapScheduler:Insert(0x7fcc12c47a10, 0x7fcc12c48240, 0, 9)
0s Queue:Queue()
0s DropTailQueue:DropTailQueue()
0s ObjectBase:ConstructSelf(): construct tid=ns3::DropTailQueue, params=3
0s ObjectBase:ConstructSelf(): try to construct "ns3::DropTailQueue::Mode"
0s DropTailQueue:SetMode(2)
0s ObjectBase:ConstructSelf(): construct "ns3::DropTailQueue::Mode"
0s ObjectBase:ConstructSelf(): try to construct "ns3::DropTailQueue::MaxPackets"
0s ObjectBase:ConstructSelf(): construct "ns3::DropTailQueue::MaxPackets" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::DropTailQueue::MaxBytes"
0s ObjectBase:ConstructSelf(): construct "ns3::DropTailQueue::MaxBytes"
0s ObjectBase:ConstructSelf(): construct tid=ns3::Queue, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s PointToPointNetDevice:SetQueue(0x7fcc12c480b0, 0x7fcc12c484f0)
0s Channel:Channel()
0s ChannelList:ChannelListPriv()
0s ObjectBase:ConstructSelf(): construct tid=ns3::ChannelListPriv, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::ChannelListPriv::ChannelList"
0s ObjectBase:ConstructSelf(): construct "ns3::ChannelListPriv::ChannelList" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Simulator:ScheduleDestroy(1)
0s PointToPointChannel:PointToPointChannel()
0s ObjectBase:ConstructSelf(): construct tid=ns3::PointToPointChannel, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::PointToPointChannel::Delay"
0s ObjectBase:ConstructSelf(): construct "ns3::PointToPointChannel::Delay"
0s ObjectBase:ConstructSelf(): construct tid=ns3::Channel, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Channel::Id"
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s PointToPointNetDevice:Attach(0x7fcc12c47da0, 0x7fff6d4024f0)
0s PointToPointChannel:Attach(0x7fcc12c485c0, 0x7fcc12c47da0)
0s PointToPointNetDevice:Attach(0x7fcc12c480b0, 0x7fff6d4024f8)
0s PointToPointChannel:Attach(0x7fcc12c485c0, 0x7fcc12c480b0)
0s PointToPointNetDevice:PointToPointNetDevice(0x7fcc12c48a00)
0s ObjectBase:ConstructSelf(): construct tid=ns3::PointToPointNetDevice, params=6
0s ObjectBase:ConstructSelf(): try to construct "ns3::PointToPointNetDevice::Mtu"
0s PointToPointNetDevice:SetMtu(0x7fcc12c48a00, 1500)
0s ObjectBase:ConstructSelf(): construct "ns3::PointToPointNetDevice::Mtu" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::PointToPointNetDevice::Address"
0s ObjectBase:ConstructSelf(): construct "ns3::PointToPointNetDevice::Address" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::PointToPointNetDevice::DataRate"
0s ObjectBase:ConstructSelf(): construct "ns3::PointToPointNetDevice::DataRate"
0s ObjectBase:ConstructSelf(): try to construct "ns3::PointToPointNetDevice::ReceiveErrorModel"
0s ObjectBase:ConstructSelf(): construct "ns3::PointToPointNetDevice::ReceiveErrorModel" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::PointToPointNetDevice::InterframeGap"
0s ObjectBase:ConstructSelf(): construct "ns3::PointToPointNetDevice::InterframeGap" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::PointToPointNetDevice::TxQueue"
0s ObjectBase:ConstructSelf(): construct "ns3::PointToPointNetDevice::TxQueue" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::NetDevice, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s DefaultSimulatorImpl:ScheduleWithContext(0x7fcc12c479b0, 0, 0, 0, 0x7fcc12c486b0)
0s MapScheduler:Insert(0x7fcc12c47a10, 0x7fcc12c486b0, 0, 11)
0s Queue:Queue()
0s DropTailQueue:DropTailQueue()
0s ObjectBase:ConstructSelf(): construct tid=ns3::DropTailQueue, params=3
0s ObjectBase:ConstructSelf(): try to construct "ns3::DropTailQueue::Mode"
0s DropTailQueue:SetMode(2)
0s ObjectBase:ConstructSelf(): construct "ns3::DropTailQueue::Mode"
0s ObjectBase:ConstructSelf(): try to construct "ns3::DropTailQueue::MaxPackets"
0s ObjectBase:ConstructSelf(): construct "ns3::DropTailQueue::MaxPackets" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::DropTailQueue::MaxBytes"
0s ObjectBase:ConstructSelf(): construct "ns3::DropTailQueue::MaxBytes"
0s ObjectBase:ConstructSelf(): construct tid=ns3::Queue, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s PointToPointNetDevice:SetQueue(0x7fcc12c48a00, 0x7fcc12c48b60)
0s PointToPointNetDevice:PointToPointNetDevice(0x7fcc12c48c30)
0s ObjectBase:ConstructSelf(): construct tid=ns3::PointToPointNetDevice, params=6
0s ObjectBase:ConstructSelf(): try to construct "ns3::PointToPointNetDevice::Mtu"
0s PointToPointNetDevice:SetMtu(0x7fcc12c48c30, 1500)
0s ObjectBase:ConstructSelf(): construct "ns3::PointToPointNetDevice::Mtu" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::PointToPointNetDevice::Address"
0s ObjectBase:ConstructSelf(): construct "ns3::PointToPointNetDevice::Address" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::PointToPointNetDevice::DataRate"
0s ObjectBase:ConstructSelf(): construct "ns3::PointToPointNetDevice::DataRate"
0s ObjectBase:ConstructSelf(): try to construct "ns3::PointToPointNetDevice::ReceiveErrorModel"
0s ObjectBase:ConstructSelf(): construct "ns3::PointToPointNetDevice::ReceiveErrorModel" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::PointToPointNetDevice::InterframeGap"
0s ObjectBase:ConstructSelf(): construct "ns3::PointToPointNetDevice::InterframeGap" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::PointToPointNetDevice::TxQueue"
0s ObjectBase:ConstructSelf(): construct "ns3::PointToPointNetDevice::TxQueue" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::NetDevice, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s DefaultSimulatorImpl:ScheduleWithContext(0x7fcc12c479b0, 2, 0, 0, 0x7fcc12c48dc0)
0s MapScheduler:Insert(0x7fcc12c47a10, 0x7fcc12c48dc0, 0, 12)
0s Queue:Queue()
0s DropTailQueue:DropTailQueue()
0s ObjectBase:ConstructSelf(): construct tid=ns3::DropTailQueue, params=3
0s ObjectBase:ConstructSelf(): try to construct "ns3::DropTailQueue::Mode"
0s DropTailQueue:SetMode(2)
0s ObjectBase:ConstructSelf(): construct "ns3::DropTailQueue::Mode"
0s ObjectBase:ConstructSelf(): try to construct "ns3::DropTailQueue::MaxPackets"
0s ObjectBase:ConstructSelf(): construct "ns3::DropTailQueue::MaxPackets" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::DropTailQueue::MaxBytes"
0s ObjectBase:ConstructSelf(): construct "ns3::DropTailQueue::MaxBytes"
0s ObjectBase:ConstructSelf(): construct tid=ns3::Queue, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s PointToPointNetDevice:SetQueue(0x7fcc12c48c30, 0x7fcc12c49030)
0s Channel:Channel()
0s PointToPointChannel:PointToPointChannel()
0s ObjectBase:ConstructSelf(): construct tid=ns3::PointToPointChannel, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::PointToPointChannel::Delay"
0s ObjectBase:ConstructSelf(): construct "ns3::PointToPointChannel::Delay"
0s ObjectBase:ConstructSelf(): construct tid=ns3::Channel, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Channel::Id"
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s PointToPointNetDevice:Attach(0x7fcc12c48a00, 0x7fff6d4024f0)
0s PointToPointChannel:Attach(0x7fcc12c49100, 0x7fcc12c48a00)
0s PointToPointNetDevice:Attach(0x7fcc12c48c30, 0x7fff6d4024f8)
0s PointToPointChannel:Attach(0x7fcc12c49100, 0x7fcc12c48c30)
```

All of the above are generated by the line

    PointToPointStarHelper star (2, pointToPoint);

Only this line start the action of creating topology, which first involves the simulator and hence created as well. Here we see that
  - There is a NodeList in global space
  - For each `Node`, there is a `DeviceList` and `ApplicationList`
  - The `NodeList` scheduled a clean-up upon `Simulator` ends, i.e. `ScheduleDestroy` call
  - After node creation, the `PointToPointHelper` created the `PointToPointNetDevice` and the channels

## Install TCP/IP

(line 199-1117)
```
0s VerySimple:main(): Install internet stack on all nodes.
0s ArpL3Protocol:ArpL3Protocol(0x7fcc12c491e0)
0s ObjectBase:ConstructSelf(): construct tid=ns3::ArpL3Protocol, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::ArpL3Protocol::CacheList"
0s ObjectBase:ConstructSelf(): construct "ns3::ArpL3Protocol::CacheList" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s ArpL3Protocol:SetNode(0x7fcc12c491e0)
0s Ipv4L3Protocol:Ipv4L3Protocol(0x7fcc12c49540)
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4L3Protocol, params=3
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv4L3Protocol::DefaultTtl"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv4L3Protocol::DefaultTtl" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv4L3Protocol::FragmentExpirationTimeout"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv4L3Protocol::FragmentExpirationTimeout" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv4L3Protocol::InterfaceList"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv4L3Protocol::InterfaceList" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4, params=2
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv4::IpForward"
0s Ipv4L3Protocol:SetIpForward(0x7fcc12c49540, 1)
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv4::IpForward" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv4::WeakEsModel"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv4::WeakEsModel" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv4L3Protocol:SetupLoopback(0x7fcc12c49540)
0s Ipv4Interface:Ipv4Interface(0x7fcc12c49270)
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4Interface, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv4Interface::ArpCache"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv4Interface::ArpCache" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s LoopbackNetDevice:LoopbackNetDevice()
0s ObjectBase:ConstructSelf(): construct tid=ns3::LoopbackNetDevice, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::NetDevice, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s DefaultSimulatorImpl:ScheduleWithContext(0x7fcc12c479b0, 0, 0, 0, 0x7fcc12c49700)
0s MapScheduler:Insert(0x7fcc12c47a10, 0x7fcc12c49700, 0, 13)
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401e38)
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401fe8)
0s Ipv4Interface:AddAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fcc12c49780)
0s Ipv4L3Protocol:AddIpv4Interface(0x7fcc12c49540, 0x7fcc12c49270)
0s Ipv4Interface:SetUp()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Icmpv4L4Protocol, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4L4Protocol, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv4L4Protocol::ProtocolNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv4L4Protocol::ProtocolNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4RawSocketFactory, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::SocketFactory, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv4EndPointDemux:Ipv4EndPointDemux()
0s UdpL4Protocol:UdpL4Protocol()
0s ObjectBase:ConstructSelf(): construct tid=ns3::UdpL4Protocol, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::UdpL4Protocol::SocketList"
0s ObjectBase:ConstructSelf(): construct "ns3::UdpL4Protocol::SocketList" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4L4Protocol, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv4L4Protocol::ProtocolNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv4L4Protocol::ProtocolNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::UdpSocketFactory, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::SocketFactory, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv4EndPointDemux:Ipv4EndPointDemux()
0s TcpL4Protocol:TcpL4Protocol()
0s TcpL4Protocol:TcpL4Protocol(): Made a TcpL4Protocol 0x7fcc12c499a0
0s ObjectBase:ConstructSelf(): construct tid=ns3::TcpL4Protocol, params=3
0s ObjectBase:ConstructSelf(): try to construct "ns3::TcpL4Protocol::RttEstimatorType"
0s ObjectBase:ConstructSelf(): construct "ns3::TcpL4Protocol::RttEstimatorType" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::TcpL4Protocol::SocketType"
0s ObjectBase:ConstructSelf(): construct "ns3::TcpL4Protocol::SocketType" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::TcpL4Protocol::SocketList"
0s ObjectBase:ConstructSelf(): construct "ns3::TcpL4Protocol::SocketList" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4L4Protocol, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv4L4Protocol::ProtocolNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv4L4Protocol::ProtocolNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::TcpSocketFactory, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::SocketFactory, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::PacketSocketFactory, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::SocketFactory, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv4ListRouting:Ipv4ListRouting()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4ListRouting, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4RoutingProtocol, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv4StaticRouting:Ipv4StaticRouting(0x7fcc12c49a70)
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4StaticRouting, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4RoutingProtocol, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv4ListRouting:AddRoutingProtocol(0x7fcc12c49960, ns3::Ipv4StaticRouting, 0)
0s GlobalRoutingHelper:Create(): Adding GlobalRouter interface to node 0
0s GlobalRouter:GlobalRouter()
0s ObjectBase:ConstructSelf(): construct tid=ns3::GlobalRouter, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s GlobalRoutingHelper:Create(): Adding GlobalRouting Protocol to node 0
0s Ipv4GlobalRouting:Ipv4GlobalRouting()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4GlobalRouting, params=2
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv4GlobalRouting::RandomEcmpRouting"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv4GlobalRouting::RandomEcmpRouting" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv4GlobalRouting::RespondToInterfaceEvents"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv4GlobalRouting::RespondToInterfaceEvents" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv4ListRouting:AddRoutingProtocol(0x7fcc12c49960, ns3::Ipv4GlobalRouting, -10)
0s Ipv4L3Protocol:SetRoutingProtocol(0x7fcc12c49540)
0s Ipv4ListRouting:SetIpv4(0x7fcc12c49960, 0x7fcc12c49540)
0s Ipv4StaticRouting:SetIpv4(0x7fcc12c49a70, 0x7fcc12c49540)
0s Ipv4Interface:IsUp()
0s 0 [node 0] Ipv4StaticRouting:NotifyInterfaceUp(0x7fcc12c49a70, 0)
0s Ipv4Interface:GetNAddresses()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401bd8)
0s Ipv4InterfaceAddress:GetLocal()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401c10)
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401c50)
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401c80)
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401cb8)
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401ce8)
0s Ipv4InterfaceAddress:GetLocal()
0s 0 [node 0] Ipv4StaticRouting:AddNetworkRouteTo(0x7fcc12c49a70, 127.0.0.0,  , 255.0.0.0,  , 0,  , 0)
0s Ipv4Interface:GetNAddresses()
0s Ipv4GlobalRouting:SetIpv4(0x7fcc12c49ac0, 0x7fcc12c49540)
0s Ipv6L3Protocol:Ipv6L3Protocol()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6L3Protocol, params=2
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6L3Protocol::DefaultTtl"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6L3Protocol::DefaultTtl" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6L3Protocol::InterfaceList"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6L3Protocol::InterfaceList" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6::IpForward"
0s Ipv6L3Protocol:SetIpForward(0x7fcc12c49c80, 0)
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6::IpForward" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6L3Protocol:NotifyNewAggregate()
0s Ipv6L3Protocol:SetNode(0x7fcc12c49c80, 0x7fcc12c478e0)
0s Ipv6L3Protocol:SetupLoopback()
0s Ipv6Interface:Ipv6Interface(0x7fcc12c49da0)
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6Interface, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Interface:SetDevice(0x7fcc12c49da0, 0x7fcc12c49680)
0s Ipv6Interface:DoSetup()
0s Ipv6Interface:SetNode(0x7fcc12c49da0, 0x7fcc12c478e0)
0s Ipv6Interface:DoSetup()
0s Ipv6InterfaceAddress:Ipv6InterfaceAddress(0x7fff6d401d50, 0000:0000:0000:0000:0000:0000:0000:0001, ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff)
0s Ipv6InterfaceAddress:SetAddress(0x7fff6d401d50, 0000:0000:0000:0000:0000:0000:0000:0001)
0s Ipv6InterfaceAddress:SetState(0x7fff6d401d50, 5)
0s Ipv6Interface:AddAddress()
0s Ipv6InterfaceAddress:GetAddress()
0s Ipv6L3Protocol:GetIcmpv6()
0s Icmpv6L4Protocol:GetStaticProtocolNumber()
0s Ipv6L3Protocol:GetProtocol(0x7fcc12c49c80, 58)
0s Ipv6InterfaceAddress:~Ipv6InterfaceAddress()
0s Ipv6L3Protocol:AddIpv6Interface(0x7fcc12c49c80, 0x7fcc12c49da0)
0s Ipv6Interface:SetUp()
0s Ipv6InterfaceAddress:~Ipv6InterfaceAddress()
0s Icmpv6L4Protocol:Icmpv6L4Protocol()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Icmpv6L4Protocol, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Icmpv6L4Protocol::DAD"
0s ObjectBase:ConstructSelf(): construct "ns3::Icmpv6L4Protocol::DAD" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6L4Protocol, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6L4Protocol::ProtocolNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6L4Protocol::ProtocolNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6L3Protocol:NotifyNewAggregate()
0s Icmpv6L4Protocol:NotifyNewAggregate()
0s Icmpv6L4Protocol:SetNode(0x7fcc12c49b90, 0x7fcc12c478e0)
0s Ipv6L3Protocol:Insert(0x7fcc12c49c80, 0x7fcc12c49b90)
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6RawSocketFactory, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::SocketFactory, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6L3Protocol:NotifyNewAggregate()
0s Icmpv6L4Protocol:NotifyNewAggregate()
0s Ipv6ListRouting:Ipv6ListRouting()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6ListRouting, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6RoutingProtocol, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6StaticRouting:Ipv6StaticRouting()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6StaticRouting, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6RoutingProtocol, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6ListRouting:AddRoutingProtocol(0x7fcc12c49d30, ns3::Ipv6StaticRouting, 0)
0s Ipv6L3Protocol:SetRoutingProtocol(0x7fcc12c49c80, 0x7fcc12c49d30)
0s Ipv6ListRouting:SetIpv6(0x7fcc12c49d30, 0x7fcc12c49c80)
0s Ipv6StaticRouting:SetIpv6(0x7fcc12c49ef0, 0x7fcc12c49c80)
0s Ipv6L3Protocol:GetNInterfaces()
0s Ipv6L3Protocol:IsUp(0x7fcc12c49c80, 0)
0s Ipv6L3Protocol:GetInterface(0x7fcc12c49c80, 0)
0s Ipv6Interface:IsUp()
0s Ipv6L3Protocol:GetNAddresses(0x7fcc12c49c80, 0)
0s Ipv6L3Protocol:GetInterface(0x7fcc12c49c80, 0)
0s Ipv6Interface:GetNAddresses()
0s Ipv6L3Protocol:GetAddress(0x7fcc12c49c80, 0, 0)
0s Ipv6L3Protocol:GetInterface(0x7fcc12c49c80, 0)
0s Ipv6Interface:GetAddress(0x7fcc12c49da0, 0)
0s Ipv6InterfaceAddress:GetAddress()
0s Ipv6L3Protocol:GetAddress(0x7fcc12c49c80, 0, 0)
0s Ipv6L3Protocol:GetInterface(0x7fcc12c49c80, 0)
0s Ipv6Interface:GetAddress(0x7fcc12c49da0, 0)
0s Ipv6InterfaceAddress:GetPrefix()
0s Ipv6InterfaceAddress:~Ipv6InterfaceAddress()
0s Ipv6InterfaceAddress:~Ipv6InterfaceAddress()
0s Ipv6L3Protocol:GetAddress(0x7fcc12c49c80, 0, 0)
0s Ipv6L3Protocol:GetInterface(0x7fcc12c49c80, 0)
0s Ipv6Interface:GetAddress(0x7fcc12c49da0, 0)
0s Ipv6InterfaceAddress:GetPrefix()
0s Ipv6InterfaceAddress:~Ipv6InterfaceAddress()
0s Ipv6L3Protocol:GetAddress(0x7fcc12c49c80, 0, 0)
0s Ipv6L3Protocol:GetInterface(0x7fcc12c49c80, 0)
0s Ipv6Interface:GetAddress(0x7fcc12c49da0, 0)
0s Ipv6InterfaceAddress:GetAddress()
0s Ipv6StaticRouting:AddHostRouteTo(0x7fcc12c49ef0, 0000:0000:0000:0000:0000:0000:0000:0001, 0, 0)
0s Ipv6StaticRouting:AddNetworkRouteTo(0x7fcc12c49ef0, 0000:0000:0000:0000:0000:0000:0000:0001, ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff, 0)
0s Ipv6InterfaceAddress:~Ipv6InterfaceAddress()
0s Ipv6L3Protocol:GetNAddresses(0x7fcc12c49c80, 0)
0s Ipv6L3Protocol:GetInterface(0x7fcc12c49c80, 0)
0s Ipv6Interface:GetNAddresses()
0s Ipv6L3Protocol:GetNInterfaces()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6ExtensionDemux, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6ExtensionDemux::Extensions"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6ExtensionDemux::Extensions" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Extension:Ipv6ExtensionHopByHop()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6ExtensionHopByHop, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6Extension, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6Extension::ExtensionNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6Extension::ExtensionNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Extension:SetNode(0x7fcc12c4a070, 0x7fcc12c478e0)
0s Ipv6Extension:Ipv6ExtensionDestination()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6ExtensionDestination, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6Extension, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6Extension::ExtensionNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6Extension::ExtensionNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Extension:SetNode(0x7fcc12c4a0b0, 0x7fcc12c478e0)
0s Ipv6Extension:Ipv6ExtensionFragment()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6ExtensionFragment, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6Extension, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6Extension::ExtensionNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6Extension::ExtensionNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Extension:SetNode(0x7fcc12c4a0f0, 0x7fcc12c478e0)
0s Ipv6Extension:Ipv6ExtensionRouting()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6ExtensionRouting, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6Extension, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6Extension::ExtensionNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6Extension::ExtensionNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Extension:SetNode(0x7fcc12c4a160, 0x7fcc12c478e0)
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6ExtensionRoutingDemux, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6ExtensionRoutingDemux::Routing Extensions"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6ExtensionRoutingDemux::Routing Extensions" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Extension:Ipv6ExtensionRouting()
0s Ipv6Extension:Ipv6ExtensionLooseRouting()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6ExtensionLooseRouting, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6ExtensionRouting, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6Extension, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6Extension::ExtensionNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6Extension::ExtensionNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Extension:SetNode(0x7fcc12c4a260, 0x7fcc12c478e0)
0s Ipv6L3Protocol:NotifyNewAggregate()
0s Icmpv6L4Protocol:NotifyNewAggregate()
0s Ipv6L3Protocol:NotifyNewAggregate()
0s Icmpv6L4Protocol:NotifyNewAggregate()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6OptionDemux, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6OptionDemux::Options"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6OptionDemux::Options" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Option:Ipv6OptionPad1()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6OptionPad1, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6Option, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6Option::OptionNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6Option::OptionNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Option:SetNode(0x7fcc12c49c50, 0x7fcc12c478e0)
0s Ipv6Option:Ipv6OptionPadn()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6OptionPadn, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6Option, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6Option::OptionNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6Option::OptionNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Option:SetNode(0x7fcc12c4a300, 0x7fcc12c478e0)
0s Ipv6Option:Ipv6OptionJumbogram()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6OptionJumbogram, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6Option, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6Option::OptionNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6Option::OptionNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Option:SetNode(0x7fcc12c49f60, 0x7fcc12c478e0)
0s Ipv6Option:Ipv6OptionRouterAlert()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6OptionRouterAlert, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6Option, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6Option::OptionNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6Option::OptionNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Option:SetNode(0x7fcc12c49f90, 0x7fcc12c478e0)
0s Ipv6L3Protocol:NotifyNewAggregate()
0s Icmpv6L4Protocol:NotifyNewAggregate()
0s ArpL3Protocol:ArpL3Protocol(0x7fcc12c4a370)
0s ObjectBase:ConstructSelf(): construct tid=ns3::ArpL3Protocol, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::ArpL3Protocol::CacheList"
0s ObjectBase:ConstructSelf(): construct "ns3::ArpL3Protocol::CacheList" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s ArpL3Protocol:SetNode(0x7fcc12c4a370)
0s Ipv4L3Protocol:Ipv4L3Protocol(0x7fcc12c4a4e0)
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4L3Protocol, params=3
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv4L3Protocol::DefaultTtl"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv4L3Protocol::DefaultTtl" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv4L3Protocol::FragmentExpirationTimeout"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv4L3Protocol::FragmentExpirationTimeout" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv4L3Protocol::InterfaceList"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv4L3Protocol::InterfaceList" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4, params=2
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv4::IpForward"
0s Ipv4L3Protocol:SetIpForward(0x7fcc12c4a4e0, 1)
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv4::IpForward" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv4::WeakEsModel"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv4::WeakEsModel" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv4L3Protocol:SetupLoopback(0x7fcc12c4a4e0)
0s Ipv4Interface:Ipv4Interface(0x7fcc12c4a640)
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4Interface, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv4Interface::ArpCache"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv4Interface::ArpCache" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s LoopbackNetDevice:LoopbackNetDevice()
0s ObjectBase:ConstructSelf(): construct tid=ns3::LoopbackNetDevice, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::NetDevice, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s DefaultSimulatorImpl:ScheduleWithContext(0x7fcc12c479b0, 1, 0, 0, 0x7fcc12c4a720)
0s MapScheduler:Insert(0x7fcc12c47a10, 0x7fcc12c4a720, 0, e)
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401e38)
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401fe8)
0s Ipv4Interface:AddAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fcc12c4a7a0)
0s Ipv4L3Protocol:AddIpv4Interface(0x7fcc12c4a4e0, 0x7fcc12c4a640)
0s Ipv4Interface:SetUp()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Icmpv4L4Protocol, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4L4Protocol, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv4L4Protocol::ProtocolNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv4L4Protocol::ProtocolNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4RawSocketFactory, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::SocketFactory, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv4EndPointDemux:Ipv4EndPointDemux()
0s UdpL4Protocol:UdpL4Protocol()
0s ObjectBase:ConstructSelf(): construct tid=ns3::UdpL4Protocol, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::UdpL4Protocol::SocketList"
0s ObjectBase:ConstructSelf(): construct "ns3::UdpL4Protocol::SocketList" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4L4Protocol, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv4L4Protocol::ProtocolNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv4L4Protocol::ProtocolNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::UdpSocketFactory, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::SocketFactory, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv4EndPointDemux:Ipv4EndPointDemux()
0s TcpL4Protocol:TcpL4Protocol()
0s TcpL4Protocol:TcpL4Protocol(): Made a TcpL4Protocol 0x7fcc12c4a9f0
0s ObjectBase:ConstructSelf(): construct tid=ns3::TcpL4Protocol, params=3
0s ObjectBase:ConstructSelf(): try to construct "ns3::TcpL4Protocol::RttEstimatorType"
0s ObjectBase:ConstructSelf(): construct "ns3::TcpL4Protocol::RttEstimatorType" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::TcpL4Protocol::SocketType"
0s ObjectBase:ConstructSelf(): construct "ns3::TcpL4Protocol::SocketType" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::TcpL4Protocol::SocketList"
0s ObjectBase:ConstructSelf(): construct "ns3::TcpL4Protocol::SocketList" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4L4Protocol, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv4L4Protocol::ProtocolNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv4L4Protocol::ProtocolNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::TcpSocketFactory, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::SocketFactory, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::PacketSocketFactory, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::SocketFactory, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv4ListRouting:Ipv4ListRouting()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4ListRouting, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4RoutingProtocol, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv4StaticRouting:Ipv4StaticRouting(0x7fcc12c4aaa0)
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4StaticRouting, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4RoutingProtocol, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv4ListRouting:AddRoutingProtocol(0x7fcc12c4a9b0, ns3::Ipv4StaticRouting, 0)
0s GlobalRoutingHelper:Create(): Adding GlobalRouter interface to node 1
0s GlobalRouter:GlobalRouter()
0s ObjectBase:ConstructSelf(): construct tid=ns3::GlobalRouter, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s GlobalRoutingHelper:Create(): Adding GlobalRouting Protocol to node 1
0s Ipv4GlobalRouting:Ipv4GlobalRouting()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4GlobalRouting, params=2
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv4GlobalRouting::RandomEcmpRouting"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv4GlobalRouting::RandomEcmpRouting" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv4GlobalRouting::RespondToInterfaceEvents"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv4GlobalRouting::RespondToInterfaceEvents" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv4ListRouting:AddRoutingProtocol(0x7fcc12c4a9b0, ns3::Ipv4GlobalRouting, fff6)
0s Ipv4L3Protocol:SetRoutingProtocol(0x7fcc12c4a4e0)
0s Ipv4ListRouting:SetIpv4(0x7fcc12c4a9b0, 0x7fcc12c4a4e0)
0s Ipv4StaticRouting:SetIpv4(0x7fcc12c4aaa0, 0x7fcc12c4a4e0)
0s Ipv4Interface:IsUp()
0s 0 [node 1] Ipv4StaticRouting:NotifyInterfaceUp(0x7fcc12c4aaa0, 0)
0s Ipv4Interface:GetNAddresses()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401bd8)
0s Ipv4InterfaceAddress:GetLocal()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401c10)
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401c50)
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401c80)
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401cb8)
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401ce8)
0s Ipv4InterfaceAddress:GetLocal()
0s 0 [node 1] Ipv4StaticRouting:AddNetworkRouteTo(0x7fcc12c4aaa0, 7f.0.0.0,  , ff.0.0.0,  , 0,  , 0)
0s Ipv4Interface:GetNAddresses()
0s Ipv4GlobalRouting:SetIpv4(0x7fcc12c4aaf0, 0x7fcc12c4a4e0)
0s Ipv6L3Protocol:Ipv6L3Protocol()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6L3Protocol, params=2
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6L3Protocol::DefaultTtl"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6L3Protocol::DefaultTtl" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6L3Protocol::InterfaceList"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6L3Protocol::InterfaceList" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6::IpForward"
0s Ipv6L3Protocol:SetIpForward(0x7fcc12c4acd0, 0)
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6::IpForward" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6L3Protocol:NotifyNewAggregate()
0s Ipv6L3Protocol:SetNode(0x7fcc12c4acd0, 0x7fcc12c47b40)
0s Ipv6L3Protocol:SetupLoopback()
0s Ipv6Interface:Ipv6Interface(0x7fcc12c4ae00)
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6Interface, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Interface:SetDevice(0x7fcc12c4ae00, 0x7fcc12c4a690)
0s Ipv6Interface:DoSetup()
0s Ipv6Interface:SetNode(0x7fcc12c4ae00, 0x7fcc12c47b40)
0s Ipv6Interface:DoSetup()
0s Ipv6InterfaceAddress:Ipv6InterfaceAddress(0x7fff6d401d50, 0000:0000:0000:0000:0000:0000:0000:0001, ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff)
0s Ipv6InterfaceAddress:SetAddress(0x7fff6d401d50, 0000:0000:0000:0000:0000:0000:0000:0001)
0s Ipv6InterfaceAddress:SetState(0x7fff6d401d50, 5)
0s Ipv6Interface:AddAddress()
0s Ipv6InterfaceAddress:GetAddress()
0s Ipv6L3Protocol:GetIcmpv6()
0s Icmpv6L4Protocol:GetStaticProtocolNumber()
0s Ipv6L3Protocol:GetProtocol(0x7fcc12c4acd0, 58)
0s Ipv6InterfaceAddress:~Ipv6InterfaceAddress()
0s Ipv6L3Protocol:AddIpv6Interface(0x7fcc12c4acd0, 0x7fcc12c4ae00)
0s Ipv6Interface:SetUp()
0s Ipv6InterfaceAddress:~Ipv6InterfaceAddress()
0s Icmpv6L4Protocol:Icmpv6L4Protocol()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Icmpv6L4Protocol, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Icmpv6L4Protocol::DAD"
0s ObjectBase:ConstructSelf(): construct "ns3::Icmpv6L4Protocol::DAD" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6L4Protocol, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6L4Protocol::ProtocolNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6L4Protocol::ProtocolNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6L3Protocol:NotifyNewAggregate()
0s Icmpv6L4Protocol:NotifyNewAggregate()
0s Icmpv6L4Protocol:SetNode(0x7fcc12c4abd0, 0x7fcc12c47b40)
0s Ipv6L3Protocol:Insert(0x7fcc12c4acd0, 0x7fcc12c4abd0)
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6RawSocketFactory, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::SocketFactory, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6L3Protocol:NotifyNewAggregate()
0s Icmpv6L4Protocol:NotifyNewAggregate()
0s Ipv6ListRouting:Ipv6ListRouting()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6ListRouting, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6RoutingProtocol, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6StaticRouting:Ipv6StaticRouting()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6StaticRouting, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6RoutingProtocol, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6ListRouting:AddRoutingProtocol(0x7fcc12c4ad80, ns3::Ipv6StaticRouting, 0)
0s Ipv6L3Protocol:SetRoutingProtocol(0x7fcc12c4acd0, 0x7fcc12c4ad80)
0s Ipv6ListRouting:SetIpv6(0x7fcc12c4ad80, 0x7fcc12c4acd0)
0s Ipv6StaticRouting:SetIpv6(0x7fcc12c4af30, 0x7fcc12c4acd0)
0s Ipv6L3Protocol:GetNInterfaces()
0s Ipv6L3Protocol:IsUp(0x7fcc12c4acd0, 0)
0s Ipv6L3Protocol:GetInterface(0x7fcc12c4acd0, 0)
0s Ipv6Interface:IsUp()
0s Ipv6L3Protocol:GetNAddresses(0x7fcc12c4acd0, 0)
0s Ipv6L3Protocol:GetInterface(0x7fcc12c4acd0, 0)
0s Ipv6Interface:GetNAddresses()
0s Ipv6L3Protocol:GetAddress(0x7fcc12c4acd0, 0, 0)
0s Ipv6L3Protocol:GetInterface(0x7fcc12c4acd0, 0)
0s Ipv6Interface:GetAddress(0x7fcc12c4ae00, 0)
0s Ipv6InterfaceAddress:GetAddress()
0s Ipv6L3Protocol:GetAddress(0x7fcc12c4acd0, 0, 0)
0s Ipv6L3Protocol:GetInterface(0x7fcc12c4acd0, 0)
0s Ipv6Interface:GetAddress(0x7fcc12c4ae00, 0)
0s Ipv6InterfaceAddress:GetPrefix()
0s Ipv6InterfaceAddress:~Ipv6InterfaceAddress()
0s Ipv6InterfaceAddress:~Ipv6InterfaceAddress()
0s Ipv6L3Protocol:GetAddress(0x7fcc12c4acd0, 0, 0)
0s Ipv6L3Protocol:GetInterface(0x7fcc12c4acd0, 0)
0s Ipv6Interface:GetAddress(0x7fcc12c4ae00, 0)
0s Ipv6InterfaceAddress:GetPrefix()
0s Ipv6InterfaceAddress:~Ipv6InterfaceAddress()
0s Ipv6L3Protocol:GetAddress(0x7fcc12c4acd0, 0, 0)
0s Ipv6L3Protocol:GetInterface(0x7fcc12c4acd0, 0)
0s Ipv6Interface:GetAddress(0x7fcc12c4ae00, 0)
0s Ipv6InterfaceAddress:GetAddress()
0s Ipv6StaticRouting:AddHostRouteTo(0x7fcc12c4af30, 0000:0000:0000:0000:0000:0000:0000:0001, 0, 0)
0s Ipv6StaticRouting:AddNetworkRouteTo(0x7fcc12c4af30, 0000:0000:0000:0000:0000:0000:0000:0001, ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff, 0)
0s Ipv6InterfaceAddress:~Ipv6InterfaceAddress()
0s Ipv6L3Protocol:GetNAddresses(0x7fcc12c4acd0, 0)
0s Ipv6L3Protocol:GetInterface(0x7fcc12c4acd0, 0)
0s Ipv6Interface:GetNAddresses()
0s Ipv6L3Protocol:GetNInterfaces()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6ExtensionDemux, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6ExtensionDemux::Extensions"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6ExtensionDemux::Extensions" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Extension:Ipv6ExtensionHopByHop()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6ExtensionHopByHop, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6Extension, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6Extension::ExtensionNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6Extension::ExtensionNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Extension:SetNode(0x7fcc12c4b0c0, 0x7fcc12c47b40)
0s Ipv6Extension:Ipv6ExtensionDestination()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6ExtensionDestination, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6Extension, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6Extension::ExtensionNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6Extension::ExtensionNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Extension:SetNode(0x7fcc12c4b110, 0x7fcc12c47b40)
0s Ipv6Extension:Ipv6ExtensionFragment()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6ExtensionFragment, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6Extension, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6Extension::ExtensionNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6Extension::ExtensionNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Extension:SetNode(0x7fcc12c4b160, 0x7fcc12c47b40)
0s Ipv6Extension:Ipv6ExtensionRouting()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6ExtensionRouting, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6Extension, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6Extension::ExtensionNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6Extension::ExtensionNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Extension:SetNode(0x7fcc12c4b1e0, 0x7fcc12c47b40)
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6ExtensionRoutingDemux, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6ExtensionRoutingDemux::Routing Extensions"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6ExtensionRoutingDemux::Routing Extensions" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Extension:Ipv6ExtensionRouting()
0s Ipv6Extension:Ipv6ExtensionLooseRouting()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6ExtensionLooseRouting, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6ExtensionRouting, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6Extension, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6Extension::ExtensionNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6Extension::ExtensionNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Extension:SetNode(0x7fcc12c4b300, 0x7fcc12c47b40)
0s Ipv6L3Protocol:NotifyNewAggregate()
0s Icmpv6L4Protocol:NotifyNewAggregate()
0s Ipv6L3Protocol:NotifyNewAggregate()
0s Icmpv6L4Protocol:NotifyNewAggregate()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6OptionDemux, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6OptionDemux::Options"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6OptionDemux::Options" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Option:Ipv6OptionPad1()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6OptionPad1, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6Option, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6Option::OptionNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6Option::OptionNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Option:SetNode(0x7fcc12c4aca0, 0x7fcc12c47b40)
0s Ipv6Option:Ipv6OptionPadn()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6OptionPadn, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6Option, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6Option::OptionNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6Option::OptionNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Option:SetNode(0x7fcc12c4b3b0, 0x7fcc12c47b40)
0s Ipv6Option:Ipv6OptionJumbogram()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6OptionJumbogram, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6Option, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6Option::OptionNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6Option::OptionNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Option:SetNode(0x7fcc12c4afb0, 0x7fcc12c47b40)
0s Ipv6Option:Ipv6OptionRouterAlert()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6OptionRouterAlert, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6Option, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6Option::OptionNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6Option::OptionNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Option:SetNode(0x7fcc12c4aff0, 0x7fcc12c47b40)
0s Ipv6L3Protocol:NotifyNewAggregate()
0s Icmpv6L4Protocol:NotifyNewAggregate()
0s ArpL3Protocol:ArpL3Protocol(0x7fcc12c4b420)
0s ObjectBase:ConstructSelf(): construct tid=ns3::ArpL3Protocol, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::ArpL3Protocol::CacheList"
0s ObjectBase:ConstructSelf(): construct "ns3::ArpL3Protocol::CacheList" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s ArpL3Protocol:SetNode(0x7fcc12c4b420)
0s Ipv4L3Protocol:Ipv4L3Protocol(0x7fcc12c4b5b0)
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4L3Protocol, params=3
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv4L3Protocol::DefaultTtl"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv4L3Protocol::DefaultTtl" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv4L3Protocol::FragmentExpirationTimeout"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv4L3Protocol::FragmentExpirationTimeout" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv4L3Protocol::InterfaceList"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv4L3Protocol::InterfaceList" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4, params=2
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv4::IpForward"
0s Ipv4L3Protocol:SetIpForward(0x7fcc12c4b5b0, 1)
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv4::IpForward" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv4::WeakEsModel"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv4::WeakEsModel" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv4L3Protocol:SetupLoopback(0x7fcc12c4b5b0)
0s Ipv4Interface:Ipv4Interface(0x7fcc12c4b710)
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4Interface, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv4Interface::ArpCache"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv4Interface::ArpCache" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s LoopbackNetDevice:LoopbackNetDevice()
0s ObjectBase:ConstructSelf(): construct tid=ns3::LoopbackNetDevice, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::NetDevice, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s DefaultSimulatorImpl:ScheduleWithContext(0x7fcc12c479b0, 2, 0, 0, 0x7fcc12c4b7f0)
0s MapScheduler:Insert(0x7fcc12c47a10, 0x7fcc12c4b7f0, 0, f)
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401e38)
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401fe8)
0s Ipv4Interface:AddAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fcc12c4b870)
0s Ipv4L3Protocol:AddIpv4Interface(0x7fcc12c4b5b0, 0x7fcc12c4b710)
0s Ipv4Interface:SetUp()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Icmpv4L4Protocol, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4L4Protocol, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv4L4Protocol::ProtocolNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv4L4Protocol::ProtocolNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4RawSocketFactory, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::SocketFactory, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv4EndPointDemux:Ipv4EndPointDemux()
0s UdpL4Protocol:UdpL4Protocol()
0s ObjectBase:ConstructSelf(): construct tid=ns3::UdpL4Protocol, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::UdpL4Protocol::SocketList"
0s ObjectBase:ConstructSelf(): construct "ns3::UdpL4Protocol::SocketList" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4L4Protocol, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv4L4Protocol::ProtocolNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv4L4Protocol::ProtocolNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::UdpSocketFactory, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::SocketFactory, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv4EndPointDemux:Ipv4EndPointDemux()
0s TcpL4Protocol:TcpL4Protocol()
0s TcpL4Protocol:TcpL4Protocol(): Made a TcpL4Protocol 0x7fcc12c4bac0
0s ObjectBase:ConstructSelf(): construct tid=ns3::TcpL4Protocol, params=3
0s ObjectBase:ConstructSelf(): try to construct "ns3::TcpL4Protocol::RttEstimatorType"
0s ObjectBase:ConstructSelf(): construct "ns3::TcpL4Protocol::RttEstimatorType" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::TcpL4Protocol::SocketType"
0s ObjectBase:ConstructSelf(): construct "ns3::TcpL4Protocol::SocketType" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::TcpL4Protocol::SocketList"
0s ObjectBase:ConstructSelf(): construct "ns3::TcpL4Protocol::SocketList" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4L4Protocol, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv4L4Protocol::ProtocolNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv4L4Protocol::ProtocolNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::TcpSocketFactory, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::SocketFactory, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::PacketSocketFactory, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::SocketFactory, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv4ListRouting:Ipv4ListRouting()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4ListRouting, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4RoutingProtocol, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv4StaticRouting:Ipv4StaticRouting(0x7fcc12c4bb70)
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4StaticRouting, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4RoutingProtocol, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv4ListRouting:AddRoutingProtocol(0x7fcc12c4ba80, ns3::Ipv4StaticRouting, 0)
0s GlobalRoutingHelper:Create(): Adding GlobalRouter interface to node 2
0s GlobalRouter:GlobalRouter()
0s ObjectBase:ConstructSelf(): construct tid=ns3::GlobalRouter, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s GlobalRoutingHelper:Create(): Adding GlobalRouting Protocol to node 2
0s Ipv4GlobalRouting:Ipv4GlobalRouting()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4GlobalRouting, params=2
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv4GlobalRouting::RandomEcmpRouting"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv4GlobalRouting::RandomEcmpRouting" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv4GlobalRouting::RespondToInterfaceEvents"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv4GlobalRouting::RespondToInterfaceEvents" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv4ListRouting:AddRoutingProtocol(0x7fcc12c4ba80, ns3::Ipv4GlobalRouting, fff6)
0s Ipv4L3Protocol:SetRoutingProtocol(0x7fcc12c4b5b0)
0s Ipv4ListRouting:SetIpv4(0x7fcc12c4ba80, 0x7fcc12c4b5b0)
0s Ipv4StaticRouting:SetIpv4(0x7fcc12c4bb70, 0x7fcc12c4b5b0)
0s Ipv4Interface:IsUp()
0s 0 [node 2] Ipv4StaticRouting:NotifyInterfaceUp(0x7fcc12c4bb70, 0)
0s Ipv4Interface:GetNAddresses()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401bd8)
0s Ipv4InterfaceAddress:GetLocal()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401c10)
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401c50)
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401c80)
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401cb8)
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401ce8)
0s Ipv4InterfaceAddress:GetLocal()
0s 0 [node 2] Ipv4StaticRouting:AddNetworkRouteTo(0x7fcc12c4bb70, 7f.0.0.0,  , ff.0.0.0,  , 0,  , 0)
0s Ipv4Interface:GetNAddresses()
0s Ipv4GlobalRouting:SetIpv4(0x7fcc12c4bbc0, 0x7fcc12c4b5b0)
0s Ipv6L3Protocol:Ipv6L3Protocol()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6L3Protocol, params=2
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6L3Protocol::DefaultTtl"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6L3Protocol::DefaultTtl" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6L3Protocol::InterfaceList"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6L3Protocol::InterfaceList" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6::IpForward"
0s Ipv6L3Protocol:SetIpForward(0x7fcc12c4bda0, 0)
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6::IpForward" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6L3Protocol:NotifyNewAggregate()
0s Ipv6L3Protocol:SetNode(0x7fcc12c4bda0, 0x7fcc12c47ca0)
0s Ipv6L3Protocol:SetupLoopback()
0s Ipv6Interface:Ipv6Interface(0x7fcc12c4bed0)
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6Interface, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Interface:SetDevice(0x7fcc12c4bed0, 0x7fcc12c4b760)
0s Ipv6Interface:DoSetup()
0s Ipv6Interface:SetNode(0x7fcc12c4bed0, 0x7fcc12c47ca0)
0s Ipv6Interface:DoSetup()
0s Ipv6InterfaceAddress:Ipv6InterfaceAddress(0x7fff6d401d50, 0000:0000:0000:0000:0000:0000:0000:0001, ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff)
0s Ipv6InterfaceAddress:SetAddress(0x7fff6d401d50, 0000:0000:0000:0000:0000:0000:0000:0001)
0s Ipv6InterfaceAddress:SetState(0x7fff6d401d50, 5)
0s Ipv6Interface:AddAddress()
0s Ipv6InterfaceAddress:GetAddress()
0s Ipv6L3Protocol:GetIcmpv6()
0s Icmpv6L4Protocol:GetStaticProtocolNumber()
0s Ipv6L3Protocol:GetProtocol(0x7fcc12c4bda0, 58)
0s Ipv6InterfaceAddress:~Ipv6InterfaceAddress()
0s Ipv6L3Protocol:AddIpv6Interface(0x7fcc12c4bda0, 0x7fcc12c4bed0)
0s Ipv6Interface:SetUp()
0s Ipv6InterfaceAddress:~Ipv6InterfaceAddress()
0s Icmpv6L4Protocol:Icmpv6L4Protocol()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Icmpv6L4Protocol, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Icmpv6L4Protocol::DAD"
0s ObjectBase:ConstructSelf(): construct "ns3::Icmpv6L4Protocol::DAD" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6L4Protocol, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6L4Protocol::ProtocolNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6L4Protocol::ProtocolNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6L3Protocol:NotifyNewAggregate()
0s Icmpv6L4Protocol:NotifyNewAggregate()
0s Icmpv6L4Protocol:SetNode(0x7fcc12c4bca0, 0x7fcc12c47ca0)
0s Ipv6L3Protocol:Insert(0x7fcc12c4bda0, 0x7fcc12c4bca0)
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6RawSocketFactory, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::SocketFactory, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6L3Protocol:NotifyNewAggregate()
0s Icmpv6L4Protocol:NotifyNewAggregate()
0s Ipv6ListRouting:Ipv6ListRouting()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6ListRouting, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6RoutingProtocol, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6StaticRouting:Ipv6StaticRouting()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6StaticRouting, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6RoutingProtocol, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6ListRouting:AddRoutingProtocol(0x7fcc12c4be50, ns3::Ipv6StaticRouting, 0)
0s Ipv6L3Protocol:SetRoutingProtocol(0x7fcc12c4bda0, 0x7fcc12c4be50)
0s Ipv6ListRouting:SetIpv6(0x7fcc12c4be50, 0x7fcc12c4bda0)
0s Ipv6StaticRouting:SetIpv6(0x7fcc12c4c000, 0x7fcc12c4bda0)
0s Ipv6L3Protocol:GetNInterfaces()
0s Ipv6L3Protocol:IsUp(0x7fcc12c4bda0, 0)
0s Ipv6L3Protocol:GetInterface(0x7fcc12c4bda0, 0)
0s Ipv6Interface:IsUp()
0s Ipv6L3Protocol:GetNAddresses(0x7fcc12c4bda0, 0)
0s Ipv6L3Protocol:GetInterface(0x7fcc12c4bda0, 0)
0s Ipv6Interface:GetNAddresses()
0s Ipv6L3Protocol:GetAddress(0x7fcc12c4bda0, 0, 0)
0s Ipv6L3Protocol:GetInterface(0x7fcc12c4bda0, 0)
0s Ipv6Interface:GetAddress(0x7fcc12c4bed0, 0)
0s Ipv6InterfaceAddress:GetAddress()
0s Ipv6L3Protocol:GetAddress(0x7fcc12c4bda0, 0, 0)
0s Ipv6L3Protocol:GetInterface(0x7fcc12c4bda0, 0)
0s Ipv6Interface:GetAddress(0x7fcc12c4bed0, 0)
0s Ipv6InterfaceAddress:GetPrefix()
0s Ipv6InterfaceAddress:~Ipv6InterfaceAddress()
0s Ipv6InterfaceAddress:~Ipv6InterfaceAddress()
0s Ipv6L3Protocol:GetAddress(0x7fcc12c4bda0, 0, 0)
0s Ipv6L3Protocol:GetInterface(0x7fcc12c4bda0, 0)
0s Ipv6Interface:GetAddress(0x7fcc12c4bed0, 0)
0s Ipv6InterfaceAddress:GetPrefix()
0s Ipv6InterfaceAddress:~Ipv6InterfaceAddress()
0s Ipv6L3Protocol:GetAddress(0x7fcc12c4bda0, 0, 0)
0s Ipv6L3Protocol:GetInterface(0x7fcc12c4bda0, 0)
0s Ipv6Interface:GetAddress(0x7fcc12c4bed0, 0)
0s Ipv6InterfaceAddress:GetAddress()
0s Ipv6StaticRouting:AddHostRouteTo(0x7fcc12c4c000, 0000:0000:0000:0000:0000:0000:0000:0001, 0, 0)
0s Ipv6StaticRouting:AddNetworkRouteTo(0x7fcc12c4c000, 0000:0000:0000:0000:0000:0000:0000:0001, ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff, 0)
0s Ipv6InterfaceAddress:~Ipv6InterfaceAddress()
0s Ipv6L3Protocol:GetNAddresses(0x7fcc12c4bda0, 0)
0s Ipv6L3Protocol:GetInterface(0x7fcc12c4bda0, 0)
0s Ipv6Interface:GetNAddresses()
0s Ipv6L3Protocol:GetNInterfaces()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6ExtensionDemux, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6ExtensionDemux::Extensions"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6ExtensionDemux::Extensions" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Extension:Ipv6ExtensionHopByHop()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6ExtensionHopByHop, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6Extension, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6Extension::ExtensionNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6Extension::ExtensionNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Extension:SetNode(0x7fcc12c4c190, 0x7fcc12c47ca0)
0s Ipv6Extension:Ipv6ExtensionDestination()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6ExtensionDestination, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6Extension, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6Extension::ExtensionNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6Extension::ExtensionNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Extension:SetNode(0x7fcc12c4c1e0, 0x7fcc12c47ca0)
0s Ipv6Extension:Ipv6ExtensionFragment()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6ExtensionFragment, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6Extension, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6Extension::ExtensionNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6Extension::ExtensionNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Extension:SetNode(0x7fcc12c4c230, 0x7fcc12c47ca0)
0s Ipv6Extension:Ipv6ExtensionRouting()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6ExtensionRouting, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6Extension, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6Extension::ExtensionNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6Extension::ExtensionNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Extension:SetNode(0x7fcc12c4c2b0, 0x7fcc12c47ca0)
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6ExtensionRoutingDemux, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6ExtensionRoutingDemux::Routing Extensions"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6ExtensionRoutingDemux::Routing Extensions" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Extension:Ipv6ExtensionRouting()
0s Ipv6Extension:Ipv6ExtensionLooseRouting()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6ExtensionLooseRouting, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6ExtensionRouting, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6Extension, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6Extension::ExtensionNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6Extension::ExtensionNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Extension:SetNode(0x7fcc12c4c3d0, 0x7fcc12c47ca0)
0s Ipv6L3Protocol:NotifyNewAggregate()
0s Icmpv6L4Protocol:NotifyNewAggregate()
0s Ipv6L3Protocol:NotifyNewAggregate()
0s Icmpv6L4Protocol:NotifyNewAggregate()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6OptionDemux, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6OptionDemux::Options"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6OptionDemux::Options" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Option:Ipv6OptionPad1()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6OptionPad1, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6Option, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6Option::OptionNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6Option::OptionNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Option:SetNode(0x7fcc12c4bd70, 0x7fcc12c47ca0)
0s Ipv6Option:Ipv6OptionPadn()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6OptionPadn, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6Option, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6Option::OptionNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6Option::OptionNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Option:SetNode(0x7fcc12c4c480, 0x7fcc12c47ca0)
0s Ipv6Option:Ipv6OptionJumbogram()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6OptionJumbogram, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6Option, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6Option::OptionNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6Option::OptionNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Option:SetNode(0x7fcc12c4c080, 0x7fcc12c47ca0)
0s Ipv6Option:Ipv6OptionRouterAlert()
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6OptionRouterAlert, params=0
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv6Option, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv6Option::OptionNumber"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv6Option::OptionNumber" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv6Option:SetNode(0x7fcc12c4c0c0, 0x7fcc12c47ca0)
0s Ipv6L3Protocol:NotifyNewAggregate()
0s Icmpv6L4Protocol:NotifyNewAggregate()
```

These roughly a thousand line is due to

    star.InstallStack (internet);

Here, for each node, we created the ARP, IPv4 layer-3 protocol, the LoopbackNetDevice (so the loopback is the last NetDevice in a node in this script), UDP & TCP protocols (the layer-4 demultiplexers), Ipv4ListRouting as the default routing protocol (actually a meta routing protocol), then installed Ipv4StaticRouting and Ipv4GlobalRouting with the loopback address 127.0.0.1 populated to the routing tables. Then similar is being done for IPv6 counterparts.

We can also found ECMP support in in Ipv4GlobalRouting, presented as an attribute.

set up address (1118-1419):
```
0s Ipv4AddressHelper:Ipv4AddressHelper()
0s Ipv4AddressHelper:SetBase()
0s Ipv4AddressHelper:NumAddressBits()
0s Ipv4AddressHelper:NumAddressBits(): NumAddressBits -> 8
0s Ipv4AddressHelper:SetBase(): m_network == a0101
0s Ipv4AddressHelper:SetBase(): m_mask == ffffff00
0s Ipv4AddressHelper:SetBase(): m_address == 1
0s Ipv4AddressHelper:Assign()
0s Ipv4L3Protocol:AddInterface(0x7fcc12c49540, 0x7fff6d4025f0)
0s Ipv4Interface:Ipv4Interface(0x7fcc12c4c660)
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4Interface, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv4Interface::ArpCache"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv4Interface::ArpCache" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv4Interface:SetForwarding()
0s Ipv4L3Protocol:AddIpv4Interface(0x7fcc12c49540, 0x7fcc12c4c660)
0s Ipv4AddressGenerator:AddAllocated()
0s Ipv4AddressGenerator:Ipv4AddressGeneratorImpl()
0s Ipv4AddressGenerator:Reset()
0s Simulator:ScheduleDestroy(1)
0s Ipv4AddressGenerator:AddAllocated()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d402560)
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d402618)
0s Ipv4L3Protocol:AddAddress(0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d402278)
0x7fcc12c49540, 1, 0s Ipv4InterfaceAddress:IsSecondary()
0s Ipv4InterfaceAddress:GetScope()
0s Ipv4InterfaceAddress:GetBroadcast()
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4InterfaceAddress:GetLocal()
m_local=a.1.1.1; m_mask=ff.ff.ff.0; m_broadcast=a.1.1.ff; m_scope=2; m_secondary=0)
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d4023b0)
0s Ipv4Interface:AddAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fcc12c47890)
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d4023d0)
0s Ipv4ListRouting:NotifyAddAddress(0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d402030)
0x7fcc12c49960, 1, 0s Ipv4InterfaceAddress:IsSecondary()
0s Ipv4InterfaceAddress:GetScope()
0s Ipv4InterfaceAddress:GetBroadcast()
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4InterfaceAddress:GetLocal()
m_local=a.1.1.1; m_mask=ff.ff.ff.0; m_broadcast=a.1.1.ff; m_scope=2; m_secondary=0)
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d4021a0)
0s 0 [node 0] Ipv4StaticRouting:NotifyAddAddress(0s Ipv4InterfaceAddress:GetLocal()
0x7fcc12c49a70, 1,  , a.1.1.1)
0s Ipv4Interface:IsUp()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d4021a0)
0s Ipv4GlobalRouting:NotifyAddAddress(0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401e30)
0x7fcc12c49ac0, 1, 0s Ipv4InterfaceAddress:IsSecondary()
0s Ipv4InterfaceAddress:GetScope()
0s Ipv4InterfaceAddress:GetBroadcast()
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4InterfaceAddress:GetLocal()
m_local=a.1.1.1; m_mask=ff.ff.ff.0; m_broadcast=a.1.1.ff; m_scope=2; m_secondary=0)
0s Ipv4L3Protocol:SetMetric(0x7fcc12c49540, 1, 1)
0s Ipv4Interface:SetMetric(1)
0s Ipv4L3Protocol:SetUp(0x7fcc12c49540, 1)
0s Ipv4Interface:SetUp()
0s Ipv4ListRouting:NotifyInterfaceUp(0x7fcc12c49960, 1)
0s 0 [node 0] Ipv4StaticRouting:NotifyInterfaceUp(0x7fcc12c49a70, 1)
0s Ipv4Interface:GetNAddresses()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401ef8)
0s Ipv4InterfaceAddress:GetLocal()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401f30)
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401f70)
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401fa0)
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401fd8)
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d402008)
0s Ipv4InterfaceAddress:GetLocal()
0s 0 [node 0] Ipv4StaticRouting:AddNetworkRouteTo(0x7fcc12c49a70, a.1.1.0,  , ff.ff.ff.0,  , 1,  , 0)
0s Ipv4Interface:GetNAddresses()
0s Ipv4GlobalRouting:NotifyInterfaceUp(0x7fcc12c49ac0, 1)
0s Ipv4AddressHelper:Assign()
0s Ipv4L3Protocol:AddInterface(0x7fcc12c4a4e0, 0x7fff6d4025f0)
0s Ipv4Interface:Ipv4Interface(0x7fcc12c4ca00)
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4Interface, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv4Interface::ArpCache"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv4Interface::ArpCache" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv4Interface:SetForwarding()
0s Ipv4L3Protocol:AddIpv4Interface(0x7fcc12c4a4e0, 0x7fcc12c4ca00)
0s Ipv4AddressGenerator:AddAllocated()
0s Ipv4AddressGenerator:AddAllocated()
0s Ipv4AddressGenerator:AddAllocated(): examine entry: a.1.1.1 to a.1.1.1
0s Ipv4AddressGenerator:AddAllocated(): New addrHigh = a.1.1.2
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d402560)
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d402618)
0s Ipv4L3Protocol:AddAddress(0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d402278)
0x7fcc12c4a4e0, 1, 0s Ipv4InterfaceAddress:IsSecondary()
0s Ipv4InterfaceAddress:GetScope()
0s Ipv4InterfaceAddress:GetBroadcast()
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4InterfaceAddress:GetLocal()
m_local=a.1.1.2; m_mask=ff.ff.ff.0; m_broadcast=a.1.1.ff; m_scope=2; m_secondary=0)
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d4023b0)
0s Ipv4Interface:AddAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fcc12c4ca70)
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d4023d0)
0s Ipv4ListRouting:NotifyAddAddress(0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d402030)
0x7fcc12c4a9b0, 1, 0s Ipv4InterfaceAddress:IsSecondary()
0s Ipv4InterfaceAddress:GetScope()
0s Ipv4InterfaceAddress:GetBroadcast()
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4InterfaceAddress:GetLocal()
m_local=a.1.1.2; m_mask=ff.ff.ff.0; m_broadcast=a.1.1.ff; m_scope=2; m_secondary=0)
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d4021a0)
0s 0 [node 1] Ipv4StaticRouting:NotifyAddAddress(0s Ipv4InterfaceAddress:GetLocal()
0x7fcc12c4aaa0, 1,  , a.1.1.2)
0s Ipv4Interface:IsUp()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d4021a0)
0s Ipv4GlobalRouting:NotifyAddAddress(0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401e30)
0x7fcc12c4aaf0, 1, 0s Ipv4InterfaceAddress:IsSecondary()
0s Ipv4InterfaceAddress:GetScope()
0s Ipv4InterfaceAddress:GetBroadcast()
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4InterfaceAddress:GetLocal()
m_local=a.1.1.2; m_mask=ff.ff.ff.0; m_broadcast=a.1.1.ff; m_scope=2; m_secondary=0)
0s Ipv4L3Protocol:SetMetric(0x7fcc12c4a4e0, 1, 1)
0s Ipv4Interface:SetMetric(1)
0s Ipv4L3Protocol:SetUp(0x7fcc12c4a4e0, 1)
0s Ipv4Interface:SetUp()
0s Ipv4ListRouting:NotifyInterfaceUp(0x7fcc12c4a9b0, 1)
0s 0 [node 1] Ipv4StaticRouting:NotifyInterfaceUp(0x7fcc12c4aaa0, 1)
0s Ipv4Interface:GetNAddresses()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401ef8)
0s Ipv4InterfaceAddress:GetLocal()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401f30)
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401f70)
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401fa0)
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401fd8)
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d402008)
0s Ipv4InterfaceAddress:GetLocal()
0s 0 [node 1] Ipv4StaticRouting:AddNetworkRouteTo(0x7fcc12c4aaa0, a.1.1.0,  , ff.ff.ff.0,  , 1,  , 0)
0s Ipv4Interface:GetNAddresses()
0s Ipv4GlobalRouting:NotifyInterfaceUp(0x7fcc12c4aaf0, 1)
0s Ipv4AddressHelper:NewNetwork()
0s Ipv4AddressHelper:Assign()
0s Ipv4L3Protocol:AddInterface(0x7fcc12c49540, 0x7fff6d4025f0)
0s Ipv4Interface:Ipv4Interface(0x7fcc12c4cbf0)
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4Interface, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv4Interface::ArpCache"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv4Interface::ArpCache" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv4Interface:SetForwarding()
0s Ipv4L3Protocol:AddIpv4Interface(0x7fcc12c49540, 0x7fcc12c4cbf0)
0s Ipv4AddressGenerator:AddAllocated()
0s Ipv4AddressGenerator:AddAllocated()
0s Ipv4AddressGenerator:AddAllocated(): examine entry: a.1.1.1 to a.1.1.2
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d402560)
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d402618)
0s Ipv4L3Protocol:AddAddress(0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d402278)
0x7fcc12c49540, 2, 0s Ipv4InterfaceAddress:IsSecondary()
0s Ipv4InterfaceAddress:GetScope()
0s Ipv4InterfaceAddress:GetBroadcast()
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4InterfaceAddress:GetLocal()
m_local=a.1.2.1; m_mask=ff.ff.ff.0; m_broadcast=a.1.2.ff; m_scope=2; m_secondary=0)
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d4023b0)
0s Ipv4Interface:AddAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fcc12c4c4f0)
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d4023d0)
0s Ipv4ListRouting:NotifyAddAddress(0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d402030)
0x7fcc12c49960, 2, 0s Ipv4InterfaceAddress:IsSecondary()
0s Ipv4InterfaceAddress:GetScope()
0s Ipv4InterfaceAddress:GetBroadcast()
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4InterfaceAddress:GetLocal()
m_local=a.1.2.1; m_mask=ff.ff.ff.0; m_broadcast=a.1.2.ff; m_scope=2; m_secondary=0)
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d4021a0)
0s 0 [node 0] Ipv4StaticRouting:NotifyAddAddress(0s Ipv4InterfaceAddress:GetLocal()
0x7fcc12c49a70, 2,  , a.1.2.1)
0s Ipv4Interface:IsUp()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d4021a0)
0s Ipv4GlobalRouting:NotifyAddAddress(0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401e30)
0x7fcc12c49ac0, 2, 0s Ipv4InterfaceAddress:IsSecondary()
0s Ipv4InterfaceAddress:GetScope()
0s Ipv4InterfaceAddress:GetBroadcast()
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4InterfaceAddress:GetLocal()
m_local=a.1.2.1; m_mask=ff.ff.ff.0; m_broadcast=a.1.2.ff; m_scope=2; m_secondary=0)
0s Ipv4L3Protocol:SetMetric(0x7fcc12c49540, 2, 1)
0s Ipv4Interface:SetMetric(1)
0s Ipv4L3Protocol:SetUp(0x7fcc12c49540, 2)
0s Ipv4Interface:SetUp()
0s Ipv4ListRouting:NotifyInterfaceUp(0x7fcc12c49960, 2)
0s 0 [node 0] Ipv4StaticRouting:NotifyInterfaceUp(0x7fcc12c49a70, 2)
0s Ipv4Interface:GetNAddresses()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401ef8)
0s Ipv4InterfaceAddress:GetLocal()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401f30)
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401f70)
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401fa0)
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401fd8)
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d402008)
0s Ipv4InterfaceAddress:GetLocal()
0s 0 [node 0] Ipv4StaticRouting:AddNetworkRouteTo(0x7fcc12c49a70, a.1.2.0,  , ff.ff.ff.0,  , 2,  , 0)
0s Ipv4Interface:GetNAddresses()
0s Ipv4GlobalRouting:NotifyInterfaceUp(0x7fcc12c49ac0, 2)
0s Ipv4AddressHelper:Assign()
0s Ipv4L3Protocol:AddInterface(0x7fcc12c4b5b0, 0x7fff6d4025f0)
0s Ipv4Interface:Ipv4Interface(0x7fcc12c4cd30)
0s ObjectBase:ConstructSelf(): construct tid=ns3::Ipv4Interface, params=1
0s ObjectBase:ConstructSelf(): try to construct "ns3::Ipv4Interface::ArpCache"
0s ObjectBase:ConstructSelf(): construct "ns3::Ipv4Interface::ArpCache" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s Ipv4Interface:SetForwarding()
0s Ipv4L3Protocol:AddIpv4Interface(0x7fcc12c4b5b0, 0x7fcc12c4cd30)
0s Ipv4AddressGenerator:AddAllocated()
0s Ipv4AddressGenerator:AddAllocated()
0s Ipv4AddressGenerator:AddAllocated(): examine entry: a.1.1.1 to a.1.1.2
0s Ipv4AddressGenerator:AddAllocated(): examine entry: a.1.2.1 to a.1.2.1
0s Ipv4AddressGenerator:AddAllocated(): New addrHigh = a.1.2.2
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d402560)
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d402618)
0s Ipv4L3Protocol:AddAddress(0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d402278)
0x7fcc12c4b5b0, 1, 0s Ipv4InterfaceAddress:IsSecondary()
0s Ipv4InterfaceAddress:GetScope()
0s Ipv4InterfaceAddress:GetBroadcast()
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4InterfaceAddress:GetLocal()
m_local=a.1.2.2; m_mask=ff.ff.ff.0; m_broadcast=a.1.2.ff; m_scope=2; m_secondary=0)
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d4023b0)
0s Ipv4Interface:AddAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fcc12c4cd90)
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d4023d0)
0s Ipv4ListRouting:NotifyAddAddress(0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d402030)
0x7fcc12c4ba80, 1, 0s Ipv4InterfaceAddress:IsSecondary()
0s Ipv4InterfaceAddress:GetScope()
0s Ipv4InterfaceAddress:GetBroadcast()
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4InterfaceAddress:GetLocal()
m_local=a.1.2.2; m_mask=ff.ff.ff.0; m_broadcast=a.1.2.ff; m_scope=2; m_secondary=0)
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d4021a0)
0s 0 [node 2] Ipv4StaticRouting:NotifyAddAddress(0s Ipv4InterfaceAddress:GetLocal()
0x7fcc12c4bb70, 1,  , a.1.2.2)
0s Ipv4Interface:IsUp()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d4021a0)
0s Ipv4GlobalRouting:NotifyAddAddress(0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401e30)
0x7fcc12c4bbc0, 1, 0s Ipv4InterfaceAddress:IsSecondary()
0s Ipv4InterfaceAddress:GetScope()
0s Ipv4InterfaceAddress:GetBroadcast()
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4InterfaceAddress:GetLocal()
m_local=a.1.2.2; m_mask=ff.ff.ff.0; m_broadcast=a.1.2.ff; m_scope=2; m_secondary=0)
0s Ipv4L3Protocol:SetMetric(0x7fcc12c4b5b0, 1, 1)
0s Ipv4Interface:SetMetric(1)
0s Ipv4L3Protocol:SetUp(0x7fcc12c4b5b0, 1)
0s Ipv4Interface:SetUp()
0s Ipv4ListRouting:NotifyInterfaceUp(0x7fcc12c4ba80, 1)
0s 0 [node 2] Ipv4StaticRouting:NotifyInterfaceUp(0x7fcc12c4bb70, 1)
0s Ipv4Interface:GetNAddresses()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401ef8)
0s Ipv4InterfaceAddress:GetLocal()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401f30)
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401f70)
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401fa0)
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401fd8)
0s Ipv4InterfaceAddress:GetMask()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d402008)
0s Ipv4InterfaceAddress:GetLocal()
0s 0 [node 2] Ipv4StaticRouting:AddNetworkRouteTo(0x7fcc12c4bb70, a.1.2.0,  , ff.ff.ff.0,  , 1,  , 0)
0s Ipv4Interface:GetNAddresses()
0s Ipv4GlobalRouting:NotifyInterfaceUp(0x7fcc12c4bbc0, 1)
0s Ipv4AddressHelper:NewNetwork()
```

These three hundred lines is due to

    star.AssignIpv4Addresses (Ipv4AddressHelper ("10.1.1.0", "255.255.255.0"));

This draws an IP address from the pool for each NetDevice interface. After configuring the NetDevice, the address is also populated to the routing table of the corresponding node (on Ipv4StaticRouting)

## Routing

routing (line 1420-2893):
```
0s VerySimple:main(): Enable static global routing.
0s GlobalRouteManager:GlobalRouteManagerImpl()
0s GlobalRouteManager:GlobalRouteManagerLSDB()
0s Simulator:ScheduleDestroy(1)
0s GlobalRouteManager:BuildGlobalRoutingDatabase()
0s GlobalRouter:DiscoverLSAs()
0s GlobalRouter:DiscoverLSAs(): For node 0
0s GlobalRouter:ClearLSAs()
0s GlobalRouter:ClearLSAs(): Clear list of LSAs
0s GlobalRouter:GlobalRoutingLSA()
0s GlobalRouter:SetLSType()
0s GlobalRouter:SetLinkStateId()
0s GlobalRouter:SetAdvertisingRouter()
0s GlobalRouter:SetStatus()
0s GlobalRouter:SetNode(0x7fcc12c478e0)
0s GlobalRouter:NetDeviceIsBridged(0x7fcc12c47da0)
0s GlobalRouter:NetDeviceIsBridged(): Examine device 0 0x7fcc12c47da0
0s GlobalRouter:NetDeviceIsBridged(): Examine device 1 0x7fcc12c48a00
0s GlobalRouter:NetDeviceIsBridged(): Examine device 2 0x7fcc12c49680
0s GlobalRouter:NetDeviceIsBridged(): Net device 0x7fcc12c47da0 is not bridged
0s Ipv4Interface:IsUp()
0s Ipv4L3Protocol:IsForwarding(0x7fcc12c49540, 1)
0s Ipv4L3Protocol:IsForwarding(): 0s Ipv4Interface:IsForwarding()
Forwarding state: 1
0s Ipv4Interface:IsForwarding()
0s GlobalRouter:DiscoverLSAs(): Point=to-point link
0s GlobalRouter:ProcessPointToPointLink(0x7fcc12c47da0, 0x7fcc12c4cec0)
0s GlobalRouter:FindInterfaceForDevice()
0s GlobalRouter:FindInterfaceForDevice(): For node 0 for net device 0x7fcc12c47da0
0s GlobalRouter:FindInterfaceForDevice(): Device 0x7fcc12c47da0 has associated ipv4 index 1
0s Ipv4Interface:GetNAddresses()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401c38)
0s Ipv4InterfaceAddress:GetLocal()
0s GlobalRouter:ProcessPointToPointLink(): Working with local address a.1.1.1
0s Ipv4Interface:GetMetric()
0s GlobalRouter:GetAdjacent()
0s PointToPointChannel:GetNDevices()
0s PointToPointChannel:GetDevice()
0s PointToPointChannel:GetPointToPointDevice()
0s PointToPointChannel:GetDevice()
0s PointToPointChannel:GetPointToPointDevice()
0s GlobalRouter:GetRouterId()
0s GlobalRouter:ProcessPointToPointLink(): Working with remote router 0.0.0.1
0s GlobalRouter:FindInterfaceForDevice()
0s GlobalRouter:FindInterfaceForDevice(): For node 1 for net device 0x7fcc12c480b0
0s GlobalRouter:FindInterfaceForDevice(): Device 0x7fcc12c480b0 has associated ipv4 index 1
0s Ipv4Interface:GetNAddresses()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401cc8)
0s Ipv4InterfaceAddress:GetLocal()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401cf0)
0s Ipv4InterfaceAddress:GetMask()
0s GlobalRouter:ProcessPointToPointLink(): Working with remote address a.1.1.2
0s Ipv4Interface:IsUp()
0s GlobalRouter:ProcessPointToPointLink(): Remote side interface 1 is up-- add a type 1 link
0s GlobalRouter:GlobalRoutingLinkRecord()
0s GlobalRouter:SetLinkType()
0s GlobalRouter:SetLinkId()
0s GlobalRouter:SetLinkData()
0s GlobalRouter:SetMetric()
0s GlobalRouter:AddLinkRecord()
0s GlobalRouter:GlobalRoutingLinkRecord()
0s GlobalRouter:SetLinkType()
0s GlobalRouter:SetLinkId()
0s GlobalRouter:SetLinkData()
0s GlobalRouter:SetMetric()
0s GlobalRouter:AddLinkRecord()
0s GlobalRouter:NetDeviceIsBridged(0x7fcc12c48a00)
0s GlobalRouter:NetDeviceIsBridged(): Examine device 0 0x7fcc12c47da0
0s GlobalRouter:NetDeviceIsBridged(): Examine device 1 0x7fcc12c48a00
0s GlobalRouter:NetDeviceIsBridged(): Examine device 2 0x7fcc12c49680
0s GlobalRouter:NetDeviceIsBridged(): Net device 0x7fcc12c48a00 is not bridged
0s Ipv4Interface:IsUp()
0s Ipv4L3Protocol:IsForwarding(0x7fcc12c49540, 2)
0s Ipv4L3Protocol:IsForwarding(): 0s Ipv4Interface:IsForwarding()
Forwarding state: 1
0s Ipv4Interface:IsForwarding()
0s GlobalRouter:DiscoverLSAs(): Point=to-point link
0s GlobalRouter:ProcessPointToPointLink(0x7fcc12c48a00, 0x7fcc12c4cec0)
0s GlobalRouter:FindInterfaceForDevice()
0s GlobalRouter:FindInterfaceForDevice(): For node 0 for net device 0x7fcc12c48a00
0s GlobalRouter:FindInterfaceForDevice(): Device 0x7fcc12c48a00 has associated ipv4 index 2
0s Ipv4Interface:GetNAddresses()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401c38)
0s Ipv4InterfaceAddress:GetLocal()
0s GlobalRouter:ProcessPointToPointLink(): Working with local address a.1.2.1
0s Ipv4Interface:GetMetric()
0s GlobalRouter:GetAdjacent()
0s PointToPointChannel:GetNDevices()
0s PointToPointChannel:GetDevice()
0s PointToPointChannel:GetPointToPointDevice()
0s PointToPointChannel:GetDevice()
0s PointToPointChannel:GetPointToPointDevice()
0s GlobalRouter:GetRouterId()
0s GlobalRouter:ProcessPointToPointLink(): Working with remote router 0.0.0.2
0s GlobalRouter:FindInterfaceForDevice()
0s GlobalRouter:FindInterfaceForDevice(): For node 2 for net device 0x7fcc12c48c30
0s GlobalRouter:FindInterfaceForDevice(): Device 0x7fcc12c48c30 has associated ipv4 index 1
0s Ipv4Interface:GetNAddresses()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401cc8)
0s Ipv4InterfaceAddress:GetLocal()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401cf0)
0s Ipv4InterfaceAddress:GetMask()
0s GlobalRouter:ProcessPointToPointLink(): Working with remote address a.1.2.2
0s Ipv4Interface:IsUp()
0s GlobalRouter:ProcessPointToPointLink(): Remote side interface 1 is up-- add a type 1 link
0s GlobalRouter:GlobalRoutingLinkRecord()
0s GlobalRouter:SetLinkType()
0s GlobalRouter:SetLinkId()
0s GlobalRouter:SetLinkData()
0s GlobalRouter:SetMetric()
0s GlobalRouter:AddLinkRecord()
0s GlobalRouter:GlobalRoutingLinkRecord()
0s GlobalRouter:SetLinkType()
0s GlobalRouter:SetLinkId()
0s GlobalRouter:SetLinkData()
0s GlobalRouter:SetMetric()
0s GlobalRouter:AddLinkRecord()
0s GlobalRouter:NetDeviceIsBridged(0x7fcc12c49680)
0s GlobalRouter:NetDeviceIsBridged(): Examine device 0 0x7fcc12c47da0
0s GlobalRouter:NetDeviceIsBridged(): Examine device 1 0x7fcc12c48a00
0s GlobalRouter:NetDeviceIsBridged(): Examine device 2 0x7fcc12c49680
0s GlobalRouter:NetDeviceIsBridged(): Net device 0x7fcc12c49680 is not bridged
0s Ipv4Interface:IsUp()
0s Ipv4L3Protocol:IsForwarding(0x7fcc12c49540, 0)
0s Ipv4L3Protocol:IsForwarding(): 0s Ipv4Interface:IsForwarding()
Forwarding state: 1
0s Ipv4Interface:IsForwarding()
0s GlobalRouter:DiscoverLSAs(): Broadcast link
0s GlobalRouter:ProcessBroadcastLink(0x7fcc12c49680, 0x7fcc12c4cec0, 0x7fff6d402348)
0s GlobalRouter:ProcessSingleBroadcastLink(0x7fcc12c49680, 0x7fcc12c4cec0, 0x7fff6d402348)
0s GlobalRouter:GlobalRoutingLinkRecord()
0s GlobalRouter:FindInterfaceForDevice()
0s GlobalRouter:FindInterfaceForDevice(): For node 0 for net device 0x7fcc12c49680
0s GlobalRouter:FindInterfaceForDevice(): Device 0x7fcc12c49680 has associated ipv4 index 0
0s Ipv4Interface:GetNAddresses()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401ae8)
0s Ipv4InterfaceAddress:GetLocal()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401b10)
0s Ipv4InterfaceAddress:GetMask()
0s GlobalRouter:ProcessSingleBroadcastLink(): Working with local address 7f.0.0.1
0s Ipv4Interface:GetMetric()
0s GlobalRouter:AnotherRouterOnLink(0x7fcc12c49680, 1)
0s GlobalRouter:ProcessSingleBroadcastLink(): Router-LSA Stub Network
0s GlobalRouter:SetLinkType()
0s GlobalRouter:SetLinkId()
0s GlobalRouter:SetLinkData()
0s GlobalRouter:SetMetric()
0s GlobalRouter:AddLinkRecord()
0s GlobalRouter:DiscoverLSAs(): ========== LSA for node 0 ==========
0s GlobalRouter:DiscoverLSAs(): 
========== Global Routing LSA ==========
m_lsType = 1 (GlobalRoutingLSA::RouterLSA)
m_linkStateId = 0.0.0.0 (Router ID)
m_advertisingRtr = 0.0.0.0 (Router ID)
---------- RouterLSA Link Record ----------
m_linkType = 1 (GlobalRoutingLinkRecord::PointToPoint)
m_linkId = 0.0.0.1
m_linkData = a.1.1.1
m_metric = 1
---------- End RouterLSA Link Record ----------
---------- RouterLSA Link Record ----------
m_linkType = 3 (GlobalRoutingLinkRecord::StubNetwork)
m_linkId = a.1.1.2 (Network number of attached network)
m_linkData = ff.ff.ff.0 (Network mask of attached network)
m_metric = 1
---------- End RouterLSA Link Record ----------
---------- RouterLSA Link Record ----------
m_linkType = 1 (GlobalRoutingLinkRecord::PointToPoint)
m_linkId = 0.0.0.2
m_linkData = a.1.2.1
m_metric = 1
---------- End RouterLSA Link Record ----------
---------- RouterLSA Link Record ----------
m_linkType = 3 (GlobalRoutingLinkRecord::StubNetwork)
m_linkId = a.1.2.2 (Network number of attached network)
m_linkData = ff.ff.ff.0 (Network mask of attached network)
m_metric = 1
---------- End RouterLSA Link Record ----------
---------- RouterLSA Link Record ----------
m_linkType = 3 (GlobalRoutingLinkRecord::StubNetwork)
m_linkId = 7f.0.0.0 (Network number of attached network)
m_linkData = ff.0.0.0 (Network mask of attached network)
m_metric = 1
---------- End RouterLSA Link Record ----------
========== End Global Routing LSA ==========

0s GlobalRouteManager:BuildGlobalRoutingDatabase(): Found 1 LSAs
0s GlobalRouter:GlobalRoutingLSA()
0s GlobalRouter:GetLSA()
0s GlobalRouter:IsEmpty()
0s GlobalRouter:operator=()
0s GlobalRouter:ClearLinkRecords()
0s GlobalRouter:ClearLinkRecords(): Clear list
0s GlobalRouter:CopyLinkRecords()
0s GlobalRouter:GlobalRoutingLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:SetLinkType()
0s GlobalRouter:GetLinkId()
0s GlobalRouter:SetLinkId()
0s GlobalRouter:GetLinkData()
0s GlobalRouter:SetLinkData()
0s GlobalRouter:GetMetric()
0s GlobalRouter:SetMetric()
0s GlobalRouter:GlobalRoutingLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:SetLinkType()
0s GlobalRouter:GetLinkId()
0s GlobalRouter:SetLinkId()
0s GlobalRouter:GetLinkData()
0s GlobalRouter:SetLinkData()
0s GlobalRouter:GetMetric()
0s GlobalRouter:SetMetric()
0s GlobalRouter:GlobalRoutingLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:SetLinkType()
0s GlobalRouter:GetLinkId()
0s GlobalRouter:SetLinkId()
0s GlobalRouter:GetLinkData()
0s GlobalRouter:SetLinkData()
0s GlobalRouter:GetMetric()
0s GlobalRouter:SetMetric()
0s GlobalRouter:GlobalRoutingLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:SetLinkType()
0s GlobalRouter:GetLinkId()
0s GlobalRouter:SetLinkId()
0s GlobalRouter:GetLinkData()
0s GlobalRouter:SetLinkData()
0s GlobalRouter:GetMetric()
0s GlobalRouter:SetMetric()
0s GlobalRouter:GlobalRoutingLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:SetLinkType()
0s GlobalRouter:GetLinkId()
0s GlobalRouter:SetLinkId()
0s GlobalRouter:GetLinkData()
0s GlobalRouter:SetLinkData()
0s GlobalRouter:GetMetric()
0s GlobalRouter:SetMetric()
0s GlobalRouteManager:BuildGlobalRoutingDatabase(): 
========== Global Routing LSA ==========
m_lsType = 1 (GlobalRoutingLSA::RouterLSA)
m_linkStateId = 0.0.0.0 (Router ID)
m_advertisingRtr = 0.0.0.0 (Router ID)
---------- RouterLSA Link Record ----------
m_linkType = 1 (GlobalRoutingLinkRecord::PointToPoint)
m_linkId = 0.0.0.1
m_linkData = a.1.1.1
m_metric = 1
---------- End RouterLSA Link Record ----------
---------- RouterLSA Link Record ----------
m_linkType = 3 (GlobalRoutingLinkRecord::StubNetwork)
m_linkId = a.1.1.2 (Network number of attached network)
m_linkData = ff.ff.ff.0 (Network mask of attached network)
m_metric = 1
---------- End RouterLSA Link Record ----------
---------- RouterLSA Link Record ----------
m_linkType = 1 (GlobalRoutingLinkRecord::PointToPoint)
m_linkId = 0.0.0.2
m_linkData = a.1.2.1
m_metric = 1
---------- End RouterLSA Link Record ----------
---------- RouterLSA Link Record ----------
m_linkType = 3 (GlobalRoutingLinkRecord::StubNetwork)
m_linkId = a.1.2.2 (Network number of attached network)
m_linkData = ff.ff.ff.0 (Network mask of attached network)
m_metric = 1
---------- End RouterLSA Link Record ----------
---------- RouterLSA Link Record ----------
m_linkType = 3 (GlobalRoutingLinkRecord::StubNetwork)
m_linkId = 7f.0.0.0 (Network number of attached network)
m_linkData = ff.0.0.0 (Network mask of attached network)
m_metric = 1
---------- End RouterLSA Link Record ----------
========== End Global Routing LSA ==========

0s GlobalRouter:GetLinkStateId()
0s GlobalRouteManager:Insert(0.0.0.0, 0x7fcc12c4cfe0)
0s GlobalRouter:GetLSType()
0s GlobalRouter:DiscoverLSAs()
0s GlobalRouter:DiscoverLSAs(): For node 1
0s GlobalRouter:ClearLSAs()
0s GlobalRouter:ClearLSAs(): Clear list of LSAs
0s GlobalRouter:GlobalRoutingLSA()
0s GlobalRouter:SetLSType()
0s GlobalRouter:SetLinkStateId()
0s GlobalRouter:SetAdvertisingRouter()
0s GlobalRouter:SetStatus()
0s GlobalRouter:SetNode(0x7fcc12c47b40)
0s GlobalRouter:NetDeviceIsBridged(0x7fcc12c480b0)
0s GlobalRouter:NetDeviceIsBridged(): Examine device 0 0x7fcc12c480b0
0s GlobalRouter:NetDeviceIsBridged(): Examine device 1 0x7fcc12c4a690
0s GlobalRouter:NetDeviceIsBridged(): Net device 0x7fcc12c480b0 is not bridged
0s Ipv4Interface:IsUp()
0s Ipv4L3Protocol:IsForwarding(0x7fcc12c4a4e0, 1)
0s Ipv4L3Protocol:IsForwarding(): 0s Ipv4Interface:IsForwarding()
Forwarding state: 1
0s Ipv4Interface:IsForwarding()
0s GlobalRouter:DiscoverLSAs(): Point=to-point link
0s GlobalRouter:ProcessPointToPointLink(0x7fcc12c480b0, 0x7fcc12c4d140)
0s GlobalRouter:FindInterfaceForDevice()
0s GlobalRouter:FindInterfaceForDevice(): For node 1 for net device 0x7fcc12c480b0
0s GlobalRouter:FindInterfaceForDevice(): Device 0x7fcc12c480b0 has associated ipv4 index 1
0s Ipv4Interface:GetNAddresses()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401c38)
0s Ipv4InterfaceAddress:GetLocal()
0s GlobalRouter:ProcessPointToPointLink(): Working with local address a.1.1.2
0s Ipv4Interface:GetMetric()
0s GlobalRouter:GetAdjacent()
0s PointToPointChannel:GetNDevices()
0s PointToPointChannel:GetDevice()
0s PointToPointChannel:GetPointToPointDevice()
0s PointToPointChannel:GetDevice()
0s PointToPointChannel:GetPointToPointDevice()
0s GlobalRouter:GetRouterId()
0s GlobalRouter:ProcessPointToPointLink(): Working with remote router 0.0.0.0
0s GlobalRouter:FindInterfaceForDevice()
0s GlobalRouter:FindInterfaceForDevice(): For node 0 for net device 0x7fcc12c47da0
0s GlobalRouter:FindInterfaceForDevice(): Device 0x7fcc12c47da0 has associated ipv4 index 1
0s Ipv4Interface:GetNAddresses()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401cc8)
0s Ipv4InterfaceAddress:GetLocal()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401cf0)
0s Ipv4InterfaceAddress:GetMask()
0s GlobalRouter:ProcessPointToPointLink(): Working with remote address a.1.1.1
0s Ipv4Interface:IsUp()
0s GlobalRouter:ProcessPointToPointLink(): Remote side interface 1 is up-- add a type 1 link
0s GlobalRouter:GlobalRoutingLinkRecord()
0s GlobalRouter:SetLinkType()
0s GlobalRouter:SetLinkId()
0s GlobalRouter:SetLinkData()
0s GlobalRouter:SetMetric()
0s GlobalRouter:AddLinkRecord()
0s GlobalRouter:GlobalRoutingLinkRecord()
0s GlobalRouter:SetLinkType()
0s GlobalRouter:SetLinkId()
0s GlobalRouter:SetLinkData()
0s GlobalRouter:SetMetric()
0s GlobalRouter:AddLinkRecord()
0s GlobalRouter:NetDeviceIsBridged(0x7fcc12c4a690)
0s GlobalRouter:NetDeviceIsBridged(): Examine device 0 0x7fcc12c480b0
0s GlobalRouter:NetDeviceIsBridged(): Examine device 1 0x7fcc12c4a690
0s GlobalRouter:NetDeviceIsBridged(): Net device 0x7fcc12c4a690 is not bridged
0s Ipv4Interface:IsUp()
0s Ipv4L3Protocol:IsForwarding(0x7fcc12c4a4e0, 0)
0s Ipv4L3Protocol:IsForwarding(): 0s Ipv4Interface:IsForwarding()
Forwarding state: 1
0s Ipv4Interface:IsForwarding()
0s GlobalRouter:DiscoverLSAs(): Broadcast link
0s GlobalRouter:ProcessBroadcastLink(0x7fcc12c4a690, 0x7fcc12c4d140, 0x7fff6d402348)
0s GlobalRouter:ProcessSingleBroadcastLink(0x7fcc12c4a690, 0x7fcc12c4d140, 0x7fff6d402348)
0s GlobalRouter:GlobalRoutingLinkRecord()
0s GlobalRouter:FindInterfaceForDevice()
0s GlobalRouter:FindInterfaceForDevice(): For node 1 for net device 0x7fcc12c4a690
0s GlobalRouter:FindInterfaceForDevice(): Device 0x7fcc12c4a690 has associated ipv4 index 0
0s Ipv4Interface:GetNAddresses()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401ae8)
0s Ipv4InterfaceAddress:GetLocal()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401b10)
0s Ipv4InterfaceAddress:GetMask()
0s GlobalRouter:ProcessSingleBroadcastLink(): Working with local address 7f.0.0.1
0s Ipv4Interface:GetMetric()
0s GlobalRouter:AnotherRouterOnLink(0x7fcc12c4a690, 1)
0s GlobalRouter:ProcessSingleBroadcastLink(): Router-LSA Stub Network
0s GlobalRouter:SetLinkType()
0s GlobalRouter:SetLinkId()
0s GlobalRouter:SetLinkData()
0s GlobalRouter:SetMetric()
0s GlobalRouter:AddLinkRecord()
0s GlobalRouter:DiscoverLSAs(): ========== LSA for node 1 ==========
0s GlobalRouter:DiscoverLSAs(): 
========== Global Routing LSA ==========
m_lsType = 1 (GlobalRoutingLSA::RouterLSA)
m_linkStateId = 0.0.0.1 (Router ID)
m_advertisingRtr = 0.0.0.1 (Router ID)
---------- RouterLSA Link Record ----------
m_linkType = 1 (GlobalRoutingLinkRecord::PointToPoint)
m_linkId = 0.0.0.0
m_linkData = a.1.1.2
m_metric = 1
---------- End RouterLSA Link Record ----------
---------- RouterLSA Link Record ----------
m_linkType = 3 (GlobalRoutingLinkRecord::StubNetwork)
m_linkId = a.1.1.1 (Network number of attached network)
m_linkData = ff.ff.ff.0 (Network mask of attached network)
m_metric = 1
---------- End RouterLSA Link Record ----------
---------- RouterLSA Link Record ----------
m_linkType = 3 (GlobalRoutingLinkRecord::StubNetwork)
m_linkId = 7f.0.0.0 (Network number of attached network)
m_linkData = ff.0.0.0 (Network mask of attached network)
m_metric = 1
---------- End RouterLSA Link Record ----------
========== End Global Routing LSA ==========

0s GlobalRouteManager:BuildGlobalRoutingDatabase(): Found 1 LSAs
0s GlobalRouter:GlobalRoutingLSA()
0s GlobalRouter:GetLSA()
0s GlobalRouter:IsEmpty()
0s GlobalRouter:operator=()
0s GlobalRouter:ClearLinkRecords()
0s GlobalRouter:ClearLinkRecords(): Clear list
0s GlobalRouter:CopyLinkRecords()
0s GlobalRouter:GlobalRoutingLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:SetLinkType()
0s GlobalRouter:GetLinkId()
0s GlobalRouter:SetLinkId()
0s GlobalRouter:GetLinkData()
0s GlobalRouter:SetLinkData()
0s GlobalRouter:GetMetric()
0s GlobalRouter:SetMetric()
0s GlobalRouter:GlobalRoutingLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:SetLinkType()
0s GlobalRouter:GetLinkId()
0s GlobalRouter:SetLinkId()
0s GlobalRouter:GetLinkData()
0s GlobalRouter:SetLinkData()
0s GlobalRouter:GetMetric()
0s GlobalRouter:SetMetric()
0s GlobalRouter:GlobalRoutingLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:SetLinkType()
0s GlobalRouter:GetLinkId()
0s GlobalRouter:SetLinkId()
0s GlobalRouter:GetLinkData()
0s GlobalRouter:SetLinkData()
0s GlobalRouter:GetMetric()
0s GlobalRouter:SetMetric()
0s GlobalRouteManager:BuildGlobalRoutingDatabase(): 
========== Global Routing LSA ==========
m_lsType = 1 (GlobalRoutingLSA::RouterLSA)
m_linkStateId = 0.0.0.1 (Router ID)
m_advertisingRtr = 0.0.0.1 (Router ID)
---------- RouterLSA Link Record ----------
m_linkType = 1 (GlobalRoutingLinkRecord::PointToPoint)
m_linkId = 0.0.0.0
m_linkData = a.1.1.2
m_metric = 1
---------- End RouterLSA Link Record ----------
---------- RouterLSA Link Record ----------
m_linkType = 3 (GlobalRoutingLinkRecord::StubNetwork)
m_linkId = a.1.1.1 (Network number of attached network)
m_linkData = ff.ff.ff.0 (Network mask of attached network)
m_metric = 1
---------- End RouterLSA Link Record ----------
---------- RouterLSA Link Record ----------
m_linkType = 3 (GlobalRoutingLinkRecord::StubNetwork)
m_linkId = 7f.0.0.0 (Network number of attached network)
m_linkData = ff.0.0.0 (Network mask of attached network)
m_metric = 1
---------- End RouterLSA Link Record ----------
========== End Global Routing LSA ==========

0s GlobalRouter:GetLinkStateId()
0s GlobalRouteManager:Insert(0.0.0.1, 0x7fcc12c4d230)
0s GlobalRouter:GetLSType()
0s GlobalRouter:DiscoverLSAs()
0s GlobalRouter:DiscoverLSAs(): For node 2
0s GlobalRouter:ClearLSAs()
0s GlobalRouter:ClearLSAs(): Clear list of LSAs
0s GlobalRouter:GlobalRoutingLSA()
0s GlobalRouter:SetLSType()
0s GlobalRouter:SetLinkStateId()
0s GlobalRouter:SetAdvertisingRouter()
0s GlobalRouter:SetStatus()
0s GlobalRouter:SetNode(0x7fcc12c47ca0)
0s GlobalRouter:NetDeviceIsBridged(0x7fcc12c48c30)
0s GlobalRouter:NetDeviceIsBridged(): Examine device 0 0x7fcc12c48c30
0s GlobalRouter:NetDeviceIsBridged(): Examine device 1 0x7fcc12c4b760
0s GlobalRouter:NetDeviceIsBridged(): Net device 0x7fcc12c48c30 is not bridged
0s Ipv4Interface:IsUp()
0s Ipv4L3Protocol:IsForwarding(0x7fcc12c4b5b0, 1)
0s Ipv4L3Protocol:IsForwarding(): 0s Ipv4Interface:IsForwarding()
Forwarding state: 1
0s Ipv4Interface:IsForwarding()
0s GlobalRouter:DiscoverLSAs(): Point=to-point link
0s GlobalRouter:ProcessPointToPointLink(0x7fcc12c48c30, 0x7fcc12c4d330)
0s GlobalRouter:FindInterfaceForDevice()
0s GlobalRouter:FindInterfaceForDevice(): For node 2 for net device 0x7fcc12c48c30
0s GlobalRouter:FindInterfaceForDevice(): Device 0x7fcc12c48c30 has associated ipv4 index 1
0s Ipv4Interface:GetNAddresses()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401c38)
0s Ipv4InterfaceAddress:GetLocal()
0s GlobalRouter:ProcessPointToPointLink(): Working with local address a.1.2.2
0s Ipv4Interface:GetMetric()
0s GlobalRouter:GetAdjacent()
0s PointToPointChannel:GetNDevices()
0s PointToPointChannel:GetDevice()
0s PointToPointChannel:GetPointToPointDevice()
0s PointToPointChannel:GetDevice()
0s PointToPointChannel:GetPointToPointDevice()
0s GlobalRouter:GetRouterId()
0s GlobalRouter:ProcessPointToPointLink(): Working with remote router 0.0.0.0
0s GlobalRouter:FindInterfaceForDevice()
0s GlobalRouter:FindInterfaceForDevice(): For node 0 for net device 0x7fcc12c48a00
0s GlobalRouter:FindInterfaceForDevice(): Device 0x7fcc12c48a00 has associated ipv4 index 2
0s Ipv4Interface:GetNAddresses()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401cc8)
0s Ipv4InterfaceAddress:GetLocal()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401cf0)
0s Ipv4InterfaceAddress:GetMask()
0s GlobalRouter:ProcessPointToPointLink(): Working with remote address a.1.2.1
0s Ipv4Interface:IsUp()
0s GlobalRouter:ProcessPointToPointLink(): Remote side interface 2 is up-- add a type 1 link
0s GlobalRouter:GlobalRoutingLinkRecord()
0s GlobalRouter:SetLinkType()
0s GlobalRouter:SetLinkId()
0s GlobalRouter:SetLinkData()
0s GlobalRouter:SetMetric()
0s GlobalRouter:AddLinkRecord()
0s GlobalRouter:GlobalRoutingLinkRecord()
0s GlobalRouter:SetLinkType()
0s GlobalRouter:SetLinkId()
0s GlobalRouter:SetLinkData()
0s GlobalRouter:SetMetric()
0s GlobalRouter:AddLinkRecord()
0s GlobalRouter:NetDeviceIsBridged(0x7fcc12c4b760)
0s GlobalRouter:NetDeviceIsBridged(): Examine device 0 0x7fcc12c48c30
0s GlobalRouter:NetDeviceIsBridged(): Examine device 1 0x7fcc12c4b760
0s GlobalRouter:NetDeviceIsBridged(): Net device 0x7fcc12c4b760 is not bridged
0s Ipv4Interface:IsUp()
0s Ipv4L3Protocol:IsForwarding(0x7fcc12c4b5b0, 0)
0s Ipv4L3Protocol:IsForwarding(): 0s Ipv4Interface:IsForwarding()
Forwarding state: 1
0s Ipv4Interface:IsForwarding()
0s GlobalRouter:DiscoverLSAs(): Broadcast link
0s GlobalRouter:ProcessBroadcastLink(0x7fcc12c4b760, 0x7fcc12c4d330, 0x7fff6d402348)
0s GlobalRouter:ProcessSingleBroadcastLink(0x7fcc12c4b760, 0x7fcc12c4d330, 0x7fff6d402348)
0s GlobalRouter:GlobalRoutingLinkRecord()
0s GlobalRouter:FindInterfaceForDevice()
0s GlobalRouter:FindInterfaceForDevice(): For node 2 for net device 0x7fcc12c4b760
0s GlobalRouter:FindInterfaceForDevice(): Device 0x7fcc12c4b760 has associated ipv4 index 0
0s Ipv4Interface:GetNAddresses()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401ae8)
0s Ipv4InterfaceAddress:GetLocal()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401b10)
0s Ipv4InterfaceAddress:GetMask()
0s GlobalRouter:ProcessSingleBroadcastLink(): Working with local address 7f.0.0.1
0s Ipv4Interface:GetMetric()
0s GlobalRouter:AnotherRouterOnLink(0x7fcc12c4b760, 1)
0s GlobalRouter:ProcessSingleBroadcastLink(): Router-LSA Stub Network
0s GlobalRouter:SetLinkType()
0s GlobalRouter:SetLinkId()
0s GlobalRouter:SetLinkData()
0s GlobalRouter:SetMetric()
0s GlobalRouter:AddLinkRecord()
0s GlobalRouter:DiscoverLSAs(): ========== LSA for node 2 ==========
0s GlobalRouter:DiscoverLSAs(): 
========== Global Routing LSA ==========
m_lsType = 1 (GlobalRoutingLSA::RouterLSA)
m_linkStateId = 0.0.0.2 (Router ID)
m_advertisingRtr = 0.0.0.2 (Router ID)
---------- RouterLSA Link Record ----------
m_linkType = 1 (GlobalRoutingLinkRecord::PointToPoint)
m_linkId = 0.0.0.0
m_linkData = a.1.2.2
m_metric = 1
---------- End RouterLSA Link Record ----------
---------- RouterLSA Link Record ----------
m_linkType = 3 (GlobalRoutingLinkRecord::StubNetwork)
m_linkId = a.1.2.1 (Network number of attached network)
m_linkData = ff.ff.ff.0 (Network mask of attached network)
m_metric = 1
---------- End RouterLSA Link Record ----------
---------- RouterLSA Link Record ----------
m_linkType = 3 (GlobalRoutingLinkRecord::StubNetwork)
m_linkId = 7f.0.0.0 (Network number of attached network)
m_linkData = ff.0.0.0 (Network mask of attached network)
m_metric = 1
---------- End RouterLSA Link Record ----------
========== End Global Routing LSA ==========

0s GlobalRouteManager:BuildGlobalRoutingDatabase(): Found 1 LSAs
0s GlobalRouter:GlobalRoutingLSA()
0s GlobalRouter:GetLSA()
0s GlobalRouter:IsEmpty()
0s GlobalRouter:operator=()
0s GlobalRouter:ClearLinkRecords()
0s GlobalRouter:ClearLinkRecords(): Clear list
0s GlobalRouter:CopyLinkRecords()
0s GlobalRouter:GlobalRoutingLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:SetLinkType()
0s GlobalRouter:GetLinkId()
0s GlobalRouter:SetLinkId()
0s GlobalRouter:GetLinkData()
0s GlobalRouter:SetLinkData()
0s GlobalRouter:GetMetric()
0s GlobalRouter:SetMetric()
0s GlobalRouter:GlobalRoutingLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:SetLinkType()
0s GlobalRouter:GetLinkId()
0s GlobalRouter:SetLinkId()
0s GlobalRouter:GetLinkData()
0s GlobalRouter:SetLinkData()
0s GlobalRouter:GetMetric()
0s GlobalRouter:SetMetric()
0s GlobalRouter:GlobalRoutingLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:SetLinkType()
0s GlobalRouter:GetLinkId()
0s GlobalRouter:SetLinkId()
0s GlobalRouter:GetLinkData()
0s GlobalRouter:SetLinkData()
0s GlobalRouter:GetMetric()
0s GlobalRouter:SetMetric()
0s GlobalRouteManager:BuildGlobalRoutingDatabase(): 
========== Global Routing LSA ==========
m_lsType = 1 (GlobalRoutingLSA::RouterLSA)
m_linkStateId = 0.0.0.2 (Router ID)
m_advertisingRtr = 0.0.0.2 (Router ID)
---------- RouterLSA Link Record ----------
m_linkType = 1 (GlobalRoutingLinkRecord::PointToPoint)
m_linkId = 0.0.0.0
m_linkData = a.1.2.2
m_metric = 1
---------- End RouterLSA Link Record ----------
---------- RouterLSA Link Record ----------
m_linkType = 3 (GlobalRoutingLinkRecord::StubNetwork)
m_linkId = a.1.2.1 (Network number of attached network)
m_linkData = ff.ff.ff.0 (Network mask of attached network)
m_metric = 1
---------- End RouterLSA Link Record ----------
---------- RouterLSA Link Record ----------
m_linkType = 3 (GlobalRoutingLinkRecord::StubNetwork)
m_linkId = 7f.0.0.0 (Network number of attached network)
m_linkData = ff.0.0.0 (Network mask of attached network)
m_metric = 1
---------- End RouterLSA Link Record ----------
========== End Global Routing LSA ==========

0s GlobalRouter:GetLinkStateId()
0s GlobalRouteManager:Insert(0.0.0.2, 0x7fcc12c4d420)
0s GlobalRouter:GetLSType()
0s GlobalRouteManager:InitializeRoutes()
0s GlobalRouteManager:InitializeRoutes(): About to start SPF calculation
0s GlobalRouter:GetNumLSAs()
0s GlobalRouter:GetRouterId()
0s GlobalRouteManager:SPFCalculate(0x7fcc12c4ce00, 0.0.0.0)
0s GlobalRouteManager:Initialize()
0s GlobalRouter:SetStatus()
0s GlobalRouter:SetStatus()
0s GlobalRouter:SetStatus()
0s CandidateQueue:CandidateQueue()
0s CandidateQueue:Size()
0s GlobalRouteManager:GetLSA(0.0.0.0)
0s GlobalRouter:GetLinkStateId()
0s GlobalRouteManager:SPFVertex()
0s GlobalRouter:GetLSType()
0s GlobalRouteManager:SPFVertex(): Setting m_vertexType to VertexRouter
0s GlobalRouteManager:SetDistanceFromRoot(0)
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:SetStatus()
0s GlobalRouteManager:SPFCalculate(): Starting SPFCalculate for node 0.0.0.0
0s GlobalRouteManager:CheckForStubNode(0.0.0.0)
0s GlobalRouteManager:GetLSA(0.0.0.0)
0s GlobalRouter:GetLinkStateId()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:SPFNext(0x7fcc12c4d520, 0x7fff6d4024a8)
0s GlobalRouteManager:GetVertexType()
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:GetVertexType()
0s GlobalRouteManager:GetVertexType()
0s GlobalRouteManager:SPFNext(): 0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:GetVertexId()
Examining link 0 of 0.0.0.0's 5 link records
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetLinkId()
0s GlobalRouteManager:GetLSA(0.0.0.1)
0s GlobalRouteManager:SPFNext(): 0s GlobalRouter:GetLinkStateId()
0s GlobalRouteManager:GetVertexId()
Found a P2P record from 0.0.0.0 to 0.0.0.1
0s GlobalRouteManager:GetVertexType()
0s GlobalRouter:GetStatus()
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetLSType()
0s GlobalRouteManager:GetDistanceFromRoot()
0s GlobalRouter:GetMetric()
0s GlobalRouteManager:SPFNext(): 0s GlobalRouter:GetLinkStateId()
Considering w_lsa 0.0.0.1
0s GlobalRouter:GetStatus()
0s GlobalRouter:GetLinkStateId()
0s GlobalRouteManager:SPFVertex()
0s GlobalRouter:GetLSType()
0s GlobalRouteManager:SPFVertex(): Setting m_vertexType to VertexRouter
0s GlobalRouteManager:SPFNexthopCalculation(0x7fcc12c4d520, 0x7fcc12c4d580, 0x7fcc12c4d020, 1)
0s GlobalRouteManager:GetVertexType()
0s GlobalRouteManager:SPFGetNextLink(0x7fcc12c4d580, 0x7fcc12c4d520, 0)
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouteManager:GetVertexId()
0s GlobalRouter:GetLinkId()
0s GlobalRouteManager:SPFGetNextLink(): 0s GlobalRouter:GetLinkData()
0s GlobalRouter:GetLinkId()
Found matching link l:  linkId = 0.0.0.0 linkData = a.1.1.2
0s GlobalRouteManager:SPFGetNextLink(): Returning the found link
0s GlobalRouter:GetLinkData()
0s GlobalRouter:GetLinkData()
0s GlobalRouteManager:FindOutgoingInterfaceId(a.1.1.1, ff.ff.ff.ff)
0s GlobalRouteManager:GetVertexId()
0s GlobalRouter:GetRouterId()
0s Ipv4Interface:GetNAddresses()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401538)
0s Ipv4InterfaceAddress:GetLocal()
0s Ipv4Interface:GetNAddresses()
0s Ipv4Interface:GetNAddresses()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401538)
0s Ipv4InterfaceAddress:GetLocal()
0s GlobalRouteManager:SetRootExitDirection(a.1.1.2, 1)
0s GlobalRouteManager:SetDistanceFromRoot(1)
0s GlobalRouteManager:SetParent(0x7fcc12c4d520)
0s GlobalRouteManager:SPFNexthopCalculation(): 0s GlobalRouteManager:GetVertexId()
0s GlobalRouteManager:GetVertexId()
Next hop from 0.0.0.0 to 0.0.0.1 goes through next hop a.1.1.2 via outgoing interface 1 with distance 1
0s GlobalRouteManager:SetDistanceFromRoot(1)
0s GlobalRouteManager:SetParent(0x7fcc12c4d520)
0s GlobalRouter:SetStatus()
0s CandidateQueue:Push(0x7fff6d4024a8, 0x7fcc12c4d580)
0s GlobalRouteManager:SPFNext(): 0s GlobalRouteManager:GetDistanceFromRoot()
0s GlobalRouteManager:GetVertexId()
0s GlobalRouteManager:GetVertexId()
Pushing 0.0.0.1, parent vertexId: 0.0.0.0, distance: 1
0s GlobalRouteManager:GetVertexType()
0s GlobalRouteManager:SPFNext(): 0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:GetVertexId()
Examining link 1 of 0.0.0.0's 5 link records
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouteManager:SPFNext(): 0s GlobalRouter:GetLinkId()
Found a Stub record to a.1.1.2
0s GlobalRouteManager:GetVertexType()
0s GlobalRouteManager:SPFNext(): 0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:GetVertexId()
Examining link 2 of 0.0.0.0's 5 link records
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetLinkId()
0s GlobalRouteManager:GetLSA(0.0.0.2)
0s GlobalRouteManager:SPFNext(): 0s GlobalRouter:GetLinkStateId()
0s GlobalRouteManager:GetVertexId()
Found a P2P record from 0.0.0.0 to 0.0.0.2
0s GlobalRouteManager:GetVertexType()
0s GlobalRouter:GetStatus()
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetLSType()
0s GlobalRouteManager:GetDistanceFromRoot()
0s GlobalRouter:GetMetric()
0s GlobalRouteManager:SPFNext(): 0s GlobalRouter:GetLinkStateId()
Considering w_lsa 0.0.0.2
0s GlobalRouter:GetStatus()
0s GlobalRouter:GetLinkStateId()
0s GlobalRouteManager:SPFVertex()
0s GlobalRouter:GetLSType()
0s GlobalRouteManager:SPFVertex(): Setting m_vertexType to VertexRouter
0s GlobalRouteManager:SPFNexthopCalculation(0x7fcc12c4d520, 0x7fcc12c4d640, 0x7fcc12c4d080, 1)
0s GlobalRouteManager:GetVertexType()
0s GlobalRouteManager:SPFGetNextLink(0x7fcc12c4d640, 0x7fcc12c4d520, 0)
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouteManager:GetVertexId()
0s GlobalRouter:GetLinkId()
0s GlobalRouteManager:SPFGetNextLink(): 0s GlobalRouter:GetLinkData()
0s GlobalRouter:GetLinkId()
Found matching link l:  linkId = 0.0.0.0 linkData = a.1.2.2
0s GlobalRouteManager:SPFGetNextLink(): Returning the found link
0s GlobalRouter:GetLinkData()
0s GlobalRouter:GetLinkData()
0s GlobalRouteManager:FindOutgoingInterfaceId(a.1.2.1, ff.ff.ff.ff)
0s GlobalRouteManager:GetVertexId()
0s GlobalRouter:GetRouterId()
0s Ipv4Interface:GetNAddresses()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401538)
0s Ipv4InterfaceAddress:GetLocal()
0s Ipv4Interface:GetNAddresses()
0s Ipv4Interface:GetNAddresses()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401538)
0s Ipv4InterfaceAddress:GetLocal()
0s Ipv4Interface:GetNAddresses()
0s Ipv4Interface:GetNAddresses()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401538)
0s Ipv4InterfaceAddress:GetLocal()
0s GlobalRouteManager:SetRootExitDirection(a.1.2.2, 2)
0s GlobalRouteManager:SetDistanceFromRoot(1)
0s GlobalRouteManager:SetParent(0x7fcc12c4d520)
0s GlobalRouteManager:SPFNexthopCalculation(): 0s GlobalRouteManager:GetVertexId()
0s GlobalRouteManager:GetVertexId()
Next hop from 0.0.0.0 to 0.0.0.2 goes through next hop a.1.2.2 via outgoing interface 2 with distance 1
0s GlobalRouteManager:SetDistanceFromRoot(1)
0s GlobalRouteManager:SetParent(0x7fcc12c4d520)
0s GlobalRouter:SetStatus()
0s CandidateQueue:Push(0x7fff6d4024a8, 0x7fcc12c4d640)
0s CandidateQueue:CompareSPFVertex(0x7fff6d4019e8, 0x7fff6d4019e0)
0s GlobalRouteManager:GetDistanceFromRoot()
0s GlobalRouteManager:GetDistanceFromRoot()
0s GlobalRouteManager:GetDistanceFromRoot()
0s GlobalRouteManager:GetDistanceFromRoot()
0s GlobalRouteManager:GetVertexType()
0s GlobalRouteManager:SPFNext(): 0s GlobalRouteManager:GetDistanceFromRoot()
0s GlobalRouteManager:GetVertexId()
0s GlobalRouteManager:GetVertexId()
Pushing 0.0.0.2, parent vertexId: 0.0.0.0, distance: 1
0s GlobalRouteManager:GetVertexType()
0s GlobalRouteManager:SPFNext(): 0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:GetVertexId()
Examining link 3 of 0.0.0.0's 5 link records
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouteManager:SPFNext(): 0s GlobalRouter:GetLinkId()
Found a Stub record to a.1.2.2
0s GlobalRouteManager:GetVertexType()
0s GlobalRouteManager:SPFNext(): 0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:GetVertexId()
Examining link 4 of 0.0.0.0's 5 link records
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouteManager:SPFNext(): 0s GlobalRouter:GetLinkId()
Found a Stub record to 7f.0.0.0
0s CandidateQueue:Size()
0s GlobalRouteManager:SPFCalculate(): *** CandidateQueue Begin (<id, distance, LSA-type>) ***
0s GlobalRouteManager:GetVertexType()
0s GlobalRouteManager:GetDistanceFromRoot()
0s GlobalRouteManager:GetVertexId()
<0.0.0.1, 1, router>
0s GlobalRouteManager:GetVertexType()
0s GlobalRouteManager:GetDistanceFromRoot()
0s GlobalRouteManager:GetVertexId()
<0.0.0.2, 1, router>
*** CandidateQueue End ***
0s CandidateQueue:Pop()
0s GlobalRouteManager:SPFCalculate(): 0s GlobalRouteManager:GetVertexId()
Popped vertex 0.0.0.1
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:SetStatus()
0s GlobalRouteManager:SPFVertexAddParent(0x7fcc12c4d580)
0s GlobalRouteManager:GetParent()
0s GlobalRouteManager:AddChild(0x7fcc12c4d580)
0s GlobalRouteManager:GetParent()
0s GlobalRouteManager:GetParent(): Index to SPFVertex's parent is out-of-range.
0s GlobalRouteManager:GetVertexType()
0s GlobalRouteManager:SPFIntraAddRouter(0x7fcc12c4d580)
0s GlobalRouteManager:GetVertexId()
0s GlobalRouteManager:SPFIntraAddRouter(): Vertex ID = 0.0.0.0
0s GlobalRouteManager:SPFIntraAddRouter(): 0s GlobalRouter:GetRouterId()
Considering router 0.0.0.0
0s GlobalRouter:GetRouterId()
0s GlobalRouteManager:SPFIntraAddRouter(): Setting routes for node 0
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:SPFIntraAddRouter(): 0s GlobalRouter:GetLinkStateId()
 Node 0 found 3 link records in LSA 0x7fcc12c4d230with LinkStateId 0.0.0.1
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouteManager:GetNRootExitDirections()
0s GlobalRouteManager:GetRootExitDirection(0)
0s GlobalRouter:GetLinkData()
0s Ipv4GlobalRouting:AddHostRouteTo(a.1.1.2, a.1.1.2, 1)
0s GlobalRouteManager:SPFIntraAddRouter(): 0s GlobalRouter:GetLinkData()
(Route 0) Node 0 adding host route to a.1.1.2 using next hop a.1.1.2 and outgoing interface 1
0s GlobalRouteManager:GetNRootExitDirections()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouteManager:SPFNext(0x7fcc12c4d580, 0x7fff6d4024a8)
0s GlobalRouteManager:GetVertexType()
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:GetVertexType()
0s GlobalRouteManager:GetVertexType()
0s GlobalRouteManager:SPFNext(): 0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:GetVertexId()
Examining link 0 of 0.0.0.1's 3 link records
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetLinkId()
0s GlobalRouteManager:GetLSA(0.0.0.0)
0s GlobalRouteManager:SPFNext(): 0s GlobalRouter:GetLinkStateId()
0s GlobalRouteManager:GetVertexId()
Found a P2P record from 0.0.0.1 to 0.0.0.0
0s GlobalRouteManager:GetVertexType()
0s GlobalRouter:GetStatus()
0s GlobalRouteManager:SPFNext(): 0s GlobalRouter:GetLinkStateId()
Skipping ->  LSA 0.0.0.0 already in SPF tree
0s GlobalRouteManager:GetVertexType()
0s GlobalRouteManager:SPFNext(): 0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:GetVertexId()
Examining link 1 of 0.0.0.1's 3 link records
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouteManager:SPFNext(): 0s GlobalRouter:GetLinkId()
Found a Stub record to a.1.1.1
0s GlobalRouteManager:GetVertexType()
0s GlobalRouteManager:SPFNext(): 0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:GetVertexId()
Examining link 2 of 0.0.0.1's 3 link records
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouteManager:SPFNext(): 0s GlobalRouter:GetLinkId()
Found a Stub record to 7f.0.0.0
0s CandidateQueue:Size()
0s GlobalRouteManager:SPFCalculate(): *** CandidateQueue Begin (<id, distance, LSA-type>) ***
0s GlobalRouteManager:GetVertexType()
0s GlobalRouteManager:GetDistanceFromRoot()
0s GlobalRouteManager:GetVertexId()
<0.0.0.2, 1, router>
*** CandidateQueue End ***
0s CandidateQueue:Pop()
0s GlobalRouteManager:SPFCalculate(): 0s GlobalRouteManager:GetVertexId()
Popped vertex 0.0.0.2
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:SetStatus()
0s GlobalRouteManager:SPFVertexAddParent(0x7fcc12c4d640)
0s GlobalRouteManager:GetParent()
0s GlobalRouteManager:AddChild(0x7fcc12c4d640)
0s GlobalRouteManager:GetParent()
0s GlobalRouteManager:GetParent(): Index to SPFVertex's parent is out-of-range.
0s GlobalRouteManager:GetVertexType()
0s GlobalRouteManager:SPFIntraAddRouter(0x7fcc12c4d640)
0s GlobalRouteManager:GetVertexId()
0s GlobalRouteManager:SPFIntraAddRouter(): Vertex ID = 0.0.0.0
0s GlobalRouteManager:SPFIntraAddRouter(): 0s GlobalRouter:GetRouterId()
Considering router 0.0.0.0
0s GlobalRouter:GetRouterId()
0s GlobalRouteManager:SPFIntraAddRouter(): Setting routes for node 0
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:SPFIntraAddRouter(): 0s GlobalRouter:GetLinkStateId()
 Node 0 found 3 link records in LSA 0x7fcc12c4d420with LinkStateId 0.0.0.2
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouteManager:GetNRootExitDirections()
0s GlobalRouteManager:GetRootExitDirection(0)
0s GlobalRouter:GetLinkData()
0s Ipv4GlobalRouting:AddHostRouteTo(a.1.2.2, a.1.2.2, 2)
0s GlobalRouteManager:SPFIntraAddRouter(): 0s GlobalRouter:GetLinkData()
(Route 0) Node 0 adding host route to a.1.2.2 using next hop a.1.2.2 and outgoing interface 2
0s GlobalRouteManager:GetNRootExitDirections()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouteManager:SPFNext(0x7fcc12c4d640, 0x7fff6d4024a8)
0s GlobalRouteManager:GetVertexType()
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:GetVertexType()
0s GlobalRouteManager:GetVertexType()
0s GlobalRouteManager:SPFNext(): 0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:GetVertexId()
Examining link 0 of 0.0.0.2's 3 link records
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetLinkId()
0s GlobalRouteManager:GetLSA(0.0.0.0)
0s GlobalRouteManager:SPFNext(): 0s GlobalRouter:GetLinkStateId()
0s GlobalRouteManager:GetVertexId()
Found a P2P record from 0.0.0.2 to 0.0.0.0
0s GlobalRouteManager:GetVertexType()
0s GlobalRouter:GetStatus()
0s GlobalRouteManager:SPFNext(): 0s GlobalRouter:GetLinkStateId()
Skipping ->  LSA 0.0.0.0 already in SPF tree
0s GlobalRouteManager:GetVertexType()
0s GlobalRouteManager:SPFNext(): 0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:GetVertexId()
Examining link 1 of 0.0.0.2's 3 link records
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouteManager:SPFNext(): 0s GlobalRouter:GetLinkId()
Found a Stub record to a.1.2.1
0s GlobalRouteManager:GetVertexType()
0s GlobalRouteManager:SPFNext(): 0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:GetVertexId()
Examining link 2 of 0.0.0.2's 3 link records
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouteManager:SPFNext(): 0s GlobalRouter:GetLinkId()
Found a Stub record to 7f.0.0.0
0s CandidateQueue:Size()
0s GlobalRouteManager:SPFProcessStubs()
0s GlobalRouteManager:SPFProcessStubs(): 0s GlobalRouteManager:GetVertexId()
Processing stubs for 0.0.0.0
0s GlobalRouteManager:GetVertexType()
0s GlobalRouteManager:GetLSA()
0s GlobalRouteManager:SPFProcessStubs(): 0s GlobalRouter:GetLinkStateId()
Processing router LSA with id 0.0.0.0
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:SPFProcessStubs(): 0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:GetVertexId()
Examining link 0 of 0.0.0.0's 5 link records
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:SPFProcessStubs(): 0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:GetVertexId()
Examining link 1 of 0.0.0.0's 5 link records
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouteManager:SPFProcessStubs(): 0s GlobalRouter:GetLinkId()
Found a Stub record to a.1.1.2
0s GlobalRouteManager:SPFIntraAddStub()
0s GlobalRouteManager:GetVertexId()
0s GlobalRouteManager:GetVertexId()
0s GlobalRouteManager:SPFIntraAddStub(): 0s GlobalRouteManager:GetVertexId()
Stub is on local host: 0.0.0.0; returning
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:SPFProcessStubs(): 0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:GetVertexId()
Examining link 2 of 0.0.0.0's 5 link records
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:SPFProcessStubs(): 0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:GetVertexId()
Examining link 3 of 0.0.0.0's 5 link records
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouteManager:SPFProcessStubs(): 0s GlobalRouter:GetLinkId()
Found a Stub record to a.1.2.2
0s GlobalRouteManager:SPFIntraAddStub()
0s GlobalRouteManager:GetVertexId()
0s GlobalRouteManager:GetVertexId()
0s GlobalRouteManager:SPFIntraAddStub(): 0s GlobalRouteManager:GetVertexId()
Stub is on local host: 0.0.0.0; returning
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:SPFProcessStubs(): 0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:GetVertexId()
Examining link 4 of 0.0.0.0's 5 link records
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouteManager:SPFProcessStubs(): 0s GlobalRouter:GetLinkId()
Found a Stub record to 7f.0.0.0
0s GlobalRouteManager:SPFIntraAddStub()
0s GlobalRouteManager:GetVertexId()
0s GlobalRouteManager:GetVertexId()
0s GlobalRouteManager:SPFIntraAddStub(): 0s GlobalRouteManager:GetVertexId()
Stub is on local host: 0.0.0.0; returning
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:GetNChildren()
0s GlobalRouteManager:GetChild(0)
0s GlobalRouteManager:GetChild(0)
0s GlobalRouteManager:SPFProcessStubs()
0s GlobalRouteManager:SPFProcessStubs(): 0s GlobalRouteManager:GetVertexId()
Processing stubs for 0.0.0.1
0s GlobalRouteManager:GetVertexType()
0s GlobalRouteManager:GetLSA()
0s GlobalRouteManager:SPFProcessStubs(): 0s GlobalRouter:GetLinkStateId()
Processing router LSA with id 0.0.0.1
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:SPFProcessStubs(): 0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:GetVertexId()
Examining link 0 of 0.0.0.1's 3 link records
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:SPFProcessStubs(): 0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:GetVertexId()
Examining link 1 of 0.0.0.1's 3 link records
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouteManager:SPFProcessStubs(): 0s GlobalRouter:GetLinkId()
Found a Stub record to a.1.1.1
0s GlobalRouteManager:SPFIntraAddStub()
0s GlobalRouteManager:GetVertexId()
0s GlobalRouteManager:GetVertexId()
0s GlobalRouteManager:SPFIntraAddStub(): 0s GlobalRouteManager:GetVertexId()
Stub is on remote host: 0.0.0.1; installing
0s GlobalRouteManager:GetVertexId()
0s GlobalRouteManager:SPFIntraAddStub(): Vertex ID = 0.0.0.0
0s GlobalRouteManager:SPFIntraAddStub(): 0s GlobalRouter:GetRouterId()
Considering router 0.0.0.0
0s GlobalRouter:GetRouterId()
0s GlobalRouteManager:SPFIntraAddStub(): Setting routes for node 0
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetLinkData()
0s GlobalRouter:GetLinkId()
0s GlobalRouteManager:GetNRootExitDirections()
0s GlobalRouteManager:GetRootExitDirection(0)
0s Ipv4GlobalRouting:AddNetworkRouteTo(a.1.1.0, ff.ff.ff.0, a.1.1.2, 1)
0s GlobalRouteManager:SPFIntraAddStub(): (Route 0) Node 0 add network route to a.1.1.0 using next hop a.1.1.2 via interface 1
0s GlobalRouteManager:GetNRootExitDirections()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:SPFProcessStubs(): 0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:GetVertexId()
Examining link 2 of 0.0.0.1's 3 link records
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouteManager:SPFProcessStubs(): 0s GlobalRouter:GetLinkId()
Found a Stub record to 7f.0.0.0
0s GlobalRouteManager:SPFIntraAddStub()
0s GlobalRouteManager:GetVertexId()
0s GlobalRouteManager:GetVertexId()
0s GlobalRouteManager:SPFIntraAddStub(): 0s GlobalRouteManager:GetVertexId()
Stub is on remote host: 0.0.0.1; installing
0s GlobalRouteManager:GetVertexId()
0s GlobalRouteManager:SPFIntraAddStub(): Vertex ID = 0.0.0.0
0s GlobalRouteManager:SPFIntraAddStub(): 0s GlobalRouter:GetRouterId()
Considering router 0.0.0.0
0s GlobalRouter:GetRouterId()
0s GlobalRouteManager:SPFIntraAddStub(): Setting routes for node 0
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetLinkData()
0s GlobalRouter:GetLinkId()
0s GlobalRouteManager:GetNRootExitDirections()
0s GlobalRouteManager:GetRootExitDirection(0)
0s Ipv4GlobalRouting:AddNetworkRouteTo(7f.0.0.0, ff.0.0.0, a.1.1.2, 1)
0s GlobalRouteManager:SPFIntraAddStub(): (Route 0) Node 0 add network route to 7f.0.0.0 using next hop a.1.1.2 via interface 1
0s GlobalRouteManager:GetNRootExitDirections()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:GetNChildren()
0s GlobalRouteManager:GetChild(0)
0s GlobalRouteManager:GetNChildren()
0s GlobalRouteManager:GetChild(1)
0s GlobalRouteManager:GetChild(1)
0s GlobalRouteManager:SPFProcessStubs()
0s GlobalRouteManager:SPFProcessStubs(): 0s GlobalRouteManager:GetVertexId()
Processing stubs for 0.0.0.2
0s GlobalRouteManager:GetVertexType()
0s GlobalRouteManager:GetLSA()
0s GlobalRouteManager:SPFProcessStubs(): 0s GlobalRouter:GetLinkStateId()
Processing router LSA with id 0.0.0.2
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:SPFProcessStubs(): 0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:GetVertexId()
Examining link 0 of 0.0.0.2's 3 link records
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:SPFProcessStubs(): 0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:GetVertexId()
Examining link 1 of 0.0.0.2's 3 link records
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouteManager:SPFProcessStubs(): 0s GlobalRouter:GetLinkId()
Found a Stub record to a.1.2.1
0s GlobalRouteManager:SPFIntraAddStub()
0s GlobalRouteManager:GetVertexId()
0s GlobalRouteManager:GetVertexId()
0s GlobalRouteManager:SPFIntraAddStub(): 0s GlobalRouteManager:GetVertexId()
Stub is on remote host: 0.0.0.2; installing
0s GlobalRouteManager:GetVertexId()
0s GlobalRouteManager:SPFIntraAddStub(): Vertex ID = 0.0.0.0
0s GlobalRouteManager:SPFIntraAddStub(): 0s GlobalRouter:GetRouterId()
Considering router 0.0.0.0
0s GlobalRouter:GetRouterId()
0s GlobalRouteManager:SPFIntraAddStub(): Setting routes for node 0
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetLinkData()
0s GlobalRouter:GetLinkId()
0s GlobalRouteManager:GetNRootExitDirections()
0s GlobalRouteManager:GetRootExitDirection(0)
0s Ipv4GlobalRouting:AddNetworkRouteTo(a.1.2.0, ff.ff.ff.0, a.1.2.2, 2)
0s GlobalRouteManager:SPFIntraAddStub(): (Route 0) Node 0 add network route to a.1.2.0 using next hop a.1.2.2 via interface 2
0s GlobalRouteManager:GetNRootExitDirections()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:SPFProcessStubs(): 0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:GetVertexId()
Examining link 2 of 0.0.0.2's 3 link records
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouteManager:SPFProcessStubs(): 0s GlobalRouter:GetLinkId()
Found a Stub record to 7f.0.0.0
0s GlobalRouteManager:SPFIntraAddStub()
0s GlobalRouteManager:GetVertexId()
0s GlobalRouteManager:GetVertexId()
0s GlobalRouteManager:SPFIntraAddStub(): 0s GlobalRouteManager:GetVertexId()
Stub is on remote host: 0.0.0.2; installing
0s GlobalRouteManager:GetVertexId()
0s GlobalRouteManager:SPFIntraAddStub(): Vertex ID = 0.0.0.0
0s GlobalRouteManager:SPFIntraAddStub(): 0s GlobalRouter:GetRouterId()
Considering router 0.0.0.0
0s GlobalRouter:GetRouterId()
0s GlobalRouteManager:SPFIntraAddStub(): Setting routes for node 0
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:GetLinkData()
0s GlobalRouter:GetLinkId()
0s GlobalRouteManager:GetNRootExitDirections()
0s GlobalRouteManager:GetRootExitDirection(0)
0s Ipv4GlobalRouting:AddNetworkRouteTo(7f.0.0.0, ff.0.0.0, a.1.2.2, 2)
0s GlobalRouteManager:SPFIntraAddStub(): (Route 0) Node 0 add network route to 7f.0.0.0 using next hop a.1.2.2 via interface 2
0s GlobalRouteManager:GetNRootExitDirections()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouteManager:GetNChildren()
0s GlobalRouteManager:GetChild(1)
0s GlobalRouteManager:GetNChildren()
0s GlobalRouteManager:~SPFVertex(0.0.0.0)
0s GlobalRouteManager:~SPFVertex(): Children vertices - {0.0.0.1, 0.0.0.2}
0s GlobalRouteManager:~SPFVertex(): Parent verteices - {}
0s GlobalRouteManager:~SPFVertex(): 0s GlobalRouteManager:GetVertexId()
Parent vertex-0.0.0.0 deleting its child vertex-0.0.0.1
0s GlobalRouteManager:~SPFVertex(0.0.0.1)
0s GlobalRouteManager:~SPFVertex(): Children vertices - {}
0s GlobalRouteManager:~SPFVertex(): Parent verteices - {0.0.0.0}
0s GlobalRouteManager:~SPFVertex(): Vertex-0.0.0.1 completed deleted
0s GlobalRouteManager:~SPFVertex(): 0s GlobalRouteManager:GetVertexId()
Parent vertex-0.0.0.0 deleting its child vertex-0.0.0.2
0s GlobalRouteManager:~SPFVertex(0.0.0.2)
0s GlobalRouteManager:~SPFVertex(): Children vertices - {}
0s GlobalRouteManager:~SPFVertex(): Parent verteices - {0.0.0.0}
0s GlobalRouteManager:~SPFVertex(): Vertex-0.0.0.2 completed deleted
0s GlobalRouteManager:~SPFVertex(): Vertex-0.0.0.0 completed deleted
0s CandidateQueue:~CandidateQueue()
0s CandidateQueue:Clear()
0s GlobalRouter:GetNumLSAs()
0s GlobalRouter:GetRouterId()
0s GlobalRouteManager:SPFCalculate(0x7fcc12c4ce00, 0.0.0.1)
0s GlobalRouteManager:Initialize()
0s GlobalRouter:SetStatus()
0s GlobalRouter:SetStatus()
0s GlobalRouter:SetStatus()
0s CandidateQueue:CandidateQueue()
0s CandidateQueue:Size()
0s GlobalRouteManager:GetLSA(0.0.0.1)
0s GlobalRouter:GetLinkStateId()
0s GlobalRouteManager:SPFVertex()
0s GlobalRouter:GetLSType()
0s GlobalRouteManager:SPFVertex(): Setting m_vertexType to VertexRouter
0s GlobalRouteManager:SetDistanceFromRoot(0)
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:SetStatus()
0s GlobalRouteManager:SPFCalculate(): Starting SPFCalculate for node 0.0.0.1
0s GlobalRouteManager:CheckForStubNode(0.0.0.1)
0s GlobalRouteManager:GetLSA(0.0.0.1)
0s GlobalRouter:GetLinkStateId()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetLinkId()
0s GlobalRouteManager:GetLSA(0.0.0.0)
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetLinkId()
0s GlobalRouter:GetNode()
0s GlobalRouter:GetLinkData()
0s GlobalRouteManager:FindOutgoingInterfaceId(a.1.1.2, ff.ff.ff.ff)
0s GlobalRouteManager:GetVertexId()
0s GlobalRouter:GetRouterId()
0s GlobalRouter:GetRouterId()
0s Ipv4Interface:GetNAddresses()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401a18)
0s Ipv4InterfaceAddress:GetLocal()
0s Ipv4Interface:GetNAddresses()
0s Ipv4Interface:GetNAddresses()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401a18)
0s Ipv4InterfaceAddress:GetLocal()
0s GlobalRouter:GetLinkData()
0s Ipv4GlobalRouting:AddNetworkRouteTo(0.0.0.0, 0.0.0.0, a.1.1.1, 1)
0s GlobalRouteManager:CheckForStubNode(): 0s GlobalRouter:GetLinkData()
0s GlobalRouteManager:FindOutgoingInterfaceId(a.1.1.2, ff.ff.ff.ff)
0s GlobalRouteManager:GetVertexId()
0s GlobalRouter:GetRouterId()
0s GlobalRouter:GetRouterId()
0s Ipv4Interface:GetNAddresses()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401a18)
0s Ipv4InterfaceAddress:GetLocal()
0s Ipv4Interface:GetNAddresses()
0s Ipv4Interface:GetNAddresses()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401a18)
0s Ipv4InterfaceAddress:GetLocal()
0s GlobalRouter:GetLinkData()
Inserting default route for node 0.0.0.1 to next hop a.1.1.1 via interface 1
0s GlobalRouteManager:SPFCalculate(): SPFCalculate truncated for stub node 0.0.0.1
0s GlobalRouteManager:~SPFVertex(0.0.0.1)
0s GlobalRouteManager:~SPFVertex(): Children vertices - {}
0s GlobalRouteManager:~SPFVertex(): Parent verteices - {}
0s GlobalRouteManager:~SPFVertex(): Vertex-0.0.0.1 completed deleted
0s CandidateQueue:~CandidateQueue()
0s CandidateQueue:Clear()
0s GlobalRouter:GetNumLSAs()
0s GlobalRouter:GetRouterId()
0s GlobalRouteManager:SPFCalculate(0x7fcc12c4ce00, 0.0.0.2)
0s GlobalRouteManager:Initialize()
0s GlobalRouter:SetStatus()
0s GlobalRouter:SetStatus()
0s GlobalRouter:SetStatus()
0s CandidateQueue:CandidateQueue()
0s CandidateQueue:Size()
0s GlobalRouteManager:GetLSA(0.0.0.2)
0s GlobalRouter:GetLinkStateId()
0s GlobalRouteManager:SPFVertex()
0s GlobalRouter:GetLSType()
0s GlobalRouteManager:SPFVertex(): Setting m_vertexType to VertexRouter
0s GlobalRouteManager:SetDistanceFromRoot(0)
0s GlobalRouteManager:GetLSA()
0s GlobalRouter:SetStatus()
0s GlobalRouteManager:SPFCalculate(): Starting SPFCalculate for node 0.0.0.2
0s GlobalRouteManager:CheckForStubNode(0.0.0.2)
0s GlobalRouteManager:GetLSA(0.0.0.2)
0s GlobalRouter:GetLinkStateId()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetLinkId()
0s GlobalRouteManager:GetLSA(0.0.0.0)
0s GlobalRouter:GetNLinkRecords()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetLinkId()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetLinkRecord()
0s GlobalRouter:GetLinkType()
0s GlobalRouter:GetLinkId()
0s GlobalRouter:GetNode()
0s GlobalRouter:GetLinkData()
0s GlobalRouteManager:FindOutgoingInterfaceId(a.1.2.2, ff.ff.ff.ff)
0s GlobalRouteManager:GetVertexId()
0s GlobalRouter:GetRouterId()
0s GlobalRouter:GetRouterId()
0s GlobalRouter:GetRouterId()
0s Ipv4Interface:GetNAddresses()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401a18)
0s Ipv4InterfaceAddress:GetLocal()
0s Ipv4Interface:GetNAddresses()
0s Ipv4Interface:GetNAddresses()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401a18)
0s Ipv4InterfaceAddress:GetLocal()
0s GlobalRouter:GetLinkData()
0s Ipv4GlobalRouting:AddNetworkRouteTo(0.0.0.0, 0.0.0.0, a.1.2.1, 1)
0s GlobalRouteManager:CheckForStubNode(): 0s GlobalRouter:GetLinkData()
0s GlobalRouteManager:FindOutgoingInterfaceId(a.1.2.2, ff.ff.ff.ff)
0s GlobalRouteManager:GetVertexId()
0s GlobalRouter:GetRouterId()
0s GlobalRouter:GetRouterId()
0s GlobalRouter:GetRouterId()
0s Ipv4Interface:GetNAddresses()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401a18)
0s Ipv4InterfaceAddress:GetLocal()
0s Ipv4Interface:GetNAddresses()
0s Ipv4Interface:GetNAddresses()
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d401a18)
0s Ipv4InterfaceAddress:GetLocal()
0s GlobalRouter:GetLinkData()
Inserting default route for node 0.0.0.2 to next hop a.1.2.1 via interface 1
0s GlobalRouteManager:SPFCalculate(): SPFCalculate truncated for stub node 0.0.0.2
0s GlobalRouteManager:~SPFVertex(0.0.0.2)
0s GlobalRouteManager:~SPFVertex(): Children vertices - {}
0s GlobalRouteManager:~SPFVertex(): Parent verteices - {}
0s GlobalRouteManager:~SPFVertex(): Vertex-0.0.0.2 completed deleted
0s CandidateQueue:~CandidateQueue()
0s CandidateQueue:Clear()
0s GlobalRouteManager:InitializeRoutes(): Finished SPF calculation
```

These large piece is due to

    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

It runs a routing protocol to populate routing tables to all the nodes. It does no periodic learning, so it is called the static global routing. The routing tables are built with link state advertisements.

## Applications
Install sink application (line 2894-2908):
```
0s VerySimple:main(): Create applications.
0s PacketSink:PacketSink(0x7fcc12c4d640)
0s ObjectBase:ConstructSelf(): construct tid=ns3::PacketSink, params=2
0s ObjectBase:ConstructSelf(): try to construct "ns3::PacketSink::Local"
0s ObjectBase:ConstructSelf(): construct "ns3::PacketSink::Local"
0s ObjectBase:ConstructSelf(): try to construct "ns3::PacketSink::Protocol"
0s ObjectBase:ConstructSelf(): construct "ns3::PacketSink::Protocol"
0s ObjectBase:ConstructSelf(): construct tid=ns3::Application, params=2
0s ObjectBase:ConstructSelf(): try to construct "ns3::Application::StartTime"
0s ObjectBase:ConstructSelf(): construct "ns3::Application::StartTime" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::Application::StopTime"
0s ObjectBase:ConstructSelf(): construct "ns3::Application::StopTime" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s DefaultSimulatorImpl:ScheduleWithContext(0x7fcc12c479b0, 1, 0, 0, 0x7fcc12c4d5e0)
0s MapScheduler:Insert(0x7fcc12c47a10, 0x7fcc12c4d5e0, 0, 12)
```

    ApplicationContainer hubApp = packetSinkHelper.Install (star.GetSpokeNode (0));

Sender's destination address (line 2909-2911):
```
0s Ipv4Interface:GetAddress()
0s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d402780)
0s Ipv4InterfaceAddress:GetLocal()
```

    AddressValue remoteAddress (InetSocketAddress (star.GetSpokeIpv4Address (0), port));

Install sender application (line 2912-2935):
```
0s OnOffApplication:OnOffApplication()
0s ObjectBase:ConstructSelf(): construct tid=ns3::OnOffApplication, params=7
0s ObjectBase:ConstructSelf(): try to construct "ns3::OnOffApplication::DataRate"
0s ObjectBase:ConstructSelf(): construct "ns3::OnOffApplication::DataRate" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::OnOffApplication::PacketSize"
0s ObjectBase:ConstructSelf(): construct "ns3::OnOffApplication::PacketSize" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::OnOffApplication::Remote"
0s ObjectBase:ConstructSelf(): construct "ns3::OnOffApplication::Remote"
0s ObjectBase:ConstructSelf(): try to construct "ns3::OnOffApplication::OnTime"
0s ObjectBase:ConstructSelf(): construct "ns3::OnOffApplication::OnTime"
0s ObjectBase:ConstructSelf(): try to construct "ns3::OnOffApplication::OffTime"
0s ObjectBase:ConstructSelf(): construct "ns3::OnOffApplication::OffTime"
0s ObjectBase:ConstructSelf(): try to construct "ns3::OnOffApplication::MaxBytes"
0s ObjectBase:ConstructSelf(): construct "ns3::OnOffApplication::MaxBytes"
0s ObjectBase:ConstructSelf(): try to construct "ns3::OnOffApplication::Protocol"
0s ObjectBase:ConstructSelf(): construct "ns3::OnOffApplication::Protocol"
0s ObjectBase:ConstructSelf(): construct tid=ns3::Application, params=2
0s ObjectBase:ConstructSelf(): try to construct "ns3::Application::StartTime"
0s ObjectBase:ConstructSelf(): construct "ns3::Application::StartTime" from initial value.
0s ObjectBase:ConstructSelf(): try to construct "ns3::Application::StopTime"
0s ObjectBase:ConstructSelf(): construct "ns3::Application::StopTime" from initial value.
0s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
0s DefaultSimulatorImpl:ScheduleWithContext(0x7fcc12c479b0, 2, 0, 0, 0x7fcc12c4d9c0)
0s MapScheduler:Insert(0x7fcc12c47a10, 0x7fcc12c4d9c0, 0, 13)
```

    spokeApps.Add (onOffHelper.Install (star.GetSpokeNode (1)));

## Packet trace

Turn on packet trace (line 2936-3765):

```
0s Packet:EnablePrinting()
0s Config:ParsePath(/NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/MacRx, /NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice, MacRx)
0s Config:LookupMatches(/NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice)
0s Config:DoResolve(/NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/, 0x7fcc12c47970)
0s Names:NamesPriv()
0s Names:Find(0x7fcc12c47970, NodeList)
0s Names:IsNamed(0x7fcc12c47970)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=NodeList on path=/
0s Config:Matches(): Array 0 matches 0
0s Config:DoResolve(/DeviceList/0/$ns3::PointToPointNetDevice/, 0x7fcc12c478e0)
0s Names:Find(0x7fcc12c478e0, DeviceList)
0s Names:IsNamed(0x7fcc12c478e0)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=DeviceList on path=/NodeList/0/
0s Config:Matches(): Array 0 matches 0
0s Config:DoResolve(/$ns3::PointToPointNetDevice/, 0x7fcc12c47da0)
0s Names:Find(0x7fcc12c47da0, $ns3::PointToPointNetDevice)
0s Names:IsNamed(0x7fcc12c47da0)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetObject=ns3::PointToPointNetDevice on path=/NodeList/0/DeviceList/0/
0s Config:DoResolve(/, 0x7fcc12c47da0)
0s Config:DoResolveOne(): resolved=/NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/
0s Config:Matches(): Array 1 does not match 0
0s Config:Matches(): Array 2 does not match 0
0s Config:Matches(): Array 1 does not match 0
0s Config:Matches(): Array 2 does not match 0
0s Config:DoResolve(/NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/, 0x7fcc12c48270)
0s Names:Find(0x7fcc12c48270, NodeList)
0s Names:IsNamed(0x7fcc12c48270)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): Requested item=NodeList does not exist on path=/
0s Config:DoResolve(/NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/, 0)
0s Names:Find(0, NodeList)
0s Names:Find(): Zero context implies root NameNode
0s Names:Find(): Name does not exist in name map
0s Config:ParsePath(/NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/Enqueue, /NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue, Enqueue)
0s Config:LookupMatches(/NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue)
0s Config:DoResolve(/NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c47970)
0s Names:Find(0x7fcc12c47970, NodeList)
0s Names:IsNamed(0x7fcc12c47970)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=NodeList on path=/
0s Config:Matches(): Array 0 matches 0
0s Config:DoResolve(/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c478e0)
0s Names:Find(0x7fcc12c478e0, DeviceList)
0s Names:IsNamed(0x7fcc12c478e0)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=DeviceList on path=/NodeList/0/
0s Config:Matches(): Array 0 matches 0
0s Config:DoResolve(/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c47da0)
0s Names:Find(0x7fcc12c47da0, $ns3::PointToPointNetDevice)
0s Names:IsNamed(0x7fcc12c47da0)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetObject=ns3::PointToPointNetDevice on path=/NodeList/0/DeviceList/0/
0s Config:DoResolve(/TxQueue/, 0x7fcc12c47da0)
0s Names:Find(0x7fcc12c47da0, TxQueue)
0s Names:IsNamed(0x7fcc12c47da0)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(ptr)=TxQueue on path=/NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/
0s Config:DoResolve(/, 0x7fcc12c47fa0)
0s Config:DoResolveOne(): resolved=/NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/
0s Config:Matches(): Array 1 does not match 0
0s Config:Matches(): Array 2 does not match 0
0s Config:Matches(): Array 1 does not match 0
0s Config:Matches(): Array 2 does not match 0
0s Config:DoResolve(/NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c48270)
0s Names:Find(0x7fcc12c48270, NodeList)
0s Names:IsNamed(0x7fcc12c48270)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): Requested item=NodeList does not exist on path=/
0s Config:DoResolve(/NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/, 0)
0s Names:Find(0, NodeList)
0s Names:Find(): Zero context implies root NameNode
0s Names:Find(): Name does not exist in name map
0s Config:ParsePath(/NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/Dequeue, /NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue, Dequeue)
0s Config:LookupMatches(/NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue)
0s Config:DoResolve(/NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c47970)
0s Names:Find(0x7fcc12c47970, NodeList)
0s Names:IsNamed(0x7fcc12c47970)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=NodeList on path=/
0s Config:Matches(): Array 0 matches 0
0s Config:DoResolve(/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c478e0)
0s Names:Find(0x7fcc12c478e0, DeviceList)
0s Names:IsNamed(0x7fcc12c478e0)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=DeviceList on path=/NodeList/0/
0s Config:Matches(): Array 0 matches 0
0s Config:DoResolve(/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c47da0)
0s Names:Find(0x7fcc12c47da0, $ns3::PointToPointNetDevice)
0s Names:IsNamed(0x7fcc12c47da0)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetObject=ns3::PointToPointNetDevice on path=/NodeList/0/DeviceList/0/
0s Config:DoResolve(/TxQueue/, 0x7fcc12c47da0)
0s Names:Find(0x7fcc12c47da0, TxQueue)
0s Names:IsNamed(0x7fcc12c47da0)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(ptr)=TxQueue on path=/NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/
0s Config:DoResolve(/, 0x7fcc12c47fa0)
0s Config:DoResolveOne(): resolved=/NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/
0s Config:Matches(): Array 1 does not match 0
0s Config:Matches(): Array 2 does not match 0
0s Config:Matches(): Array 1 does not match 0
0s Config:Matches(): Array 2 does not match 0
0s Config:DoResolve(/NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c48270)
0s Names:Find(0x7fcc12c48270, NodeList)
0s Names:IsNamed(0x7fcc12c48270)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): Requested item=NodeList does not exist on path=/
0s Config:DoResolve(/NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/, 0)
0s Names:Find(0, NodeList)
0s Names:Find(): Zero context implies root NameNode
0s Names:Find(): Name does not exist in name map
0s Config:ParsePath(/NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/Drop, /NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue, Drop)
0s Config:LookupMatches(/NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue)
0s Config:DoResolve(/NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c47970)
0s Names:Find(0x7fcc12c47970, NodeList)
0s Names:IsNamed(0x7fcc12c47970)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=NodeList on path=/
0s Config:Matches(): Array 0 matches 0
0s Config:DoResolve(/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c478e0)
0s Names:Find(0x7fcc12c478e0, DeviceList)
0s Names:IsNamed(0x7fcc12c478e0)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=DeviceList on path=/NodeList/0/
0s Config:Matches(): Array 0 matches 0
0s Config:DoResolve(/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c47da0)
0s Names:Find(0x7fcc12c47da0, $ns3::PointToPointNetDevice)
0s Names:IsNamed(0x7fcc12c47da0)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetObject=ns3::PointToPointNetDevice on path=/NodeList/0/DeviceList/0/
0s Config:DoResolve(/TxQueue/, 0x7fcc12c47da0)
0s Names:Find(0x7fcc12c47da0, TxQueue)
0s Names:IsNamed(0x7fcc12c47da0)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(ptr)=TxQueue on path=/NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/
0s Config:DoResolve(/, 0x7fcc12c47fa0)
0s Config:DoResolveOne(): resolved=/NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/
0s Config:Matches(): Array 1 does not match 0
0s Config:Matches(): Array 2 does not match 0
0s Config:Matches(): Array 1 does not match 0
0s Config:Matches(): Array 2 does not match 0
0s Config:DoResolve(/NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c48270)
0s Names:Find(0x7fcc12c48270, NodeList)
0s Names:IsNamed(0x7fcc12c48270)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): Requested item=NodeList does not exist on path=/
0s Config:DoResolve(/NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/, 0)
0s Names:Find(0, NodeList)
0s Names:Find(): Zero context implies root NameNode
0s Names:Find(): Name does not exist in name map
0s Config:ParsePath(/NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/PhyRxDrop, /NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice, PhyRxDrop)
0s Config:LookupMatches(/NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice)
0s Config:DoResolve(/NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/, 0x7fcc12c47970)
0s Names:Find(0x7fcc12c47970, NodeList)
0s Names:IsNamed(0x7fcc12c47970)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=NodeList on path=/
0s Config:Matches(): Array 0 matches 0
0s Config:DoResolve(/DeviceList/0/$ns3::PointToPointNetDevice/, 0x7fcc12c478e0)
0s Names:Find(0x7fcc12c478e0, DeviceList)
0s Names:IsNamed(0x7fcc12c478e0)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=DeviceList on path=/NodeList/0/
0s Config:Matches(): Array 0 matches 0
0s Config:DoResolve(/$ns3::PointToPointNetDevice/, 0x7fcc12c47da0)
0s Names:Find(0x7fcc12c47da0, $ns3::PointToPointNetDevice)
0s Names:IsNamed(0x7fcc12c47da0)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetObject=ns3::PointToPointNetDevice on path=/NodeList/0/DeviceList/0/
0s Config:DoResolve(/, 0x7fcc12c47da0)
0s Config:DoResolveOne(): resolved=/NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/
0s Config:Matches(): Array 1 does not match 0
0s Config:Matches(): Array 2 does not match 0
0s Config:Matches(): Array 1 does not match 0
0s Config:Matches(): Array 2 does not match 0
0s Config:DoResolve(/NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/, 0x7fcc12c48270)
0s Names:Find(0x7fcc12c48270, NodeList)
0s Names:IsNamed(0x7fcc12c48270)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): Requested item=NodeList does not exist on path=/
0s Config:DoResolve(/NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/, 0)
0s Names:Find(0, NodeList)
0s Names:Find(): Zero context implies root NameNode
0s Names:Find(): Name does not exist in name map
0s Packet:EnablePrinting()
0s Config:ParsePath(/NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice/MacRx, /NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice, MacRx)
0s Config:LookupMatches(/NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice)
0s Config:DoResolve(/NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice/, 0x7fcc12c47970)
0s Names:Find(0x7fcc12c47970, NodeList)
0s Names:IsNamed(0x7fcc12c47970)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=NodeList on path=/
0s Config:Matches(): Array 0 matches 0
0s Config:DoResolve(/DeviceList/1/$ns3::PointToPointNetDevice/, 0x7fcc12c478e0)
0s Names:Find(0x7fcc12c478e0, DeviceList)
0s Names:IsNamed(0x7fcc12c478e0)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=DeviceList on path=/NodeList/0/
0s Config:Matches(): Array 0 does not match 1
0s Config:Matches(): Array 1 matches 1
0s Config:DoResolve(/$ns3::PointToPointNetDevice/, 0x7fcc12c48a00)
0s Names:Find(0x7fcc12c48a00, $ns3::PointToPointNetDevice)
0s Names:IsNamed(0x7fcc12c48a00)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetObject=ns3::PointToPointNetDevice on path=/NodeList/0/DeviceList/1/
0s Config:DoResolve(/, 0x7fcc12c48a00)
0s Config:DoResolveOne(): resolved=/NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice/
0s Config:Matches(): Array 2 does not match 1
0s Config:Matches(): Array 1 does not match 0
0s Config:Matches(): Array 2 does not match 0
0s Config:DoResolve(/NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice/, 0x7fcc12c48270)
0s Names:Find(0x7fcc12c48270, NodeList)
0s Names:IsNamed(0x7fcc12c48270)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): Requested item=NodeList does not exist on path=/
0s Config:DoResolve(/NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice/, 0)
0s Names:Find(0, NodeList)
0s Names:Find(): Zero context implies root NameNode
0s Names:Find(): Name does not exist in name map
0s Config:ParsePath(/NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice/TxQueue/Enqueue, /NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice/TxQueue, Enqueue)
0s Config:LookupMatches(/NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice/TxQueue)
0s Config:DoResolve(/NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c47970)
0s Names:Find(0x7fcc12c47970, NodeList)
0s Names:IsNamed(0x7fcc12c47970)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=NodeList on path=/
0s Config:Matches(): Array 0 matches 0
0s Config:DoResolve(/DeviceList/1/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c478e0)
0s Names:Find(0x7fcc12c478e0, DeviceList)
0s Names:IsNamed(0x7fcc12c478e0)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=DeviceList on path=/NodeList/0/
0s Config:Matches(): Array 0 does not match 1
0s Config:Matches(): Array 1 matches 1
0s Config:DoResolve(/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c48a00)
0s Names:Find(0x7fcc12c48a00, $ns3::PointToPointNetDevice)
0s Names:IsNamed(0x7fcc12c48a00)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetObject=ns3::PointToPointNetDevice on path=/NodeList/0/DeviceList/1/
0s Config:DoResolve(/TxQueue/, 0x7fcc12c48a00)
0s Names:Find(0x7fcc12c48a00, TxQueue)
0s Names:IsNamed(0x7fcc12c48a00)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(ptr)=TxQueue on path=/NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice/
0s Config:DoResolve(/, 0x7fcc12c48b60)
0s Config:DoResolveOne(): resolved=/NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice/TxQueue/
0s Config:Matches(): Array 2 does not match 1
0s Config:Matches(): Array 1 does not match 0
0s Config:Matches(): Array 2 does not match 0
0s Config:DoResolve(/NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c48270)
0s Names:Find(0x7fcc12c48270, NodeList)
0s Names:IsNamed(0x7fcc12c48270)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): Requested item=NodeList does not exist on path=/
0s Config:DoResolve(/NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice/TxQueue/, 0)
0s Names:Find(0, NodeList)
0s Names:Find(): Zero context implies root NameNode
0s Names:Find(): Name does not exist in name map
0s Config:ParsePath(/NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice/TxQueue/Dequeue, /NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice/TxQueue, Dequeue)
0s Config:LookupMatches(/NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice/TxQueue)
0s Config:DoResolve(/NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c47970)
0s Names:Find(0x7fcc12c47970, NodeList)
0s Names:IsNamed(0x7fcc12c47970)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=NodeList on path=/
0s Config:Matches(): Array 0 matches 0
0s Config:DoResolve(/DeviceList/1/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c478e0)
0s Names:Find(0x7fcc12c478e0, DeviceList)
0s Names:IsNamed(0x7fcc12c478e0)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=DeviceList on path=/NodeList/0/
0s Config:Matches(): Array 0 does not match 1
0s Config:Matches(): Array 1 matches 1
0s Config:DoResolve(/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c48a00)
0s Names:Find(0x7fcc12c48a00, $ns3::PointToPointNetDevice)
0s Names:IsNamed(0x7fcc12c48a00)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetObject=ns3::PointToPointNetDevice on path=/NodeList/0/DeviceList/1/
0s Config:DoResolve(/TxQueue/, 0x7fcc12c48a00)
0s Names:Find(0x7fcc12c48a00, TxQueue)
0s Names:IsNamed(0x7fcc12c48a00)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(ptr)=TxQueue on path=/NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice/
0s Config:DoResolve(/, 0x7fcc12c48b60)
0s Config:DoResolveOne(): resolved=/NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice/TxQueue/
0s Config:Matches(): Array 2 does not match 1
0s Config:Matches(): Array 1 does not match 0
0s Config:Matches(): Array 2 does not match 0
0s Config:DoResolve(/NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c48270)
0s Names:Find(0x7fcc12c48270, NodeList)
0s Names:IsNamed(0x7fcc12c48270)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): Requested item=NodeList does not exist on path=/
0s Config:DoResolve(/NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice/TxQueue/, 0)
0s Names:Find(0, NodeList)
0s Names:Find(): Zero context implies root NameNode
0s Names:Find(): Name does not exist in name map
0s Config:ParsePath(/NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice/TxQueue/Drop, /NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice/TxQueue, Drop)
0s Config:LookupMatches(/NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice/TxQueue)
0s Config:DoResolve(/NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c47970)
0s Names:Find(0x7fcc12c47970, NodeList)
0s Names:IsNamed(0x7fcc12c47970)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=NodeList on path=/
0s Config:Matches(): Array 0 matches 0
0s Config:DoResolve(/DeviceList/1/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c478e0)
0s Names:Find(0x7fcc12c478e0, DeviceList)
0s Names:IsNamed(0x7fcc12c478e0)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=DeviceList on path=/NodeList/0/
0s Config:Matches(): Array 0 does not match 1
0s Config:Matches(): Array 1 matches 1
0s Config:DoResolve(/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c48a00)
0s Names:Find(0x7fcc12c48a00, $ns3::PointToPointNetDevice)
0s Names:IsNamed(0x7fcc12c48a00)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetObject=ns3::PointToPointNetDevice on path=/NodeList/0/DeviceList/1/
0s Config:DoResolve(/TxQueue/, 0x7fcc12c48a00)
0s Names:Find(0x7fcc12c48a00, TxQueue)
0s Names:IsNamed(0x7fcc12c48a00)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(ptr)=TxQueue on path=/NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice/
0s Config:DoResolve(/, 0x7fcc12c48b60)
0s Config:DoResolveOne(): resolved=/NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice/TxQueue/
0s Config:Matches(): Array 2 does not match 1
0s Config:Matches(): Array 1 does not match 0
0s Config:Matches(): Array 2 does not match 0
0s Config:DoResolve(/NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c48270)
0s Names:Find(0x7fcc12c48270, NodeList)
0s Names:IsNamed(0x7fcc12c48270)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): Requested item=NodeList does not exist on path=/
0s Config:DoResolve(/NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice/TxQueue/, 0)
0s Names:Find(0, NodeList)
0s Names:Find(): Zero context implies root NameNode
0s Names:Find(): Name does not exist in name map
0s Config:ParsePath(/NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice/PhyRxDrop, /NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice, PhyRxDrop)
0s Config:LookupMatches(/NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice)
0s Config:DoResolve(/NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice/, 0x7fcc12c47970)
0s Names:Find(0x7fcc12c47970, NodeList)
0s Names:IsNamed(0x7fcc12c47970)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=NodeList on path=/
0s Config:Matches(): Array 0 matches 0
0s Config:DoResolve(/DeviceList/1/$ns3::PointToPointNetDevice/, 0x7fcc12c478e0)
0s Names:Find(0x7fcc12c478e0, DeviceList)
0s Names:IsNamed(0x7fcc12c478e0)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=DeviceList on path=/NodeList/0/
0s Config:Matches(): Array 0 does not match 1
0s Config:Matches(): Array 1 matches 1
0s Config:DoResolve(/$ns3::PointToPointNetDevice/, 0x7fcc12c48a00)
0s Names:Find(0x7fcc12c48a00, $ns3::PointToPointNetDevice)
0s Names:IsNamed(0x7fcc12c48a00)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetObject=ns3::PointToPointNetDevice on path=/NodeList/0/DeviceList/1/
0s Config:DoResolve(/, 0x7fcc12c48a00)
0s Config:DoResolveOne(): resolved=/NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice/
0s Config:Matches(): Array 2 does not match 1
0s Config:Matches(): Array 1 does not match 0
0s Config:Matches(): Array 2 does not match 0
0s Config:DoResolve(/NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice/, 0x7fcc12c48270)
0s Names:Find(0x7fcc12c48270, NodeList)
0s Names:IsNamed(0x7fcc12c48270)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): Requested item=NodeList does not exist on path=/
0s Config:DoResolve(/NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice/, 0)
0s Names:Find(0, NodeList)
0s Names:Find(): Zero context implies root NameNode
0s Names:Find(): Name does not exist in name map
0s PointToPointHelper:EnableAsciiInternal(): PointToPointHelper::EnableAsciiInternal(): Device 0 not of type ns3::PointToPointNetDevice
0s Packet:EnablePrinting()
0s Config:ParsePath(/NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/MacRx, /NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice, MacRx)
0s Config:LookupMatches(/NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice)
0s Config:DoResolve(/NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/, 0x7fcc12c47970)
0s Names:Find(0x7fcc12c47970, NodeList)
0s Names:IsNamed(0x7fcc12c47970)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=NodeList on path=/
0s Config:Matches(): Array 0 does not match 1
0s Config:Matches(): Array 1 matches 1
0s Config:DoResolve(/DeviceList/0/$ns3::PointToPointNetDevice/, 0x7fcc12c47b40)
0s Names:Find(0x7fcc12c47b40, DeviceList)
0s Names:IsNamed(0x7fcc12c47b40)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=DeviceList on path=/NodeList/1/
0s Config:Matches(): Array 0 matches 0
0s Config:DoResolve(/$ns3::PointToPointNetDevice/, 0x7fcc12c480b0)
0s Names:Find(0x7fcc12c480b0, $ns3::PointToPointNetDevice)
0s Names:IsNamed(0x7fcc12c480b0)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetObject=ns3::PointToPointNetDevice on path=/NodeList/1/DeviceList/0/
0s Config:DoResolve(/, 0x7fcc12c480b0)
0s Config:DoResolveOne(): resolved=/NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/
0s Config:Matches(): Array 1 does not match 0
0s Config:Matches(): Array 2 does not match 1
0s Config:DoResolve(/NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/, 0x7fcc12c48270)
0s Names:Find(0x7fcc12c48270, NodeList)
0s Names:IsNamed(0x7fcc12c48270)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): Requested item=NodeList does not exist on path=/
0s Config:DoResolve(/NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/, 0)
0s Names:Find(0, NodeList)
0s Names:Find(): Zero context implies root NameNode
0s Names:Find(): Name does not exist in name map
0s Config:ParsePath(/NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/Enqueue, /NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue, Enqueue)
0s Config:LookupMatches(/NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue)
0s Config:DoResolve(/NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c47970)
0s Names:Find(0x7fcc12c47970, NodeList)
0s Names:IsNamed(0x7fcc12c47970)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=NodeList on path=/
0s Config:Matches(): Array 0 does not match 1
0s Config:Matches(): Array 1 matches 1
0s Config:DoResolve(/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c47b40)
0s Names:Find(0x7fcc12c47b40, DeviceList)
0s Names:IsNamed(0x7fcc12c47b40)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=DeviceList on path=/NodeList/1/
0s Config:Matches(): Array 0 matches 0
0s Config:DoResolve(/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c480b0)
0s Names:Find(0x7fcc12c480b0, $ns3::PointToPointNetDevice)
0s Names:IsNamed(0x7fcc12c480b0)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetObject=ns3::PointToPointNetDevice on path=/NodeList/1/DeviceList/0/
0s Config:DoResolve(/TxQueue/, 0x7fcc12c480b0)
0s Names:Find(0x7fcc12c480b0, TxQueue)
0s Names:IsNamed(0x7fcc12c480b0)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(ptr)=TxQueue on path=/NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/
0s Config:DoResolve(/, 0x7fcc12c484f0)
0s Config:DoResolveOne(): resolved=/NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/
0s Config:Matches(): Array 1 does not match 0
0s Config:Matches(): Array 2 does not match 1
0s Config:DoResolve(/NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c48270)
0s Names:Find(0x7fcc12c48270, NodeList)
0s Names:IsNamed(0x7fcc12c48270)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): Requested item=NodeList does not exist on path=/
0s Config:DoResolve(/NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/, 0)
0s Names:Find(0, NodeList)
0s Names:Find(): Zero context implies root NameNode
0s Names:Find(): Name does not exist in name map
0s Config:ParsePath(/NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/Dequeue, /NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue, Dequeue)
0s Config:LookupMatches(/NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue)
0s Config:DoResolve(/NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c47970)
0s Names:Find(0x7fcc12c47970, NodeList)
0s Names:IsNamed(0x7fcc12c47970)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=NodeList on path=/
0s Config:Matches(): Array 0 does not match 1
0s Config:Matches(): Array 1 matches 1
0s Config:DoResolve(/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c47b40)
0s Names:Find(0x7fcc12c47b40, DeviceList)
0s Names:IsNamed(0x7fcc12c47b40)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=DeviceList on path=/NodeList/1/
0s Config:Matches(): Array 0 matches 0
0s Config:DoResolve(/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c480b0)
0s Names:Find(0x7fcc12c480b0, $ns3::PointToPointNetDevice)
0s Names:IsNamed(0x7fcc12c480b0)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetObject=ns3::PointToPointNetDevice on path=/NodeList/1/DeviceList/0/
0s Config:DoResolve(/TxQueue/, 0x7fcc12c480b0)
0s Names:Find(0x7fcc12c480b0, TxQueue)
0s Names:IsNamed(0x7fcc12c480b0)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(ptr)=TxQueue on path=/NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/
0s Config:DoResolve(/, 0x7fcc12c484f0)
0s Config:DoResolveOne(): resolved=/NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/
0s Config:Matches(): Array 1 does not match 0
0s Config:Matches(): Array 2 does not match 1
0s Config:DoResolve(/NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c48270)
0s Names:Find(0x7fcc12c48270, NodeList)
0s Names:IsNamed(0x7fcc12c48270)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): Requested item=NodeList does not exist on path=/
0s Config:DoResolve(/NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/, 0)
0s Names:Find(0, NodeList)
0s Names:Find(): Zero context implies root NameNode
0s Names:Find(): Name does not exist in name map
0s Config:ParsePath(/NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/Drop, /NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue, Drop)
0s Config:LookupMatches(/NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue)
0s Config:DoResolve(/NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c47970)
0s Names:Find(0x7fcc12c47970, NodeList)
0s Names:IsNamed(0x7fcc12c47970)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=NodeList on path=/
0s Config:Matches(): Array 0 does not match 1
0s Config:Matches(): Array 1 matches 1
0s Config:DoResolve(/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c47b40)
0s Names:Find(0x7fcc12c47b40, DeviceList)
0s Names:IsNamed(0x7fcc12c47b40)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=DeviceList on path=/NodeList/1/
0s Config:Matches(): Array 0 matches 0
0s Config:DoResolve(/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c480b0)
0s Names:Find(0x7fcc12c480b0, $ns3::PointToPointNetDevice)
0s Names:IsNamed(0x7fcc12c480b0)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetObject=ns3::PointToPointNetDevice on path=/NodeList/1/DeviceList/0/
0s Config:DoResolve(/TxQueue/, 0x7fcc12c480b0)
0s Names:Find(0x7fcc12c480b0, TxQueue)
0s Names:IsNamed(0x7fcc12c480b0)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(ptr)=TxQueue on path=/NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/
0s Config:DoResolve(/, 0x7fcc12c484f0)
0s Config:DoResolveOne(): resolved=/NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/
0s Config:Matches(): Array 1 does not match 0
0s Config:Matches(): Array 2 does not match 1
0s Config:DoResolve(/NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c48270)
0s Names:Find(0x7fcc12c48270, NodeList)
0s Names:IsNamed(0x7fcc12c48270)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): Requested item=NodeList does not exist on path=/
0s Config:DoResolve(/NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/, 0)
0s Names:Find(0, NodeList)
0s Names:Find(): Zero context implies root NameNode
0s Names:Find(): Name does not exist in name map
0s Config:ParsePath(/NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/PhyRxDrop, /NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice, PhyRxDrop)
0s Config:LookupMatches(/NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice)
0s Config:DoResolve(/NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/, 0x7fcc12c47970)
0s Names:Find(0x7fcc12c47970, NodeList)
0s Names:IsNamed(0x7fcc12c47970)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=NodeList on path=/
0s Config:Matches(): Array 0 does not match 1
0s Config:Matches(): Array 1 matches 1
0s Config:DoResolve(/DeviceList/0/$ns3::PointToPointNetDevice/, 0x7fcc12c47b40)
0s Names:Find(0x7fcc12c47b40, DeviceList)
0s Names:IsNamed(0x7fcc12c47b40)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=DeviceList on path=/NodeList/1/
0s Config:Matches(): Array 0 matches 0
0s Config:DoResolve(/$ns3::PointToPointNetDevice/, 0x7fcc12c480b0)
0s Names:Find(0x7fcc12c480b0, $ns3::PointToPointNetDevice)
0s Names:IsNamed(0x7fcc12c480b0)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetObject=ns3::PointToPointNetDevice on path=/NodeList/1/DeviceList/0/
0s Config:DoResolve(/, 0x7fcc12c480b0)
0s Config:DoResolveOne(): resolved=/NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/
0s Config:Matches(): Array 1 does not match 0
0s Config:Matches(): Array 2 does not match 1
0s Config:DoResolve(/NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/, 0x7fcc12c48270)
0s Names:Find(0x7fcc12c48270, NodeList)
0s Names:IsNamed(0x7fcc12c48270)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): Requested item=NodeList does not exist on path=/
0s Config:DoResolve(/NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/, 0)
0s Names:Find(0, NodeList)
0s Names:Find(): Zero context implies root NameNode
0s Names:Find(): Name does not exist in name map
0s PointToPointHelper:EnableAsciiInternal(): PointToPointHelper::EnableAsciiInternal(): Device 0 not of type ns3::PointToPointNetDevice
0s Packet:EnablePrinting()
0s Config:ParsePath(/NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice/MacRx, /NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice, MacRx)
0s Config:LookupMatches(/NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice)
0s Config:DoResolve(/NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice/, 0x7fcc12c47970)
0s Names:Find(0x7fcc12c47970, NodeList)
0s Names:IsNamed(0x7fcc12c47970)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=NodeList on path=/
0s Config:Matches(): Array 0 does not match 2
0s Config:Matches(): Array 1 does not match 2
0s Config:Matches(): Array 2 matches 2
0s Config:DoResolve(/DeviceList/0/$ns3::PointToPointNetDevice/, 0x7fcc12c47ca0)
0s Names:Find(0x7fcc12c47ca0, DeviceList)
0s Names:IsNamed(0x7fcc12c47ca0)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=DeviceList on path=/NodeList/2/
0s Config:Matches(): Array 0 matches 0
0s Config:DoResolve(/$ns3::PointToPointNetDevice/, 0x7fcc12c48c30)
0s Names:Find(0x7fcc12c48c30, $ns3::PointToPointNetDevice)
0s Names:IsNamed(0x7fcc12c48c30)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetObject=ns3::PointToPointNetDevice on path=/NodeList/2/DeviceList/0/
0s Config:DoResolve(/, 0x7fcc12c48c30)
0s Config:DoResolveOne(): resolved=/NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice/
0s Config:Matches(): Array 1 does not match 0
0s Config:DoResolve(/NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice/, 0x7fcc12c48270)
0s Names:Find(0x7fcc12c48270, NodeList)
0s Names:IsNamed(0x7fcc12c48270)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): Requested item=NodeList does not exist on path=/
0s Config:DoResolve(/NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice/, 0)
0s Names:Find(0, NodeList)
0s Names:Find(): Zero context implies root NameNode
0s Names:Find(): Name does not exist in name map
0s Config:ParsePath(/NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/Enqueue, /NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue, Enqueue)
0s Config:LookupMatches(/NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue)
0s Config:DoResolve(/NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c47970)
0s Names:Find(0x7fcc12c47970, NodeList)
0s Names:IsNamed(0x7fcc12c47970)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=NodeList on path=/
0s Config:Matches(): Array 0 does not match 2
0s Config:Matches(): Array 1 does not match 2
0s Config:Matches(): Array 2 matches 2
0s Config:DoResolve(/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c47ca0)
0s Names:Find(0x7fcc12c47ca0, DeviceList)
0s Names:IsNamed(0x7fcc12c47ca0)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=DeviceList on path=/NodeList/2/
0s Config:Matches(): Array 0 matches 0
0s Config:DoResolve(/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c48c30)
0s Names:Find(0x7fcc12c48c30, $ns3::PointToPointNetDevice)
0s Names:IsNamed(0x7fcc12c48c30)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetObject=ns3::PointToPointNetDevice on path=/NodeList/2/DeviceList/0/
0s Config:DoResolve(/TxQueue/, 0x7fcc12c48c30)
0s Names:Find(0x7fcc12c48c30, TxQueue)
0s Names:IsNamed(0x7fcc12c48c30)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(ptr)=TxQueue on path=/NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice/
0s Config:DoResolve(/, 0x7fcc12c49030)
0s Config:DoResolveOne(): resolved=/NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/
0s Config:Matches(): Array 1 does not match 0
0s Config:DoResolve(/NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c48270)
0s Names:Find(0x7fcc12c48270, NodeList)
0s Names:IsNamed(0x7fcc12c48270)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): Requested item=NodeList does not exist on path=/
0s Config:DoResolve(/NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/, 0)
0s Names:Find(0, NodeList)
0s Names:Find(): Zero context implies root NameNode
0s Names:Find(): Name does not exist in name map
0s Config:ParsePath(/NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/Dequeue, /NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue, Dequeue)
0s Config:LookupMatches(/NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue)
0s Config:DoResolve(/NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c47970)
0s Names:Find(0x7fcc12c47970, NodeList)
0s Names:IsNamed(0x7fcc12c47970)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=NodeList on path=/
0s Config:Matches(): Array 0 does not match 2
0s Config:Matches(): Array 1 does not match 2
0s Config:Matches(): Array 2 matches 2
0s Config:DoResolve(/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c47ca0)
0s Names:Find(0x7fcc12c47ca0, DeviceList)
0s Names:IsNamed(0x7fcc12c47ca0)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=DeviceList on path=/NodeList/2/
0s Config:Matches(): Array 0 matches 0
0s Config:DoResolve(/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c48c30)
0s Names:Find(0x7fcc12c48c30, $ns3::PointToPointNetDevice)
0s Names:IsNamed(0x7fcc12c48c30)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetObject=ns3::PointToPointNetDevice on path=/NodeList/2/DeviceList/0/
0s Config:DoResolve(/TxQueue/, 0x7fcc12c48c30)
0s Names:Find(0x7fcc12c48c30, TxQueue)
0s Names:IsNamed(0x7fcc12c48c30)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(ptr)=TxQueue on path=/NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice/
0s Config:DoResolve(/, 0x7fcc12c49030)
0s Config:DoResolveOne(): resolved=/NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/
0s Config:Matches(): Array 1 does not match 0
0s Config:DoResolve(/NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c48270)
0s Names:Find(0x7fcc12c48270, NodeList)
0s Names:IsNamed(0x7fcc12c48270)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): Requested item=NodeList does not exist on path=/
0s Config:DoResolve(/NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/, 0)
0s Names:Find(0, NodeList)
0s Names:Find(): Zero context implies root NameNode
0s Names:Find(): Name does not exist in name map
0s Config:ParsePath(/NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/Drop, /NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue, Drop)
0s Config:LookupMatches(/NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue)
0s Config:DoResolve(/NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c47970)
0s Names:Find(0x7fcc12c47970, NodeList)
0s Names:IsNamed(0x7fcc12c47970)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=NodeList on path=/
0s Config:Matches(): Array 0 does not match 2
0s Config:Matches(): Array 1 does not match 2
0s Config:Matches(): Array 2 matches 2
0s Config:DoResolve(/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c47ca0)
0s Names:Find(0x7fcc12c47ca0, DeviceList)
0s Names:IsNamed(0x7fcc12c47ca0)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=DeviceList on path=/NodeList/2/
0s Config:Matches(): Array 0 matches 0
0s Config:DoResolve(/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c48c30)
0s Names:Find(0x7fcc12c48c30, $ns3::PointToPointNetDevice)
0s Names:IsNamed(0x7fcc12c48c30)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetObject=ns3::PointToPointNetDevice on path=/NodeList/2/DeviceList/0/
0s Config:DoResolve(/TxQueue/, 0x7fcc12c48c30)
0s Names:Find(0x7fcc12c48c30, TxQueue)
0s Names:IsNamed(0x7fcc12c48c30)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(ptr)=TxQueue on path=/NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice/
0s Config:DoResolve(/, 0x7fcc12c49030)
0s Config:DoResolveOne(): resolved=/NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/
0s Config:Matches(): Array 1 does not match 0
0s Config:DoResolve(/NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/, 0x7fcc12c48270)
0s Names:Find(0x7fcc12c48270, NodeList)
0s Names:IsNamed(0x7fcc12c48270)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): Requested item=NodeList does not exist on path=/
0s Config:DoResolve(/NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/, 0)
0s Names:Find(0, NodeList)
0s Names:Find(): Zero context implies root NameNode
0s Names:Find(): Name does not exist in name map
0s Config:ParsePath(/NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice/PhyRxDrop, /NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice, PhyRxDrop)
0s Config:LookupMatches(/NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice)
0s Config:DoResolve(/NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice/, 0x7fcc12c47970)
0s Names:Find(0x7fcc12c47970, NodeList)
0s Names:IsNamed(0x7fcc12c47970)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=NodeList on path=/
0s Config:Matches(): Array 0 does not match 2
0s Config:Matches(): Array 1 does not match 2
0s Config:Matches(): Array 2 matches 2
0s Config:DoResolve(/DeviceList/0/$ns3::PointToPointNetDevice/, 0x7fcc12c47ca0)
0s Names:Find(0x7fcc12c47ca0, DeviceList)
0s Names:IsNamed(0x7fcc12c47ca0)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetAttribute(vector)=DeviceList on path=/NodeList/2/
0s Config:Matches(): Array 0 matches 0
0s Config:DoResolve(/$ns3::PointToPointNetDevice/, 0x7fcc12c48c30)
0s Names:Find(0x7fcc12c48c30, $ns3::PointToPointNetDevice)
0s Names:IsNamed(0x7fcc12c48c30)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): GetObject=ns3::PointToPointNetDevice on path=/NodeList/2/DeviceList/0/
0s Config:DoResolve(/, 0x7fcc12c48c30)
0s Config:DoResolveOne(): resolved=/NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice/
0s Config:Matches(): Array 1 does not match 0
0s Config:DoResolve(/NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice/, 0x7fcc12c48270)
0s Names:Find(0x7fcc12c48270, NodeList)
0s Names:IsNamed(0x7fcc12c48270)
0s Names:IsNamed(): Object does not exist in object map, returning NameNode 0
0s Names:Find(): Context does not point to a previously named node
0s Config:DoResolve(): Requested item=NodeList does not exist on path=/
0s Config:DoResolve(/NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice/, 0)
0s Names:Find(0, NodeList)
0s Names:Find(): Zero context implies root NameNode
0s Names:Find(): Name does not exist in name map
0s PointToPointHelper:EnableAsciiInternal(): PointToPointHelper::EnableAsciiInternal(): Device 0 not of type ns3::PointToPointNetDevice
```

Due to

    pointToPoint.EnableAsciiAll (Create<OutputStreamWrapper>(&std::cout));

It install hooks to all the relevant objects (PointToPointNetDevice and DropTailQueue) to print something upon an event interested.

## Simulation

Run (line 3766-4582):

```
0s VerySimple:main(): Run Simulation.
0s Simulator:Run()
0s MapScheduler:RemoveNext(0x7fcc12c47a10)
0s MapScheduler:RemoveNext(): 0x7fcc12c47a100x7fcc12c47a6005
0s DefaultSimulatorImpl:ProcessOneEvent(): handle 0
0s MapScheduler:RemoveNext(0x7fcc12c47a10)
0s MapScheduler:RemoveNext(): 0x7fcc12c47a100x7fcc12c47bd006
0s DefaultSimulatorImpl:ProcessOneEvent(): handle 0
0s MapScheduler:Insert(0x7fcc12c47a10, 0x7fcc12c47a60, 3b9aca00, 14)
0s MapScheduler:Insert(0x7fcc12c47a10, 0x7fcc12c4ec70, 2540be400, 15)
0s MapScheduler:RemoveNext(0x7fcc12c47a10)
0s MapScheduler:RemoveNext(): 0x7fcc12c47a100x7fcc12c47d3007
0s DefaultSimulatorImpl:ProcessOneEvent(): handle 0
0s MapScheduler:Insert(0x7fcc12c47a10, 0x7fcc12c47bd0, 3b9aca00, 16)
0s MapScheduler:Insert(0x7fcc12c47a10, 0x7fcc12c4ef80, 2540be400, 17)
0s MapScheduler:RemoveNext(0x7fcc12c47a10)
0s MapScheduler:RemoveNext(): 0x7fcc12c47a100x7fcc12c47f3008
0s DefaultSimulatorImpl:ProcessOneEvent(): handle 0
0s MapScheduler:RemoveNext(0x7fcc12c47a10)
0s MapScheduler:RemoveNext(): 0x7fcc12c47a100x7fcc12c4824009
0s DefaultSimulatorImpl:ProcessOneEvent(): handle 0
0s MapScheduler:RemoveNext(0x7fcc12c47a10)
0s MapScheduler:RemoveNext(): 0x7fcc12c47a100x7fcc12c486b00b
0s DefaultSimulatorImpl:ProcessOneEvent(): handle 0
0s MapScheduler:RemoveNext(0x7fcc12c47a10)
0s MapScheduler:RemoveNext(): 0x7fcc12c47a100x7fcc12c48dc00c
0s DefaultSimulatorImpl:ProcessOneEvent(): handle 0
0s MapScheduler:RemoveNext(0x7fcc12c47a10)
0s MapScheduler:RemoveNext(): 0x7fcc12c47a100x7fcc12c497000d
0s DefaultSimulatorImpl:ProcessOneEvent(): handle 0
0s MapScheduler:RemoveNext(0x7fcc12c47a10)
0s MapScheduler:RemoveNext(): 0x7fcc12c47a100x7fcc12c4a7200e
0s DefaultSimulatorImpl:ProcessOneEvent(): handle 0
0s MapScheduler:RemoveNext(0x7fcc12c47a10)
0s MapScheduler:RemoveNext(): 0x7fcc12c47a100x7fcc12c4b7f00f
0s DefaultSimulatorImpl:ProcessOneEvent(): handle 0
0s MapScheduler:RemoveNext(0x7fcc12c47a10)
0s MapScheduler:RemoveNext(): 0x7fcc12c47a100x7fcc12c4d5e0012
0s DefaultSimulatorImpl:ProcessOneEvent(): handle 0
0s MapScheduler:RemoveNext(0x7fcc12c47a10)
0s MapScheduler:RemoveNext(): 0x7fcc12c47a100x7fcc12c4d9c0013
0s DefaultSimulatorImpl:ProcessOneEvent(): handle 0
0s MapScheduler:RemoveNext(0x7fcc12c47a10)
0s MapScheduler:RemoveNext(): 0x7fcc12c47a100x7fcc12c47a603b9aca0014
0s DefaultSimulatorImpl:ProcessOneEvent(): handle 3b9aca00
1s PacketSink:StartApplication(0x7fcc12c4d640)
1s UdpL4Protocol:CreateSocket()
1s Socket:Socket()
1s UdpSocket:UdpSocket()
1s UdpSocketImpl:UdpSocketImpl()
1s ObjectBase:ConstructSelf(): construct tid=ns3::UdpSocketImpl, params=1
1s ObjectBase:ConstructSelf(): try to construct "ns3::UdpSocketImpl::IcmpCallback"
1s ObjectBase:ConstructSelf(): construct "ns3::UdpSocketImpl::IcmpCallback" from initial value.
1s ObjectBase:ConstructSelf(): construct tid=ns3::UdpSocket, params=6
1s ObjectBase:ConstructSelf(): try to construct "ns3::UdpSocket::RcvBufSize"
1s ObjectBase:ConstructSelf(): construct "ns3::UdpSocket::RcvBufSize" from initial value.
1s ObjectBase:ConstructSelf(): try to construct "ns3::UdpSocket::IpTtl"
1s ObjectBase:ConstructSelf(): construct "ns3::UdpSocket::IpTtl" from initial value.
1s ObjectBase:ConstructSelf(): try to construct "ns3::UdpSocket::IpMulticastTtl"
1s ObjectBase:ConstructSelf(): construct "ns3::UdpSocket::IpMulticastTtl" from initial value.
1s ObjectBase:ConstructSelf(): try to construct "ns3::UdpSocket::IpMulticastIf"
1s ObjectBase:ConstructSelf(): construct "ns3::UdpSocket::IpMulticastIf" from initial value.
1s ObjectBase:ConstructSelf(): try to construct "ns3::UdpSocket::IpMulticastLoop"
1s ObjectBase:ConstructSelf(): construct "ns3::UdpSocket::IpMulticastLoop" from initial value.
1s ObjectBase:ConstructSelf(): try to construct "ns3::UdpSocket::MtuDiscover"
1s ObjectBase:ConstructSelf(): construct "ns3::UdpSocket::MtuDiscover" from initial value.
1s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
1s UdpSocketImpl:SetNode()
1s UdpSocketImpl:SetUdp()
1s UdpSocketImpl:Bind(0x7fcc12c4f440, 03-06-00:00:00:00:50:c3)
1s UdpL4Protocol:Allocate(0x7fcc12c4a8d0, 50000)
1s Ipv4EndPointDemux:Allocate(0x7fcc12c4a620, 50000)
1s Ipv4EndPointDemux:Allocate(0x7fcc12c4a620, 0.0.0.0, 50000)
1s Ipv4EndPointDemux:LookupLocal()
1s Ipv4EndPointDemux:Allocate(): Now have >>1<< endpoints.
1s UdpSocketImpl:FinishBind()
1s UdpSocketImpl:ShutdownSend()
1s Socket:SetRecvCallback()
1s Socket:SetAcceptCallback()
1s Socket:SetCloseCallbacks()
1s MapScheduler:RemoveNext(0x7fcc12c47a10)
1s MapScheduler:RemoveNext(): 0x7fcc12c47a100x7fcc12c47bd0100000000022
1s DefaultSimulatorImpl:ProcessOneEvent(): handle 1000000000
1s OnOffApplication:StartApplication()
1s UdpL4Protocol:CreateSocket()
1s Socket:Socket()
1s UdpSocket:UdpSocket()
1s UdpSocketImpl:UdpSocketImpl()
1s ObjectBase:ConstructSelf(): construct tid=ns3::UdpSocketImpl, params=1
1s ObjectBase:ConstructSelf(): try to construct "ns3::UdpSocketImpl::IcmpCallback"
1s ObjectBase:ConstructSelf(): construct "ns3::UdpSocketImpl::IcmpCallback" from initial value.
1s ObjectBase:ConstructSelf(): construct tid=ns3::UdpSocket, params=6
1s ObjectBase:ConstructSelf(): try to construct "ns3::UdpSocket::RcvBufSize"
1s ObjectBase:ConstructSelf(): construct "ns3::UdpSocket::RcvBufSize" from initial value.
1s ObjectBase:ConstructSelf(): try to construct "ns3::UdpSocket::IpTtl"
1s ObjectBase:ConstructSelf(): construct "ns3::UdpSocket::IpTtl" from initial value.
1s ObjectBase:ConstructSelf(): try to construct "ns3::UdpSocket::IpMulticastTtl"
1s ObjectBase:ConstructSelf(): construct "ns3::UdpSocket::IpMulticastTtl" from initial value.
1s ObjectBase:ConstructSelf(): try to construct "ns3::UdpSocket::IpMulticastIf"
1s ObjectBase:ConstructSelf(): construct "ns3::UdpSocket::IpMulticastIf" from initial value.
1s ObjectBase:ConstructSelf(): try to construct "ns3::UdpSocket::IpMulticastLoop"
1s ObjectBase:ConstructSelf(): construct "ns3::UdpSocket::IpMulticastLoop" from initial value.
1s ObjectBase:ConstructSelf(): try to construct "ns3::UdpSocket::MtuDiscover"
1s ObjectBase:ConstructSelf(): construct "ns3::UdpSocket::MtuDiscover" from initial value.
1s ObjectBase:ConstructSelf(): construct tid=ns3::Object, params=0
1s UdpSocketImpl:SetNode()
1s UdpSocketImpl:SetUdp()
1s UdpSocketImpl:Bind()
1s UdpL4Protocol:Allocate()
1s Ipv4EndPointDemux:Allocate()
1s Ipv4EndPointDemux:AllocateEphemeralPort()
1s Ipv4EndPointDemux:LookupPortLocal()
1s Ipv4EndPointDemux:Allocate(): Now have >>1<< endpoints.
1s UdpSocketImpl:FinishBind()
1s UdpSocketImpl:Connect(0x7fcc12c4f560, 03-06-0a:01:01:02:50:c3)
1s Socket:NotifyConnectionSucceeded()
1s UdpSocketImpl:ShutdownRecv()
1s OnOffApplication:CancelEvents()
1s Simulator:IsExpired(0x7fcc12c4db50)
1s Simulator:Cancel(0x7fcc12c4db50)
1s Simulator:Cancel(0x7fcc12c4db38)
1s OnOffApplication:ScheduleStartEvent()
1s OnOffApplication:ScheduleStartEvent(): start at +0.0ns
1s MapScheduler:Insert(0x7fcc12c47a10, 0x7fcc12c47d60, 1000000000, 24)
1s MapScheduler:RemoveNext(0x7fcc12c47a10)
1s MapScheduler:RemoveNext(): 0x7fcc12c47a100x7fcc12c47d60100000000024
1s DefaultSimulatorImpl:ProcessOneEvent(): handle 1000000000
1s OnOffApplication:StartSending()
1s OnOffApplication:ScheduleNextTx()
1s OnOffApplication:ScheduleNextTx(): bits = 4096
1s OnOffApplication:ScheduleNextTx(): nextTime = +8191999.0ns
1s MapScheduler:Insert(0x7fcc12c47a10, 0x7fcc12c4f7d0, 1008191999, 25)
1s OnOffApplication:ScheduleStopEvent()
1s OnOffApplication:ScheduleStopEvent(): stop at +1000000000.0ns
1s MapScheduler:Insert(0x7fcc12c47a10, 0x7fcc12c4de80, 2000000000, 26)
1s MapScheduler:RemoveNext(0x7fcc12c47a10)
1s MapScheduler:RemoveNext(): 0x7fcc12c47a100x7fcc12c4f7d0100819199925
1s DefaultSimulatorImpl:ProcessOneEvent(): handle 1008191999
1.00819s OnOffApplication:SendPacket()
1.00819s OnOffApplication:SendPacket(): sending packet at +1008191999.0ns
1.00819s Simulator:IsExpired(0x7fcc12c4db50)
1.00819s Buffer:Buffer(0x7fcc12c4e0c8, 512)
1.00819s Buffer:Initialize(0x7fcc12c4e0c8, 512)
1.00819s ByteTagList:ByteTagList(0x7fcc12c4e0e8)
1.00819s Simulator:GetSystemId()
1.00819s PacketMetadata:Create(): create size=10, max=0
1.00819s PacketMetadata:Create(): create alloc size=10
1.00819s PacketMetadata:DoAddHeader(0x7fcc12c4e100, 0, 512)
1.00819s PacketMetadata:AddSmall(0x7fcc12c4e100, 65535, 65535, 0, 512, 0)
1.00819s PacketMetadata:UpdateHead(0x7fcc12c4e100, 9)
1.00819s Socket:Send()
1.00819s UdpSocketImpl:Send(0x7fcc12c4f560, 0x7fcc12c4e0c0, 0)
1.00819s UdpSocketImpl:DoSend(0x7fcc12c4f560, 0x7fcc12c4e0c0)
1.00819s UdpSocketImpl:DoSendTo(0x7fcc12c4f560, 0x7fcc12c4e0c0, 10.1.1.2, 50000)
1.00819s UdpSocketImpl:GetTxAvailable()
1.00819s Packet:RemovePacketTag(0x7fcc12c4e0c0, ns3::SocketSetDontFragmentTag, 1)
1.00819s PacketTagList:Remove(0x7fcc12c4e0f8, ns3::SocketSetDontFragmentTag)
1.00819s Packet:AddPacketTag(0x7fcc12c4e0c0, ns3::SocketSetDontFragmentTag, 1)
1.00819s PacketTagList:Add(0x7fcc12c4e0f8, ns3::SocketSetDontFragmentTag)
1.00819s PacketTagList:AllocData()
1.00819s Ipv4ListRouting:RouteOutput(0x7fcc12c4ba80, 10.1.1.2,  , 102.102.102.102,  , 0)
1.00819s Ipv4ListRouting:RouteOutput(): Checking protocol ns3::Ipv4StaticRouting with priority 0
1.00819s Ipv4ListRouting:RouteOutput(): Requesting source address for destination 10.1.1.2
1.00819s 1.00819 [node 2] Ipv4StaticRouting:RouteOutput(0x7fcc12c4bb70, tos 0x0 ttl 0 id 0 protocol 17 offset (bytes) 0 flags [none] length: 20 102.102.102.102 > 10.1.1.2, 0)
1.00819s 1.00819 [node 2] Ipv4StaticRouting:LookupStatic(0x7fcc12c4bb70, 10.1.1.2,  , 0)
1.00819s 1.00819 [node 2] Ipv4StaticRouting:LookupStatic(): Searching for route to 10.1.1.2, checking against route to 127.0.0.0/8
1.00819s 1.00819 [node 2] Ipv4StaticRouting:LookupStatic(): Searching for route to 10.1.1.2, checking against route to 10.1.2.0/24
1.00819s 1.00819 [node 2] Ipv4StaticRouting:LookupStatic(): No matching route to 10.1.1.2 found
1.00819s Ipv4ListRouting:RouteOutput(): Checking protocol ns3::Ipv4GlobalRouting with priority -10
1.00819s Ipv4ListRouting:RouteOutput(): Requesting source address for destination 10.1.1.2
1.00819s Ipv4GlobalRouting:RouteOutput(): Unicast destination- looking up
1.00819s Ipv4GlobalRouting:LookupGlobal()
1.00819s Ipv4GlobalRouting:LookupGlobal(): Looking for route for destination 10.1.1.2
1.00819s Ipv4GlobalRouting:LookupGlobal(): Number of m_hostRoutes = 0
1.00819s Ipv4GlobalRouting:LookupGlobal(): Number of m_networkRoutes1
1.00819s Ipv4GlobalRouting:LookupGlobal(): 1Found global network route0x7fcc12c4d5b0
1.00819s Ipv4Interface:GetAddress()
1.00819s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d400ff8)
1.00819s Ipv4InterfaceAddress:GetLocal()
1.00819s Ipv4ListRouting:RouteOutput(): Found route 0x7fcc12c4f800
1.00819s UdpSocketImpl:DoSendTo(): Route exists
1.00819s ByteTagList:ByteTagList(0x7fcc12c4e148, 0x7fcc12c4e0e8)
1.00819s UdpL4Protocol:Send(0x7fcc12c4b9a0, 0x7fcc12c4e120, 10.1.2.2, 10.1.1.2, 49153, 50000, 0x7fcc12c4f800)
1.00819s Packet:AddHeader(0x7fcc12c4e120, ns3::UdpHeader, 8)
1.00819s Buffer:AddAtStart(0x7fcc12c4e128, 8)
1.00819s Buffer:AddAtStart(): add start=8, start=0, end=520, zero start=8, zero end=520, count=1, size=8, dirty start=0, dirty end=520
1.00819s ByteTagList:AddAtStart(0x7fcc12c4e148, 8, 8)
1.00819s ByteTagList:ByteTagList(0x7fff6d401118)
1.00819s ByteTagList:BeginAll(0x7fcc12c4e148)
1.00819s ByteTagList:Begin(0x7fcc12c4e148, 0, 2147483647)
1.00819s ByteTagList:operator=(0x7fcc12c4e148, 0x7fff6d401118)
1.00819s ByteTagList:Deallocate(0x7fcc12c4e148, 0)
1.00819s ByteTagList:~ByteTagList(0x7fff6d401118)
1.00819s ByteTagList:Deallocate(0x7fff6d401118, 0)
1.00819s PacketMetadata:ReadItems(0x7fcc12c4e160, 0)
1.00819s PacketMetadata:DoAddHeader(0x7fcc12c4e160, 98, 8)
1.00819s PacketMetadata:AddSmall(0x7fcc12c4e160, 0, 65535, 98, 8, 1)
1.00819s PacketMetadata:Create(): create size=17, max=10
1.00819s PacketMetadata:Create(): create alloc size=17
1.00819s PacketMetadata:UpdateHead(0x7fcc12c4e160, 8)
1.00819s PacketMetadata:ReadItems(0x7fcc12c4e160, 9)
1.00819s PacketMetadata:ReadItems(0x7fcc12c4e160, 0)
1.00819s Ipv4L3Protocol:Send(0x7fcc12c4b5b0, 0x7fcc12c4e120, 10.1.2.2, 10.1.1.2, 17, 0x7fcc12c4f800)
1.00819s Packet:RemovePacketTag(0x7fcc12c4e120, ns3::SocketIpTtlTag, 1)
1.00819s PacketTagList:Remove(0x7fcc12c4e158, ns3::SocketIpTtlTag)
1.00819s Ipv4Interface:GetNAddresses()
1.00819s Ipv4Interface:GetAddress()
1.00819s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d400c20)
1.00819s Ipv4L3Protocol:Send(): 1.00819s Ipv4InterfaceAddress:GetMask()
1.00819s Ipv4InterfaceAddress:GetLocal()
Testing address 127.0.0.1 with mask 255.0.0.0
1.00819s Ipv4InterfaceAddress:GetMask()
1.00819s Ipv4Interface:GetNAddresses()
1.00819s Ipv4Interface:GetNAddresses()
1.00819s Ipv4Interface:GetAddress()
1.00819s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d400c20)
1.00819s Ipv4L3Protocol:Send(): 1.00819s Ipv4InterfaceAddress:GetMask()
1.00819s Ipv4InterfaceAddress:GetLocal()
Testing address 10.1.2.2 with mask 255.255.255.0
1.00819s Ipv4InterfaceAddress:GetMask()
1.00819s Ipv4Interface:GetNAddresses()
1.00819s Ipv4L3Protocol:Send(): Ipv4L3Protocol::Send case 3:  passed in with route
1.00819s Ipv4L3Protocol:BuildHeader(0x7fcc12c4b5b0, 10.1.2.2, 10.1.1.2, 17, 520, 64, 1)
1.00819s ByteTagList:ByteTagList(0x7fcc12c4e1a8, 0x7fcc12c4e148)
1.00819s Ipv4L3Protocol:SendRealOut(0x7fcc12c4b5b0, 0x7fcc12c4e180, 0x7fff6d400cc8)
1.00819s Packet:AddHeader(0x7fcc12c4e180, ns3::Ipv4Header, 20)
1.00819s Buffer:AddAtStart(0x7fcc12c4e188, 20)
1.00819s Buffer:AddAtStart(): add start=20, start=0, end=540, zero start=28, zero end=540, count=1, size=28, dirty start=0, dirty end=540
1.00819s ByteTagList:AddAtStart(0x7fcc12c4e1a8, 20, 20)
1.00819s ByteTagList:ByteTagList(0x7fff6d3ff938)
1.00819s ByteTagList:BeginAll(0x7fcc12c4e1a8)
1.00819s ByteTagList:Begin(0x7fcc12c4e1a8, 0, 2147483647)
1.00819s ByteTagList:operator=(0x7fcc12c4e1a8, 0x7fff6d3ff938)
1.00819s ByteTagList:Deallocate(0x7fcc12c4e1a8, 0)
1.00819s ByteTagList:~ByteTagList(0x7fff6d3ff938)
1.00819s ByteTagList:Deallocate(0x7fff6d3ff938, 0)
1.00819s PacketMetadata:ReadItems(0x7fcc12c4e1c0, 9)
1.00819s PacketMetadata:ReadItems(0x7fcc12c4e1c0, 0)
1.00819s PacketMetadata:DoAddHeader(0x7fcc12c4e1c0, 250, 20)
1.00819s PacketMetadata:AddSmall(0x7fcc12c4e1c0, 9, 65535, 250, 20, 2)
1.00819s PacketMetadata:Create(): create size=26, max=17
1.00819s PacketMetadata:Create(): create alloc size=26
1.00819s PacketMetadata:UpdateHead(0x7fcc12c4e1c0, 9)
1.00819s PacketMetadata:ReadItems(0x7fcc12c4e1c0, 17)
1.00819s PacketMetadata:ReadItems(0x7fcc12c4e1c0, 9)
1.00819s PacketMetadata:ReadItems(0x7fcc12c4e1c0, 0)
1.00819s Ipv4L3Protocol:SendRealOut(): Send via NetDevice ifIndex 0 ipv4InterfaceIndex 1
1.00819s Ipv4Interface:IsUp()
1.00819s Ipv4L3Protocol:SendRealOut(): Send to gateway 10.1.2.1
1.00819s PointToPointNetDevice:GetMtu()
1.00819s Ipv4Interface:Send(1.00819s ByteTagList:ByteTagList(0x7fff6d3ff890, 0x7fcc12c4e1a8)
10.1.2.1, 1.00819s Buffer:~Buffer(0x7fff6d3ff148)
1.00819s Buffer:~Buffer(0x7fff6d3ff398)
1.00819s PacketMetadata:ReadItems(0x7fff6d3ff8a8, 17)
1.00819s Buffer:RemoveAtStart(0x7fff6d3ff0b8, 0)
1.00819s Buffer:RemoveAtStart(): rem start=0, start=0, end=540, zero start=28, zero end=540, count=4, size=28, dirty start=0, dirty end=540
1.00819s Buffer:RemoveAtEnd(0x7fff6d3ff0b8, 520)
1.00819s Buffer:RemoveAtEnd(): rem end=520, start=0, end=20, zero start=20, zero end=20, count=4, size=28, dirty start=0, dirty end=540
1.00819s Buffer:~Buffer(0x7fff6d3ff0b8)
ns3::Ipv4Header (tos 0x0 ttl 64 id 0 protocol 17 offset (bytes) 0 flags [none] length: 540 10.1.2.2 > 10.1.1.2) 1.00819s PacketMetadata:ReadItems(0x7fff6d3ff8a8, 9)
1.00819s Buffer:RemoveAtStart(0x7fff6d3ff0b8, 20)
1.00819s Buffer:RemoveAtStart(): rem start=20, start=20, end=540, zero start=28, zero end=540, count=4, size=28, dirty start=0, dirty end=540
1.00819s Buffer:RemoveAtEnd(0x7fff6d3ff0b8, 512)
1.00819s Buffer:RemoveAtEnd(): rem end=512, start=20, end=28, zero start=28, zero end=28, count=4, size=28, dirty start=0, dirty end=540
1.00819s Buffer:~Buffer(0x7fff6d3ff0b8)
ns3::UdpHeader (length: 520 49153 > 50000) 1.00819s PacketMetadata:ReadItems(0x7fff6d3ff8a8, 0)
Payload (size=512)1.00819s Buffer:~Buffer(0x7fff6d3ff368)
1.00819s ByteTagList:~ByteTagList(0x7fff6d3ff890)
1.00819s ByteTagList:Deallocate(0x7fff6d3ff890, 0)
1.00819s Buffer:~Buffer(0x7fff6d3ff870)
)
1.00819s Ipv4Interface:IsUp()
1.00819s Ipv4InterfaceAddress:GetLocal()
1.00819s Ipv4Interface:Send(): Doesn't need ARP
1.00819s PointToPointNetDevice:Send()
1.00819s PointToPointNetDevice:Send(): p=0x7fcc12c4e180, dest=0x7fff6d3ffbe0
1.00819s PointToPointNetDevice:Send(): UID is 0
1.00819s PointToPointNetDevice:AddHeader()
1.00819s Packet:AddHeader(0x7fcc12c4e180, ns3::PppHeader, 2)
1.00819s Buffer:AddAtStart(0x7fcc12c4e188, 2)
1.00819s Buffer:AddAtStart(): add start=2, start=0, end=542, zero start=30, zero end=542, count=1, size=30, dirty start=0, dirty end=542
1.00819s ByteTagList:AddAtStart(0x7fcc12c4e1a8, 2, 2)
1.00819s ByteTagList:ByteTagList(0x7fff6d3feed8)
1.00819s ByteTagList:BeginAll(0x7fcc12c4e1a8)
1.00819s ByteTagList:Begin(0x7fcc12c4e1a8, 0, 2147483647)
1.00819s ByteTagList:operator=(0x7fcc12c4e1a8, 0x7fff6d3feed8)
1.00819s ByteTagList:Deallocate(0x7fcc12c4e1a8, 0)
1.00819s ByteTagList:~ByteTagList(0x7fff6d3feed8)
1.00819s ByteTagList:Deallocate(0x7fff6d3feed8, 0)
1.00819s PacketMetadata:ReadItems(0x7fcc12c4e1c0, 17)
1.00819s PacketMetadata:ReadItems(0x7fcc12c4e1c0, 9)
1.00819s PacketMetadata:ReadItems(0x7fcc12c4e1c0, 0)
1.00819s PacketMetadata:DoAddHeader(0x7fcc12c4e1c0, 352, 2)
1.00819s PacketMetadata:AddSmall(0x7fcc12c4e1c0, 17, 65535, 352, 2, 3)
1.00819s PacketMetadata:Create(): create size=35, max=26
1.00819s PacketMetadata:Create(): create alloc size=35
1.00819s PacketMetadata:Recycle(): recycle size=26, list=0
1.00819s PacketMetadata:UpdateHead(0x7fcc12c4e1c0, 9)
1.00819s PacketMetadata:ReadItems(0x7fcc12c4e1c0, 26)
1.00819s PacketMetadata:ReadItems(0x7fcc12c4e1c0, 17)
1.00819s PacketMetadata:ReadItems(0x7fcc12c4e1c0, 9)
1.00819s PacketMetadata:ReadItems(0x7fcc12c4e1c0, 0)
1.00819s Queue:Enqueue(0x7fcc12c49030, 0x7fcc12c4e180)
1.00819s DropTailQueue:DoEnqueue(0x7fcc12c49030, 0x7fcc12c4e180)
1.00819s DropTailQueue:DoEnqueue(): Number packets 1
1.00819s DropTailQueue:DoEnqueue(): Number bytes 542
1.00819s Queue:Enqueue(): m_traceEnqueue (p)
1.00819s TraceHelper:DefaultEnqueueSinkWithContext(0x7fcc12c4d860, 0x7fcc12c4e180)
1.00819s Buffer:~Buffer(0x7fff6d3fe958)
1.00819s Buffer:~Buffer(0x7fff6d3feba8)
1.00819s PacketMetadata:ReadItems(0x7fcc12c4e1c0, 26)
1.00819s Buffer:RemoveAtStart(0x7fff6d3fe8c8, 0)
1.00819s Buffer:RemoveAtStart(): rem start=0, start=0, end=542, zero start=30, zero end=542, count=3, size=30, dirty start=0, dirty end=542
1.00819s Buffer:RemoveAtEnd(0x7fff6d3fe8c8, 540)
1.00819s Buffer:RemoveAtEnd(): rem end=540, start=0, end=2, zero start=2, zero end=2, count=3, size=30, dirty start=0, dirty end=542
1.00819s Buffer:~Buffer(0x7fff6d3fe8c8)
1.00819s PacketMetadata:ReadItems(0x7fcc12c4e1c0, 17)
1.00819s Buffer:RemoveAtStart(0x7fff6d3fe8c8, 2)
1.00819s Buffer:RemoveAtStart(): rem start=2, start=2, end=542, zero start=30, zero end=542, count=3, size=30, dirty start=0, dirty end=542
1.00819s Buffer:RemoveAtEnd(0x7fff6d3fe8c8, 520)
1.00819s Buffer:RemoveAtEnd(): rem end=520, start=2, end=22, zero start=22, zero end=22, count=3, size=30, dirty start=0, dirty end=542
1.00819s Buffer:~Buffer(0x7fff6d3fe8c8)
1.00819s PacketMetadata:ReadItems(0x7fcc12c4e1c0, 9)
1.00819s Buffer:RemoveAtStart(0x7fff6d3fe8c8, 22)
1.00819s Buffer:RemoveAtStart(): rem start=22, start=22, end=542, zero start=30, zero end=542, count=3, size=30, dirty start=0, dirty end=542
1.00819s Buffer:RemoveAtEnd(0x7fff6d3fe8c8, 512)
1.00819s Buffer:RemoveAtEnd(): rem end=512, start=22, end=30, zero start=30, zero end=30, count=3, size=30, dirty start=0, dirty end=542
1.00819s Buffer:~Buffer(0x7fff6d3fe8c8)
1.00819s PacketMetadata:ReadItems(0x7fcc12c4e1c0, 0)
1.00819s Buffer:~Buffer(0x7fff6d3feb78)
+ 1.00819 /NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/Enqueue ns3::PppHeader (Point-to-Point Protocol: IP (0x0021)) ns3::Ipv4Header (tos 0x0 ttl 64 id 0 protocol 17 offset (bytes) 0 flags [none] length: 540 10.1.2.2 > 10.1.1.2) ns3::UdpHeader (length: 520 49153 > 50000) Payload (size=512)
1.00819s Queue:Dequeue(0x7fcc12c49030)
1.00819s DropTailQueue:DoDequeue(0x7fcc12c49030)
1.00819s DropTailQueue:DoDequeue(): Popped 0x7fcc12c4e180
1.00819s DropTailQueue:DoDequeue(): Number packets 0
1.00819s DropTailQueue:DoDequeue(): Number bytes 0
1.00819s Queue:Dequeue(): m_traceDequeue (packet)
1.00819s TraceHelper:DefaultDequeueSinkWithContext(0x7fcc12c4d860, 0x7fcc12c4e180)
1.00819s Buffer:~Buffer(0x7fff6d3fe8d8)
1.00819s Buffer:~Buffer(0x7fff6d3feb28)
1.00819s PacketMetadata:ReadItems(0x7fcc12c4e1c0, 26)
1.00819s Buffer:RemoveAtStart(0x7fff6d3fe848, 0)
1.00819s Buffer:RemoveAtStart(): rem start=0, start=0, end=542, zero start=30, zero end=542, count=3, size=30, dirty start=0, dirty end=542
1.00819s Buffer:RemoveAtEnd(0x7fff6d3fe848, 540)
1.00819s Buffer:RemoveAtEnd(): rem end=540, start=0, end=2, zero start=2, zero end=2, count=3, size=30, dirty start=0, dirty end=542
1.00819s Buffer:~Buffer(0x7fff6d3fe848)
1.00819s PacketMetadata:ReadItems(0x7fcc12c4e1c0, 17)
1.00819s Buffer:RemoveAtStart(0x7fff6d3fe848, 2)
1.00819s Buffer:RemoveAtStart(): rem start=2, start=2, end=542, zero start=30, zero end=542, count=3, size=30, dirty start=0, dirty end=542
1.00819s Buffer:RemoveAtEnd(0x7fff6d3fe848, 520)
1.00819s Buffer:RemoveAtEnd(): rem end=520, start=2, end=22, zero start=22, zero end=22, count=3, size=30, dirty start=0, dirty end=542
1.00819s Buffer:~Buffer(0x7fff6d3fe848)
1.00819s PacketMetadata:ReadItems(0x7fcc12c4e1c0, 9)
1.00819s Buffer:RemoveAtStart(0x7fff6d3fe848, 22)
1.00819s Buffer:RemoveAtStart(): rem start=22, start=22, end=542, zero start=30, zero end=542, count=3, size=30, dirty start=0, dirty end=542
1.00819s Buffer:RemoveAtEnd(0x7fff6d3fe848, 512)
1.00819s Buffer:RemoveAtEnd(): rem end=512, start=22, end=30, zero start=30, zero end=30, count=3, size=30, dirty start=0, dirty end=542
1.00819s Buffer:~Buffer(0x7fff6d3fe848)
1.00819s PacketMetadata:ReadItems(0x7fcc12c4e1c0, 0)
1.00819s Buffer:~Buffer(0x7fff6d3feaf8)
- 1.00819 /NodeList/2/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/Dequeue ns3::PppHeader (Point-to-Point Protocol: IP (0x0021)) ns3::Ipv4Header (tos 0x0 ttl 64 id 0 protocol 17 offset (bytes) 0 flags [none] length: 540 10.1.2.2 > 10.1.1.2) ns3::UdpHeader (length: 520 49153 > 50000) Payload (size=512)
1.00819s PointToPointNetDevice:TransmitStart(0x7fcc12c48c30, 0x7fcc12c4e180)
1.00819s PointToPointNetDevice:TransmitStart(): UID is 0)
1.00819s PointToPointNetDevice:TransmitStart(): Schedule TransmitCompleteEvent in 4.335e-06sec
1.00819s MapScheduler:Insert(0x7fcc12c47a10, 0x7fcc12c4e240, 1008196334, 27)
1.00819s PointToPointChannel:TransmitStart(0x7fcc12c49100, 0x7fcc12c4e180, 0x7fcc12c48c30)
1.00819s PointToPointChannel:TransmitStart(): UID is 0)
1.00819s DefaultSimulatorImpl:ScheduleWithContext(0x7fcc12c479b0, 0, 104335, 1008191999, 0x7fcc12c4e2b0)
1.00819s MapScheduler:Insert(0x7fcc12c47a10, 0x7fcc12c4e2b0, 1008296334, 28)
1.00819s PacketMetadata:Recycle(): recycle size=17, list=0
1.00819s ByteTagList:~ByteTagList(0x7fcc12c4e148)
1.00819s ByteTagList:Deallocate(0x7fcc12c4e148, 0)
1.00819s Buffer:~Buffer(0x7fcc12c4e128)
1.00819s Socket:NotifyDataSent()
1.00819s OnOffApplication:ScheduleNextTx()
1.00819s OnOffApplication:StopApplication()
1.00819s OnOffApplication:CancelEvents()
1.00819s Simulator:IsExpired(0x7fcc12c4db50)
1.00819s Simulator:Cancel(0x7fcc12c4db50)
1.00819s Simulator:Cancel(0x7fcc12c4db38)
1.00819s UdpSocketImpl:Close()
1.00819s PacketMetadata:Recycle(): recycle size=10, list=0
1.00819s ByteTagList:~ByteTagList(0x7fcc12c4e0e8)
1.00819s ByteTagList:Deallocate(0x7fcc12c4e0e8, 0)
1.00819s Buffer:~Buffer(0x7fcc12c4e0c8)
1.00819s MapScheduler:RemoveNext(0x7fcc12c47a10)
1.00819s MapScheduler:RemoveNext(): 0x7fcc12c47a100x7fcc12c4e240100819633427
1.00819s DefaultSimulatorImpl:ProcessOneEvent(): handle 1008196334
1.0082s PointToPointNetDevice:TransmitComplete()
1.0082s Queue:Dequeue(0x7fcc12c49030)
1.0082s DropTailQueue:DoDequeue(0x7fcc12c49030)
1.0082s DropTailQueue:DoDequeue(): Queue empty
1.0082s MapScheduler:RemoveNext(0x7fcc12c47a10)
1.0082s MapScheduler:RemoveNext(): 0x7fcc12c47a100x7fcc12c4e2b0100829633428
1.0082s DefaultSimulatorImpl:ProcessOneEvent(): handle 1008296334
1.0083s PointToPointNetDevice:Receive(0x7fcc12c48a00, 0x7fcc12c4e180)
1.0083s PointToPointNetDevice:ProcessHeader()
1.0083s Packet:RemoveHeader(0x7fcc12c4e180, ns3::PppHeader, 2)
1.0083s Buffer:RemoveAtStart(0x7fcc12c4e188, 2)
1.0083s Buffer:RemoveAtStart(): rem start=2, start=2, end=542, zero start=30, zero end=542, count=1, size=30, dirty start=0, dirty end=542
1.0083s PacketMetadata:RemoveHeader(0x7fcc12c4e1c0, 352, 2)
1.0083s PacketMetadata:ReadItems(0x7fcc12c4e1c0, 26)
1.0083s PacketMetadata:ReadItems(0x7fcc12c4e1c0, 17)
1.0083s PacketMetadata:ReadItems(0x7fcc12c4e1c0, 9)
1.0083s PacketMetadata:ReadItems(0x7fcc12c4e1c0, 0)
1.0083s PacketMetadata:ReadItems(0x7fcc12c4e1c0, 26)
1.0083s PacketMetadata:ReadItems(0x7fcc12c4e1c0, 17)
1.0083s PacketMetadata:ReadItems(0x7fcc12c4e1c0, 9)
1.0083s PacketMetadata:ReadItems(0x7fcc12c4e1c0, 0)
1.0083s TraceHelper:DefaultReceiveSinkWithContext(0x7fcc12c4d860, 0x7fcc12c4e180)
1.0083s Buffer:~Buffer(0x7fff6d401b98)
1.0083s Buffer:~Buffer(0x7fff6d401de8)
1.0083s PacketMetadata:ReadItems(0x7fcc12c4e1c0, 17)
1.0083s Buffer:RemoveAtStart(0x7fff6d401b08, 0)
1.0083s Buffer:RemoveAtStart(): rem start=0, start=2, end=542, zero start=30, zero end=542, count=3, size=30, dirty start=0, dirty end=542
1.0083s Buffer:RemoveAtEnd(0x7fff6d401b08, 520)
1.0083s Buffer:RemoveAtEnd(): rem end=520, start=2, end=22, zero start=22, zero end=22, count=3, size=30, dirty start=0, dirty end=542
1.0083s Buffer:~Buffer(0x7fff6d401b08)
1.0083s PacketMetadata:ReadItems(0x7fcc12c4e1c0, 9)
1.0083s Buffer:RemoveAtStart(0x7fff6d401b08, 20)
1.0083s Buffer:RemoveAtStart(): rem start=20, start=22, end=542, zero start=30, zero end=542, count=3, size=30, dirty start=0, dirty end=542
1.0083s Buffer:RemoveAtEnd(0x7fff6d401b08, 512)
1.0083s Buffer:RemoveAtEnd(): rem end=512, start=22, end=30, zero start=30, zero end=30, count=3, size=30, dirty start=0, dirty end=542
1.0083s Buffer:~Buffer(0x7fff6d401b08)
1.0083s PacketMetadata:ReadItems(0x7fcc12c4e1c0, 0)
1.0083s Buffer:~Buffer(0x7fff6d401db8)
r 1.0083 /NodeList/0/DeviceList/1/$ns3::PointToPointNetDevice/MacRx ns3::Ipv4Header (tos 0x0 ttl 64 id 0 protocol 17 offset (bytes) 0 flags [none] length: 540 10.1.2.2 > 10.1.1.2) ns3::UdpHeader (length: 520 49153 > 50000) Payload (size=512)
1.0083s PointToPointChannel:GetNDevices()
1.0083s PointToPointChannel:GetNDevices()
1.0083s PointToPointChannel:GetDevice()
1.0083s PointToPointChannel:GetPointToPointDevice()
1.0083s PointToPointChannel:GetNDevices()
1.0083s PointToPointChannel:GetDevice()
1.0083s PointToPointChannel:GetPointToPointDevice()
1.0083s Node:NonPromiscReceiveFromDevice(0x7fcc12c478e0)
1.0083s Node:ReceiveFromDevice(): Node 0 ReceiveFromDevice:  dev 1 (type=ns3::PointToPointNetDevice) Packet UID 0
1.0083s Ipv4L3Protocol:Receive(0x7fcc12c49540, 0x7fff6d401cd8, 0x7fcc12c4e180, 2048, 02-06-00:00:00:00:00:04)
1.0083s Ipv4L3Protocol:Receive(): Packet from 02-06-00:00:00:00:00:04 received on node 0
1.0083s ByteTagList:ByteTagList(0x7fcc12c4e268, 0x7fcc12c4e1a8)
1.0083s Ipv4Interface:IsUp()
1.0083s Packet:RemoveHeader(0x7fcc12c4e240, ns3::Ipv4Header, 20)
1.0083s Buffer:RemoveAtStart(0x7fcc12c4e248, 20)
1.0083s Buffer:RemoveAtStart(): rem start=20, start=22, end=542, zero start=30, zero end=542, count=2, size=30, dirty start=0, dirty end=542
1.0083s PacketMetadata:RemoveHeader(0x7fcc12c4e280, 250, 20)
1.0083s PacketMetadata:ReadItems(0x7fcc12c4e280, 17)
1.0083s PacketMetadata:ReadItems(0x7fcc12c4e280, 9)
1.0083s PacketMetadata:ReadItems(0x7fcc12c4e280, 0)
1.0083s PacketMetadata:ReadItems(0x7fcc12c4e280, 17)
1.0083s PacketMetadata:ReadItems(0x7fcc12c4e280, 9)
1.0083s PacketMetadata:ReadItems(0x7fcc12c4e280, 0)
1.0083s Ipv4ListRouting:RouteInput(0x7fcc12c4e240, tos 0x0 ttl 64 id 0 protocol 17 offset (bytes) 0 flags [none] length: 540 10.1.2.2 > 10.1.1.2, 0x7fcc12c48a00)
1.0083s Ipv4ListRouting:RouteInput(): RouteInput logic for node: 0
1.0083s Ipv4Interface:GetNAddresses()
1.0083s Ipv4Interface:GetAddress()
1.0083s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d400d98)
1.0083s Ipv4InterfaceAddress:GetLocal()
1.0083s Ipv4InterfaceAddress:GetBroadcast()
1.0083s Ipv4Interface:GetNAddresses()
1.0083s Ipv4Interface:GetNAddresses()
1.0083s Ipv4Interface:GetAddress()
1.0083s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d400d38)
1.0083s Ipv4InterfaceAddress:GetLocal()
1.0083s Ipv4InterfaceAddress:GetBroadcast()
1.0083s Ipv4Interface:GetNAddresses()
1.0083s Ipv4Interface:GetNAddresses()
1.0083s Ipv4Interface:GetAddress()
1.0083s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d400d38)
1.0083s Ipv4InterfaceAddress:GetLocal()
1.0083s Ipv4InterfaceAddress:GetBroadcast()
1.0083s Ipv4Interface:GetNAddresses()
1.0083s Ipv4L3Protocol:IsForwarding(0x7fcc12c49540, 2)
1.0083s Ipv4L3Protocol:IsForwarding(): 1.0083s Ipv4Interface:IsForwarding()
Forwarding state: 1
1.0083s Ipv4Interface:IsForwarding()
1.0083s 1.0083 [node 0] Ipv4StaticRouting:RouteInput(0x7fcc12c49a70, 0x7fcc12c4e240, tos 0x0 ttl 64 id 0 protocol 17 offset (bytes) 0 flags [none] length: 540 10.1.2.2 > 10.1.1.2, 10.1.2.2, 10.1.1.2, 0x7fcc12c48a00)
1.0083s 1.0083 [node 0] Ipv4StaticRouting:RouteInput(): Unicast destination
1.0083s Ipv4Interface:GetNAddresses()
1.0083s Ipv4Interface:GetAddress()
1.0083s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d400580)
1.0083s Ipv4InterfaceAddress:GetLocal()
1.0083s Ipv4InterfaceAddress:GetBroadcast()
1.0083s 1.0083 [node 0] Ipv4StaticRouting:RouteInput(): Address 127.0.0.1 not a match
1.0083s Ipv4Interface:GetNAddresses()
1.0083s Ipv4Interface:GetNAddresses()
1.0083s Ipv4Interface:GetAddress()
1.0083s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d400580)
1.0083s Ipv4InterfaceAddress:GetLocal()
1.0083s Ipv4InterfaceAddress:GetBroadcast()
1.0083s 1.0083 [node 0] Ipv4StaticRouting:RouteInput(): Address 10.1.1.1 not a match
1.0083s Ipv4Interface:GetNAddresses()
1.0083s Ipv4Interface:GetNAddresses()
1.0083s Ipv4Interface:GetAddress()
1.0083s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d400580)
1.0083s Ipv4InterfaceAddress:GetLocal()
1.0083s Ipv4InterfaceAddress:GetBroadcast()
1.0083s 1.0083 [node 0] Ipv4StaticRouting:RouteInput(): Address 10.1.2.1 not a match
1.0083s Ipv4Interface:GetNAddresses()
1.0083s Ipv4L3Protocol:IsForwarding(0x7fcc12c49540, 2)
1.0083s Ipv4L3Protocol:IsForwarding(): 1.0083s Ipv4Interface:IsForwarding()
Forwarding state: 1
1.0083s Ipv4Interface:IsForwarding()
1.0083s 1.0083 [node 0] Ipv4StaticRouting:LookupStatic(0x7fcc12c49a70, 10.1.1.2,  , 0)
1.0083s 1.0083 [node 0] Ipv4StaticRouting:LookupStatic(): Searching for route to 10.1.1.2, checking against route to 127.0.0.0/8
1.0083s 1.0083 [node 0] Ipv4StaticRouting:LookupStatic(): Searching for route to 10.1.1.2, checking against route to 10.1.1.0/24
1.0083s 1.0083 [node 0] Ipv4StaticRouting:LookupStatic(): Found global network route 0x7fcc12c26a30, mask length 24, metric 0
1.0083s 1.0083 [node 0] Ipv4StaticRouting:SourceAddressSelection(0x7fcc12c49a70, 1,  , 10.1.1.0)
1.0083s Ipv4Interface:GetNAddresses()
1.0083s Ipv4Interface:GetAddress()
1.0083s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d3ff3d8)
1.0083s Ipv4InterfaceAddress:GetLocal()
1.0083s 1.0083 [node 0] Ipv4StaticRouting:LookupStatic(): Searching for route to 10.1.1.2, checking against route to 10.1.2.0/24
1.0083s 1.0083 [node 0] Ipv4StaticRouting:LookupStatic(): Matching route via 0.0.0.0 at the end
1.0083s 1.0083 [node 0] Ipv4StaticRouting:RouteInput(): Found unicast destination- calling unicast callback
1.0083s Ipv4L3Protocol:IpForward(0x7fcc12c49540, 0x7fcc12c48740, 0x7fcc12c4e240, tos 0x0 ttl 64 id 0 protocol 17 offset (bytes) 0 flags [none] length: 540 10.1.2.2 > 10.1.1.2)
1.0083s Ipv4L3Protocol:IpForward(): Forwarding logic for node: 0
1.0083s ByteTagList:ByteTagList(0x7fcc12c4e118, 0x7fcc12c4e268)
1.0083s Ipv4L3Protocol:SendRealOut(0x7fcc12c49540, 0x7fcc12c4e0f0, 0x7fff6d3ffb90)
1.0083s Packet:AddHeader(0x7fcc12c4e0f0, ns3::Ipv4Header, 20)
1.0083s Buffer:AddAtStart(0x7fcc12c4e0f8, 20)
1.0083s Buffer:AddAtStart(): add start=20, start=0, end=540, zero start=28, zero end=540, count=1, size=28, dirty start=0, dirty end=540
1.0083s ByteTagList:AddAtStart(0x7fcc12c4e118, -2, 20)
1.0083s ByteTagList:ByteTagList(0x7fff6d3fec68)
1.0083s ByteTagList:BeginAll(0x7fcc12c4e118)
1.0083s ByteTagList:Begin(0x7fcc12c4e118, 0, 2147483647)
1.0083s ByteTagList:operator=(0x7fcc12c4e118, 0x7fff6d3fec68)
1.0083s ByteTagList:Deallocate(0x7fcc12c4e118, 0)
1.0083s ByteTagList:~ByteTagList(0x7fff6d3fec68)
1.0083s ByteTagList:Deallocate(0x7fff6d3fec68, 0)
1.0083s PacketMetadata:ReadItems(0x7fcc12c4e130, 9)
1.0083s PacketMetadata:ReadItems(0x7fcc12c4e130, 0)
1.0083s PacketMetadata:DoAddHeader(0x7fcc12c4e130, 250, 20)
1.0083s PacketMetadata:AddSmall(0x7fcc12c4e130, 9, 65535, 250, 20, 4)
1.0083s PacketMetadata:Create(): create size=26, max=35
1.0083s PacketMetadata:Create(): create alloc size=35
1.0083s PacketMetadata:UpdateHead(0x7fcc12c4e130, 9)
1.0083s PacketMetadata:ReadItems(0x7fcc12c4e130, 17)
1.0083s PacketMetadata:ReadItems(0x7fcc12c4e130, 9)
1.0083s PacketMetadata:ReadItems(0x7fcc12c4e130, 0)
1.0083s Ipv4L3Protocol:SendRealOut(): Send via NetDevice ifIndex 0 ipv4InterfaceIndex 1
1.0083s Ipv4Interface:IsUp()
1.0083s Ipv4L3Protocol:SendRealOut(): Send to destination 10.1.1.2
1.0083s PointToPointNetDevice:GetMtu()
1.0083s Ipv4Interface:Send(1.0083s ByteTagList:ByteTagList(0x7fff6d3febc0, 0x7fcc12c4e118)
10.1.1.2, 1.0083s Buffer:~Buffer(0x7fff6d3fe478)
1.0083s Buffer:~Buffer(0x7fff6d3fe6c8)
1.0083s PacketMetadata:ReadItems(0x7fff6d3febd8, 17)
1.0083s Buffer:RemoveAtStart(0x7fff6d3fe3e8, 0)
1.0083s Buffer:RemoveAtStart(): rem start=0, start=0, end=540, zero start=28, zero end=540, count=4, size=28, dirty start=0, dirty end=540
1.0083s Buffer:RemoveAtEnd(0x7fff6d3fe3e8, 520)
1.0083s Buffer:RemoveAtEnd(): rem end=520, start=0, end=20, zero start=20, zero end=20, count=4, size=28, dirty start=0, dirty end=540
1.0083s Buffer:~Buffer(0x7fff6d3fe3e8)
ns3::Ipv4Header (tos 0x0 ttl 63 id 0 protocol 17 offset (bytes) 0 flags [none] length: 540 10.1.2.2 > 10.1.1.2) 1.0083s PacketMetadata:ReadItems(0x7fff6d3febd8, 9)
1.0083s Buffer:RemoveAtStart(0x7fff6d3fe3e8, 20)
1.0083s Buffer:RemoveAtStart(): rem start=20, start=20, end=540, zero start=28, zero end=540, count=4, size=28, dirty start=0, dirty end=540
1.0083s Buffer:RemoveAtEnd(0x7fff6d3fe3e8, 512)
1.0083s Buffer:RemoveAtEnd(): rem end=512, start=20, end=28, zero start=28, zero end=28, count=4, size=28, dirty start=0, dirty end=540
1.0083s Buffer:~Buffer(0x7fff6d3fe3e8)
ns3::UdpHeader (length: 520 49153 > 50000) 1.0083s PacketMetadata:ReadItems(0x7fff6d3febd8, 0)
Payload (size=512)1.0083s Buffer:~Buffer(0x7fff6d3fe698)
1.0083s ByteTagList:~ByteTagList(0x7fff6d3febc0)
1.0083s ByteTagList:Deallocate(0x7fff6d3febc0, 0)
1.0083s Buffer:~Buffer(0x7fff6d3feba0)
)
1.0083s Ipv4Interface:IsUp()
1.0083s Ipv4InterfaceAddress:GetLocal()
1.0083s Ipv4Interface:Send(): Doesn't need ARP
1.0083s PointToPointNetDevice:Send()
1.0083s PointToPointNetDevice:Send(): p=0x7fcc12c4e0f0, dest=0x7fff6d3fef10
1.0083s PointToPointNetDevice:Send(): UID is 0
1.0083s PointToPointNetDevice:AddHeader()
1.0083s Packet:AddHeader(0x7fcc12c4e0f0, ns3::PppHeader, 2)
1.0083s Buffer:AddAtStart(0x7fcc12c4e0f8, 2)
1.0083s Buffer:AddAtStart(): add start=2, start=0, end=542, zero start=30, zero end=542, count=1, size=30, dirty start=0, dirty end=542
1.0083s ByteTagList:AddAtStart(0x7fcc12c4e118, 2, 2)
1.0083s ByteTagList:ByteTagList(0x7fff6d3fe208)
1.0083s ByteTagList:BeginAll(0x7fcc12c4e118)
1.0083s ByteTagList:Begin(0x7fcc12c4e118, 0, 2147483647)
1.0083s ByteTagList:operator=(0x7fcc12c4e118, 0x7fff6d3fe208)
1.0083s ByteTagList:Deallocate(0x7fcc12c4e118, 0)
1.0083s ByteTagList:~ByteTagList(0x7fff6d3fe208)
1.0083s ByteTagList:Deallocate(0x7fff6d3fe208, 0)
1.0083s PacketMetadata:ReadItems(0x7fcc12c4e130, 17)
1.0083s PacketMetadata:ReadItems(0x7fcc12c4e130, 9)
1.0083s PacketMetadata:ReadItems(0x7fcc12c4e130, 0)
1.0083s PacketMetadata:DoAddHeader(0x7fcc12c4e130, 352, 2)
1.0083s PacketMetadata:AddSmall(0x7fcc12c4e130, 17, 65535, 352, 2, 5)
1.0083s PacketMetadata:UpdateHead(0x7fcc12c4e130, 9)
1.0083s PacketMetadata:ReadItems(0x7fcc12c4e130, 26)
1.0083s PacketMetadata:ReadItems(0x7fcc12c4e130, 17)
1.0083s PacketMetadata:ReadItems(0x7fcc12c4e130, 9)
1.0083s PacketMetadata:ReadItems(0x7fcc12c4e130, 0)
1.0083s Queue:Enqueue(0x7fcc12c47fa0, 0x7fcc12c4e0f0)
1.0083s DropTailQueue:DoEnqueue(0x7fcc12c47fa0, 0x7fcc12c4e0f0)
1.0083s DropTailQueue:DoEnqueue(): Number packets 1
1.0083s DropTailQueue:DoEnqueue(): Number bytes 542
1.0083s Queue:Enqueue(): m_traceEnqueue (p)
1.0083s TraceHelper:DefaultEnqueueSinkWithContext(0x7fcc12c4d860, 0x7fcc12c4e0f0)
1.0083s Buffer:~Buffer(0x7fff6d3fdc88)
1.0083s Buffer:~Buffer(0x7fff6d3fded8)
1.0083s PacketMetadata:ReadItems(0x7fcc12c4e130, 26)
1.0083s Buffer:RemoveAtStart(0x7fff6d3fdbf8, 0)
1.0083s Buffer:RemoveAtStart(): rem start=0, start=0, end=542, zero start=30, zero end=542, count=3, size=30, dirty start=0, dirty end=542
1.0083s Buffer:RemoveAtEnd(0x7fff6d3fdbf8, 540)
1.0083s Buffer:RemoveAtEnd(): rem end=540, start=0, end=2, zero start=2, zero end=2, count=3, size=30, dirty start=0, dirty end=542
1.0083s Buffer:~Buffer(0x7fff6d3fdbf8)
1.0083s PacketMetadata:ReadItems(0x7fcc12c4e130, 17)
1.0083s Buffer:RemoveAtStart(0x7fff6d3fdbf8, 2)
1.0083s Buffer:RemoveAtStart(): rem start=2, start=2, end=542, zero start=30, zero end=542, count=3, size=30, dirty start=0, dirty end=542
1.0083s Buffer:RemoveAtEnd(0x7fff6d3fdbf8, 520)
1.0083s Buffer:RemoveAtEnd(): rem end=520, start=2, end=22, zero start=22, zero end=22, count=3, size=30, dirty start=0, dirty end=542
1.0083s Buffer:~Buffer(0x7fff6d3fdbf8)
1.0083s PacketMetadata:ReadItems(0x7fcc12c4e130, 9)
1.0083s Buffer:RemoveAtStart(0x7fff6d3fdbf8, 22)
1.0083s Buffer:RemoveAtStart(): rem start=22, start=22, end=542, zero start=30, zero end=542, count=3, size=30, dirty start=0, dirty end=542
1.0083s Buffer:RemoveAtEnd(0x7fff6d3fdbf8, 512)
1.0083s Buffer:RemoveAtEnd(): rem end=512, start=22, end=30, zero start=30, zero end=30, count=3, size=30, dirty start=0, dirty end=542
1.0083s Buffer:~Buffer(0x7fff6d3fdbf8)
1.0083s PacketMetadata:ReadItems(0x7fcc12c4e130, 0)
1.0083s Buffer:~Buffer(0x7fff6d3fdea8)
+ 1.0083 /NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/Enqueue ns3::PppHeader (Point-to-Point Protocol: IP (0x0021)) ns3::Ipv4Header (tos 0x0 ttl 63 id 0 protocol 17 offset (bytes) 0 flags [none] length: 540 10.1.2.2 > 10.1.1.2) ns3::UdpHeader (length: 520 49153 > 50000) Payload (size=512)
1.0083s Queue:Dequeue(0x7fcc12c47fa0)
1.0083s DropTailQueue:DoDequeue(0x7fcc12c47fa0)
1.0083s DropTailQueue:DoDequeue(): Popped 0x7fcc12c4e0f0
1.0083s DropTailQueue:DoDequeue(): Number packets 0
1.0083s DropTailQueue:DoDequeue(): Number bytes 0
1.0083s Queue:Dequeue(): m_traceDequeue (packet)
1.0083s TraceHelper:DefaultDequeueSinkWithContext(0x7fcc12c4d860, 0x7fcc12c4e0f0)
1.0083s Buffer:~Buffer(0x7fff6d3fdc08)
1.0083s Buffer:~Buffer(0x7fff6d3fde58)
1.0083s PacketMetadata:ReadItems(0x7fcc12c4e130, 26)
1.0083s Buffer:RemoveAtStart(0x7fff6d3fdb78, 0)
1.0083s Buffer:RemoveAtStart(): rem start=0, start=0, end=542, zero start=30, zero end=542, count=3, size=30, dirty start=0, dirty end=542
1.0083s Buffer:RemoveAtEnd(0x7fff6d3fdb78, 540)
1.0083s Buffer:RemoveAtEnd(): rem end=540, start=0, end=2, zero start=2, zero end=2, count=3, size=30, dirty start=0, dirty end=542
1.0083s Buffer:~Buffer(0x7fff6d3fdb78)
1.0083s PacketMetadata:ReadItems(0x7fcc12c4e130, 17)
1.0083s Buffer:RemoveAtStart(0x7fff6d3fdb78, 2)
1.0083s Buffer:RemoveAtStart(): rem start=2, start=2, end=542, zero start=30, zero end=542, count=3, size=30, dirty start=0, dirty end=542
1.0083s Buffer:RemoveAtEnd(0x7fff6d3fdb78, 520)
1.0083s Buffer:RemoveAtEnd(): rem end=520, start=2, end=22, zero start=22, zero end=22, count=3, size=30, dirty start=0, dirty end=542
1.0083s Buffer:~Buffer(0x7fff6d3fdb78)
1.0083s PacketMetadata:ReadItems(0x7fcc12c4e130, 9)
1.0083s Buffer:RemoveAtStart(0x7fff6d3fdb78, 22)
1.0083s Buffer:RemoveAtStart(): rem start=22, start=22, end=542, zero start=30, zero end=542, count=3, size=30, dirty start=0, dirty end=542
1.0083s Buffer:RemoveAtEnd(0x7fff6d3fdb78, 512)
1.0083s Buffer:RemoveAtEnd(): rem end=512, start=22, end=30, zero start=30, zero end=30, count=3, size=30, dirty start=0, dirty end=542
1.0083s Buffer:~Buffer(0x7fff6d3fdb78)
1.0083s PacketMetadata:ReadItems(0x7fcc12c4e130, 0)
1.0083s Buffer:~Buffer(0x7fff6d3fde28)
- 1.0083 /NodeList/0/DeviceList/0/$ns3::PointToPointNetDevice/TxQueue/Dequeue ns3::PppHeader (Point-to-Point Protocol: IP (0x0021)) ns3::Ipv4Header (tos 0x0 ttl 63 id 0 protocol 17 offset (bytes) 0 flags [none] length: 540 10.1.2.2 > 10.1.1.2) ns3::UdpHeader (length: 520 49153 > 50000) Payload (size=512)
1.0083s PointToPointNetDevice:TransmitStart(0x7fcc12c47da0, 0x7fcc12c4e0f0)
1.0083s PointToPointNetDevice:TransmitStart(): UID is 0)
1.0083s PointToPointNetDevice:TransmitStart(): Schedule TransmitCompleteEvent in 4.335e-06sec
1.0083s MapScheduler:Insert(0x7fcc12c47a10, 0x7fcc12c4e150, 1008300669, 29)
1.0083s PointToPointChannel:TransmitStart(0x7fcc12c485c0, 0x7fcc12c4e0f0, 0x7fcc12c47da0)
1.0083s PointToPointChannel:TransmitStart(): UID is 0)
1.0083s DefaultSimulatorImpl:ScheduleWithContext(0x7fcc12c479b0, 1, 104335, 1008296334, 0x7fcc12c4f8d0)
1.0083s MapScheduler:Insert(0x7fcc12c47a10, 0x7fcc12c4f8d0, 1008400669, 30)
1.0083s Ipv4ListRouting:RouteInput(): Route found to forward packet in protocol ns3::Ipv4StaticRouting
1.0083s ByteTagList:~ByteTagList(0x7fcc12c4e268)
1.0083s ByteTagList:Deallocate(0x7fcc12c4e268, 0)
1.0083s Buffer:~Buffer(0x7fcc12c4e248)
1.0083s PacketMetadata:Recycle(): recycle size=35, list=0
1.0083s ByteTagList:~ByteTagList(0x7fcc12c4e1a8)
1.0083s ByteTagList:Deallocate(0x7fcc12c4e1a8, 0)
1.0083s Buffer:~Buffer(0x7fcc12c4e188)
1.0083s MapScheduler:RemoveNext(0x7fcc12c47a10)
1.0083s MapScheduler:RemoveNext(): 0x7fcc12c47a100x7fcc12c4e150100830066929
1.0083s DefaultSimulatorImpl:ProcessOneEvent(): handle 1008300669
1.0083s PointToPointNetDevice:TransmitComplete()
1.0083s Queue:Dequeue(0x7fcc12c47fa0)
1.0083s DropTailQueue:DoDequeue(0x7fcc12c47fa0)
1.0083s DropTailQueue:DoDequeue(): Queue empty
1.0083s MapScheduler:RemoveNext(0x7fcc12c47a10)
1.0083s MapScheduler:RemoveNext(): 0x7fcc12c47a100x7fcc12c4f8d0100840066930
1.0083s DefaultSimulatorImpl:ProcessOneEvent(): handle 1008400669
1.0084s PointToPointNetDevice:Receive(0x7fcc12c480b0, 0x7fcc12c4e0f0)
1.0084s PointToPointNetDevice:ProcessHeader()
1.0084s Packet:RemoveHeader(0x7fcc12c4e0f0, ns3::PppHeader, 2)
1.0084s Buffer:RemoveAtStart(0x7fcc12c4e0f8, 2)
1.0084s Buffer:RemoveAtStart(): rem start=2, start=2, end=542, zero start=30, zero end=542, count=1, size=30, dirty start=0, dirty end=542
1.0084s PacketMetadata:RemoveHeader(0x7fcc12c4e130, 352, 2)
1.0084s PacketMetadata:ReadItems(0x7fcc12c4e130, 26)
1.0084s PacketMetadata:ReadItems(0x7fcc12c4e130, 17)
1.0084s PacketMetadata:ReadItems(0x7fcc12c4e130, 9)
1.0084s PacketMetadata:ReadItems(0x7fcc12c4e130, 0)
1.0084s PacketMetadata:ReadItems(0x7fcc12c4e130, 26)
1.0084s PacketMetadata:ReadItems(0x7fcc12c4e130, 17)
1.0084s PacketMetadata:ReadItems(0x7fcc12c4e130, 9)
1.0084s PacketMetadata:ReadItems(0x7fcc12c4e130, 0)
1.0084s TraceHelper:DefaultReceiveSinkWithContext(0x7fcc12c4d860, 0x7fcc12c4e0f0)
1.0084s Buffer:~Buffer(0x7fff6d401b98)
1.0084s Buffer:~Buffer(0x7fff6d401de8)
1.0084s PacketMetadata:ReadItems(0x7fcc12c4e130, 17)
1.0084s Buffer:RemoveAtStart(0x7fff6d401b08, 0)
1.0084s Buffer:RemoveAtStart(): rem start=0, start=2, end=542, zero start=30, zero end=542, count=3, size=30, dirty start=0, dirty end=542
1.0084s Buffer:RemoveAtEnd(0x7fff6d401b08, 520)
1.0084s Buffer:RemoveAtEnd(): rem end=520, start=2, end=22, zero start=22, zero end=22, count=3, size=30, dirty start=0, dirty end=542
1.0084s Buffer:~Buffer(0x7fff6d401b08)
1.0084s PacketMetadata:ReadItems(0x7fcc12c4e130, 9)
1.0084s Buffer:RemoveAtStart(0x7fff6d401b08, 20)
1.0084s Buffer:RemoveAtStart(): rem start=20, start=22, end=542, zero start=30, zero end=542, count=3, size=30, dirty start=0, dirty end=542
1.0084s Buffer:RemoveAtEnd(0x7fff6d401b08, 512)
1.0084s Buffer:RemoveAtEnd(): rem end=512, start=22, end=30, zero start=30, zero end=30, count=3, size=30, dirty start=0, dirty end=542
1.0084s Buffer:~Buffer(0x7fff6d401b08)
1.0084s PacketMetadata:ReadItems(0x7fcc12c4e130, 0)
1.0084s Buffer:~Buffer(0x7fff6d401db8)
r 1.0084 /NodeList/1/DeviceList/0/$ns3::PointToPointNetDevice/MacRx ns3::Ipv4Header (tos 0x0 ttl 63 id 0 protocol 17 offset (bytes) 0 flags [none] length: 540 10.1.2.2 > 10.1.1.2) ns3::UdpHeader (length: 520 49153 > 50000) Payload (size=512)
1.0084s PointToPointChannel:GetNDevices()
1.0084s PointToPointChannel:GetNDevices()
1.0084s PointToPointChannel:GetDevice()
1.0084s PointToPointChannel:GetPointToPointDevice()
1.0084s Node:NonPromiscReceiveFromDevice(0x7fcc12c47b40)
1.0084s Node:ReceiveFromDevice(): Node 1 ReceiveFromDevice:  dev 0 (type=ns3::PointToPointNetDevice) Packet UID 0
1.0084s Ipv4L3Protocol:Receive(0x7fcc12c4a4e0, 0x7fff6d401cd8, 0x7fcc12c4e0f0, 2048, 02-06-00:00:00:00:00:01)
1.0084s Ipv4L3Protocol:Receive(): Packet from 02-06-00:00:00:00:00:01 received on node 1
1.0084s ByteTagList:ByteTagList(0x7fcc12c4e2d8, 0x7fcc12c4e118)
1.0084s Ipv4Interface:IsUp()
1.0084s Packet:RemoveHeader(0x7fcc12c4e2b0, ns3::Ipv4Header, 20)
1.0084s Buffer:RemoveAtStart(0x7fcc12c4e2b8, 20)
1.0084s Buffer:RemoveAtStart(): rem start=20, start=22, end=542, zero start=30, zero end=542, count=2, size=30, dirty start=0, dirty end=542
1.0084s PacketMetadata:RemoveHeader(0x7fcc12c4e2f0, 250, 20)
1.0084s PacketMetadata:ReadItems(0x7fcc12c4e2f0, 17)
1.0084s PacketMetadata:ReadItems(0x7fcc12c4e2f0, 9)
1.0084s PacketMetadata:ReadItems(0x7fcc12c4e2f0, 0)
1.0084s PacketMetadata:ReadItems(0x7fcc12c4e2f0, 17)
1.0084s PacketMetadata:ReadItems(0x7fcc12c4e2f0, 9)
1.0084s PacketMetadata:ReadItems(0x7fcc12c4e2f0, 0)
1.0084s Ipv4ListRouting:RouteInput(0x7fcc12c4e2b0, tos 0x0 ttl 63 id 0 protocol 17 offset (bytes) 0 flags [none] length: 540 10.1.2.2 > 10.1.1.2, 0x7fcc12c480b0)
1.0084s Ipv4ListRouting:RouteInput(): RouteInput logic for node: 1
1.0084s Ipv4Interface:GetNAddresses()
1.0084s Ipv4Interface:GetAddress()
1.0084s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d400d98)
1.0084s Ipv4InterfaceAddress:GetLocal()
1.0084s Ipv4L3Protocol:IsDestinationAddress(): For me (destination 10.1.1.2 match)
1.0084s Ipv4ListRouting:RouteInput(): Address 10.1.1.2 is a match for local delivery
1.0084s Ipv4L3Protocol:LocalDeliver(0x7fcc12c4a4e0, 0x7fcc12c4e2b0, 0x7fff6d4017b8)
1.0084s ByteTagList:ByteTagList(0x7fcc12c4e178, 0x7fcc12c4e2d8)
1.0084s ByteTagList:ByteTagList(0x7fcc12c4e1d8, 0x7fcc12c4e178)
1.0084s UdpL4Protocol:Receive(0x7fcc12c4a8d0, 0x7fcc12c4e150, tos 0x0 ttl 63 id 0 protocol 17 offset (bytes) 0 flags [none] length: 540 10.1.2.2 > 10.1.1.2)
1.0084s Packet:RemoveHeader(0x7fcc12c4e150, ns3::UdpHeader, 8)
1.0084s Buffer:RemoveAtStart(0x7fcc12c4e158, 8)
1.0084s Buffer:RemoveAtStart(): rem start=8, start=30, end=542, zero start=30, zero end=542, count=4, size=30, dirty start=0, dirty end=542
1.0084s PacketMetadata:RemoveHeader(0x7fcc12c4e190, 98, 8)
1.0084s PacketMetadata:ReadItems(0x7fcc12c4e190, 9)
1.0084s PacketMetadata:ReadItems(0x7fcc12c4e190, 0)
1.0084s PacketMetadata:ReadItems(0x7fcc12c4e190, 9)
1.0084s PacketMetadata:ReadItems(0x7fcc12c4e190, 0)
1.0084s UdpL4Protocol:Receive(): Looking up dst 10.1.1.2 port 50000
1.0084s Ipv4EndPointDemux:Lookup()
1.0084s Ipv4EndPointDemux:Lookup(0x7fcc12c4a620, 10.1.1.2, 50000, 10.1.2.2, 49153, 0x7fcc12c4ca00)
1.0084s Ipv4EndPointDemux:Lookup(): Looking up endpoint for destination address 10.1.1.2
1.0084s Ipv4EndPointDemux:Lookup(): Looking at endpoint dport=50000 daddr=0.0.0.0 sport=0 saddr=0.0.0.0
1.0084s Ipv4Interface:GetNAddresses()
1.0084s Ipv4Interface:GetAddress()
1.0084s Ipv4InterfaceAddress:Ipv4InterfaceAddress(0x7fff6d3ffe48)
1.0084s Ipv4InterfaceAddress:GetMask()
1.0084s Ipv4InterfaceAddress:GetMask()
1.0084s Ipv4InterfaceAddress:GetLocal()
1.0084s Ipv4InterfaceAddress:GetMask()
1.0084s Ipv4Interface:GetNAddresses()
1.0084s Ipv4EndPointDemux:Lookup(): dest addr 10.1.1.2 broadcast? 0
1.0084s ByteTagList:ByteTagList(0x7fcc12c4e238, 0x7fcc12c4e178)
1.0084s MapScheduler:Insert(0x7fcc12c47a10, 0x7fcc12c4f940, 1008400669, 31)
1.0084s ByteTagList:~ByteTagList(0x7fcc12c4e1d8)
1.0084s ByteTagList:Deallocate(0x7fcc12c4e1d8, 0)
1.0084s Buffer:~Buffer(0x7fcc12c4e1b8)
1.0084s ByteTagList:~ByteTagList(0x7fcc12c4e178)
1.0084s ByteTagList:Deallocate(0x7fcc12c4e178, 0)
1.0084s Buffer:~Buffer(0x7fcc12c4e158)
1.0084s ByteTagList:~ByteTagList(0x7fcc12c4e2d8)
1.0084s ByteTagList:Deallocate(0x7fcc12c4e2d8, 0)
1.0084s Buffer:~Buffer(0x7fcc12c4e2b8)
1.0084s ByteTagList:~ByteTagList(0x7fcc12c4e118)
1.0084s ByteTagList:Deallocate(0x7fcc12c4e118, 0)
1.0084s Buffer:~Buffer(0x7fcc12c4e0f8)
1.0084s MapScheduler:RemoveNext(0x7fcc12c47a10)
1.0084s MapScheduler:RemoveNext(): 0x7fcc12c47a100x7fcc12c4f940100840066931
1.0084s DefaultSimulatorImpl:ProcessOneEvent(): handle 1008400669
1.0084s UdpSocketImpl:ForwardUp(0x7fcc12c4f440, 0x7fcc12c4e210, tos 0x0 ttl 63 id 0 protocol 17 offset (bytes) 0 flags [none] length: 540 10.1.2.2 > 10.1.1.2, 49153)
1.0084s Packet:AddPacketTag(0x7fcc12c4e210, ns3::SocketAddressTag, 8)
1.0084s PacketTagList:Add(0x7fcc12c4e248, ns3::SocketAddressTag)
1.0084s PacketTagList:AllocData()
1.0084s Socket:NotifyDataRecv()
1.0084s PacketSink:HandleRead(0x7fcc12c4d640, 0x7fcc12c4f440)
1.0084s Socket:RecvFrom()
1.0084s UdpSocketImpl:RecvFrom(0x7fcc12c4f440, 4294967295, 0)
1.0084s UdpSocketImpl:Recv(0x7fcc12c4f440, 4294967295, 0)
1.0084s PacketTagList:Peek(0x7fcc12c4e248, ns3::SocketAddressTag)
1.0084s PacketSink:HandleRead(): Received 512 bytes from 10.1.2.2 [03-06-0a:01:02:02:01:c0] total Rx 512
1.0084s Socket:RecvFrom()
1.0084s UdpSocketImpl:RecvFrom(0x7fcc12c4f440, 4294967295, 0)
1.0084s UdpSocketImpl:Recv(0x7fcc12c4f440, 4294967295, 0)
1.0084s PacketMetadata:Recycle(): recycle size=35, list=1
1.0084s PacketTagList:FreeData(0x7fcc12c4e270)
1.0084s PacketTagList:FreeData(0x7fcc12c4de40)
1.0084s ByteTagList:~ByteTagList(0x7fcc12c4e238)
1.0084s ByteTagList:Deallocate(0x7fcc12c4e238, 0)
1.0084s Buffer:~Buffer(0x7fcc12c4e218)
1.0084s MapScheduler:RemoveNext(0x7fcc12c47a10)
1.0084s MapScheduler:RemoveNext(): 0x7fcc12c47a100x7fcc12c4de80200000000026
1.0084s DefaultSimulatorImpl:ProcessOneEvent(): handle 2000000000
2s MapScheduler:RemoveNext(0x7fcc12c47a10)
2s MapScheduler:RemoveNext(): 0x7fcc12c47a100x7fcc12c4ec701000000000021
2s DefaultSimulatorImpl:ProcessOneEvent(): handle 10000000000
10s PacketSink:StopApplication(0x7fcc12c4d640)
10s UdpSocketImpl:Close()
10s Socket:SetRecvCallback()
10s MapScheduler:RemoveNext(0x7fcc12c47a10)
10s MapScheduler:RemoveNext(): 0x7fcc12c47a100x7fcc12c4ef801000000000023
10s DefaultSimulatorImpl:ProcessOneEvent(): handle 10000000000
10s OnOffApplication:StopApplication()
10s OnOffApplication:CancelEvents()
10s Simulator:IsExpired(0x7fcc12c4db50)
10s Simulator:Cancel(0x7fcc12c4db50)
10s Simulator:Cancel(0x7fcc12c4db38)
10s UdpSocketImpl:Close()
```


Due to

    Simulator::Run ();

Destroy (line 4583-4789):
```
10s Simulator:Destroy()
DefaultSimulatorImpl:Destroy(): handle destroy 0x7fcc12c478c0
NodeList:Delete()
NodeList:DoDispose()
PointToPointNetDevice:DoDispose()
PointToPointNetDevice:DoDispose()
Ipv4L3Protocol:DoDispose(0x7fcc12c49540)
Ipv4Interface:DoDispose()
Ipv4Interface:~Ipv4Interface()
Ipv4Interface:DoDispose()
Ipv4Interface:~Ipv4Interface()
Ipv4Interface:DoDispose()
Ipv4Interface:~Ipv4Interface()
Ipv4ListRouting:DoDispose()
Ipv4ListRouting:~Ipv4ListRouting()
GlobalRouter:DoDispose()
Ipv4GlobalRouting:DoDispose()
Ipv4GlobalRouting:~Ipv4GlobalRouting()
Ipv6L3Protocol:DoDispose()
Ipv6Interface:DoDispose()
Ipv6Interface:~Ipv6Interface()
NetDevice:~NetDevice()
Ipv6InterfaceAddress:~Ipv6InterfaceAddress()
Ipv6ListRouting:DoDispose()
Ipv6StaticRouting:DoDispose()
Ipv6StaticRouting:~Ipv6StaticRouting()
Ipv6ListRouting:~Ipv6ListRouting()
ArpL3Protocol:DoDispose(0x7fcc12c491e0)
Icmpv4L4Protocol:DoDispose(0x7fcc12c497d0)
UdpL4Protocol:DoDispose()
Ipv4EndPointDemux:~Ipv4EndPointDemux()
10 [node 0] TcpL4Protocol:DoDispose()
Ipv4EndPointDemux:~Ipv4EndPointDemux()
Icmpv6L4Protocol:DoDispose()
Ipv6Extension:~Ipv6ExtensionLooseRouting()
Ipv6Extension:~Ipv6ExtensionRouting()
Ipv6Extension:~Ipv6Extension()
Ipv6Extension:~Ipv6ExtensionHopByHop()
Ipv6Extension:~Ipv6Extension()
Ipv6Extension:~Ipv6ExtensionDestination()
Ipv6Extension:~Ipv6Extension()
Ipv6Extension:DoDispose()
Ipv6Extension:~Ipv6ExtensionFragment()
Ipv6Extension:~Ipv6Extension()
Ipv6Extension:~Ipv6ExtensionRouting()
Ipv6Extension:~Ipv6Extension()
Ipv6Option:~Ipv6OptionPad1()
Ipv6Option:~Ipv6Option()
Ipv6Option:~Ipv6OptionPadn()
Ipv6Option:~Ipv6Option()
Ipv6Option:~Ipv6OptionJumbogram()
Ipv6Option:~Ipv6Option()
Ipv6Option:~Ipv6OptionRouterAlert()
Ipv6Option:~Ipv6Option()
Ipv4L3Protocol:DoDispose(0x7fcc12c4a4e0)
Ipv4Interface:DoDispose()
Ipv4Interface:~Ipv4Interface()
Ipv4Interface:DoDispose()
Ipv4Interface:~Ipv4Interface()
Ipv4ListRouting:DoDispose()
Ipv4ListRouting:~Ipv4ListRouting()
GlobalRouter:DoDispose()
Ipv4GlobalRouting:DoDispose()
Ipv4GlobalRouting:~Ipv4GlobalRouting()
PointToPointNetDevice:DoDispose()
PacketSink:DoDispose(0x7fcc12c4d640)
Simulator:Cancel(0x7fcc12c4d678)
Simulator:Cancel(0x7fcc12c4d690)
Ipv6L3Protocol:DoDispose()
Ipv6Interface:DoDispose()
Ipv6Interface:~Ipv6Interface()
NetDevice:~NetDevice()
Ipv6InterfaceAddress:~Ipv6InterfaceAddress()
Ipv6ListRouting:DoDispose()
Ipv6StaticRouting:DoDispose()
Ipv6StaticRouting:~Ipv6StaticRouting()
Ipv6ListRouting:~Ipv6ListRouting()
ArpL3Protocol:DoDispose(0x7fcc12c4a370)
Icmpv4L4Protocol:DoDispose(0x7fcc12c4a810)
UdpL4Protocol:DoDispose()
Ipv4EndPointDemux:~Ipv4EndPointDemux()
UdpSocketImpl:Destroy()
UdpSocketImpl:~UdpSocketImpl()
UdpSocket:~UdpSocket()
Socket:~Socket()
10 [node 1] TcpL4Protocol:DoDispose()
Ipv4EndPointDemux:~Ipv4EndPointDemux()
Icmpv6L4Protocol:DoDispose()
Ipv6Extension:~Ipv6ExtensionLooseRouting()
Ipv6Extension:~Ipv6ExtensionRouting()
Ipv6Extension:~Ipv6Extension()
Ipv6Extension:~Ipv6ExtensionHopByHop()
Ipv6Extension:~Ipv6Extension()
Ipv6Extension:~Ipv6ExtensionDestination()
Ipv6Extension:~Ipv6Extension()
Ipv6Extension:DoDispose()
Ipv6Extension:~Ipv6ExtensionFragment()
Ipv6Extension:~Ipv6Extension()
Ipv6Extension:~Ipv6ExtensionRouting()
Ipv6Extension:~Ipv6Extension()
Ipv6Option:~Ipv6OptionPad1()
Ipv6Option:~Ipv6Option()
Ipv6Option:~Ipv6OptionPadn()
Ipv6Option:~Ipv6Option()
Ipv6Option:~Ipv6OptionJumbogram()
Ipv6Option:~Ipv6Option()
Ipv6Option:~Ipv6OptionRouterAlert()
Ipv6Option:~Ipv6Option()
PointToPointNetDevice:DoDispose()
OnOffApplication:DoDispose()
Simulator:Cancel(0x7fcc12c4dab8)
Simulator:Cancel(0x7fcc12c4dad0)
Ipv4L3Protocol:DoDispose(0x7fcc12c4b5b0)
Ipv4Interface:DoDispose()
Ipv4Interface:~Ipv4Interface()
Ipv4Interface:DoDispose()
Ipv4Interface:~Ipv4Interface()
Ipv4ListRouting:DoDispose()
Ipv4ListRouting:~Ipv4ListRouting()
GlobalRouter:DoDispose()
Ipv4GlobalRouting:DoDispose()
Ipv4GlobalRouting:~Ipv4GlobalRouting()
Ipv6L3Protocol:DoDispose()
Ipv6Interface:DoDispose()
Ipv6Interface:~Ipv6Interface()
NetDevice:~NetDevice()
Ipv6InterfaceAddress:~Ipv6InterfaceAddress()
Ipv6ListRouting:DoDispose()
Ipv6StaticRouting:DoDispose()
Ipv6StaticRouting:~Ipv6StaticRouting()
Ipv6ListRouting:~Ipv6ListRouting()
ArpL3Protocol:DoDispose(0x7fcc12c4b420)
Icmpv4L4Protocol:DoDispose(0x7fcc12c4b8e0)
UdpL4Protocol:DoDispose()
Ipv4EndPointDemux:~Ipv4EndPointDemux()
UdpSocketImpl:Destroy()
UdpSocketImpl:~UdpSocketImpl()
UdpSocket:~UdpSocket()
Socket:~Socket()
10 [node 2] TcpL4Protocol:DoDispose()
Ipv4EndPointDemux:~Ipv4EndPointDemux()
Icmpv6L4Protocol:DoDispose()
Ipv6Extension:~Ipv6ExtensionLooseRouting()
Ipv6Extension:~Ipv6ExtensionRouting()
Ipv6Extension:~Ipv6Extension()
Ipv6Extension:~Ipv6ExtensionHopByHop()
Ipv6Extension:~Ipv6Extension()
Ipv6Extension:~Ipv6ExtensionDestination()
Ipv6Extension:~Ipv6Extension()
Ipv6Extension:DoDispose()
Ipv6Extension:~Ipv6ExtensionFragment()
Ipv6Extension:~Ipv6Extension()
Ipv6Extension:~Ipv6ExtensionRouting()
Ipv6Extension:~Ipv6Extension()
Ipv6Option:~Ipv6OptionPad1()
Ipv6Option:~Ipv6Option()
Ipv6Option:~Ipv6OptionPadn()
Ipv6Option:~Ipv6Option()
Ipv6Option:~Ipv6OptionJumbogram()
Ipv6Option:~Ipv6Option()
Ipv6Option:~Ipv6OptionRouterAlert()
Ipv6Option:~Ipv6Option()
DefaultSimulatorImpl:Destroy(): handle destroy 0x7fcc12c48630
ChannelList:Delete()
ChannelList:DoDispose()
Channel:~Channel()
Channel:~Channel()
DefaultSimulatorImpl:Destroy(): handle destroy 0x7fcc12c48df0
Ipv4AddressGenerator:~Ipv4AddressGeneratorImpl()
DefaultSimulatorImpl:Destroy(): handle destroy 0x7fcc12c4ce70
GlobalRouteManager:~GlobalRouteManagerImpl()
GlobalRouteManager:~GlobalRouteManagerLSDB()
GlobalRouteManager:~GlobalRouteManagerLSDB(): free LSA
GlobalRouter:~GlobalRoutingLSA()
GlobalRouter:ClearLinkRecords()
GlobalRouter:ClearLinkRecords(): Free link record
GlobalRouter:~GlobalRoutingLinkRecord()
GlobalRouter:ClearLinkRecords(): Free link record
GlobalRouter:~GlobalRoutingLinkRecord()
GlobalRouter:ClearLinkRecords(): Free link record
GlobalRouter:~GlobalRoutingLinkRecord()
GlobalRouter:ClearLinkRecords(): Free link record
GlobalRouter:~GlobalRoutingLinkRecord()
GlobalRouter:ClearLinkRecords(): Free link record
GlobalRouter:~GlobalRoutingLinkRecord()
GlobalRouter:ClearLinkRecords(): Clear list
GlobalRouteManager:~GlobalRouteManagerLSDB(): free LSA
GlobalRouter:~GlobalRoutingLSA()
GlobalRouter:ClearLinkRecords()
GlobalRouter:ClearLinkRecords(): Free link record
GlobalRouter:~GlobalRoutingLinkRecord()
GlobalRouter:ClearLinkRecords(): Free link record
GlobalRouter:~GlobalRoutingLinkRecord()
GlobalRouter:ClearLinkRecords(): Free link record
GlobalRouter:~GlobalRoutingLinkRecord()
GlobalRouter:ClearLinkRecords(): Clear list
GlobalRouteManager:~GlobalRouteManagerLSDB(): free LSA
GlobalRouter:~GlobalRoutingLSA()
GlobalRouter:ClearLinkRecords()
GlobalRouter:ClearLinkRecords(): Free link record
GlobalRouter:~GlobalRoutingLinkRecord()
GlobalRouter:ClearLinkRecords(): Free link record
GlobalRouter:~GlobalRoutingLinkRecord()
GlobalRouter:ClearLinkRecords(): Free link record
GlobalRouter:~GlobalRoutingLinkRecord()
GlobalRouter:ClearLinkRecords(): Clear list
GlobalRouteManager:~GlobalRouteManagerLSDB(): clear map
```

Due to

    Simulator::Destroy ();

Remove all the residues in the simulator, and delete objects as suggested by the reference count.

## Clean up

(line 4790-4871)

```
VerySimple:main(): Done.
OnOffApplication:~OnOffApplication()
PacketSink:~PacketSink(0x7fcc12c4d640)
PointToPointNetDevice:~PointToPointNetDevice()
DropTailQueue:~DropTailQueue()
Queue:~Queue()
NetDevice:~NetDevice()
PointToPointNetDevice:~PointToPointNetDevice()
DropTailQueue:~DropTailQueue()
Queue:~Queue()
NetDevice:~NetDevice()
Ipv4L3Protocol:~Ipv4L3Protocol(0x7fcc12c4a4e0)
GlobalRouter:~GlobalRouter()
GlobalRouter:ClearLSAs()
GlobalRouter:ClearLSAs(): Free LSA
GlobalRouter:~GlobalRoutingLSA()
GlobalRouter:ClearLinkRecords()
GlobalRouter:ClearLinkRecords(): Free link record
GlobalRouter:~GlobalRoutingLinkRecord()
GlobalRouter:ClearLinkRecords(): Free link record
GlobalRouter:~GlobalRoutingLinkRecord()
GlobalRouter:ClearLinkRecords(): Free link record
GlobalRouter:~GlobalRoutingLinkRecord()
GlobalRouter:ClearLinkRecords(): Clear list
GlobalRouter:ClearLSAs(): Clear list of LSAs
Ipv6L3Protocol:~Ipv6L3Protocol()
ArpL3Protocol:~ArpL3Protocol(0x7fcc12c4a370)
UdpL4Protocol:~UdpL4Protocol()
TcpL4Protocol:~TcpL4Protocol()
Icmpv6L4Protocol:~Icmpv6L4Protocol()
Ipv4L3Protocol:~Ipv4L3Protocol(0x7fcc12c4b5b0)
GlobalRouter:~GlobalRouter()
GlobalRouter:ClearLSAs()
GlobalRouter:ClearLSAs(): Free LSA
GlobalRouter:~GlobalRoutingLSA()
GlobalRouter:ClearLinkRecords()
GlobalRouter:ClearLinkRecords(): Free link record
GlobalRouter:~GlobalRoutingLinkRecord()
GlobalRouter:ClearLinkRecords(): Free link record
GlobalRouter:~GlobalRoutingLinkRecord()
GlobalRouter:ClearLinkRecords(): Free link record
GlobalRouter:~GlobalRoutingLinkRecord()
GlobalRouter:ClearLinkRecords(): Clear list
GlobalRouter:ClearLSAs(): Clear list of LSAs
Ipv6L3Protocol:~Ipv6L3Protocol()
ArpL3Protocol:~ArpL3Protocol(0x7fcc12c4b420)
UdpL4Protocol:~UdpL4Protocol()
TcpL4Protocol:~TcpL4Protocol()
Icmpv6L4Protocol:~Icmpv6L4Protocol()
PointToPointNetDevice:~PointToPointNetDevice()
DropTailQueue:~DropTailQueue()
Queue:~Queue()
NetDevice:~NetDevice()
PointToPointNetDevice:~PointToPointNetDevice()
DropTailQueue:~DropTailQueue()
Queue:~Queue()
NetDevice:~NetDevice()
Ipv4L3Protocol:~Ipv4L3Protocol(0x7fcc12c49540)
GlobalRouter:~GlobalRouter()
GlobalRouter:ClearLSAs()
GlobalRouter:ClearLSAs(): Free LSA
GlobalRouter:~GlobalRoutingLSA()
GlobalRouter:ClearLinkRecords()
GlobalRouter:ClearLinkRecords(): Free link record
GlobalRouter:~GlobalRoutingLinkRecord()
GlobalRouter:ClearLinkRecords(): Free link record
GlobalRouter:~GlobalRoutingLinkRecord()
GlobalRouter:ClearLinkRecords(): Free link record
GlobalRouter:~GlobalRoutingLinkRecord()
GlobalRouter:ClearLinkRecords(): Free link record
GlobalRouter:~GlobalRoutingLinkRecord()
GlobalRouter:ClearLinkRecords(): Free link record
GlobalRouter:~GlobalRoutingLinkRecord()
GlobalRouter:ClearLinkRecords(): Clear list
GlobalRouter:ClearLSAs(): Clear list of LSAs
Ipv6L3Protocol:~Ipv6L3Protocol()
ArpL3Protocol:~ArpL3Protocol(0x7fcc12c491e0)
UdpL4Protocol:~UdpL4Protocol()
TcpL4Protocol:~TcpL4Protocol()
Icmpv6L4Protocol:~Icmpv6L4Protocol()
Names:~NamesPriv()
```

By the time program returns, the data structures in the heap are deleted, and the destructors are invoked, as above.
