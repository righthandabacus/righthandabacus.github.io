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
